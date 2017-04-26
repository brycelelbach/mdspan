///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_8AD8FE21_620B_4E38_BE74_D34624F192B0)
#define STD_8AD8FE21_620B_4E38_BE74_D34624F192B0

///////////////////////////////////////////////////////////////////////////////
// Order Agnostic Regular Layout Mapping - Precomputed Strides Implementation 
///////////////////////////////////////////////////////////////////////////////

#include "detail/layout_mapping_regular_base.hpp"

namespace std { namespace experimental
{

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
struct layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
> : detail::layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>
{
    ///////////////////////////////////////////////////////////////////////////
    // TYPES

    using base_type = detail::layout_mapping_regular_base<
        Dimensions
      , Steps
      , Pads
      , detail::index_sequence<OrderIndices...>
    >;

    using typename base_type::value_type;
    using typename base_type::size_type;

    using typename base_type::ordering; 

    using typename base_type::inverse_ordering;

    ///////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS AND ASSIGNMENT OPERATORS

    constexpr layout_mapping_regular_precomputed_strides() noexcept;

    constexpr layout_mapping_regular_precomputed_strides(
        layout_mapping_regular_precomputed_strides const&
        ) noexcept = default;
    constexpr layout_mapping_regular_precomputed_strides(
        layout_mapping_regular_precomputed_strides&&
        ) noexcept = default;
    layout_mapping_regular_precomputed_strides& operator=(
        layout_mapping_regular_precomputed_strides const&
        ) noexcept = default;
    layout_mapping_regular_precomputed_strides& operator=(
        layout_mapping_regular_precomputed_strides&&
        ) noexcept = default;

    template <typename... ForwardDims>
    constexpr layout_mapping_regular_precomputed_strides(
        ForwardDims&&... fdims
        ) noexcept;

    template <typename Dimensions::size_type N>
    constexpr layout_mapping_regular_precomputed_strides(
        array<value_type, N> a
        ) noexcept;

    constexpr layout_mapping_regular_precomputed_strides(
        Dimensions d
        ) noexcept;

    constexpr layout_mapping_regular_precomputed_strides(
        Dimensions d, Steps s, Pads p
        ) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    // DOMAIN SPACE

    using base_type::otr;
    using base_type::rto;

    using base_type::is_regular;

    using base_type::is_dynamic_stride;

    using base_type::stride_static;

/* FIXME
    using base_type::strides_static;
*/

    constexpr size_type stride(size_type rank) const noexcept;

/* FIXME
    constexpr dimensions<...>
    strides() const noexcept;
*/
   
    template <typename... Indices>
    constexpr size_type index(Indices... indices) const noexcept;

    template <typename Dimensions::size_type... IdxDims>
    constexpr size_type index(dimensions<IdxDims...> i) const noexcept;

    ///////////////////////////////////////////////////////////////////////////

  protected:

    ///////////////////////////////////////////////////////////////////////////

    using base_type::compute_stride;

    ///////////////////////////////////////////////////////////////////////////

  private:

    ///////////////////////////////////////////////////////////////////////////

    struct dynamic_stride_generator
    {
        using layout_mapping = layout_mapping_regular_precomputed_strides;

        constexpr dynamic_stride_generator(layout_mapping& lm) noexcept
        // {{{
          : lm_(lm) {}
        // }}}

        template <size_type Rank>
        constexpr value_type operator()(
            integral_constant<size_type,  Rank>
            ) noexcept
        { // {{{
            return lm_.template compute_stride<Rank>(
                lm_, lm_.steps(), lm_.pads(), lm_.stride_
            );
        } // }}}

      private:
        layout_mapping& lm_;
    };

    template <typename Dimensions::size_type Rank>
    constexpr size_type compute_dynamic_stride(
        Dimensions   d
      , Steps        s
      , Pads         p
        ) const noexcept;

    ///////////////////////////////////////////////////////////////////////////

    // TODO: Move to layout_mapping_regular_base

    // Pass Rank as a template parameter instead of a function parameter to 
    // avoid recursion on the same function signature.

    // Base case.
    template <
        typename Dimensions::size_type    Rank
      , typename Dimensions::size_type... IdxDims
        >
    constexpr detail::enable_if_t<
         detail::is_rank_unit_stride<Rank, Dimensions, ordering>::value
      , size_type
    >
    compute_index(dimensions<IdxDims...> i) const noexcept
    { // {{{
        static_assert(
            Dimensions::rank() == sizeof...(IdxDims)
          , "The ranks of Dimensions and dimensions<IdxDims...> are not equal."
        );
        return i[Rank] * stride(Rank);
    } // }}}

    template <
        typename Dimensions::size_type    Rank
      , typename Dimensions::size_type... IdxDims
        >
    constexpr detail::enable_if_t<
        !detail::is_rank_unit_stride<Rank, Dimensions, ordering>::value
      , size_type
    >
    compute_index(dimensions<IdxDims...> i) const noexcept
    { // {{{
        static_assert(
            Dimensions::rank() == sizeof...(IdxDims)
          , "The ranks of Dimensions and dimensions<IdxDims...> are not equal."
        );
        return i[Rank] * stride(Rank) + compute_index<otr(rto(Rank) - 1)>(i);
    } // }}}

    ///////////////////////////////////////////////////////////////////////////

    // Indexed by order, not rank.
    dimensions<stride_static(otr(OrderIndices))...> stride_;
};

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::layout_mapping_regular_precomputed_strides() noexcept
  : base_type()
  , stride_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <typename... ForwardDims>
constexpr layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::layout_mapping_regular_precomputed_strides(ForwardDims&&... fdims) noexcept
  : base_type(std::forward<ForwardDims>(fdims)...)
  , stride_(
//        compute_dynamic_stride<otr(OrderIndices)>(*this, Steps(), Pads())...
        dynamic_stride_generator(*this)
    )
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <typename Dimensions::size_type N>
constexpr layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::layout_mapping_regular_precomputed_strides(array<value_type, N> a) noexcept
  : base_type(a)
  , stride_(
//        compute_dynamic_stride<otr(OrderIndices)>(*this, Steps(), Pads())...
        dynamic_stride_generator(*this)
    )
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::layout_mapping_regular_precomputed_strides(
    Dimensions d, Steps s, Pads p
    ) noexcept
  : base_type(d, s, p)
  , stride_(
//        compute_dynamic_stride<otr(OrderIndices)>(d, s, p)...
        dynamic_stride_generator(*this)
    )
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::layout_mapping_regular_precomputed_strides(Dimensions d) noexcept
  : base_type(d)
  , stride_(
//        compute_dynamic_stride<otr(OrderIndices)>(d, Steps(), Pads())...
        dynamic_stride_generator(*this)
    )
{}

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::stride(size_type rank) const noexcept
{
    return stride_[rto(rank)];
}

/* FIXME
template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr dimensions<
    detail::layout_mapping_regular_base<
        Dimensions
      , Steps
      , Pads
      , detail::index_sequence<OrderIndices...>
    >::computed_static_stride_[otr(OrderIndices)]...
> layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::strides() const noexcept
{
    return dimensions<
        layout_mapping_regular_precomputed_strides<
            Dimensions
          , Steps
          , Pads
          , detail::index_sequence<OrderIndices...>
        >::computed_static_stride_[otr(OrderIndices)]...
    >(stride(otr(OrderIndices))...); 
}
*/

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <
    typename... Indices 
    >
inline constexpr typename layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::index(Indices... indices) const noexcept
{
    static_assert(
        Dimensions::rank() == sizeof...(Indices)
      , "The ranks of Dimensions and Indices are not equal."
    );
    return index(detail::make_filled_dims_t<Dimensions::rank(), dyn>(indices...));
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <
    typename Dimensions::size_type... IdxDims 
    >
inline constexpr typename layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::size_type 
layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::index(dimensions<IdxDims...> i) const noexcept
{
    static_assert(
        Dimensions::rank() == sizeof...(IdxDims)
      , "The ranks of Dimensions and dimensions<IdxDims...> are not equal."
    );
    return compute_index<Dimensions::rank() - 1>(i);
}

///////////////////////////////////////////////////////////////////////////////

/*
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
inline constexpr typename layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_precomputed_strides<
    Dimensions
  , Steps
  , Pads
  , detail::index_sequence<OrderIndices...>
>::compute_dynamic_stride(
    Dimensions   d
  , Steps        s
  , Pads         p
    ) const noexcept
{
    return ( is_dynamic_stride(Rank)
           ? compute_stride<Rank>(d, s, p, stride_)
           : computed_static_stride_[rto(Rank)]
           );
}
*/

///////////////////////////////////////////////////////////////////////////////

}} // std::experimental

#endif // STD_8AD8FE21_620B_4E38_BE74_D34624F192B0

