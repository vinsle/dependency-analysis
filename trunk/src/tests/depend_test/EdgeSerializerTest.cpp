//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/EdgeSerializer.h"
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
            : unitsVisitor     ( 0 )
            , dependencyVisitor( 0 )
        {
            MOCK_EXPECT( units, Apply ).once().in( s ).with( mock::retrieve( unitsVisitor ) );
            MOCK_EXPECT( dependencyMetric, Apply ).once().in( s ).with( mock::retrieve( dependencyVisitor ) );
        }
        mock::sequence s;
        MockVisitable< UnitObserver_ABC > units;
        UnitObserver_ABC* unitsVisitor;
        MockVisitable< DependencyVisitor_ABC > dependencyMetric;
        DependencyVisitor_ABC* dependencyVisitor;
        MockFilter filter;
    };
    class SerializeFixture : public Fixture
    {
    public:
        SerializeFixture()
            : serializer( dependencyMetric, units )
        {
            BOOST_REQUIRE( unitsVisitor );
            BOOST_REQUIRE( dependencyVisitor );
        }
        EdgeSerializer serializer;
    };
}

BOOST_FIXTURE_TEST_CASE( serialize_edges_in_xml, SerializeFixture )
{
    unitsVisitor->NotifyUnit( "module1", "context" );
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context1" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module1", "context2" );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost", "context3" );
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).returns( true );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<nodes>"
        "    <node name='module1'>"
        "        <efferent-dependencies>"
        "            <dependency name='module2'>"
        "                <context>context1</context>"
        "            </dependency>"
        "        </efferent-dependencies>"
        "        <external-dependencies>"
        "            <dependency name='boost'>"
        "                <context>context3</context>"
        "            </dependency>"
        "        </external-dependencies>"
        "    </node>"
        "    <node name='module2'>"
        "        <efferent-dependencies>"
        "            <dependency name='module1'>"
        "                <context>context2</context>"
        "            </dependency>"
        "        </efferent-dependencies>"
        "        <external-dependencies/>"
        "    </node>"
        "</nodes>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( serialize_units_without_dependency_in_xml, SerializeFixture )
{
    unitsVisitor->NotifyUnit( "module", "context" );
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).returns( true );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<nodes>"
        "    <node name='module'>"
        "        <efferent-dependencies/>"
        "        <external-dependencies/>"
        "    </node>"
        "</nodes>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( serialize_edges_with_module_filter, SerializeFixture )
{
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context1" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module4", "context2" );
    dependencyVisitor->NotifyInternalDependency( "module3", "module1", "context3" );
    dependencyVisitor->NotifyInternalDependency( "module3", "module2", "context4" );
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context5" );
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).with( "module1" ).returns( true );
    MOCK_EXPECT( filter, Check ).with( "module2" ).returns( true );
    MOCK_EXPECT( filter, Check ).with( "module3" ).returns( true );
    MOCK_EXPECT( filter, Check ).returns( false );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<nodes>"
        "    <node name='module1'>"
        "        <efferent-dependencies>"
        "            <dependency name='module2'>"
        "                <context>context1</context>"
        "                <context>context5</context>"
        "            </dependency>"
        "        </efferent-dependencies>"
        "        <external-dependencies/>"
        "    </node>"
        "    <node name='module2'>"
        "        <efferent-dependencies/>"
        "        <external-dependencies/>"
        "    </node>"
        "    <node name='module3'>"
        "        <efferent-dependencies>"
        "            <dependency name='module1'>"
        "                <context>context3</context>"
        "            </dependency>"
        "            <dependency name='module2'>"
        "                <context>context4</context>"
        "            </dependency>"
        "        </efferent-dependencies>"
        "        <external-dependencies/>"
        "    </node>"
        "</nodes>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
