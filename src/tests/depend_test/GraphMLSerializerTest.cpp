//
//  Copyright Silvin Lubecki 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/GraphMLSerializer.h"
#include "MockVisitable.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( graph_ml_serializer_serializes_edges_with_graph_ml_format )
{
    MockVisitable< UnitObserver_ABC > units;
    MockVisitable< DependencyVisitor_ABC > dependencyMetric;
    UnitObserver_ABC* unitsVisitor = 0;
    DependencyVisitor_ABC* dependencyVisitor = 0;
    MOCK_EXPECT( units.Apply ).once().with( mock::retrieve( unitsVisitor ) );
    MOCK_EXPECT( dependencyMetric.Apply ).once().with( mock::retrieve( dependencyVisitor ) );
    GraphMLSerializer serializer( dependencyMetric, units );
    BOOST_REQUIRE( unitsVisitor );
    BOOST_REQUIRE( dependencyVisitor );
    unitsVisitor->NotifyUnit( "module1", "context" );
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context1" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module1", "context2" );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost", "context3" );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    const std::string expected =
        "<graphml>"
        "  <graph edgedefault='directed' id='G'>"
        "    <node id='boost'/>"
        "    <node id='module1'/>"
        "    <node id='module2'/>"
        "    <edge source='module1' target='boost'/>"
        "    <edge source='module1' target='module2'/>"
        "    <edge source='module2' target='module1'/>"
        "  </graph>"
        "</graphml>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
