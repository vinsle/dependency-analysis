//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/FileVisitor.h"
#include "MockFileObserver.h"

using namespace depend;

BOOST_AUTO_TEST_CASE( file_subject_lists_all_include_files_and_notify_listeners )
{
    FileVisitor visitor;
    MockFileObserver observer;
    visitor.Register( observer );
    MOCK_EXPECT( observer, Notify ).once().with( "header.h" );
    MOCK_EXPECT( observer, Notify ).once().with( "module/module-header.h" );
    visitor.Visit( BOOST_RESOLVE( "file_subject_lists_all_include_files_and_notify_listeners/" ) );
    visitor.Unregister( observer );
}
