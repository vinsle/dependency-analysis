//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/UnitMetrics.h"
#include "MockVisitable.h"
#include "MockMetricsVisitor.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : unitObserver     ( 0 )
            , classVisitor     ( 0 )
            , dependencyVisitor( 0 )
        {
            MOCK_EXPECT( units, Apply ).once().with( mock::retrieve( unitObserver ) );
            MOCK_EXPECT( classMetric, Apply ).once().with( mock::retrieve( classVisitor ) );
            MOCK_EXPECT( dependencyMetric, Apply ).once().with( mock::retrieve( dependencyVisitor ) );
        }
        MockVisitable< UnitObserver_ABC > units;
        MockVisitable< ClassMetricVisitor_ABC > classMetric;
        MockVisitable< DependencyVisitor_ABC > dependencyMetric;
        UnitObserver_ABC* unitObserver;
        ClassMetricVisitor_ABC* classVisitor;
        DependencyVisitor_ABC* dependencyVisitor;
    };
    class MetricsFixture : public Fixture
    {
    public:
        MetricsFixture()
            : metrics( units, dependencyMetric, classMetric )
        {
            BOOST_REQUIRE( unitObserver );
            BOOST_REQUIRE( classVisitor );
            BOOST_REQUIRE( dependencyVisitor );
        }
        UnitMetrics metrics;
    };
}

BOOST_FIXTURE_TEST_CASE( metrics_are_null_by_default, MetricsFixture )
{
    unitObserver->NotifyUnit( "module", "context" );
    MockMetricsVisitor visitor;
    MOCK_EXPECT( visitor, NotifyMetrics ).once().with( "module", 0u, 0u, 0u, 0u, 0u, 0u, 0u, 100u );
    metrics.Apply( visitor );
}

BOOST_FIXTURE_TEST_CASE( metrics_are_computed, MetricsFixture )
{
    classVisitor->NotifyClassMetric( "module1", 4u, 2u );
    classVisitor->NotifyClassMetric( "module2", 4u, 2u );
    dependencyVisitor->NotifyInternalDependency( "module1", "module2", "context" );
    dependencyVisitor->NotifyInternalDependency( "module2", "module1", "context" );
    dependencyVisitor->NotifyExternalDependency( "module1", "boost", "context" );
    MockMetricsVisitor visitor;
    MOCK_EXPECT( visitor, NotifyMetrics ).once().with( "module1", 1u, 1u, 1u, 4u, 2u, 50u, 50u, 0u );
    MOCK_EXPECT( visitor, NotifyMetrics ).once().with( "module2", 1u, 1u, 0u, 4u, 2u, 50u, 50u, 0u );
    metrics.Apply( visitor );
}
