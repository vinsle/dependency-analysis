//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ClassMetric.h"
#include "MockSubject.h"
#include "MockClassMetricVisitor.h"

using namespace depend;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : classObserver ( 0 )
            , moduleObserver( 0 )
        {
            MOCK_EXPECT( mockModuleVisitor.Register ).once().with( mock::retrieve( moduleObserver ) );
            MOCK_EXPECT( mockModuleVisitor.Unregister ).once();
            MOCK_EXPECT( mockClassVisitor.Register ).once().with( mock::retrieve( classObserver ) );
            MOCK_EXPECT( mockClassVisitor.Unregister ).once();
        }
        ClassObserver_ABC* classObserver;
        UnitObserver_ABC* moduleObserver;
        MockSubject< UnitObserver_ABC > mockModuleVisitor;
        MockSubject< ClassObserver_ABC > mockClassVisitor;
    };
    class MetricFixture : public Fixture
    {
    public:
        MetricFixture()
            : metric( mockModuleVisitor, mockClassVisitor )
        {
            BOOST_REQUIRE( classObserver );
            BOOST_REQUIRE( moduleObserver );
        }
        ClassMetric metric;
    };
}

BOOST_FIXTURE_TEST_CASE( class_metric_count_classes_and_abstract_classes_on_module, MetricFixture )
{
    moduleObserver->NotifyUnit( "module", "context" );
    classObserver->NotifyClass( "class", "context" );
    classObserver->NotifyAbstractness();
    MockClassMetricVisitor visitor;
    MOCK_EXPECT( visitor.NotifyClassMetric ).once().with( "module", 1u, 1u );
    metric.Apply( visitor );
}
