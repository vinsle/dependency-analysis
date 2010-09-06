//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/UnitSerializer.h"
#include "MockSubject.h"
#include "MockFilter.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( simple_unit_serialization )
{
    MockSubject< UnitObserver_ABC > mockSubject;
    UnitObserver_ABC* observer = 0;
    MOCK_EXPECT( mockSubject, Register ).once().with( mock::retrieve( observer ) );
    MOCK_EXPECT( mockSubject, Unregister ).once();
    UnitSerializer serializer( mockSubject );
    BOOST_REQUIRE( observer );
    observer->NotifyUnit( "unit" );
    xml::xostringstream xos;
    MockFilter filter;
    MOCK_EXPECT( filter, Check ).returns( true );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<units>"
        "    <unit>unit</unit>"
        "</units>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

BOOST_AUTO_TEST_CASE( unit_serialization_can_be_filtered )
{
    MockSubject< UnitObserver_ABC > mockSubject;
    UnitObserver_ABC* observer = 0;
    MOCK_EXPECT( mockSubject, Register ).once().with( mock::retrieve( observer ) );
    MOCK_EXPECT( mockSubject, Unregister ).once();
    UnitSerializer serializer( mockSubject );
    BOOST_REQUIRE( observer );
    observer->NotifyUnit( "unit1" );
    observer->NotifyUnit( "unit2" );
    xml::xostringstream xos;
    MockFilter filter;
    MOCK_EXPECT( filter, Check ).once().with( "unit1" ).returns( true );
    MOCK_EXPECT( filter, Check ).returns( false );
    serializer.Serialize( xos, filter );
    const std::string expected =
        "<units>"
        "    <unit>unit1</unit>"
        "</units>";
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
