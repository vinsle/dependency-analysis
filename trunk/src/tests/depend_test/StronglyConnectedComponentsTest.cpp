//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/StronglyConnectedComponents.h"
#include "MockDependencyMetric.h"
#include <xeumeuleu/xml.hpp>
#include <boost/assign.hpp>

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
        MockDependencyMetric dependencies;
        DependencyMetricVisitor_ABC* visitor;
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
    components.Serialize( xos, StronglyConnectedComponents::T_Filter() );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( simple_strongly_connected_components_detection, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "something" );
    visitor->NotifyInternalDependency( "to", "from", "something" );
    visitor->NotifyInternalDependency( "other", "to", "something" );
    const std::string expected = 
        "<strongly-connected-components>"
        "    <component>"
        "        <module>from</module>"
        "        <module>to</module>"
        "    </component>"
        "</strongly-connected-components>";
    xml::xostringstream xos;
    components.Serialize( xos, StronglyConnectedComponents::T_Filter() );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( filtered_components_with_only_one_module_is_empty, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "something" );
    visitor->NotifyInternalDependency( "to", "from", "something" );
    visitor->NotifyInternalDependency( "other", "to", "something" );
    const std::string expected = "<strongly-connected-components/>";
    xml::xostringstream xos;
    components.Serialize( xos, boost::assign::list_of( "from" ) );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( simple_strongly_connected_components_are_filtered, ComponentFixture )
{
    visitor->NotifyInternalDependency( "from", "to", "something" );
    visitor->NotifyInternalDependency( "to", "from", "something" );
    visitor->NotifyInternalDependency( "other", "to", "something" );
    visitor->NotifyInternalDependency( "from", "other", "something" );
    const std::string expected = 
        "<strongly-connected-components>"
        "    <component>"
        "        <module>from</module>"
        "        <module>to</module>"
        "    </component>"
        "</strongly-connected-components>";
    xml::xostringstream xos;
    components.Serialize( xos, boost::assign::list_of( "from" )( "to" ) );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
