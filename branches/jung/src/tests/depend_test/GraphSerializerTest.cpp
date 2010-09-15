//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/GraphSerializer.h"
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : filename( BOOST_RESOLVE( "png_serialization/test.png" ) )
        {
            boost::filesystem::remove( filename );
        }
        ~Fixture()
        {
            boost::filesystem::remove( filename );
        }
        std::string filename;
    };
}

BOOST_FIXTURE_TEST_CASE( simple_png_serialization, Fixture )
{
    const std::string dot =
          "digraph G {"
          "\"module1\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];"
          "\"module2\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];"
          "\"module3\"[label=\"\\N\\nI=0 A=0 D=10\",shape=rectangle,style=filled,color=\"0.25 1.0 1.0\"];"
          "\"module1\"->\"module2\"[label=\"1\",color=\"1 1.0 1.0\"];"
          "\"module2\"->\"module1\"[label=\"1\",color=\"1 1.0 1.0\"];"
          "\"module3\"->\"module1\"[label=\"1\"];"
          "}";
    xml::xistringstream xis(
        "<graph-options>"
        "   <layout>dot</layout>"
        "   <format>png</format>"
        "   <graph/>"
        "   <node/>"
        "   <edge/>"
        "</graph-options>" );
    GraphSerializer serializer( xis );
    serializer.Serialize( dot, filename );
    BOOST_CHECK( boost::filesystem::exists( filename ) );
}
