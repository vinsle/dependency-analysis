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
#include "ExternalModuleResolver_ABC.h"
#include "InternalModuleResolver_ABC.h"
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
                                                Subject< IncludeObserver_ABC >& includeObserver, const ExternalModuleResolver_ABC& externalResolver,
                                                const InternalModuleResolver_ABC& internalResolver, const Log_ABC& log )
    : Observer< UnitObserver_ABC >   ( unitObserver )
    , Observer< FileObserver_ABC >   ( fileObserver )
    , Observer< IncludeObserver_ABC >( includeObserver )
    , externalResolver_( externalResolver )
    , internalResolver_( internalResolver )
    , log_             ( log )
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
    template< typename T, typename U >
    bool Notify( boost::function< T > notify, const std::string& currentUnit, const std::string& unit, const U& contexts )
    {
        if( unit.empty() )
            return false;
        if( currentUnit != unit )
            BOOST_FOREACH( const std::string& context, contexts )
                notify( currentUnit, unit, context );
        return true;
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
            if( !Notify( NotifyInternal, metric.unit_, internalResolver_.Resolve( metric.unit_, include.file_, include.include_ ), include.contexts_ ) )
                if( !Notify( NotifyExternal, metric.unit_, externalResolver_.Resolve( include.include_ ), include.contexts_ ) )
                    if( !externalResolver_.IsExcluded( include.include_ ) )
                        BOOST_FOREACH( const std::string& context, include.contexts_ )
                            log_.Warn( "Warning: include \"" + include.include_ + "\" in unit '" + metric.unit_ + "' cannot be resolved", context );
        BOOST_FOREACH( const T_Dependency& include, metric.external_ )
            if( !Notify( NotifyExternal, metric.unit_, externalResolver_.Resolve( include.include_ ), include.contexts_ ) )
                if( !Notify( NotifyInternal, metric.unit_, internalResolver_.Resolve( metric.unit_, include.file_, include.include_ ), include.contexts_ ) )
                    if( !externalResolver_.IsExcluded( include.include_ ) )
                        BOOST_FOREACH( const std::string& context, include.contexts_ )
                            log_.Warn( "Warning: include <" + include.include_ + "> in unit '" + metric.unit_ + "' cannot be resolved", context );
    }
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
void ModuleDependencyMetric::NotifyFile( const std::string& path, std::istream& /*stream*/, const std::string& /*context*/ )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid file '" + path + "' out of a unit" );
    metrics_.back().files_.insert( T_Dependency( path, path ) );
    lastFile_ = path;
}

namespace
{
    template< typename T >
    void Insert( T& dependencies, const std::string& include, const std::string& context, const std::string& file )
    {
        const T::value_type dependency( include, file );
        T::iterator it = dependencies.find( dependency );
        if( it == dependencies.end() )
            it = dependencies.insert( dependency ).first;
        it->contexts_.push_back( context );
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyInternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyInternalInclude( const std::string& include, const std::string& context )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + include + "' out of a unit" );
    Insert( metrics_.back().internal_, include, context, lastFile_ );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetric::NotifyExternalInclude
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void ModuleDependencyMetric::NotifyExternalInclude( const std::string& include, const std::string& context )
{
    if( metrics_.empty() )
        throw std::runtime_error( "invalid include '" + include + "' out of a unit" );
    Insert( metrics_.back().external_, include, context, lastFile_ );
}
