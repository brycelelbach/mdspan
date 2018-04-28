///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_AF26FDB8_0F80_44F9_BBDB_3BC8BEB16454)
#define STD_AF26FDB8_0F80_44F9_BBDB_3BC8BEB16454

#include "detail/fwd.hpp"
#include "detail/integer_sequence.hpp"

namespace std { namespace experimental { namespace detail
{

///////////////////////////////////////////////////////////////////////////////

template <
    template <typename, typename, typename, typename> class LayoutMappingRegular
  , typename MakeRegularOrdering
>
struct layout_regular_impl
{
    template <std::size_t... Steps>
    struct stepping
    {
        template <std::size_t... Pads>
        struct padding
        {
            // Stepping specified, padding specified.
            template <typename Dimensions>
            using mapping = LayoutMappingRegular<
                Dimensions
              , dimensions<Steps...>
              , dimensions<Pads...>
              , typename MakeRegularOrdering::template apply<Dimensions>
            >;
        };

        // Stepping specified, padding defaulted.
        template <typename Dimensions>
        using mapping = LayoutMappingRegular<
            Dimensions
          , dimensions<Steps...>
          , detail::make_filled_dims_t<Dimensions::rank(), 0>
          , typename MakeRegularOrdering::template apply<Dimensions>
        >;
    };

    template <std::size_t... Pads>
    struct padding
    {
        template <std::size_t... Steps>
        struct stepping
        {
            // Stepping specified, padding specified.
            template <typename Dimensions>
            using mapping = LayoutMappingRegular<
                Dimensions
              , dimensions<Steps...>
              , dimensions<Pads...> 
              , typename MakeRegularOrdering::template apply<Dimensions>
            >;
        };

        // Stepping defaulted, padding specified.
        template <typename Dimensions>
        using mapping = LayoutMappingRegular<
            Dimensions
          , detail::make_filled_dims_t<Dimensions::rank(), 1>
          , dimensions<Pads...>
          , typename MakeRegularOrdering::template apply<Dimensions>
        >;
    };

    // Stepping defaulted, padding defaulted.
    template <typename Dimensions>
    using mapping = LayoutMappingRegular<
        Dimensions
      , detail::make_filled_dims_t<Dimensions::rank(), 1>
      , detail::make_filled_dims_t<Dimensions::rank(), 0>
      , typename MakeRegularOrdering::template apply<Dimensions>
    >;
};

}}} // std::experimental::detail

#endif // STD_AF26FDB8_0F80_44F9_BBDB_3BC8BEB16454

