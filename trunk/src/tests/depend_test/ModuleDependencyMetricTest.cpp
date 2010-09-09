//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ModuleDependencyMetric.h"
#include "MockSubject.h"
#include "MockDependencyMetricVisitor.h"
#include "MockModuleResolver.h"
#include <boost/assign.hpp>

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : unitObserver ( 0 )
            , fileObserver   ( 0 )
            , includeObserver( 0 )
        {
            MOCK_EXPECT( mockUnitObserver, Register ).once().with( mock::retrieve( unitObserver ) );
            MOCK_EXPECT( mockUnitObserver, Unregister ).once();
            MOCK_EXPECT( mockFileVisitor, Register ).once().with( mock::retrieve( fileObserver ) );
            MOCK_EXPECT( mockFileVisitor, Unregister ).once();
            MOCK_EXPECT( mockIncludeVisitor, Register ).once().with( mock::retrieve( includeObserver ) );
            MOCK_EXPECT( mockIncludeVisitor, Unregister ).once();
        }
        UnitObserver_ABC* unitObserver;
        FileObserver_ABC* fileObserver;
        IncludeObserver_ABC* includeObserver;
        MockSubject< UnitObserver_ABC > mockUnitObserver;
        MockSubject< FileObserver_ABC > mockFileVisitor;
        MockSubject< IncludeObserver_ABC > mockIncludeVisitor;
    };
    class MetricFixture : public Fixture
    {
    public:
        MetricFixture()
            : metric( mockUnitObserver, mockFileVisitor, mockIncludeVisitor, resolver )
        {
            BOOST_REQUIRE( unitObserver );
            BOOST_REQUIRE( fileObserver );
            BOOST_REQUIRE( includeObserver );
        }
        MockModuleResolver resolver;
        ModuleDependencyMetric metric;
    };
}

BOOST_FIXTURE_TEST_CASE( external_dependencies_are_notified, MetricFixture )
{
    unitObserver->NotifyUnit( "module" );
    includeObserver->NotifyExternalInclude( "include1" );
    includeObserver->NotifyExternalInclude( "include2" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "include1" ).returns( "external" );
    MOCK_EXPECT( resolver, Resolve ).once().with( "include2" ).returns( "external2" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "external" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "external2" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( external_dependencies_are_uniq, MetricFixture )
{
    unitObserver->NotifyUnit( "module" );
    includeObserver->NotifyExternalInclude( "include" );
    includeObserver->NotifyExternalInclude( "include" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "include" ).returns( "external" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "external" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_begining_with_module_itself_does_not_count_as_dependency, MetricFixture )
{
    unitObserver->NotifyUnit( "module" );
    includeObserver->NotifyInternalInclude( "module/internal" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "module/internal" ).returns( "" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_can_be_an_external_dependency, MetricFixture )
{
    unitObserver->NotifyUnit( "module" );
    includeObserver->NotifyInternalInclude( "other/internal" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "other/internal" ).returns( "other" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "other" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( external_include_can_be_an_internal_dependency, MetricFixture )
{
    std::istringstream is;
    unitObserver->NotifyUnit( "module" );
    fileObserver->NotifyFile( "file", is );
    includeObserver->NotifyExternalInclude( "module2/file2" );
    unitObserver->NotifyUnit( "module2" );
    fileObserver->NotifyFile( "file2", is );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "module2/file2" ).returns( "" );
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "module", "module2" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_already_added_as_file_in_module_does_not_count_as_dependency, MetricFixture )
{
    unitObserver->NotifyUnit( "module" );
    std::istringstream is;
    fileObserver->NotifyFile( "file", is );
    includeObserver->NotifyInternalInclude( "file" );
    includeObserver->NotifyInternalInclude( "file2" );
    fileObserver->NotifyFile( "file2", is );
    MockDependencyMetricVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( dependency_metric_detects_module_dependencies_with_internal_include, MetricFixture )
{
    std::istringstream is;
    unitObserver->NotifyUnit( "module" );
    fileObserver->NotifyFile( "file", is );
    includeObserver->NotifyInternalInclude( "file" );
    includeObserver->NotifyInternalInclude( "module2/file2" );
    unitObserver->NotifyUnit( "module2" );
    fileObserver->NotifyFile( "file2", is );
    includeObserver->NotifyInternalInclude( "file2" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "module", "module2" );
    metric.Apply( visitor );
}
