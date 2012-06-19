//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/InternalModuleResolver.h"
#include "MockFinder.h"
#include "MockSubject.h"
#include <sstream>
#include <xeumeuleu/xml.hpp>

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : unitObserver( 0 )
        {
            MOCK_EXPECT( mockUnitObserver.Register ).once().with( mock::retrieve( unitObserver ) );
            MOCK_EXPECT( mockUnitObserver.Unregister ).once();
        }
        UnitObserver_ABC* unitObserver;
        MockSubject< UnitObserver_ABC > mockUnitObserver;
    };
    class ResolverFixture : public Fixture
    {
    public:
        ResolverFixture()
            : xis( "<paths>"
                   "    <path>libraries</path>"
                   "    <path>applications</path>"
                   "</paths>" )
            , resolver( xis, finder, mockUnitObserver )
        {
            BOOST_REQUIRE( unitObserver );
        }
        xml::xistringstream xis;
        MockFinder finder;
        InternalModuleResolver resolver;
    };
}

BOOST_FIXTURE_TEST_CASE( resolver_returns_empty_if_include_module_is_unknown, ResolverFixture )
{
    MOCK_EXPECT( finder.Find ).once().with( "libraries/module/module2/include" ).returns( false );
    MOCK_EXPECT( finder.Find ).once().with( "applications/module/module2/include" ).returns( false );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( "module", "file", "module2/include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_tests_all_paths, ResolverFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    MOCK_EXPECT( finder.Find ).once().with( "libraries/module/include" ).returns( false );
    MOCK_EXPECT( finder.Find ).once().with( "applications/module/include" ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module", "file", "include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_finds_include_relative_to_current_file, ResolverFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    MOCK_EXPECT( finder.Find ).once().with( "libraries/module/include" ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module", "file", "include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_finds_include_relative_to_current_file_in_subfolder, ResolverFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    MOCK_EXPECT( finder.Find ).once().with( "libraries/module/subfolder/include" ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module", "subfolder/file", "include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_finds_include_with_absolute_context, ResolverFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    mock::sequence s;
    MOCK_EXPECT( finder.Find ).once().in( s ).with( "libraries/module/subfolder/module/include" ).returns( false );
    MOCK_EXPECT( finder.Find ).once().in( s ).with( "applications/module/subfolder/module/include" ).returns( false );
    MOCK_EXPECT( finder.Find ).once().in( s ).with( "libraries/module/include" ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module", "subfolder/file", "module/include" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_finds_include_from_other_modules, ResolverFixture )
{
    unitObserver->NotifyUnit( "module2", "module context" );
    mock::sequence s;
    MOCK_EXPECT( finder.Find ).once().in( s ).with( "libraries/module/module2/include" ).returns( false );
    MOCK_EXPECT( finder.Find ).once().in( s ).with( "applications/module/module2/include" ).returns( false );
    MOCK_EXPECT( finder.Find ).once().in( s ).with( "libraries/module2/include" ).returns( true );
    BOOST_CHECK_EQUAL( "module2", resolver.Resolve( "module", "file", "module2/include" ) );
}
