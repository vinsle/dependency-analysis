//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ModuleVisitor.h"
#include "MockUnitObserver.h"
#include "MockLog.h"

using namespace depend;


BOOST_AUTO_TEST_CASE( visiting_invalid_directory_throws )
{
    MockLog log;
    ModuleVisitor visitor( log );
    MOCK_EXPECT( log.Warn ).once();
    visitor.Visit( "invalid_directory", "" );
}

BOOST_AUTO_TEST_CASE( module_visitor_lists_all_first_level_directories_and_notifies_listeners )
{
    MockLog log;
    ModuleVisitor visitor( log );
    MockUnitObserver observer;
    visitor.Register( observer );
    MOCK_EXPECT( observer.NotifyUnit ).once().with( "first", "first" );
    MOCK_EXPECT( observer.NotifyUnit ).once().with( "second", "second" );
    visitor.Visit( BOOST_RESOLVE( "module_visitor_lists_all_first_level_directories_and_notifies_listeners" ), "" );
    visitor.Unregister( observer );
}
