//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/LineVisitor.h"
#include "MockLineObserver.h"

using namespace depend;

BOOST_AUTO_TEST_CASE( line_visitor_notifies_listener_with_every_line_in_a_stream )
{
    std::stringstream ss( "first line\nsecond line" );
    LineVisitor visitor;
    MockLineObserver observer;
    visitor.Register( observer );
    mock::sequence s;
    MOCK_EXPECT( observer, Notify ).once().in( s ).with( "first line" );
    MOCK_EXPECT( observer, Notify ).once().in( s ).with( "second line" );
    visitor.Visit( ss );
    visitor.Unregister( observer );
}