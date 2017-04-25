////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

// FIXME: Temporary
#include "detail/layout_mapping_regular_base.hpp"

#include <mdspan> 

#include <boost/core/lightweight_test.hpp>

using std::is_same;

using std::experimental::dyn;
using std::experimental::dimensions;
using std::experimental::detail::integer_sequence;
using std::experimental::detail::layout_mapping_regular_base;

int main()
{
    // Empty.
    {
        using lri_type = layout_mapping_regular_base<
            dimensions<>                  // Dimensions
          , dimensions<>                  // Stepping
          , dimensions<>                  // Padding
          , integer_sequence<std::size_t> // Ordering
        >;
        
        // Types.

        BOOST_TEST((is_same<
            lri_type::base_type, dimensions<>
        >::value));

        BOOST_TEST((is_same<
            lri_type::value_type, dimensions<>::value_type
        >::value));
        BOOST_TEST((is_same<
            lri_type::value_type, dimensions<>::size_type
        >::value));

        BOOST_TEST((is_same<
            lri_type::ordering,         integer_sequence<std::size_t> 
        >::value));
        BOOST_TEST((is_same<
            lri_type::inverse_ordering, integer_sequence<std::size_t> 
        >::value));

        // Constructors and assignment operators.

        lri_type const lri0;            // Default ctor.

        lri_type lri1(lri0);            // Copy constructible.
        lri_type lri2(std::move(lri1)); // Move constructible.

        lri1 = lri0;                    // Copy assignable.
        lri2 = std::move(lri0);         // Move assignable.

        // Variadic dims constructors are SFINAE'd out.

        lri_type const lri3(dimensions<>{});

        lri_type const lri4(dimensions<>{}, dimensions<>{}, dimensions<>{});

        // Domain space.

        // otr and rto are always UB for a 0-rank layout_mapping_regular_base.

        BOOST_TEST_EQ((lri_type::is_regular()), true);

        // is_dynamic_stride is always UB for a 0-rank layout_mapping_regular_base.

        BOOST_TEST_EQ((lri0.span()), 1);

        // step is always UB for a 0-rank layout_mapping_regular_base.

        BOOST_TEST((is_same<
            decltype(lri0.steps()), dimensions<> 
        >::value));

        // pad is always UB for a 0-rank layout_mapping_regular_base.

        BOOST_TEST((is_same<
            decltype(lri0.pads()), dimensions<> 
        >::value));
    }

    return boost::report_errors();
}

