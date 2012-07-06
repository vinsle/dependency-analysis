//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/Finder.h"

using namespace depend;

BOOST_AUTO_TEST_CASE( unknown_file_is_not_found )
{
    const Finder finder;
    BOOST_CHECK( !finder.Find( "unknown_file" ) );
}

BOOST_AUTO_TEST_CASE( existing_file_is_found )
{
    const Finder finder;
    BOOST_CHECK( finder.Find( BOOST_RESOLVE( "finder_test/file" ) ) );
}

BOOST_AUTO_TEST_CASE( existing_directory_is_found )
{
    const Finder finder;
    BOOST_CHECK( finder.Find( BOOST_RESOLVE( "finder_test" ) ) );
}
