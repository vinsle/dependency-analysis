//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ProxyModuleResolver.h"
#include "MockExternalModuleResolver.h"

using namespace depend;

BOOST_AUTO_TEST_CASE( first_resolver_call_is_forwarded )
{
    MockExternalModuleResolver mockResolver;
    ProxyModuleResolver resolver( mockResolver );
    MOCK_EXPECT( mockResolver.Resolve ).once().with( "include" ).returns( "module" );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "include" ) );
}

BOOST_AUTO_TEST_CASE( other_calls_are_proxified )
{
    MockExternalModuleResolver mockResolver;
    ProxyModuleResolver resolver( mockResolver );
    MOCK_EXPECT( mockResolver.Resolve ).once().with( "include" ).returns( "module" );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "include" ) );
    mock::verify();
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "include" ) );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "include" ) );
}

BOOST_AUTO_TEST_CASE( excludes_are_proxified )
{
    MockExternalModuleResolver mockResolver;
    ProxyModuleResolver resolver( mockResolver );
    MOCK_EXPECT( mockResolver.IsExcluded ).once().with( "include" ).returns( true );
    BOOST_CHECK( resolver.IsExcluded( "include" ) );
    mock::verify();
    BOOST_CHECK( resolver.IsExcluded( "include" ) );
    BOOST_CHECK( resolver.IsExcluded( "include" ) );
}
