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
        "  <dependencies>"
        "    <dependency name='module1'/>"
        "    <dependency name='module2'/>"
        "    <dependency name='module3'/>"
        "  </dependencies>"
        "  <categories>"
        "    <category name='module1'>"
        "      <efferent-dependencies Ce='1'>"
        "        <dependency name='module2' number='1'/>"
        "      </efferent-dependencies>"
        "      <afferent-dependencies Ca='2'>"
        "        <dependency name='module2' number='1'/>"
        "        <dependency name='module3' number='1'/>"
        "      </afferent-dependencies>"
        "      <external-dependencies Ce='0'/>"
        "      <metrics>"
        "        <number-of-classes>0</number-of-classes>"
        "        <number-of-abstract-classes>0</number-of-abstract-classes>"
        "        <abstractness>0</abstractness>"
        "        <instability>0</instability>"
        "        <distance>10</distance>"
        "      </metrics>"
        "    </category>"
        "    <category name='module2'>"
        "      <efferent-dependencies Ce='1'>"
        "        <dependency name='module1' number='1'/>"
        "      </efferent-dependencies>"
        "      <afferent-dependencies Ca='1'>"
        "        <dependency name='module1' number='1'/>"
        "      </afferent-dependencies>"
        "      <external-dependencies Ce='0'/>"
        "      <metrics>"
        "        <number-of-classes>0</number-of-classes>"
        "        <number-of-abstract-classes>0</number-of-abstract-classes>"
        "        <abstractness>0</abstractness>"
        "        <instability>0</instability>"
        "        <distance>10</distance>"
        "      </metrics>"
        "    </category>"
        "    <category name='module3'>"
        "      <efferent-dependencies Ce='1'>"
        "        <dependency name='module1' number='1'/>"
        "      </efferent-dependencies>"
        "      <afferent-dependencies Ca='0'/>"
        "      <external-dependencies Ce='0'/>"
        "      <metrics>"
        "        <number-of-classes>0</number-of-classes>"
        "        <number-of-abstract-classes>0</number-of-abstract-classes>"
        "        <abstractness>0</abstractness>"
        "        <instability>0</instability>"
        "        <distance>10</distance>"
        "      </metrics>"
        "    </category>"
        "  </categories>"
        "  <strongly-connected-components>"
        "    <component>"
        "      <module>module1</module>"
        "      <module>module2</module>"
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
        "\"module1\"->\"module2\"[label=\"1\",color=\"1 1.0 1.0\"];"
        "\"module2\"->\"module1\"[label=\"1\",color=\"1 1.0 1.0\"];"
        "\"module3\"->\"module1\"[label=\"1\"];"
        "}\n";
    BOOST_CHECK_EQUAL( expected, os.str() );
}