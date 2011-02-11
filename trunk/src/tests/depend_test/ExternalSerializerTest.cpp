//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ExternalSerializer.h"
#include "MockDependencyMetric.h"
#include "MockFilter.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( simple_external_serialization )
{
    MockDependencyMetric dependency;
    DependencyMetricVisitor_ABC* visitor = 0;
    MOCK_EXPECT( dependency, Apply ).once().with( mock::retrieve( visitor ) );
    MockFilter filter;
    ExternalSerializer serializer( dependency, filter );
    BOOST_REQUIRE( visitor );
    MOCK_EXPECT( filter, Check ).once().with( "from" ).returns( true );
    visitor->NotifyExternalDependency( "from", "external", "context" );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    const std::string expected =
        "<externals>"
        "    <external>external</external>"
        "</externals>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_AUTO_TEST_CASE( external_serialization_can_be_filtered )
{
    MockDependencyMetric dependency;
    DependencyMetricVisitor_ABC* visitor = 0;
    MOCK_EXPECT( dependency, Apply ).once().with( mock::retrieve( visitor ) );
    MockFilter filter;
    ExternalSerializer serializer( dependency, filter );
    BOOST_REQUIRE( visitor );
    MOCK_EXPECT( filter, Check ).once().with( "from" ).returns( true );
    visitor->NotifyExternalDependency( "from", "external", "context" );
    MOCK_EXPECT( filter, Check ).once().with( "filtered" ).returns( false );
    visitor->NotifyExternalDependency( "filtered", "unused", "context" );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    const std::string expected =
        "<externals>"
        "    <external>external</external>"
        "</externals>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
