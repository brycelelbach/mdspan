///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E)
#define STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E

#include "detail/fwd.hpp"
#include "detail/integer_sequence.hpp"

namespace std { namespace experimental { namespace detail
{

///////////////////////////////////////////////////////////////////////////////

template <
    template <typename, typename, typename, typename> typename LayoutMapping
  , std::size_t... OrderIndices
>
struct layout_regular_impl
{
    template <std::size_t... Steps>
    struct stepping
    {
        template <std::size_t... Pads>
        struct padding
        {
            // Striding specified, padding specified.
            template <typename Dimensions>
            using mapping = LayoutMapping<
                Dimensions
              , dimensions<Steps...>
              , dimensions<Pads...>
              , index_sequence<OrderIndices...> 
            >;
        };

        // Striding specified, padding defaulted.
        template <typename Dimensions>
        using mapping = LayoutMapping<
            Dimensions
          , dimensions<Steps...>
          , detail::make_filled_dims_t<Dimensions::rank(), 0>
          , index_sequence<OrderIndices...> 
        >;
    };

    template <std::size_t... Pads>
    struct padding
    {
        template <std::size_t... Steps>
        struct stepping
        {
            // Striding specified, padding specified.
            template <typename Dimensions>
            using mapping = LayoutMapping<
                Dimensions
              , dimensions<Steps...>
              , dimensions<Pads...> 
              , index_sequence<OrderIndices...> 
            >;
        };

        // Striding defaulted, padding specified.
        template <typename Dimensions>
        using mapping = LayoutMapping<
            Dimensions
          , detail::make_filled_dims_t<Dimensions::rank(), 1>
          , dimensions<Pads...>
          , index_sequence<OrderIndices...> 
        >;
    };

    // Striding defaulted, padding defaulted.
    template <typename Dimensions>
    using mapping = LayoutMapping<
        Dimensions
      , detail::make_filled_dims_t<Dimensions::rank(), 1>
      , detail::make_filled_dims_t<Dimensions::rank(), 0>
      , index_sequence<OrderIndices...> 
    >;
};

}} // std::experimental

#endif // STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E

