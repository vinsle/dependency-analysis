//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/MetricSerializer.h"
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
            : classVisitor( 0 )
            , dependencyVisitor( 0 )
        {
            MOCK_EXPECT( classMetric, Apply ).once().with( mock::retrieve( classVisitor ) );
            MOCK_EXPECT( dependencyMetric, Apply ).once().with( mock::retrieve( dependencyVisitor ) );
        }
        MockClassMetric classMetric;
        MockDependencyMetric dependencyMetric;
        ClassMetricVisitor_ABC* classVisitor;
        DependencyMetricVisitor_ABC* dependencyVisitor;
        MockFilter filter;
    };
    class SerializeFixture : public Fixture
    {
    public:
        SerializeFixture()
            : serializer( dependencyMetric, classMetric )
        {
            BOOST_REQUIRE( classVisitor );
            BOOST_REQUIRE( dependencyVisitor );
        }
        MetricSerializer serializer;
    };
}

BOOST_FIXTURE_TEST_CASE( serialize_metrics_in_xml, SerializeFixture )
{
    classVisitor->NotifyClassMetric( "module1", 4u, 2u );
    classVisitor->NotifyClassMetric( "module2", 4u, 2u );
    dependencyVisitor->NotifyInternalDependency( "module1", "module2" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module1" );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost" );
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).returns( true );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<metrics>"
        "    <metric name='module1'>"
        "        <number-of-classes>4</number-of-classes>"
        "        <number-of-abstract-classes>2</number-of-abstract-classes>"
        "        <abstractness>50</abstractness>"
        "        <instability>50</instability>"
        "        <distance>0</distance>"
        "    </metric>"
        "    <metric name='module2'>"
        "        <number-of-classes>4</number-of-classes>"
        "        <number-of-abstract-classes>2</number-of-abstract-classes>"
        "        <abstractness>50</abstractness>"
        "        <instability>50</instability>"
        "        <distance>0</distance>"
        "    </metric>"
        "</metrics>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( serialize_metrics_with_module_filter, SerializeFixture )
{
    classVisitor->NotifyClassMetric( "module1", 4u, 2u );
    classVisitor->NotifyClassMetric( "module2", 4u, 2u );
    classVisitor->NotifyClassMetric( "module3", 4u, 2u );
    classVisitor->NotifyClassMetric( "module4", 4u, 2u );
    dependencyVisitor->NotifyInternalDependency( "module1", "module2");
    dependencyVisitor->NotifyInternalDependency( "module2", "module4" );
    dependencyVisitor->NotifyInternalDependency( "module3", "module1" );
    dependencyVisitor->NotifyInternalDependency( "module3", "module2" );
    xml::xostringstream xos;
    MOCK_EXPECT( filter, Check ).with( "module1" ).returns( true );
    MOCK_EXPECT( filter, Check ).with( "module2" ).returns( true );
    MOCK_EXPECT( filter, Check ).with( "module3" ).returns( true );
    MOCK_EXPECT( filter, Check ).returns( false );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<metrics>"
        "    <metric name='module1'>"
        "        <number-of-classes>4</number-of-classes>"
        "        <number-of-abstract-classes>2</number-of-abstract-classes>"
        "        <abstractness>50</abstractness>"
        "        <instability>50</instability>"
        "        <distance>0</distance>"
        "    </metric>"
        "    <metric name='module2'>"
        "        <number-of-classes>4</number-of-classes>"
        "        <number-of-abstract-classes>2</number-of-abstract-classes>"
        "        <abstractness>50</abstractness>"
        "        <instability>33</instability>"
        "        <distance>17</distance>"
        "    </metric>"
        "    <metric name='module3'>"
        "        <number-of-classes>4</number-of-classes>"
        "        <number-of-abstract-classes>2</number-of-abstract-classes>"
        "        <abstractness>50</abstractness>"
        "        <instability>100</instability>"
        "        <distance>50</distance>"
        "    </metric>"
        "</metrics>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
