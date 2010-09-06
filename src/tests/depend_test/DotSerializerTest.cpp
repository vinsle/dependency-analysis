//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/DotSerializer.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( simple_dot_serialization )
{
    xml::xistringstream xis(
        "<report>"
        "  <nodes>"
        "    <node>module1</node>"
        "    <node>module2</node>"
        "    <node>module3</node>"
        "    <node>module4</node>"
        "  </nodes>"
        "  <graph>"
        "    <node name='module1'>"
        "      <efferent-dependencies Ce='1'>"
        "        <dependency name='module2' number='1'/>"
        "      </efferent-dependencies>"
        "      <afferent-dependencies Ca='2'>"
        "        <dependency name='module2' number='1'/>"
        "        <dependency name='module3' number='1'/>"
        "      </afferent-dependencies>"
        "      <external-dependencies Ce='0'/>"
        "    </node>"
        "    <node name='module2'>"
        "      <efferent-dependencies Ce='1'>"
        "        <dependency name='module1' number='1'/>"
        "      </efferent-dependencies>"
        "      <afferent-dependencies Ca='1'>"
        "        <dependency name='module1' number='1'/>"
        "      </afferent-dependencies>"
        "      <external-dependencies Ce='0'/>"
        "    </node>"
        "    <node name='module3'>"
        "      <efferent-dependencies Ce='2'>"
        "        <dependency name='module1' number='1'/>"
        "        <dependency name='module4' number='1'/>"
        "      </efferent-dependencies>"
        "      <afferent-dependencies Ca='0'/>"
        "      <external-dependencies Ce='0'/>"
        "    </node>"
        "    <node name='module4'>"
        "      <efferent-dependencies Ce='1'>"
        "        <dependency name='module3' number='1'/>"
        "      </efferent-dependencies>"
        "      <afferent-dependencies Ca='0'/>"
        "      <external-dependencies Ce='0'/>"
        "    </node>"
        "  </graph>"
        "  <metrics>"
        "    <metric name='module1'>"
        "      <number-of-classes>0</number-of-classes>"
        "      <number-of-abstract-classes>0</number-of-abstract-classes>"
        "      <abstractness>0</abstractness>"
        "      <instability>0</instability>"
        "      <distance>10</distance>"
        "    </metric>"
        "    <metric name='module2'>"
        "      <number-of-classes>0</number-of-classes>"
        "      <number-of-abstract-classes>0</number-of-abstract-classes>"
        "      <abstractness>0</abstractness>"
        "      <instability>0</instability>"
        "      <distance>10</distance>"
        "    </metric>"
        "    <metric name='module3'>"
        "      <number-of-classes>0</number-of-classes>"
        "      <number-of-abstract-classes>0</number-of-abstract-classes>"
        "      <abstractness>0</abstractness>"
        "      <instability>0</instability>"
        "      <distance>10</distance>"
        "    </metric>"
        "    <metric name='module4'>"
        "      <number-of-classes>0</number-of-classes>"
        "      <number-of-abstract-classes>0</number-of-abstract-classes>"
        "      <abstractness>0</abstractness>"
        "      <instability>0</instability>"
        "      <distance>10</distance>"
        "    </metric>"
        "  </metrics>"
        "  <strongly-connected-components>"
        "    <component>"
        "      <node>module1</node>"
        "      <node>module2</node>"
        "    </component>"
        "    <component>"
        "      <node>module3</node>"
        "      <node>module4</node>"
        "    </component>"
        "  </strongly-connected-components>"
        "</report>" );
    DotSerializer serializer;
    std::ostringstream os;
    serializer.Serialize( xis, os );
    const std::string expected =
        "digraph G {"
        "\"module1\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];"
        "\"module2\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];"
        "\"module3\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];"
        "\"module4\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];"
        "\"module1\"->\"module2\"[color=\"0.5 1.0 1.0\"];"
        "\"module2\"->\"module1\"[color=\"0.5 1.0 1.0\"];"
        "\"module3\"->\"module1\";"
        "\"module3\"->\"module4\"[color=\"1 1.0 1.0\"];"
        "\"module4\"->\"module3\"[color=\"1 1.0 1.0\"];"
        "}\n";
    BOOST_CHECK_EQUAL( expected, os.str() );
}
