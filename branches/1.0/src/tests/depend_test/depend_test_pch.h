//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_pch_h
#define depend_test_pch_h

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#include <turtle/mock.hpp>

namespace mock
{
    template< typename S >
    boost::function< S > make_function( mock::function< S >& e )
    {
        return boost::function< S >( e );
    }
}

const std::string BOOST_RESOLVE( const std::string& filename );

void BOOST_CHECK_XML_EQUAL( const std::string& expected, const std::string& actual );

#pragma warning( disable : 4505 ) // unreferenced local function has been removed
#pragma warning( disable : 4355 ) // used in base member initializer list

#endif // depend_test_pch_h
