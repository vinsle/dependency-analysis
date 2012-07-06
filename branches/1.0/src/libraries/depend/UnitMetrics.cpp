//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "UnitMetrics.h"
#include "MetricsVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: UnitMetrics constructor
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
UnitMetrics::UnitMetrics( const Visitable< UnitObserver_ABC >& units, const Visitable< DependencyVisitor_ABC >& dependencies, const Visitable< ClassMetricVisitor_ABC >& classes )
{
    units.Apply( *this );
    dependencies.Apply( *this );
    classes.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitMetrics destructor
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
UnitMetrics::~UnitMetrics()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMetrics::FindClass
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
const UnitMetrics::ClassMetrics UnitMetrics::FindClass( const std::string& module ) const
{
    CIT_ClassMetrics it = classMetrics_.find( module );
    if( it == classMetrics_.end() )
        return ClassMetrics();
    return it->second;
}

namespace
{
    typedef std::pair< std::string, unsigned int > T_Number;
    template< typename T >
    unsigned int Sum( const std::string& module, const T& dependencies )
    {
        typename T::const_iterator it = dependencies.find( module );
        if( it == dependencies.end() )
            return 0u;
        unsigned int result = 0u;
        BOOST_FOREACH( const T_Number& number, it->second )
            result += number.second;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMetrics::Apply
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
void UnitMetrics::Apply( MetricsVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const std::string& module, modules_ )
    {
        const int afferent = Sum( module, afferent_ );
        const int efferent = Sum( module, efferent_ );
        const int external = Sum( module, external_ );
        const int classes = FindClass( module ).classes_;
        const int abstractClasses = FindClass( module ).abstract_;
        const int abstractness = classes == 0u ? 0u : ( 100u * abstractClasses ) / classes;
        const int instability = efferent + afferent == 0u ? 0u : ( 100u * efferent ) / ( efferent + afferent );
        const int distance = std::abs( abstractness + instability - 100 );
        visitor.NotifyMetrics( module, afferent, efferent, external, classes, abstractClasses, abstractness, instability, distance );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMetrics::NotifyUnit
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
void UnitMetrics::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    modules_.insert( unit );
    afferent_[ unit ];
    efferent_[ unit ];
    external_[ unit ];
    classMetrics_[ unit ];
}

// -----------------------------------------------------------------------------
// Name: UnitMetrics::NotifyInternalDependency
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
void UnitMetrics::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*context*/ )
{
    modules_.insert( fromModule );
    modules_.insert( toModule );
    efferent_[ fromModule ][ toModule ]++;
    afferent_[ toModule ][ fromModule ]++;
}

// -----------------------------------------------------------------------------
// Name: UnitMetrics::NotifyExternalDependency
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
void UnitMetrics::NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*context*/ )
{
    modules_.insert( fromModule );
    external_[ fromModule ][ toModule ]++;
}

// -----------------------------------------------------------------------------
// Name: UnitMetrics::NotifyClassMetric
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
void UnitMetrics::NotifyClassMetric( const std::string& module, unsigned int classes, unsigned int abstactClasses )
{
    modules_.insert( module );
    ClassMetrics& metrics = classMetrics_[ module ];
    metrics.classes_ = classes;
    metrics.abstract_ = abstactClasses;
}
