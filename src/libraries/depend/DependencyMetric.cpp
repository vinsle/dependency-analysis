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
#include <boost/algorithm/string.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: DependencyMetric constructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
DependencyMetric::DependencyMetric( Subject< ModuleObserver_ABC >& moduleObserver, Subject< FileObserver_ABC >& fileObserver,
                                    Subject< IncludeObserver_ABC >& includeObserver )
    : moduleObserver_ ( moduleObserver )
    , fileObserver_   ( fileObserver )
    , includeObserver_( includeObserver )
{
    moduleObserver_.Register( *this );
    fileObserver_.Register( *this );
    includeObserver_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric destructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
DependencyMetric::~DependencyMetric()
{
    includeObserver_.Unregister( *this );
    fileObserver_.Unregister( *this );
    moduleObserver_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric::Apply
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::Apply( DependencyMetricVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const Metric& metric, metrics_ )
    {
        DependencyMetricVisitor_ABC::T_Dependencies cleaned;
        std::set_difference( metric.internal_.begin(), metric.internal_.end(), metric.files_.begin(), metric.files_.end(), std::back_insert_iterator< DependencyMetricVisitor_ABC::T_Dependencies >( cleaned ) );
        const DependencyMetricVisitor_ABC::T_Dependencies transformed( metric.external_.begin(), metric.external_.end() );
        visitor.NotifyDependencyMetric( metric.module_, cleaned, transformed );
    }
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
// Name: DependencyMetric::NotifyFile
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::NotifyFile( const std::string& path, std::istream& /*stream*/ )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid file '" + path + "' out of a module" );
    metrics_.back().files_.insert( path );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric::NotifyInternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::NotifyInternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a module" );
    if( !boost::algorithm::starts_with( file, metrics_.back().module_ ) )
        metrics_.back().internal_.insert( file );
}

// -----------------------------------------------------------------------------
// Name: DependencyMetric::NotifyExternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void DependencyMetric::NotifyExternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a module" );
    metrics_.back().external_.insert( file );
}
