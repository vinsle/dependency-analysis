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
        "  <units>"
        "    <unit core='true'>module1</unit>"
        "    <unit core='false'>module2</unit>"
        "    <unit core='false'>module3</unit>"
        "    <unit core='false'>module4</unit>"
        "  </units>"
        "  <externals>"
        "    <external>boost</external>"
        "  </externals>"
        "  <graph>"
        "    <node name='module1'>"
        "      <efferent-dependencies>"
        "        <dependency name='module2'>"
        "            <context>context</context>"
        "        </dependency>"
        "      </efferent-dependencies>"
        "      <external-dependencies>"
        "        <dependency name='boost'/>"
        "      </external-dependencies>"
        "    </node>"
        "    <node name='module2'>"
        "      <efferent-dependencies>"
        "        <dependency name='module1'/>"
        "      </efferent-dependencies>"
        "      <external-dependencies/>"
        "    </node>"
        "    <node name='module3'>"
        "      <efferent-dependencies>"
        "        <dependency name='module1'/>"
        "        <dependency name='module4'/>"
        "      </efferent-dependencies>"
        "      <external-dependencies/>"
        "    </node>"
        "    <node name='module4'>"
        "      <efferent-dependencies>"
        "        <dependency name='module3'/>"
        "      </efferent-dependencies>"
        "      <external-dependencies/>"
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
    serializer.Serialize( xis, os, Internal );
    const std::string expected =
        "digraph G {\n"
        "\"module1\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,fontcolor=\"blue\",color=\"0.25 1.0 1.0\"];\n"
        "\"module2\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];\n"
        "\"module3\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];\n"
        "\"module4\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];\n"
        "\"module1\"->\"module2\"[color=\"0.5 1.0 1.0\"];\n"
        "\"module2\"->\"module1\"[color=\"0.5 1.0 1.0\"];\n"
        "\"module3\"->\"module1\";\n"
        "\"module3\"->\"module4\"[color=\"1 1.0 1.0\"];\n"
        "\"module4\"->\"module3\"[color=\"1 1.0 1.0\"];\n"
        "}\n";
    BOOST_CHECK_EQUAL( expected, os.str() );
}

BOOST_AUTO_TEST_CASE( external_dot_serialization )
{
    xml::xistringstream xis(
        "<report>"
        "  <units>"
        "    <unit core='false'>module1</unit>"
        "    <unit core='false'>module2</unit>"
        "  </units>"
        "  <externals>"
        "    <external>boost</external>"
        "    <external>qt</external>"
        "  </externals>"
        "  <graph>"
        "    <node name='module1'>"
        "      <efferent-dependencies>"
        "        <dependency name='module2'/>"
        "      </efferent-dependencies>"
        "      <external-dependencies>"
        "        <dependency name='boost'/>"
        "        <dependency name='qt'/>"
        "      </external-dependencies>"
        "    </node>"
        "    <node name='module2'>"
        "      <efferent-dependencies>"
        "        <dependency name='module1'/>"
        "      </efferent-dependencies>"
        "      <external-dependencies>"
        "        <dependency name='boost'/>"
        "      </external-dependencies>"
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
        "  </metrics>"
        "  <strongly-connected-components/>"
        "</report>" );
    DotSerializer serializer;
    std::ostringstream os;
    serializer.Serialize( xis, os, External );
    const std::string expected =
        "digraph G {\n"
        "\"module1\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];\n"
        "\"module2\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];\n"
        "\"boost\"[label=\"\\N\",shape=rectangle,style=filled];\n"
        "\"qt\"[label=\"\\N\",shape=rectangle,style=filled];\n"
        "\"module1\"->\"boost\";\n"
        "\"module1\"->\"qt\";\n"
        "\"module2\"->\"boost\";\n"
        "}\n";
    BOOST_CHECK_EQUAL( expected, os.str() );
}

BOOST_AUTO_TEST_CASE( both_dot_serialization )
{
    xml::xistringstream xis(
        "<report>"
        "  <units>"
        "    <unit core='false'>module1</unit>"
        "    <unit core='false'>module2</unit>"
        "  </units>"
        "  <externals>"
        "    <external>boost</external>"
        "    <external>qt</external>"
        "  </externals>"
        "  <graph>"
        "    <node name='module1'>"
        "      <efferent-dependencies>"
        "        <dependency name='module2'/>"
        "      </efferent-dependencies>"
        "      <external-dependencies>"
        "        <dependency name='boost'/>"
        "        <dependency name='qt'/>"
        "      </external-dependencies>"
        "    </node>"
        "    <node name='module2'>"
        "      <efferent-dependencies>"
        "        <dependency name='module1'/>"
        "      </efferent-dependencies>"
        "      <external-dependencies>"
        "        <dependency name='boost'/>"
        "      </external-dependencies>"
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
        "  </metrics>"
        "  <strongly-connected-components/>"
        "</report>" );
    DotSerializer serializer;
    std::ostringstream os;
    serializer.Serialize( xis, os, Both );
    const std::string expected =
        "digraph G {\n"
        "\"module1\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];\n"
        "\"module2\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];\n"
        "\"boost\"[label=\"\\N\",shape=rectangle,style=filled];\n"
        "\"qt\"[label=\"\\N\",shape=rectangle,style=filled];\n"
        "\"module1\"->\"module2\";\n"
        "\"module1\"->\"boost\";\n"
        "\"module1\"->\"qt\";\n"
        "\"module2\"->\"module1\";\n"
        "\"module2\"->\"boost\";\n"
        "}\n";
    BOOST_CHECK_EQUAL( expected, os.str() );
}