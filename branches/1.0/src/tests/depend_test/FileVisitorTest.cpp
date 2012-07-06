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
#include <xeumeuleu/xml.hpp>

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : xis    ( "<extensions>"
                       "    <extension>.h</extension>"
                       "    <extension>.hpp</extension>"
                       "</extensions>" )
            , visitor( xis )
        {
            // NOTHING
        }
        xml::xistringstream xis;
        FileVisitor visitor;
        MockFileObserver observer;
    };
}

BOOST_FIXTURE_TEST_CASE( file_visitor_lists_all_include_files_and_notifies_listeners, Fixture )
{
    visitor.Register( observer );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "header.h", mock::any, "context/header.h" );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "header.hpp", mock::any, "context/header.hpp" );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "module/module-header.h", mock::any, "context/module/module-header.h" );
    visitor.Visit( BOOST_RESOLVE( "file_visitor_lists_all_include_files_and_notifies_listeners" ), "context" );
    visitor.Unregister( observer );
}

BOOST_FIXTURE_TEST_CASE( file_visitor_is_not_sensible_to_end_slash, Fixture )
{
    visitor.Register( observer );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "header.h", mock::any, "context/header.h" );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "header.hpp", mock::any, "context/header.hpp" );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "module/module-header.h", mock::any, "context/module/module-header.h" );
    visitor.Visit( BOOST_RESOLVE( "file_visitor_lists_all_include_files_and_notifies_listeners/" ), "context" );
    visitor.Unregister( observer );
}

BOOST_AUTO_TEST_CASE( no_extension_makes_file_visitor_notify_every_file )
{
    xml::xistringstream xis( "<extensions/>" );
    FileVisitor visitor( xis );
    MockFileObserver observer;
    visitor.Register( observer );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "header.h", mock::any, "context/header.h" );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "header.hpp", mock::any, "context/header.hpp" );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "code.cpp", mock::any, "context/code.cpp" );
    MOCK_EXPECT( observer.NotifyFile ).once().with( "module/module-header.h", mock::any, "context/module/module-header.h" );
    visitor.Visit( BOOST_RESOLVE( "file_visitor_lists_all_include_files_and_notifies_listeners/" ), "context" );
    visitor.Unregister( observer );
}
