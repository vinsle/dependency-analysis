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
#include "ModuleResolver_ABC.h"
#include "Log_ABC.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric constructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
ModuleDependencyMetric::ModuleDependencyMetric( Subject< UnitObserver_ABC >& unitObserver, Subject< FileObserver_ABC >& fileObserver,
                                                Subject< IncludeObserver_ABC >& includeObserver, const ModuleResolver_ABC& resolver,
                                                const Log_ABC& log )
    : Observer< UnitObserver_ABC >   ( unitObserver )
    , Observer< FileObserver_ABC >   ( fileObserver )
    , Observer< IncludeObserver_ABC >( includeObserver )
    , resolver_( resolver )
    , log_     ( log )
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

namespace
{
    template< typename T>
    bool Notify( boost::function< T > notify, const std::string& currentUnit, const std::string& unit )
    {
        if( !unit.empty() && unit != currentUnit )
        {
            notify( currentUnit, unit );
            return true;
        }
        return false;
    }
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
        boost::function< void( const std::string&, const std::string& ) > NotifyInternal = boost::bind( &DependencyMetricVisitor_ABC::NotifyInternalDependency, &visitor, _1, _2 );
        boost::function< void( const std::string&, const std::string& ) > NotifyExternal = boost::bind( &DependencyMetricVisitor_ABC::NotifyExternalDependency, &visitor, _1, _2 );
        BOOST_FOREACH( const std::string& include, cleaned )
            if( !Notify( NotifyInternal, metric.unit_, Resolve( include ) ) )
                if( !Notify( NotifyExternal, metric.unit_, resolver_.Resolve( include ) ) )
                    if( !resolver_.IsExcluded( include ) )
                        log_.Warn( "Warning: include \"" + include + "\" in unit '" + metric.unit_ + "' cannot be resolved" );
        BOOST_FOREACH( const std::string& include, metric.external_ )
            if( !Notify( NotifyExternal, metric.unit_, resolver_.Resolve( include ) ) )
                if( !Notify( NotifyInternal, metric.unit_, Resolve( include ) ) )
                    if( !resolver_.IsExcluded( include ) )
                        log_.Warn( "Warning: include <" + include + "> in unit '" + metric.unit_ + "' cannot be resolved" );
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::Resolve
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
std::string ModuleDependencyMetric::Resolve( const std::string& include ) const
{
    const size_t position = include.find_first_of( '/' );
    const std::string unit = include.substr( 0, position );
    if( position == std::string::npos || units_.find( unit ) == units_.end() )
        return "";
    return unit;
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyUnit
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyUnit( const std::string& unit )
{
    Metric metric;
    metric.unit_ = unit;
    metrics_.push_back( metric );
    units_.insert( unit );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyFile
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyFile( const std::string& path, std::istream& /*stream*/ )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid file '" + path + "' out of a unit" );
    metrics_.back().files_.insert( path );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyInternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyInternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a unit" );
    metrics_.back().internal_.insert( file );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyExternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyExternalInclude( const std::string& file )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a unit" );
    metrics_.back().external_.insert( file );
}
