//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/StronglyConnectedComponentsSerializer.h"
#include "depend/ComponentVisitor_ABC.h"
#include "MockVisitable.h"
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
            MOCK_EXPECT( components, Apply ).once().with( mock::retrieve( visitor ) );
        }
        MockVisitable< StronglyConnectedComponentsVisitor_ABC > components;
        StronglyConnectedComponentsVisitor_ABC* visitor;
    };
    class ComponentFixture : public Fixture
    {
    public:
        ComponentFixture()
            : serializer( components )
        {
            BOOST_REQUIRE( visitor );
        }
        StronglyConnectedComponentsSerializer serializer;
    };
}

BOOST_FIXTURE_TEST_CASE( empty_strongly_connected_components_serialization, ComponentFixture )
{
    const std::string expected = "<strongly-connected-components/>";
    xml::xostringstream xos;
    serializer.Serialize( xos );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

namespace
{
    bool NotifyUnits( ComponentVisitor_ABC& visitor )
    {
        visitor.NotifyUnit( "unit1" );
        visitor.NotifyUnit( "unit2" );
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( simple_strongly_connected_component_serialization, ComponentFixture )
{
    MockVisitable< ComponentVisitor_ABC > component;
    MOCK_EXPECT( component, Apply ).once().with( &NotifyUnits );
    visitor->NotifyComponent( component );
    const std::string expected = 
        "<strongly-connected-components>"
        "    <component>"
        "        <node>unit1</node>"
        "        <node>unit2</node>"
        "    </component>"
        "</strongly-connected-components>";
    xml::xostringstream xos;
    serializer.Serialize( xos );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
