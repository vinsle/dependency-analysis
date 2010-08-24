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

using namespace depend;

BOOST_AUTO_TEST_CASE( strongly_connected_components_serialization )
{
    MockDependencyMetric dependencies;
    DependencyMetricVisitor_ABC* visitor = 0;
    MOCK_EXPECT( dependencies, Apply ).once().with( mock::retrieve( visitor ) );
    StronglyConnectedComponents components( dependencies );
    BOOST_REQUIRE( visitor );
    const std::string expected = "<strongly-connected-components/>";
    xml::xostringstream xos;
    components.Serialize( xos );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_AUTO_TEST_CASE( simple_strongly_connected_components_detection )
{
    MockDependencyMetric dependencies;
    DependencyMetricVisitor_ABC* visitor = 0;
    MOCK_EXPECT( dependencies, Apply ).once().with( mock::retrieve( visitor ) );
    StronglyConnectedComponents components( dependencies );
    BOOST_REQUIRE( visitor );
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
    components.Serialize( xos );
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
