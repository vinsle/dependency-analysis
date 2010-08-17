//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/IncludeVisitor.h"
#include "MockLineObserver.h"
#include "MockLineVisitor.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
        {
            MOCK_EXPECT( mockVisitor, Register ).once().with( mock::retrieve( lineObserver ) );
            MOCK_EXPECT( mockVisitor, Unregister ).once();
        }
        LineObserver_ABC* lineObserver;
        MockLineVisitor mockVisitor;
    };
    class IncludeFixture : public Fixture
    {
    public:
        IncludeFixture()
            : visitor( mockVisitor )
        {
            BOOST_REQUIRE( lineObserver );
        }
        IncludeVisitor visitor;
    };
}

BOOST_FIXTURE_TEST_CASE( include_visitor_forwards_visit, IncludeFixture )
{
    std::istringstream ss;
    MOCK_EXPECT( mockVisitor, Visit ).once().with( mock::same( ss ) );
    visitor.Visit( ss );
}

namespace
{
    class RegularExpressionFixture : public IncludeFixture
    {
    public:
        RegularExpressionFixture()
        {
            visitor.Register( includeObserver );
        }
        virtual ~RegularExpressionFixture()
        {
            visitor.Unregister( includeObserver );
        }
        MockLineObserver includeObserver;
    };
}

BOOST_FIXTURE_TEST_CASE( include_visitor_does_not_notify_listeners_on_empty_line, RegularExpressionFixture )
{
    lineObserver->Notify( "" );
}

BOOST_FIXTURE_TEST_CASE( include_visitor_notifies_listeners_on_include_lines, RegularExpressionFixture )
{
    MOCK_EXPECT( includeObserver, Notify ).once().with( "\"test\"" );
    lineObserver->Notify( "#include \"test\"" );
}

BOOST_FIXTURE_TEST_CASE( regular_expression_handles_spaces, RegularExpressionFixture )
{
    MOCK_EXPECT( includeObserver, Notify ).once().with( "\"test\"" );
    lineObserver->Notify( "  #  include \t  \"test\"  " );
}

BOOST_FIXTURE_TEST_CASE( commented_line_is_not_an_include, RegularExpressionFixture )
{
    lineObserver->Notify( "//#include \"test\"" );
    lineObserver->Notify( "/*#include \"test\"" );
}
