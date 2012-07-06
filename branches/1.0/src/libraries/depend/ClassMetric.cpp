//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ClassMetric.h"
#include "ClassMetricVisitor_ABC.h"
#include <boost/foreach.hpp>
#include <stdexcept>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ClassMetric constructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
ClassMetric::ClassMetric( Subject< UnitObserver_ABC >& unitObserver, Subject< ClassObserver_ABC >& classObserver )
    : Observer< UnitObserver_ABC >( unitObserver )
    , Observer< ClassObserver_ABC > ( classObserver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassMetric destructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
ClassMetric::~ClassMetric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassMetric::Apply
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ClassMetric::Apply( ClassMetricVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const Metric& metric, metrics_ )
        visitor.NotifyClassMetric( metric.unit_, metric.classes_, metric.abstract_ );
}

// -----------------------------------------------------------------------------
// Name: ClassMetric::NotifyUnit
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ClassMetric::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    Metric metric;
    metric.unit_ = unit;
    metrics_.push_back( metric );
}

// -----------------------------------------------------------------------------
// Name: ClassMetric::NotifyClass
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ClassMetric::NotifyClass( const std::string& name, const std::string& /*context*/ )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid class '" + name + "' out of a module" );
    ++( metrics_.back().classes_ );
}

// -----------------------------------------------------------------------------
// Name: ClassMetric::NotifyAbstractness
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ClassMetric::NotifyAbstractness()
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid abstract class out of a module" );
    if( metrics_.back().classes_ == metrics_.back().abstract_ )
        throw std::runtime_error( "too much abstract classes" );
    ++( metrics_.back().abstract_ );
}
