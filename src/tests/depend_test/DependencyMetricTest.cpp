//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/DependencyMetric.h"
#include "MockSubject.h"
#include "MockDependencyMetricVisitor.h"
#include <boost/assign.hpp>

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
        {
            MOCK_EXPECT( mockModuleVisitor, Register ).once().with( mock::retrieve( moduleObserver ) );
            MOCK_EXPECT( mockModuleVisitor, Unregister ).once();
            MOCK_EXPECT( mockFileVisitor, Register ).once().with( mock::retrieve( fileObserver ) );
            MOCK_EXPECT( mockFileVisitor, Unregister ).once();
            MOCK_EXPECT( mockIncludeVisitor, Register ).once().with( mock::retrieve( includeObserver ) );
            MOCK_EXPECT( mockIncludeVisitor, Unregister ).once();
        }
        ModuleObserver_ABC* moduleObserver;
        FileObserver_ABC* fileObserver;
        IncludeObserver_ABC* includeObserver;
        MockSubject< ModuleObserver_ABC > mockModuleVisitor;
        MockSubject< FileObserver_ABC > mockFileVisitor;
        MockSubject< IncludeObserver_ABC > mockIncludeVisitor;
    };
    class MetricFixture : public Fixture
    {
    public:
        MetricFixture()
            : metric( mockModuleVisitor, mockFileVisitor, mockIncludeVisitor )
        {
            BOOST_REQUIRE( moduleObserver );
            BOOST_REQUIRE( fileObserver );
            BOOST_REQUIRE( includeObserver );
        }
        DependencyMetric metric;
        const std::vector< std::string > empty;
    };
}

BOOST_FIXTURE_TEST_CASE( external_dependencies_are_always_notified, MetricFixture )
{
    moduleObserver->NotifyModule( "module" );
    includeObserver->NotifyExternalInclude( "include1" );
    includeObserver->NotifyExternalInclude( "include2" );
    MockDependencyMetricVisitor visitor;
    const std::vector< std::string > expected = boost::assign::list_of( "include1" )( "include2" );
    MOCK_EXPECT( visitor, NotifyDependencyMetric ).once().with( "module", empty, expected );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( external_dependencies_are_uniq, MetricFixture )
{
    moduleObserver->NotifyModule( "module" );
    includeObserver->NotifyExternalInclude( "include1" );
    includeObserver->NotifyExternalInclude( "include1" );
    MockDependencyMetricVisitor visitor;
    const std::vector< std::string > expected = boost::assign::list_of( "include1" );
    MOCK_EXPECT( visitor, NotifyDependencyMetric ).once().with( "module", empty, expected );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_begining_with_module_itself_does_not_count_as_dependency, MetricFixture )
{
    moduleObserver->NotifyModule( "module" );
    includeObserver->NotifyInternalInclude( "module/internal" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyDependencyMetric ).once().with( "module", empty, empty );
    metric.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( internal_include_already_added_as_file_in_module_does_not_count_as_dependency, MetricFixture )
{
    moduleObserver->NotifyModule( "module" );
    std::istringstream is;
    fileObserver->NotifyFile( "file", is );
    includeObserver->NotifyInternalInclude( "file" );
    includeObserver->NotifyInternalInclude( "file2" );
    fileObserver->NotifyFile( "file2", is );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyDependencyMetric ).once().with( "module", empty, empty );
    metric.Apply( visitor );
}