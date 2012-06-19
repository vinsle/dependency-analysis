//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/UnitCache.h"
#include "MockSubject.h"
#include "MockUnitObserver.h"

using namespace depend;

BOOST_AUTO_TEST_CASE( unit_cache_can_be_visited )
{
    MockSubject< UnitObserver_ABC > moduleSubject;
    UnitObserver_ABC* observer = 0;
    MOCK_EXPECT( moduleSubject.Register ).once().with( mock::retrieve( observer ) );
    MOCK_EXPECT( moduleSubject.Unregister ).once();
    const UnitCache cache( moduleSubject );
    BOOST_REQUIRE( observer );
    observer->NotifyUnit( "unit", "context" );
    MockUnitObserver unitObserver;
    MOCK_EXPECT( unitObserver.NotifyUnit ).once().with( "unit", "context" );
    cache.Apply( unitObserver );
}
