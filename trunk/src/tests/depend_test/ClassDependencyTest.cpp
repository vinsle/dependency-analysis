//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ClassDependency.h"
#include "MockSubject.h"
#include "MockDependencyVisitor.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : fileObserver   ( 0 )
            , includeObserver( 0 )
        {
            MOCK_EXPECT( fileVisitor, Register ).once().with( mock::retrieve( fileObserver ) );
            MOCK_EXPECT( includeVisitor, Register ).once().with( mock::retrieve( includeObserver ) );
            MOCK_EXPECT( fileVisitor, Unregister );
            MOCK_EXPECT( includeVisitor, Unregister );
        }
        MockSubject< FileObserver_ABC > fileVisitor;
        MockSubject< IncludeObserver_ABC > includeVisitor;
        FileObserver_ABC* fileObserver;
        IncludeObserver_ABC* includeObserver;
        std::stringstream ss;
    };
    class DependencyFixture : public Fixture
    {
    public:
        DependencyFixture()
            : metric( fileVisitor, includeVisitor )
        {
            BOOST_REQUIRE( fileObserver );
            BOOST_REQUIRE( includeObserver );
        }
        ClassDependency metric;
    };
}

BOOST_FIXTURE_TEST_CASE( empty_metric_does_nothing, DependencyFixture )
{
    MockDependencyVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( empty_file_does_nothing, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.h", ss, "file context" );
    MockDependencyVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( includes_in_a_unit_are_notifiyed_as_external_dependency, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.h", ss, "file context" );
    includeObserver->NotifyInternalInclude( "TestInclude", "include context" );
    MockDependencyVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude", "include context" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( includes_are_merged_between_all_files_referencing_same_class, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.cpp", ss, "file context1" );
    includeObserver->NotifyInternalInclude( "TestInclude1", "include context1" );
    fileObserver->NotifyFile( "TestClass.h", ss, "file context2" );
    includeObserver->NotifyInternalInclude( "TestInclude2", "include context2" );
    MockDependencyVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude1", "include context1" );
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude2", "include context2" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( sub_directory_is_part_of_the_unit_name, DependencyFixture )
{
    fileObserver->NotifyFile( "directory/TestClass.h", ss, "file context" );
    includeObserver->NotifyInternalInclude( "TestInclude", "include context" );
    MockDependencyVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "directory/TestClass", "TestInclude", "include context" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( self_inclusion_is_not_notified, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.cpp", ss, "file context1" );
    includeObserver->NotifyInternalInclude( "TestClass.h", "include context1" );
    fileObserver->NotifyFile( "TestClass.h", ss, "file context2" );
    includeObserver->NotifyInternalInclude( "TestInclude", "include context2" );
    MockDependencyVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude", "include context2" );
    metric.Apply( visitor );
}
