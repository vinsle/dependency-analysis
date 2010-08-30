//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/Filter.h"
#include <boost/assign.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( empty_filter_check_is_always_true )
{
    const Filter filter;
    BOOST_CHECK( filter.Check( "module" ) );
}

BOOST_AUTO_TEST_CASE( filter_checks_module )
{
    const Filter::T_Modules modules = boost::assign::list_of< std::string >( "module1" );
    const Filter filter( modules );
    BOOST_CHECK( filter.Check( "module1" ) );
    BOOST_CHECK( !filter.Check( "module2" ) );
}
