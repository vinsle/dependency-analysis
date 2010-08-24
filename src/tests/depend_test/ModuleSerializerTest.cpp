//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ModuleSerializer.h"
#include "MockSubject.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( simple_module_serialization )
{
    MockSubject< ModuleObserver_ABC > mockSubject;
    ModuleObserver_ABC* observer = 0;
    MOCK_EXPECT( mockSubject, Register ).once().with( mock::retrieve( observer ) );
    MOCK_EXPECT( mockSubject, Unregister ).once();
    ModuleSerializer serializer( mockSubject );
    BOOST_REQUIRE( observer );
    observer->NotifyModule( "module" );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    const std::string expected =
        "<dependencies>"
        "    <dependency name='module'/>"
        "</dependencies>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
