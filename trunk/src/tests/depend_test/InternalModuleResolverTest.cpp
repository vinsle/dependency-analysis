//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/InternalModuleResolver.h"
#include "MockSubject.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : unitObserver( 0 )
        {
            MOCK_EXPECT( mockUnitObserver, Register ).once().with( mock::retrieve( unitObserver ) );
            MOCK_EXPECT( mockUnitObserver, Unregister ).once();
        }
        UnitObserver_ABC* unitObserver;
        MockSubject< UnitObserver_ABC > mockUnitObserver;
    };
    class ResolverFixture : public Fixture
    {
    public:
        ResolverFixture()
            : resolver( mockUnitObserver )
        {
            BOOST_REQUIRE( unitObserver );
        }
        InternalModuleResolver resolver;
    };
}

BOOST_FIXTURE_TEST_CASE( empty_resolver_resolves_nothing, ResolverFixture )
{
    BOOST_CHECK_EQUAL( "", resolver.Resolve( "module/include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_never_excludes, ResolverFixture )
{
    BOOST_CHECK_EQUAL( false, resolver.IsExcluded( "module/include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_returns_module_if_known, ResolverFixture )
{
    unitObserver->NotifyUnit( "module", "context" );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module/include" ) );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( "module2/include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_does_not_resolve_if_no_module, ResolverFixture )
{
    unitObserver->NotifyUnit( "module", "context" );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( "include" ) );
}
