//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "DependencyMetric.h"
#include "DependencyMetricVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: DependencyMetric constructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
DependencyMetric::DependencyMetric( Subject< ModuleObserver_ABC >& moduleObserver, Subject< IncludeObserver_ABC >& includeObserver )
    : moduleObserver_ ( moduleObserver )
    , includeObserver_( includeObserver )
{
    moduleObserver_.Register( *this );
    includeObserver_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric destructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
DependencyMetric::~DependencyMetric()
{
    includeObserver_.Unregister( *this );
    moduleObserver_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric::Apply
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::Apply( DependencyMetricVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const Metric& metric, metrics_ )
        visitor.NotifyDependencyMetric( metric.module_, metric.internal_, metric.external_ );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric::NotifyModule
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::NotifyModule( const std::string& module )
{
    Metric metric;
    metric.module_ = module;
    metrics_.push_back( metric );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric::NotifyInternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::NotifyInternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a module" );
    metrics_.back().internal_.push_back( file );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric::NotifyExternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::NotifyExternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a module" );
    metrics_.back().external_.push_back( file );
}
