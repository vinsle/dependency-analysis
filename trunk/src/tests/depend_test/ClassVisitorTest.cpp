//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ClassVisitor.h"
#include "MockSubject.h"
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
        MockSubject< LineObserver_ABC > mockVisitor;
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
    lineObserver->NotifyLine( "" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_listeners_with_class_name, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->NotifyLine( "class test" );
}

BOOST_FIXTURE_TEST_CASE( class_name_can_be_valid_c_identifier, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "_test_Name_3" );
    lineObserver->NotifyLine( "class _test_Name_3" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_also_notifies_listeners_with_struct, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->NotifyLine( "struct test" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notifies_forward_declaration, ClassFixture )
{
    lineObserver->NotifyLine( "class test;" );
}

BOOST_FIXTURE_TEST_CASE( class_definition_on_same_line_is_not_a_forward_declaration, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->NotifyLine( "class test{};" );
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->NotifyLine( "class test:public test_abc{ virtual void method() { 1+1; } };" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_with_commented_class, ClassFixture )
{
    lineObserver->NotifyLine( " // class test" );
    lineObserver->NotifyLine( "/*class test" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_inheritance, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once().with( "test" );
    lineObserver->NotifyLine( "class test:public test_abc" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_abstractness, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->NotifyLine( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->NotifyLine( "void method() = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_abstractness_if_statement, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once();
    lineObserver->NotifyLine( "class test" );
    lineObserver->NotifyLine( "a=0;" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_const_abstractness, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once();
    lineObserver->NotifyLine( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once();
    lineObserver->NotifyLine( "void method() const = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_spaces, ClassFixture )
{
    MOCK_EXPECT( classObserver, NotifyClass ).once();
    lineObserver->NotifyLine( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once();
    lineObserver->NotifyLine( "void method() const=0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_abstractness_if_no_class, ClassFixture )
{
    lineObserver->NotifyLine( "void method() = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_abstractness_only_once, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->NotifyLine( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->NotifyLine( "void method() = 0" );
    lineObserver->NotifyLine( "void method() = 0" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_multiple_classes_in_the_same_file, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->NotifyLine( "class test" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->NotifyLine( "void method() = 0" );
    MOCK_EXPECT( classObserver, NotifyClass ).once().in( s );
    lineObserver->NotifyLine( "class test2" );
    MOCK_EXPECT( classObserver, NotifyAbstractness ).once().in( s );
    lineObserver->NotifyLine( "void method() = 0" );
}