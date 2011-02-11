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
#include "MockLog.h"
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
            : metric( mockUnitObserver, mockFileVisitor, mockIncludeVisitor, resolver, log )
        {
            BOOST_REQUIRE( unitObserver );
            BOOST_REQUIRE( fileObserver );
            BOOST_REQUIRE( includeObserver );
        }
        MockLog log;
        MockModuleResolver resolver;
        ModuleDependencyMetric metric;
    };
}

BOOST_FIXTURE_TEST_CASE( external_dependencies_are_notified, MetricFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    includeObserver->NotifyExternalInclude( "include1", "include context1" );
    includeObserver->NotifyExternalInclude( "include2", "include context2" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "include1" ).returns( "external" );
    MOCK_EXPECT( resolver, Resolve ).once().with( "include2" ).returns( "external2" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "external", "include context1" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "external2", "include context2" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( external_dependencies_are_uniq, MetricFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    includeObserver->NotifyExternalInclude( "include", "include context1" );
    includeObserver->NotifyExternalInclude( "include", "include context2" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "include" ).returns( "external" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "external", "include context1" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_begining_with_module_itself_does_not_count_as_dependency, MetricFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    includeObserver->NotifyInternalInclude( "module/internal", "include context" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "module/internal" ).returns( "" );
    MOCK_EXPECT( resolver, IsExcluded ).once().with( "module/internal" ).returns( false );
    MOCK_EXPECT( log, Warn ).once();
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_can_be_an_external_dependency, MetricFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    includeObserver->NotifyInternalInclude( "other/internal", "include context" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "other/internal" ).returns( "other" );
    MOCK_EXPECT( visitor, NotifyExternalDependency ).once().with( "module", "other", "include context" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( external_include_can_be_an_internal_dependency, MetricFixture )
{
    std::istringstream is;
    unitObserver->NotifyUnit( "module", "module context" );
    fileObserver->NotifyFile( "file", is, "file context1" );
    includeObserver->NotifyExternalInclude( "module2/file2", "include context" );
    unitObserver->NotifyUnit( "module2", "module context2" );
    fileObserver->NotifyFile( "file2", is, "file context2" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( resolver, Resolve ).once().with( "module2/file2" ).returns( "" );
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "module", "module2", "include context" );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_already_added_as_file_in_module_does_not_count_as_dependency, MetricFixture )
{
    unitObserver->NotifyUnit( "module", "module context" );
    std::istringstream is;
    fileObserver->NotifyFile( "file", is, "file context1" );
    includeObserver->NotifyInternalInclude( "file", "include context1" );
    includeObserver->NotifyInternalInclude( "file2", "include context2" );
    fileObserver->NotifyFile( "file2", is, "file context2" );
    MockDependencyMetricVisitor visitor;
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( dependency_metric_detects_module_dependencies_with_internal_include, MetricFixture )
{
    std::istringstream is;
    unitObserver->NotifyUnit( "module", "module context" );
    fileObserver->NotifyFile( "file", is, "file context1" );
    includeObserver->NotifyInternalInclude( "file", "include context1" );
    includeObserver->NotifyInternalInclude( "module2/file2", "include context2" );
    unitObserver->NotifyUnit( "module2", "module context2" );
    fileObserver->NotifyFile( "file2", is, "file context2" );
    includeObserver->NotifyInternalInclude( "file2", "include context3" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyInternalDependency ).once().with( "module", "module2", "include context2" );
    metric.Apply( visitor );
}
