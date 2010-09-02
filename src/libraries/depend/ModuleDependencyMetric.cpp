//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ModuleDependencyMetric.h"
#include "DependencyMetricVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric constructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
ModuleDependencyMetric::ModuleDependencyMetric( Subject< ModuleObserver_ABC >& moduleObserver, Subject< FileObserver_ABC >& fileObserver,
                                    Subject< IncludeObserver_ABC >& includeObserver )
    : Observer< ModuleObserver_ABC > ( moduleObserver )
    , Observer< FileObserver_ABC >   ( fileObserver )
    , Observer< IncludeObserver_ABC >( includeObserver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric destructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
ModuleDependencyMetric::~ModuleDependencyMetric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::Apply
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::Apply( DependencyMetricVisitor_ABC& visitor ) const
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
                visitor.NotifyInternalDependency( metric.module_, module );
        }
        BOOST_FOREACH( const std::string& include, metric.external_ )
            visitor.NotifyExternalDependency( metric.module_, include.substr( 0, include.find_first_of( '/' ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyModule
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyModule( const std::string& module )
{
    Metric metric;
    metric.module_ = module;
    metrics_.push_back( metric );
    modules_.insert( module );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyFile
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyFile( const std::string& path, std::istream& /*stream*/ )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid file '" + path + "' out of a module" );
    metrics_.back().files_.insert( path );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyInternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyInternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a module" );
    metrics_.back().internal_.insert( file );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyExternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyExternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a module" );
    metrics_.back().external_.insert( file );
}
