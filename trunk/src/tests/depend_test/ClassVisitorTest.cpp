//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ClassVisitor.h"
#include "MockLineVisitor.h"
#include "MockClassObserver.h"

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
    class ClassFixture : public Fixture
    {
    public:
        ClassFixture()
            : visitor( mockVisitor )
        {
            BOOST_REQUIRE( lineObserver );
            visitor.Register( classObserver );
        }
        virtual ~ClassFixture()
        {
            visitor.Unregister( classObserver );
        }
        ClassVisitor visitor;
        MockClassObserver classObserver;
    };
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_listeners_on_empty_line, ClassFixture )
{
    lineObserver->Notify( "" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_listeners_with_class_name, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->Notify( "class test" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_also_notifies_listeners_with_struct, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->Notify( "struct test" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_with_commented_class, ClassFixture )
{
    lineObserver->Notify( " // class test" );
    lineObserver->Notify( "/*class test" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_inheritance, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->Notify( "class test:public test_abc" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_abstractness, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->Notify( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->Notify( "void method() = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_abstractness_if_statement, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once();
    lineObserver->Notify( "class test" );
    lineObserver->Notify( "a=0;" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_const_abstractness, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once();
    lineObserver->Notify( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once();
    lineObserver->Notify( "void method() const = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_spaces, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once();
    lineObserver->Notify( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once();
    lineObserver->Notify( "void method() const=0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_abstractness_if_no_class, ClassFixture )
{
    lineObserver->Notify( "void method() = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_abstractness_only_once, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->Notify( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->Notify( "void method() = 0" );
    lineObserver->Notify( "void method() = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_multiple_classes_in_the_same_file, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->Notify( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->Notify( "void method() = 0" );
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->Notify( "class test2" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->Notify( "void method() = 0" );
}