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
            : lineObserver( 0 )
        {
            MOCK_EXPECT( mockVisitor.Register ).once().with( mock::retrieve( lineObserver ) );
            MOCK_EXPECT( mockVisitor.Unregister ).once();
        }
        UncommentedLineObserver_ABC* lineObserver;
        MockSubject< UncommentedLineObserver_ABC > mockVisitor;
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
    lineObserver->NotifyUncommentedLine( "", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_listeners_with_class_name, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context" );
    lineObserver->NotifyUncommentedLine( "class test", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_name_can_be_valid_c_identifier, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "_test_Name_3", "context" );
    lineObserver->NotifyUncommentedLine( "class _test_Name_3", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notifies_forward_declaration, ClassFixture )
{
    lineObserver->NotifyUncommentedLine( "class test;", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notifies_forward_declaration_with_namespace_definition, ClassFixture )
{
    lineObserver->NotifyUncommentedLine( "namespace space { class test; }", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_definition_on_same_line_is_not_a_forward_declaration, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context1" );
    lineObserver->NotifyUncommentedLine( "class test{};", "context1" );
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context2" );
    lineObserver->NotifyUncommentedLine( "class test:public test_abc{ virtual void method() { 1+1; } };", "context2" );
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context3" );
    lineObserver->NotifyUncommentedLine( "namespace space { class test{}; }", "context3" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_inheritance, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context" );
    lineObserver->NotifyUncommentedLine( "class test:public test_abc", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_templates, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context1" );
    lineObserver->NotifyUncommentedLine( "class< T > test", "context1" );
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context2" );
    lineObserver->NotifyUncommentedLine( "class< T, U > test", "context2" );
    MOCK_EXPECT( classObserver.NotifyClass ).once().with( "test", "context3" );
    lineObserver->NotifyUncommentedLine( "class< < T, U >, K > test", "context3" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_template_forward_declaration, ClassFixture )
{
    lineObserver->NotifyUncommentedLine( "template< typename Type > class test;", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_abstractness, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver.NotifyClass ).once().in( s );
    lineObserver->NotifyUncommentedLine( "class test", "context1" );
    MOCK_EXPECT( classObserver.NotifyAbstractness ).once().in( s );
    lineObserver->NotifyUncommentedLine( "void method() = 0", "context2" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_abstractness_if_statement, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once();
    lineObserver->NotifyUncommentedLine( "class test", "context1" );
    lineObserver->NotifyUncommentedLine( "a=0;", "context2" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_const_abstractness, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once();
    lineObserver->NotifyUncommentedLine( "class test", "context" );
    MOCK_EXPECT( classObserver.NotifyAbstractness ).once();
    lineObserver->NotifyUncommentedLine( "void method() const = 0", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_spaces, ClassFixture )
{
    MOCK_EXPECT( classObserver.NotifyClass ).once();
    lineObserver->NotifyUncommentedLine( "class test", "context" );
    MOCK_EXPECT( classObserver.NotifyAbstractness ).once();
    lineObserver->NotifyUncommentedLine( "void method() const=0", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_does_not_notify_abstractness_if_no_class, ClassFixture )
{
    lineObserver->NotifyUncommentedLine( "void method() = 0", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_notifies_abstractness_only_once, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver.NotifyClass ).once().in( s );
    lineObserver->NotifyUncommentedLine( "class test", "context" );
    MOCK_EXPECT( classObserver.NotifyAbstractness ).once().in( s );
    lineObserver->NotifyUncommentedLine( "void method() = 0", "context" );
    lineObserver->NotifyUncommentedLine( "void method() = 0", "context" );
}

BOOST_FIXTURE_TEST_CASE( class_visitor_handles_multiple_classes_in_the_same_file, ClassFixture )
{
    mock::sequence s;
    MOCK_EXPECT( classObserver.NotifyClass ).once().in( s );
    lineObserver->NotifyUncommentedLine( "class test", "context" );
    MOCK_EXPECT( classObserver.NotifyAbstractness ).once().in( s );
    lineObserver->NotifyUncommentedLine( "void method() = 0", "context" );
    MOCK_EXPECT( classObserver.NotifyClass ).once().in( s );
    lineObserver->NotifyUncommentedLine( "class test2", "context" );
    MOCK_EXPECT( classObserver.NotifyAbstractness ).once().in( s );
    lineObserver->NotifyUncommentedLine( "void method() = 0", "context" );
}