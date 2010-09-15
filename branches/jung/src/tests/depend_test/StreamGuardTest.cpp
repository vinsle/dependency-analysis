//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/istream_guard.h"

using namespace depend;

BOOST_AUTO_TEST_CASE( istream_guard_prevents_from_modifying_reading_position )
{
    std::istringstream is( "first line\nsecond line" );
    {
        istream_guard i( is );
        std::string line;
        std::getline( is, line );
        BOOST_CHECK_EQUAL( line, "first line" );
        std::getline( is, line );
        BOOST_CHECK_EQUAL( line, "second line" );
    }
    {
        istream_guard i( is );
        std::string line;
        std::getline( is, line );
        BOOST_CHECK_EQUAL( line, "first line" );
        std::getline( is, line );
        BOOST_CHECK_EQUAL( line, "second line" );
    }
}
