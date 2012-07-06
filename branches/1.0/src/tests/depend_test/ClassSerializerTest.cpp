//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ClassSerializer.h"
#include "MockSubject.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( classes_serialization )
{
    MockSubject< UnitObserver_ABC > unitSubject;
    UnitObserver_ABC* unitObserver = 0;
    MOCK_EXPECT( unitSubject.Register ).once().with( mock::retrieve( unitObserver ) );
    MOCK_EXPECT( unitSubject.Unregister ).once();
    MockSubject< ClassObserver_ABC > classSubject;
    ClassObserver_ABC* classObserver = 0;
    MOCK_EXPECT( classSubject.Register ).once().with( mock::retrieve( classObserver ) );
    MOCK_EXPECT( classSubject.Unregister ).once();
    ClassSerializer serializer( unitSubject, classSubject );
    BOOST_REQUIRE( unitObserver );
    BOOST_REQUIRE( classObserver );
    unitObserver->NotifyUnit( "module1", "context" );
    classObserver->NotifyClass( "class1", "context1" );
    classObserver->NotifyAbstractness();
    classObserver->NotifyClass( "class2", "context2" );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    const std::string expected =
        "<classes>"
        "    <node name='module1'>"
        "        <class abstract='true' context='context1' name='class1'/>"
        "        <class abstract='false' context='context2' name='class2'/>"
        "    </node>"
        "</classes>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
