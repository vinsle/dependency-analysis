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
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( serialize_metrics_in_xml )
{
    MockClassMetric classMetric;
    MockDependencyMetric dependencyMetric;
    ClassMetricVisitor_ABC* classVisitor = 0;
    DependencyMetricVisitor_ABC* dependencyVisitor = 0;
    MOCK_EXPECT( classMetric, Apply ).once().with( mock::retrieve( classVisitor ) );
    MOCK_EXPECT( dependencyMetric, Apply ).once().with( mock::retrieve( dependencyVisitor ) );
    MetricSerializer serializer( dependencyMetric, classMetric );
    BOOST_REQUIRE( classVisitor );
    BOOST_REQUIRE( dependencyVisitor );
    classVisitor->NotifyClassMetric( "module1", 4u, 2u );
    classVisitor->NotifyClassMetric( "module2", 4u, 2u );
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "module2/include" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module1", "module1/include" );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost", "boost/assign.hpp" );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    const std::string expected =
        "<report>"
        "    <dependencies>"
        "        <dependency name='module1'/>"
        "        <dependency name='module2'/>"
        "    </dependencies>"
        "    <categories>"
        "        <category name='module1'>"
        "            <efferent-dependencies Ce='1'>"
        "                <dependency name='module2' number='1'/>"
        "            </efferent-dependencies>"
        "            <afferent-dependencies Ca='1'>"
        "                <dependency name='module2' number='1'/>"
        "            </afferent-dependencies>"
        "            <external-dependencies Ce='1'>"
        "                <dependency name='boost' number='1'/>"
        "            </external-dependencies>"
        "            <metrics>"
        "                <number-of-classes>4</number-of-classes>"
        "                <number-of-abstract-classes>2</number-of-abstract-classes>"
        "                <abstractness>50</abstractness>"
        "                <instability>50</instability>"
        "                <distance>0</distance>"
        "            </metrics>"
        "        </category>"
        "        <category name='module2'>"
        "            <efferent-dependencies Ce='1'>"
        "                <dependency name='module1' number='1'/>"
        "            </efferent-dependencies>"
        "            <afferent-dependencies Ca='1'>"
        "                <dependency name='module1' number='1'/>"
        "            </afferent-dependencies>"
        "            <external-dependencies Ce='0'/>"
        "            <metrics>"
        "                <number-of-classes>4</number-of-classes>"
        "                <number-of-abstract-classes>2</number-of-abstract-classes>"
        "                <abstractness>50</abstractness>"
        "                <instability>50</instability>"
        "                <distance>0</distance>"
        "            </metrics>"
        "        </category>"
        "    </categories>"
        "</report>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_AUTO_TEST_CASE( simple_metric_serialization )
{
    MockClassMetric classMetric;
    MockDependencyMetric dependencyMetric;
    ClassMetricVisitor_ABC* classVisitor = 0;
    DependencyMetricVisitor_ABC* dependencyVisitor = 0;
    MOCK_EXPECT( classMetric, Apply ).once().with( mock::retrieve( classVisitor ) );
    MOCK_EXPECT( dependencyMetric, Apply ).once().with( mock::retrieve( dependencyVisitor ) );
    MetricSerializer serializer( dependencyMetric, classMetric );
    BOOST_REQUIRE( classVisitor );
    BOOST_REQUIRE( dependencyVisitor );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost", "boost/assign.hpp" );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    const std::string expected =
        "<report>"
        "    <dependencies>"
        "        <dependency name='module1'/>"
        "    </dependencies>"
        "    <categories>"
        "        <category name='module1'>"
        "            <efferent-dependencies Ce='0'/>"
        "            <afferent-dependencies Ca='0'/>"
        "            <external-dependencies Ce='1'>"
        "                <dependency name='boost' number='1'/>"
        "            </external-dependencies>"
        "            <metrics>"
        "                <number-of-classes>0</number-of-classes>"
        "                <number-of-abstract-classes>0</number-of-abstract-classes>"
        "                <abstractness>0</abstractness>"
        "                <instability>0</instability>"
        "                <distance>100</distance>"
        "            </metrics>"
        "        </category>"
        "    </categories>"
        "</report>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}