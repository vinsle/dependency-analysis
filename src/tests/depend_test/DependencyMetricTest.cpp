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
            MOCK_EXPECT( mockIncludeVisitor, Register ).once().with( mock::retrieve( includeObserver ) );
            MOCK_EXPECT( mockIncludeVisitor, Unregister ).once();
        }
        IncludeObserver_ABC* includeObserver;
        ModuleObserver_ABC* moduleObserver;
        MockSubject< ModuleObserver_ABC > mockModuleVisitor;
        MockSubject< IncludeObserver_ABC > mockIncludeVisitor;
    };
    class MetricFixture : public Fixture
    {
    public:
        MetricFixture()
            : metric( mockModuleVisitor, mockIncludeVisitor )
        {
            BOOST_REQUIRE( includeObserver );
            BOOST_REQUIRE( moduleObserver );
        }
        DependencyMetric metric;
    };
}

BOOST_FIXTURE_TEST_CASE( dependency_metric_computes_dependencies_on_module, MetricFixture )
{
    moduleObserver->NotifyModule( "module" );
    includeObserver->NotifyInternalInclude( "internal" );
    includeObserver->NotifyExternalInclude( "external" );
    MockDependencyMetricVisitor visitor;
    MOCK_EXPECT( visitor, NotifyDependencyMetric ).once().with( "module", boost::assign::list_of( "internal" ), boost::assign::list_of( "external" ) );
    metric.Apply( visitor );
}
