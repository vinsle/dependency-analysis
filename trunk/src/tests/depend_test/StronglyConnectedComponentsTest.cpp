//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/StronglyConnectedComponents.h"
#include "MockVisitable.h"
#include "MockFilter.h"
#include "MockStronglyConnectedComponentsVisitor.h"
#include "MockComponentVisitor.h"
#include <xeumeuleu/xml.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : visitor( 0 )
        {
            MOCK_EXPECT( dependencies.Apply ).once().with( mock::retrieve( visitor ) );
        }
        MockVisitable< DependencyVisitor_ABC > dependencies;
        DependencyVisitor_ABC* visitor;
        MockFilter filter;
    };
    class ComponentFixture : public Fixture
    {
    public:
        ComponentFixture()
            : components( dependencies, filter )
        {
            BOOST_REQUIRE( visitor );
        }
        StronglyConnectedComponents components;
        MockStronglyConnectedComponentsVisitor mockVisitor;
    };

    bool CheckComponent( const Visitable< ComponentVisitor_ABC >& actual, const std::vector< std::string >& expected )
    {
        MockComponentVisitor visitor;
        BOOST_FOREACH( const std::string& unit, expected )
            MOCK_EXPECT( visitor.NotifyUnit ).once().with( unit );
        actual.Apply( visitor );
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( empty_strongly_connected_components_does_not_notify, ComponentFixture )
{
    components.Apply( mockVisitor );
}

BOOST_FIXTURE_TEST_CASE( simple_strongly_connected_components_detection, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "context" );
    visitor->NotifyInternalDependency( "to", "from", "context" );
    visitor->NotifyInternalDependency( "other", "to", "context" );
    const std::vector< std::string > component = boost::assign::list_of( "from" )( "to" );
    MOCK_EXPECT( mockVisitor.NotifyComponent ).once().with( boost::bind( &CheckComponent, _1, boost::cref( component ) ) );
    MOCK_EXPECT( filter.Check ).returns( true );
    components.Apply( mockVisitor );
}

BOOST_FIXTURE_TEST_CASE( filtered_components_with_only_one_module_is_empty, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "context" );
    visitor->NotifyInternalDependency( "to", "from", "context" );
    visitor->NotifyInternalDependency( "other", "to", "context" );
    MOCK_EXPECT( filter.Check ).with( "from" ).returns( true );
    MOCK_EXPECT( filter.Check ).returns( false );
    components.Apply( mockVisitor );
}

BOOST_FIXTURE_TEST_CASE( simple_strongly_connected_components_are_filtered, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "context" );
    visitor->NotifyInternalDependency( "to", "from", "context" );
    visitor->NotifyInternalDependency( "other", "to", "context" );
    visitor->NotifyInternalDependency( "from", "other", "context" );
    const std::vector< std::string > component = boost::assign::list_of( "from" )( "to" );
    MOCK_EXPECT( mockVisitor.NotifyComponent ).once().with( boost::bind( &CheckComponent, _1, boost::cref( component ) ) );
    MOCK_EXPECT( filter.Check ).with( "from" ).returns( true );
    MOCK_EXPECT( filter.Check ).with( "to" ).returns( true );
    MOCK_EXPECT( filter.Check ).returns( false );
    components.Apply( mockVisitor );
}

BOOST_FIXTURE_TEST_CASE( multiple_strongly_connected_components_are_detected, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "context" );
    visitor->NotifyInternalDependency( "to", "from", "context" );
    visitor->NotifyInternalDependency( "otherFrom", "otherTo", "context" );
    visitor->NotifyInternalDependency( "otherTo", "otherFrom", "context" );
    const std::vector< std::string > firstComponent = boost::assign::list_of( "from" )( "to" );
    const std::vector< std::string > secondComponent = boost::assign::list_of( "otherFrom" )( "otherTo" );
    MOCK_EXPECT( mockVisitor.NotifyComponent ).once().with( boost::bind( &CheckComponent, _1, boost::cref( firstComponent ) ) );
    MOCK_EXPECT( mockVisitor.NotifyComponent ).once().with( boost::bind( &CheckComponent, _1, boost::cref( secondComponent ) ) );
    MOCK_EXPECT( filter.Check ).returns( true );
    components.Apply( mockVisitor );
}
