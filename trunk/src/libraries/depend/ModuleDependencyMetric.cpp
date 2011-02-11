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
    bool Notify( boost::function< T > notify, const std::string& currentUnit, const std::string& unit, const std::string& context )
    {
        if( !unit.empty() && unit != currentUnit )
        {
            notify( currentUnit, unit, context );
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
        std::vector< T_Dependency > cleaned;
        std::set_difference( metric.internal_.begin(), metric.internal_.end(), metric.files_.begin(), metric.files_.end(), std::back_insert_iterator< std::vector< T_Dependency > >( cleaned ) );
        boost::function< void( const std::string&, const std::string&, const std::string& ) > NotifyInternal = boost::bind( &DependencyMetricVisitor_ABC::NotifyInternalDependency, &visitor, _1, _2, _3 );
        boost::function< void( const std::string&, const std::string&, const std::string& ) > NotifyExternal = boost::bind( &DependencyMetricVisitor_ABC::NotifyExternalDependency, &visitor, _1, _2, _3 );
        BOOST_FOREACH( const T_Dependency& include, cleaned )
            if( !Notify( NotifyInternal, metric.unit_, Resolve( include.include_ ), include.context_ ) )
                if( !Notify( NotifyExternal, metric.unit_, resolver_.Resolve( include.include_ ), include.context_ ) )
                    if( !resolver_.IsExcluded( include.include_ ) )
                        log_.Warn( "Warning: include \"" + include.include_ + "\" in unit '" + metric.unit_ + "' cannot be resolved", include.context_ );
        BOOST_FOREACH( const T_Dependency& include, metric.external_ )
            if( !Notify( NotifyExternal, metric.unit_, resolver_.Resolve( include.include_ ), include.context_ ) )
                if( !Notify( NotifyInternal, metric.unit_, Resolve( include.include_ ), include.context_ ) )
                    if( !resolver_.IsExcluded( include.include_ ) )
                        log_.Warn( "Warning: include <" + include.include_ + "> in unit '" + metric.unit_ + "' cannot be resolved", include.context_ );
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
void ModuleDependencyMetric::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
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
void ModuleDependencyMetric::NotifyFile( const std::string& path, std::istream& /*stream*/, const std::string& context )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid file '" + path + "' out of a unit" );
    metrics_.back().files_.insert( T_Dependency( path, context ) );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyInternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyInternalInclude( const std::string& file, const std::string& context )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a unit" );
    metrics_.back().internal_.insert( T_Dependency( file, context ) );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyExternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyExternalInclude( const std::string& file, const std::string& context )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + file + "' out of a unit" );
    metrics_.back().external_.insert( T_Dependency( file, context ) );
}
