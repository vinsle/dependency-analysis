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

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ClassMetric constructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
ClassMetric::ClassMetric( Subject< ModuleObserver_ABC >& moduleObserver, Subject< ClassObserver_ABC >& classObserver )
    : moduleObserver_( moduleObserver )
    , classObserver_ ( classObserver )
{
    moduleObserver_.Register( *this );
    classObserver_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ClassMetric destructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
ClassMetric::~ClassMetric()
{
    classObserver_.Unregister( *this );
    moduleObserver_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ClassMetric::Apply
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ClassMetric::Apply( ClassMetricVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const Metric& metric, metrics_ )
        visitor.NotifyClassMetric( metric.module_, metric.classes_, metric.abstract_ );
}

// -----------------------------------------------------------------------------
// Name: ClassMetric::NotifyModule
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ClassMetric::NotifyModule( const std::string& module )
{
    Metric metric;
    metric.module_ = module;
    metrics_.push_back( metric );
}

// -----------------------------------------------------------------------------
// Name: ClassMetric::NotifyClass
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ClassMetric::NotifyClass( const std::string& name )
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
