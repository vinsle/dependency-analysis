//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/UnitDependencyLoader.h"
#include "MockDependencyVisitor.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( loader_parse_dependency_xml_and_notifies_visitor )
{
    xml::xistringstream xis(
        "<graph>"
        "  <nodes>"
        "    <node name='from'>"
        "        <efferent-dependencies>"
        "            <dependency name='to'>"
        "                <context>context1</context>"
        "                <context>context2</context>"
        "            </dependency>"
        "        </efferent-dependencies>"
        "        <external-dependencies>"
        "            <dependency name='external'>"
        "                <context>context3</context>"
        "            </dependency>"
        "        </external-dependencies>"
        "    </node>"
        "    <node name='to'>"
        "        <efferent-dependencies>"
        "            <dependency name='from'>"
        "                <context>context4</context>"
        "            </dependency>"
        "        </efferent-dependencies>"
        "        <external-dependencies/>"
        "    </node>"
        "  </nodes>"
        "</graph>" );
    UnitDependencyLoader loader( xis );
    MockDependencyVisitor visitor;
    mock::sequence s;
    MOCK_EXPECT( visitor.NotifyInternalDependency ).once().in( s ).with( "from", "to", "context1" );
    MOCK_EXPECT( visitor.NotifyInternalDependency ).once().in( s ).with( "from", "to", "context2" );
    MOCK_EXPECT( visitor.NotifyExternalDependency ).once().in( s ).with( "from", "external", "context3" );
    MOCK_EXPECT( visitor.NotifyInternalDependency ).once().in( s ).with( "to", "from", "context4" );
    loader.Apply( visitor );
}
