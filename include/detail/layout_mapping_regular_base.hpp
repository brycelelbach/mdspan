///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_A3B0F119_C38A_44E3_95E4_DD40594F4328)
#define STD_A3B0F119_C38A_44E3_95E4_DD40594F4328

///////////////////////////////////////////////////////////////////////////////
// Order Agnostic Regular Layout Mapping - Base Class 
///////////////////////////////////////////////////////////////////////////////

// d   := dims
// p   := padding 
// s   := stepping
// i   := index
// n   := rank
// rto := rank index to ordering (smallest stride dimension to largest stride dimension)
// otr := ordering to rank index (smallest rank index to largest rank index)
// 
// stride[otr[0]] = s[otr[0]]
// stride[r]      = p[otr[rto[r]-1]] + s[r] * d[otr[rto[r]-1]] * stride[otr[rto[r]-1]]
// start at r = otr[n-1]
// 
// index[otr[0]] = i[otr[0]] * stride[otr[0]]
// index[r]      = i[r]      * stride[r]      + index[otr[rto[r]-1]]
// start at r = otr[n-1]

#include "detail/fwd.hpp"
#include "detail/meta.hpp"
#include "detail/dimensions.hpp"

namespace std { namespace experimental { namespace detail
{

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
struct layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
> : Dimensions
{
    static_assert(
        Dimensions::rank() == Steps::rank() 
      , "The ranks of Dimensions and Steps are not equal."
    );
    static_assert(
        Dimensions::rank() == Pads::rank() 
      , "The ranks of Dimensions and Pads are not equal."
    );
    static_assert(
        Dimensions::rank() == sizeof...(OrderIndices)
      , "The ranks of Dimensions and index_sequence<OrderIndices...> are "
        "not equal."
    );

    ///////////////////////////////////////////////////////////////////////////
    // TYPES

    using base_type = Dimensions;

    using typename Dimensions::value_type;
    using typename Dimensions::size_type;

    // Smallest stride dimension to largest stride dimension (order to rank).
    using ordering = index_sequence<OrderIndices...>;

    // Inverse mapping of ordering (rank to order).
    using inverse_ordering = make_integer_sequence_inverse_mapping<ordering>;

    ///////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS AND ASSIGNMENT OPERATORS

    constexpr layout_mapping_regular_base() noexcept;

    constexpr layout_mapping_regular_base(
        layout_mapping_regular_base const&
        ) noexcept = default;
    constexpr layout_mapping_regular_base(
        layout_mapping_regular_base&&
        ) noexcept = default;
    layout_mapping_regular_base& operator=(
        layout_mapping_regular_base const&
        ) noexcept = default;
    layout_mapping_regular_base& operator=(
        layout_mapping_regular_base&&
        ) noexcept = default;

    template <typename... ForwardDims>
    constexpr layout_mapping_regular_base(ForwardDims&&... fdims) noexcept;

    template <std::size_t N>
    constexpr layout_mapping_regular_base(array<value_type, N> a) noexcept;

    constexpr layout_mapping_regular_base(Dimensions d) noexcept;

    constexpr layout_mapping_regular_base(Dimensions d, Steps s, Pads p) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    // DOMAIN SPACE

    // Order-to-rank mapping. Returns the rank of the dimension with the
    // specified ordering.
    static constexpr size_type otr(size_type order) noexcept
    { // {{{
        return integer_sequence_array<ordering>()[order];
    } // }}}

    // Rank-to-order mapping. Returns the ordering for the dimension with the
    // specified rank.
    static constexpr size_type rto(size_type rank) noexcept
    { // {{{
        return integer_sequence_array<inverse_ordering>()[rank];
    } // }}}

    static constexpr bool is_regular() noexcept;

    static constexpr bool is_dynamic_stride(size_type rank) noexcept
    { // {{{
        return computed_is_dynamic_stride_[rto(rank)];
    } // }}}

    constexpr size_type span() const noexcept;

    constexpr size_type step(size_type rank) const noexcept;
    constexpr Steps steps() const noexcept;

    constexpr size_type pad(size_type rank) const noexcept;
    constexpr Pads pads() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    // INDEXING

    static constexpr size_type stride_static(size_type rank) noexcept
    { // {{{
        return computed_static_stride_[rto(rank)];
    } // }}}

/* FIXME
    static constexpr dimensions<computed_static_stride_[otr(OrderIndices)]...>
    strides_static() const noexcept
    { // {{{
        return dimensions<
            computed_static_stride_[otr(OrderIndices)]...
        >(otr(OrderIndices)...); 
    } // }}}
*/

    // Must be implemented by the derived class.
    //constexpr size_type stride(size_type rank) const noexcept;

    // Must be implemented by the derived class.
    //constexpr dimensions<see-below> strides() const noexcept;

    // Must be implemented by the derived class.
    //template <typename... Idxs>
    //constexpr size_type index(Idxs... idxs) const noexcept;

    // Must be implemented by the derived class.
    //template <size_type... IdxDims>
    //constexpr size_type index(dimensions<IdxDims...> i) const noexcept;

    ///////////////////////////////////////////////////////////////////////////

  protected:

    ///////////////////////////////////////////////////////////////////////////

    // Pass Rank as a template parameter instead of a function parameter to 
    // avoid recursion on the same function signature.

    // Base case.
    template <typename Dimensions::size_type Rank>
    static constexpr enable_if_t<
         is_rank_unit_stride<Rank, Dimensions, ordering>::value, bool
    > compute_is_dynamic_stride() noexcept
    { // {{{
        return Steps::is_dynamic(Rank);
    } // }}}

    template <typename Dimensions::size_type Rank>
    static constexpr enable_if_t<
        !is_rank_unit_stride<Rank, Dimensions, ordering>::value, bool
    > compute_is_dynamic_stride() noexcept
    { // {{{
        static_assert(
            rto(Rank) > 0
          , "Rank order must be greater than 0 for this overload."
        );
        // We don't use is_dynamic_stride here because GCC chokes on it when
        // calling this function at constexpr time. 
        return computed_is_dynamic_stride_[rto(Rank) - 1]
            || Dimensions::is_dynamic(otr(rto(Rank) - 1))
            || Steps::is_dynamic(Rank)
            || Pads::is_dynamic(otr(rto(Rank) - 1));
    } // }}}

    // Indexed by order, not rank.
    static constexpr bool computed_is_dynamic_stride_[Dimensions::rank()] =
    {
        compute_is_dynamic_stride<otr(OrderIndices)>()...
    };

    ///////////////////////////////////////////////////////////////////////////

    // Pass Rank as a template parameter instead of a function parameter to 
    // avoid recursion on the same function signature.

    // Base case.
    template <typename Dimensions::size_type Rank, typename Arrayish>
    static constexpr enable_if_t<
         is_rank_unit_stride<Rank, Dimensions, ordering>::value, size_type
    > compute_stride(
        Dimensions   d
      , Steps        s
      , Pads         p
      , Arrayish const& last
        ) noexcept
    {
        return s[Rank];
    }

    template <typename Dimensions::size_type Rank, typename Arrayish>
    static constexpr enable_if_t<
        !is_rank_unit_stride<Rank, Dimensions, ordering>::value, size_type
    > compute_stride(
        Dimensions   d
      , Steps        s
      , Pads         p
      , Arrayish const& last
        ) noexcept
    {
        return p[otr(rto(Rank) - 1)]
             + s[Rank] * d[otr(rto(Rank) - 1)] * last[rto(Rank) - 1];
    }

    ///////////////////////////////////////////////////////////////////////////

    template <typename Dimensions::size_type Rank>
    static constexpr size_type compute_static_stride() noexcept;

    // Indexed by order, not rank.
    static constexpr size_type computed_static_stride_[Dimensions::rank()] =
    {
        compute_static_stride<otr(OrderIndices)>()...
    };

    ///////////////////////////////////////////////////////////////////////////

    Steps steps_; // Indexed by order, not rank.
    Pads pads_;   // Indexed by order, not rank.
};

///////////////////////////////////////////////////////////////////////////////
template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base() noexcept
  : base_type()
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <typename... ForwardDims>
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(ForwardDims&&... fdims) noexcept
  : base_type(std::forward<ForwardDims>(fdims)...)
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <std::size_t N>
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(array<value_type, N> a) noexcept
  : base_type(a)
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(Dimensions d) noexcept
  : base_type(d)
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(Dimensions d, Steps s, Pads p) noexcept
  : base_type(d)
  , steps_(s)
  , pads_(p) 
{}

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr bool layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::is_regular() noexcept
{
    return true; 
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::span() const noexcept
{
    return detail::dims_ternary_reduction<
        detail::span_by_value
      , detail::multiplies_by_value
      , detail::static_sentinel<1>
      , 0
      , Dimensions::rank()
    >()(*static_cast<Dimensions const*>(this), steps_, pads_);
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::step(size_type rank) const noexcept
{
    return steps_[rank];
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr Steps layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::steps() const noexcept
{
    return steps_;
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::pad(size_type rank) const noexcept
{
    return pads_[rank];
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr Pads layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::pads() const noexcept
{
    return pads_;
}

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr bool layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::computed_is_dynamic_stride_[];

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
// Pass Rank as a template parameter instead of a function parameter to 
// avoid recursion on the same function signature.
template <
    typename Dimensions::size_type Rank
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::compute_static_stride() noexcept
{
    return ( is_dynamic_stride(Rank)
           ? dyn
           : compute_stride<Rank>(
                Dimensions(), Steps(), Pads(), computed_static_stride_
             )
           );
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::computed_static_stride_[];

///////////////////////////////////////////////////////////////////////////////

}}} // std::experimental::detail

#endif // STD_A3B0F119_C38A_44E3_95E4_DD40594F4328

