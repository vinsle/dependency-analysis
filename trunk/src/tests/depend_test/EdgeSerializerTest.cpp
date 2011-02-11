//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/EdgeSerializer.h"
#include "MockClassMetric.h"
#include "MockDependencyMetric.h"
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
        MockDependencyMetric dependencyMetric;
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
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module1", "context" );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost", "context" );
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).returns( true );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<graph>"
        "    <node name='module1'>"
        "        <efferent-dependencies>"
        "            <dependency>module2</dependency>"
        "        </efferent-dependencies>"
        "        <afferent-dependencies>"
        "            <dependency>module2</dependency>"
        "        </afferent-dependencies>"
        "        <external-dependencies>"
        "            <dependency>boost</dependency>"
        "        </external-dependencies>"
        "    </node>"
        "    <node name='module2'>"
        "        <efferent-dependencies>"
        "            <dependency>module1</dependency>"
        "        </efferent-dependencies>"
        "        <afferent-dependencies>"
        "            <dependency>module1</dependency>"
        "        </afferent-dependencies>"
        "        <external-dependencies/>"
        "    </node>"
        "</graph>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( serialize_edges_with_module_filter, SerializeFixture )
{
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module4", "context" );
    dependencyVisitor->NotifyInternalDependency( "module3", "module1", "context" );
    dependencyVisitor->NotifyInternalDependency( "module3", "module2", "context" );
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
        "            <dependency>module2</dependency>"
        "        </efferent-dependencies>"
        "        <afferent-dependencies>"
        "            <dependency>module3</dependency>"
        "        </afferent-dependencies>"
        "        <external-dependencies/>"
        "    </node>"
        "    <node name='module2'>"
        "        <efferent-dependencies/>"
        "        <afferent-dependencies>"
        "            <dependency>module1</dependency>"
        "            <dependency>module3</dependency>"
        "        </afferent-dependencies>"
        "        <external-dependencies/>"
        "    </node>"
        "    <node name='module3'>"
        "        <efferent-dependencies>"
        "            <dependency>module1</dependency>"
        "            <dependency>module2</dependency>"
        "        </efferent-dependencies>"
        "        <afferent-dependencies/>"
        "        <external-dependencies/>"
        "    </node>"
        "</graph>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
