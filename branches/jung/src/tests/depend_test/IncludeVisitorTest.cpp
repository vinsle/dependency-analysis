//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/IncludeVisitor.h"
#include "MockIncludeObserver.h"
#include "MockSubject.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : lineObserver( 0 )
        {
            MOCK_EXPECT( mockVisitor, Register ).once().with( mock::retrieve( lineObserver ) );
            MOCK_EXPECT( mockVisitor, Unregister ).once();
        }
        UncommentedLineObserver_ABC* lineObserver;
        MockSubject< UncommentedLineObserver_ABC > mockVisitor;
    };
    class IncludeFixture : public Fixture
    {
    public:
        IncludeFixture()
            : visitor( mockVisitor )
        {
            BOOST_REQUIRE( lineObserver );
            visitor.Register( includeObserver );
        }
        virtual ~IncludeFixture()
        {
            visitor.Unregister( includeObserver );
        }
        IncludeVisitor visitor;
        MockIncludeObserver includeObserver;
    };
}

BOOST_FIXTURE_TEST_CASE( include_visitor_does_not_notify_listeners_on_empty_line, IncludeFixture )
{
    lineObserver->NotifyUncommentedLine( "" );
}

BOOST_FIXTURE_TEST_CASE( include_visitor_notifies_listeners_with_filename, IncludeFixture )
{
    MOCK_EXPECT( includeObserver, NotifyInternalInclude ).once().with( "test" );
    lineObserver->NotifyUncommentedLine( "#include \"test\"" );
}

BOOST_FIXTURE_TEST_CASE( include_visitor_notifies_with_external_include, IncludeFixture )
{
    MOCK_EXPECT( includeObserver, NotifyExternalInclude ).once().with( "test" );
    lineObserver->NotifyUncommentedLine( "#include <test>" );
}

BOOST_FIXTURE_TEST_CASE( regular_expression_handles_spaces, IncludeFixture )
{
    MOCK_EXPECT( includeObserver, NotifyInternalInclude ).once().with( "test" );
    lineObserver->NotifyUncommentedLine( "  #  include \t  \"test\"  " );
}
