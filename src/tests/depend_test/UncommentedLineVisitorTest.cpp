//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/UncommentedLineVisitor.h"
#include "MockLineObserver.h"
#include "MockUncommentedLineObserver.h"
#include "MockSubject.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
        {
            MOCK_EXPECT( mockVisitor.Register ).once().with( mock::retrieve( lineObserver ) );
            MOCK_EXPECT( mockVisitor.Unregister ).once();
        }
        LineObserver_ABC* lineObserver;
        MockSubject< LineObserver_ABC > mockVisitor;
    };
    class UncommentedLineFixture : public Fixture
    {
    public:
        UncommentedLineFixture()
            : visitor( mockVisitor )
        {
            BOOST_REQUIRE( lineObserver );
            visitor.Register( observer );
        }
        virtual ~UncommentedLineFixture()
        {
            visitor.Unregister( observer );
        }
        UncommentedLineVisitor visitor;
        MockUncommentedLineObserver observer;
    };
}

BOOST_FIXTURE_TEST_CASE( uncommented_line_is_forwarded, UncommentedLineFixture )
{
    MOCK_EXPECT( observer.NotifyUncommentedLine ).once().with( "test", "context" );
    lineObserver->NotifyLine( "test", "context" );
}

BOOST_FIXTURE_TEST_CASE( commented_line_is_not_forwarded, UncommentedLineFixture )
{
    lineObserver->NotifyLine( "// test", "context1" );
    lineObserver->NotifyLine( "/* test", "context2" );
}

BOOST_FIXTURE_TEST_CASE( partially_commented_line_is_forwarded_but_cleaned, UncommentedLineFixture )
{
    MOCK_EXPECT( observer.NotifyUncommentedLine ).once().with( "test ", "context1" );
    lineObserver->NotifyLine( "test // comment", "context1" );
    MOCK_EXPECT( observer.NotifyUncommentedLine ).once().with( "test", "context2" );
    lineObserver->NotifyLine( "test//", "context2" );
    MOCK_EXPECT( observer.NotifyUncommentedLine ).once().with( "test ", "context3" );
    lineObserver->NotifyLine( "test /* comment", "context3" );
}

// $$$$ _RC_ SLI 2010-08-19: add tests on nested comments removing