//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ModuleDependencyGuard.h"
#include "MockDependencyMetric.h"
#include "MockDependencyGuardVisitor.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;

BOOST_AUTO_TEST_CASE( dependency_guard_notifies_all_unknown_dependencies_only_once )
{
    xml::xistringstream xis(
        "<dependencies>"
        "    <module name='from'>"
        "        <dependency>to</dependency>"
        "    </module>"
        "</dependencies>" );
    MockDependencyMetric metric;
    DependencyMetricVisitor_ABC* pVisitor = 0;
    MOCK_EXPECT( metric, Apply ).once().with( mock::retrieve( pVisitor ) );
    ModuleDependencyGuard guard( xis, metric );
    BOOST_REQUIRE( pVisitor );
    pVisitor->NotifyInternalDependency( "from", "to" );
    pVisitor->NotifyInternalDependency( "from", "unknown1" );
    pVisitor->NotifyInternalDependency( "from", "unknown2" );
    pVisitor->NotifyInternalDependency( "from", "unknown1" );
    MockDependencyGuardVisitor visitor;
    mock::sequence s;
    MOCK_EXPECT( visitor, NotifyDependencyFailure ).once().in( s ).with( "from", "unknown1" );
    MOCK_EXPECT( visitor, NotifyDependencyFailure ).once().in( s ).with( "from", "unknown2" );
    guard.Apply( visitor );
}

BOOST_AUTO_TEST_CASE( dependency_guard_notifies_all_unchecked_declared_dependencies )
{
    xml::xistringstream xis(
        "<dependencies>"
        "    <module name='from'>"
        "        <dependency>to</dependency>"
        "    </module>"
        "</dependencies>" );
    MockDependencyMetric metric;
    DependencyMetricVisitor_ABC* pVisitor = 0;
    MOCK_EXPECT( metric, Apply ).once().with( mock::retrieve( pVisitor ) );
    ModuleDependencyGuard guard( xis, metric );
    BOOST_REQUIRE( pVisitor );
    MockDependencyGuardVisitor visitor;
    MOCK_EXPECT( visitor, NotifyUncheckedDependency ).once().with( "from", "to" );
    guard.Apply( visitor );
}

BOOST_AUTO_TEST_CASE( dependency_guard_notifies_all_checked_obsolete_dependencies )
{
    xml::xistringstream xis(
        "<dependencies>"
        "    <module name='from'>"
        "        <obsolete-dependency>checked</obsolete-dependency>"
        "        <obsolete-dependency>unchecked</obsolete-dependency>"
        "    </module>"
        "</dependencies>" );
    MockDependencyMetric metric;
    DependencyMetricVisitor_ABC* pVisitor = 0;
    MOCK_EXPECT( metric, Apply ).once().with( mock::retrieve( pVisitor ) );
    ModuleDependencyGuard guard( xis, metric );
    BOOST_REQUIRE( pVisitor );
    pVisitor->NotifyInternalDependency( "from", "checked" );
    MockDependencyGuardVisitor visitor;
    MOCK_EXPECT( visitor, NotifyObsoleteDependency ).once().with( "from", "checked" );
    MOCK_EXPECT( visitor, NotifyUncheckedDependency ).once().with( "from", "unchecked" );
    guard.Apply( visitor );
}
