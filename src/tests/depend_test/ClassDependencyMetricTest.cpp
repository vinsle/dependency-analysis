//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ClassDependencyMetric.h"
#include "MockSubject.h"
#include "MockDependencyMetricVisitor.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : fileObserver   ( 0 )
            , classObserver  ( 0 )
            , includeObserver( 0 )
        {
            MOCK_EXPECT( fileVisitor, Register ).once().with( mock::retrieve( fileObserver ) );
            MOCK_EXPECT( classVisitor, Register ).once().with( mock::retrieve( classObserver ) );
            MOCK_EXPECT( includeVisitor, Register ).once().with( mock::retrieve( includeObserver ) );
            MOCK_EXPECT( fileVisitor, Unregister );
            MOCK_EXPECT( classVisitor, Unregister );
            MOCK_EXPECT( includeVisitor, Unregister );
        }
        MockSubject< FileObserver_ABC > fileVisitor;
        MockSubject< ClassObserver_ABC > classVisitor;
        MockSubject< IncludeObserver_ABC > includeVisitor;
        FileObserver_ABC* fileObserver;
        ClassObserver_ABC* classObserver;
        IncludeObserver_ABC* includeObserver;
        std::stringstream ss;
    };
    class DependencyFixture : public Fixture
    {
    public:
        DependencyFixture()
            : metric( fileVisitor, classVisitor, includeVisitor )
        {
            BOOST_REQUIRE( fileObserver );
            BOOST_REQUIRE( classObserver );
            BOOST_REQUIRE( includeObserver );
        }
        ClassDependencyMetric metric;
    };
}

BOOST_FIXTURE_TEST_CASE( empty_metric_does_nothing, DependencyFixture )
{
    MockDependencyMetricVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( empty_file_does_nothing, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.h", ss );
    MockDependencyMetricVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( empty_class_file_does_nothing, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.h", ss );
    classObserver->NotifyClass( "TestClass" );
    MockDependencyMetricVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( includes_in_a_class_are_notifiyed, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.h", ss );
    classObserver->NotifyClass( "TestClass" );
    includeObserver->NotifyInternalInclude( "TestInclude" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude", "TestInclude" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( includes_are_discared_if_there_is_no_class_definition, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.h", ss );
    includeObserver->NotifyInternalInclude( "TestInclude" );
    MockDependencyMetricVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( includes_are_merged_between_all_files_referencing_same_class, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.cpp", ss );
    includeObserver->NotifyInternalInclude( "TestInclude1" );
    fileObserver->NotifyFile( "TestClass.h", ss );
    includeObserver->NotifyInternalInclude( "TestInclude2" );
    classObserver->NotifyClass( "TestClass" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude1", "TestInclude1" );
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude2", "TestInclude2" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( module_file_and_class_are_same, DependencyFixture )
{
    fileObserver->NotifyFile( "module/TestClass.h", ss );
    includeObserver->NotifyInternalInclude( "TestInclude" );
    classObserver->NotifyClass( "TestClass" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude", "TestInclude" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( includes_are_cleaned_for_notificatoin, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.h", ss );
    includeObserver->NotifyInternalInclude( "module/TestInclude.h" );
    classObserver->NotifyClass( "TestClass" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude", "TestInclude" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( self_inclusion_is_not_notified, DependencyFixture )
{
    fileObserver->NotifyFile( "TestClass.cpp", ss );
    includeObserver->NotifyInternalInclude( "TestClass.h" );
    fileObserver->NotifyFile( "TestClass.h", ss );
    includeObserver->NotifyInternalInclude( "TestInclude" );
    classObserver->NotifyClass( "TestClass" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "TestClass", "TestInclude", "TestInclude" );
    metric.Apply( visitor );
}
