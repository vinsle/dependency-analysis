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
        std::vector< std::string > cleaned;
        std::set_difference( metric.internal_.begin(), metric.internal_.end(), metric.files_.begin(), metric.files_.end(), std::back_insert_iterator< std::vector< std::string > >( cleaned ) );
        BOOST_FOREACH( const std::string& include, cleaned )
        {
            const size_t position = include.find_first_of( '/' );
            const std::string module = include.substr( 0, position );
            if( position != std::string::npos && module != metric.module_ && modules_.find( module ) != modules_.end() )  // $$$$ _RC_ SLI 2010-08-20: warn user if one of these case occurs
                visitor.NotifyInternalDependency( metric.module_, module , include );
        }
        BOOST_FOREACH( const std::string& include, metric.external_ )
            visitor.NotifyExternalDependency( metric.module_, include.substr( 0, include.find_first_of( '/' ) ), include );
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
    modules_.insert( module );
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
