//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/MetricSerializer.h"
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
            : metricsVisitor( 0 )
        {
            MOCK_EXPECT( metrics.Apply ).once().with( mock::retrieve( metricsVisitor ) );
        }
        MockVisitable< MetricsVisitor_ABC > metrics;
        MetricsVisitor_ABC* metricsVisitor;
        MockFilter filter;
    };
    class SerializeFixture : public Fixture
    {
    public:
        SerializeFixture()
            : serializer( metrics )
        {
            BOOST_REQUIRE( metricsVisitor );
        }
        MetricSerializer serializer;
    };
}

BOOST_FIXTURE_TEST_CASE( serialize_metrics_in_xml, SerializeFixture )
{
    metricsVisitor->NotifyMetrics( "module1", 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u );
    xml::xostringstream xos;
    MOCK_EXPECT( filter.Check ).returns( true );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<metrics>"
        "    <metric name='module1'>"
        "        <afferent>1</afferent>"
        "        <efferent>2</efferent>"
        "        <external>3</external>"
        "        <classes>4</classes>"
        "        <abstract-classes>5</abstract-classes>"
        "        <abstractness>6</abstractness>"
        "        <instability>7</instability>"
        "        <distance>8</distance>"
        "    </metric>"
        "</metrics>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_FIXTURE_TEST_CASE( serialize_metrics_with_module_filter, SerializeFixture )
{
    metricsVisitor->NotifyMetrics( "module1", 11u, 12u, 13u, 14u, 15u, 16u, 17u, 18u );
    metricsVisitor->NotifyMetrics( "module2", 21u, 22u, 23u, 24u, 25u, 26u, 27u, 28u );
    metricsVisitor->NotifyMetrics( "module3", 31u, 32u, 33u, 34u, 35u, 36u, 37u, 38u );
    xml::xostringstream xos;
    MOCK_EXPECT( filter.Check ).with( "module1" ).returns( true );
    MOCK_EXPECT( filter.Check ).with( "module2" ).returns( true );
    MOCK_EXPECT( filter.Check ).returns( false );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<metrics>"
        "    <metric name='module1'>"
        "        <afferent>11</afferent>"
        "        <efferent>12</efferent>"
        "        <external>13</external>"
        "        <classes>14</classes>"
        "        <abstract-classes>15</abstract-classes>"
        "        <abstractness>16</abstractness>"
        "        <instability>17</instability>"
        "        <distance>18</distance>"
        "    </metric>"
        "    <metric name='module2'>"
        "        <afferent>21</afferent>"
        "        <efferent>22</efferent>"
        "        <external>23</external>"
        "        <classes>24</classes>"
        "        <abstract-classes>25</abstract-classes>"
        "        <abstractness>26</abstractness>"
        "        <instability>27</instability>"
        "        <distance>28</distance>"
        "    </metric>"
        "</metrics>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
