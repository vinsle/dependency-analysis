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

BOOST_FIXTURE_TEST_CASE( include_visitor_notifies_listeners_only_with_include_lines, IncludeFixture )
{
    MockLineObserver includeObserver;
    visitor.Register( includeObserver );
    MOCK_EXPECT( includeObserver, Notify ).once().with( "#include \"test\"" );
    lineObserver->Notify( "#include \"test\"" );
    lineObserver->Notify( "include \"test\"" );
    lineObserver->Notify( "\"test\"" );
    visitor.Unregister( includeObserver );
}
