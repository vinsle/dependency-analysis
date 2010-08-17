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
#include <boost/assign.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( file_visitor_lists_all_include_files_and_notifies_listeners )
{
    const std::vector< std::string > extensions = boost::assign::list_of( ".h" )( ".hpp" );
    FileVisitor visitor( extensions );
    MockFileObserver observer;
    visitor.Register( observer );
    MOCK_EXPECT( observer, Notify ).once().with( "header.h" );
    MOCK_EXPECT( observer, Notify ).once().with( "header.hpp" );
    MOCK_EXPECT( observer, Notify ).once().with( "module/module-header.h" );
    visitor.Visit( BOOST_RESOLVE( "file_visitor_lists_all_include_files_and_notifies_listeners/" ) );
    visitor.Unregister( observer );
}
