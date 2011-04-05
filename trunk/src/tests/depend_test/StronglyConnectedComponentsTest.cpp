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
#include <xeumeuleu/xml.hpp>

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : visitor( 0 )
        {
            MOCK_EXPECT( dependencies, Apply ).once().with( mock::retrieve( visitor ) );
        }
        MockVisitable< DependencyMetricVisitor_ABC > dependencies;
        DependencyMetricVisitor_ABC* visitor;
        MockFilter filter;
    };
    class ComponentFixture : public Fixture
    {
    public:
        ComponentFixture()
            : components( dependencies )
        {
            BOOST_REQUIRE( visitor );
        }
        StronglyConnectedComponents components;
    };
}

BOOST_FIXTURE_TEST_CASE( strongly_connected_components_serialization, ComponentFixture )
{
    const std::string expected = "<strongly-connected-components/>";
    xml::xostringstream xos;
    BOOST_CHECK( components.Serialize( xos, filter ) );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( simple_strongly_connected_components_detection, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "context" );
    visitor->NotifyInternalDependency( "to", "from", "context" );
    visitor->NotifyInternalDependency( "other", "to", "context" );
    const std::string expected = 
        "<strongly-connected-components>"
        "    <component>"
        "        <node>from</node>"
        "        <node>to</node>"
        "    </component>"
        "</strongly-connected-components>";
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).returns( true );
    BOOST_CHECK( !components.Serialize( xos, filter ) );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( filtered_components_with_only_one_module_is_empty, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "context" );
    visitor->NotifyInternalDependency( "to", "from", "context" );
    visitor->NotifyInternalDependency( "other", "to", "context" );
    const std::string expected = "<strongly-connected-components/>";
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).with( "from" ).returns( true );
    MOCK_EXPECT( filter, Check ).returns( false );
    BOOST_CHECK( components.Serialize( xos, filter ) );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( simple_strongly_connected_components_are_filtered, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "context" );
    visitor->NotifyInternalDependency( "to", "from", "context" );
    visitor->NotifyInternalDependency( "other", "to", "context" );
    visitor->NotifyInternalDependency( "from", "other", "context" );
    const std::string expected = 
        "<strongly-connected-components>"
        "    <component>"
        "        <node>from</node>"
        "        <node>to</node>"
        "    </component>"
        "</strongly-connected-components>";
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).with( "from" ).returns( true );
    MOCK_EXPECT( filter, Check ).with( "to" ).returns( true );
    MOCK_EXPECT( filter, Check ).returns( false );
    BOOST_CHECK( !components.Serialize( xos, filter ) );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
