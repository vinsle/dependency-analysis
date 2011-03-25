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
            : dependencyVisitor( 0 )
        {
            MOCK_EXPECT( dependencyMetric, Apply ).once().with( mock::retrieve( dependencyVisitor ) );
        }
        MockVisitable< DependencyMetricVisitor_ABC > dependencyMetric;
        DependencyMetricVisitor_ABC* dependencyVisitor;
        MockFilter filter;
    };
    class SerializeFixture : public Fixture
    {
    public:
        SerializeFixture()
            : serializer( dependencyMetric )
        {
            BOOST_REQUIRE( dependencyVisitor );
        }
        EdgeSerializer serializer;
    };
}

BOOST_FIXTURE_TEST_CASE( serialize_edges_in_xml, SerializeFixture )
{
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context1" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module1", "context2" );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost", "context3" );
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).returns( true );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<graph>"
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
        "</graph>";
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
        "<graph>"
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
        "</graph>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
