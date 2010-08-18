//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ModuleVisitor.h"
#include "MockModuleObserver.h"

using namespace depend;

BOOST_AUTO_TEST_CASE( module_visitor_lists_all_first_level_directories_and_notifies_listeners )
{
    ModuleVisitor visitor;
    MockModuleObserver observer;
    visitor.Register( observer );
    MOCK_EXPECT( observer, NotifyModule ).once().with( "first" );
    MOCK_EXPECT( observer, NotifyModule ).once().with( "second" );
    visitor.Visit( BOOST_RESOLVE( "module_visitor_lists_all_first_level_directories_and_notifies_listeners" ) );
    visitor.Unregister( observer );
}
