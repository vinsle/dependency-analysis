//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ClassLoader.h"
#include "MockUnitObserver.h"
#include "MockClassObserver.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( class_loads_from_xml_and_notifies_listeners )
{
    ClassLoader loader;
    MockUnitObserver unitObserver;
    MockClassObserver classObserver;
    xml::xistringstream xis(
        "<graph>"
        "    <classes>"
        "        <node name='module1'>"
        "            <class abstract='true' context='context1' name='class1'/>"
        "            <class abstract='false' context='context2' name='class2'/>"
        "        </node>"
        "    </classes>"
        "</graph>" );
    loader.Subject< UnitObserver_ABC >::Register( unitObserver );
    loader.Subject< ClassObserver_ABC >::Register( classObserver );
    mock::sequence s;
    MOCK_EXPECT( unitObserver.NotifyUnit ).once().in( s ).with( "module1", "" );
    MOCK_EXPECT( classObserver.NotifyClass ).once().in( s ).with( "class1", "context1" );
    MOCK_EXPECT( classObserver.NotifyAbstractness ).once().in( s );
    MOCK_EXPECT( classObserver.NotifyClass ).once().in( s ).with( "class2", "context2" );
    loader.Process( xis );
}
