//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "MetricSerializer.h"
#include "DependencyMetric_ABC.h"
#include "ClassMetric_ABC.h"
#include "Filter_ABC.h"
#include "Filter.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: MetricSerializer constructor
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
MetricSerializer::MetricSerializer( const DependencyMetric_ABC& dependencies, const ClassMetric_ABC& classes )
{
    dependencies.Apply( *this );
    classes.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer destructor
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
MetricSerializer::~MetricSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer::FindClass
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
const MetricSerializer::ClassMetrics MetricSerializer::FindClass( const std::string& module ) const
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
        T::const_iterator it = dependencies.find( module );
        if( it == dependencies.end() )
            return 0u;
        unsigned int result = 0u;
        BOOST_FOREACH( const T_Number& number, it->second )
            result += number.second;
        return result;
    }
    void SerializeMetrics( xml::xostream& xos, const std::string& module, unsigned int classes, unsigned int abstractClasses, unsigned int ce, unsigned int ca )
    {
        const int abstractness = classes == 0u ? 0u : ( 100u * abstractClasses ) / classes;
        const int instability = ce + ca == 0u ? 0u : ( 100u * ce ) / ( ce + ca );
        const int distance = std::abs( abstractness + instability - 100 );
        xos << xml::start( "metric" )
                << xml::attribute( "name", module )
                << xml::content( "number-of-classes", classes )
                << xml::content( "number-of-abstract-classes", abstractClasses )
                << xml::content( "abstractness", abstractness )
                << xml::content( "instability", instability )
                << xml::content( "distance", distance )
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer::Serialize
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void MetricSerializer::Serialize( xml::xostream& xos, const Filter_ABC& filter ) const
{
    xos << xml::start( "metrics" );
    BOOST_FOREACH( const std::string& module, modules_ )
        if( filter.Check( module ) )
            SerializeMetrics( xos, module, FindClass( module ).classes_, FindClass( module ).abstract_, Sum( module, efferent_ ), Sum( module, afferent_ ) );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer::NotifyInternalDependency
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void MetricSerializer::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule )
{
    modules_.insert( fromModule );
    modules_.insert( toModule );
    efferent_[ fromModule ][ toModule ]++;
    afferent_[ toModule ][ fromModule ]++;
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer::NotifyExternalDependency
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void MetricSerializer::NotifyExternalDependency( const std::string& fromModule, const std::string& toModule )
{
    modules_.insert( fromModule );
    external_[ fromModule ][ toModule ]++;
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer::NotifyClassMetric
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void MetricSerializer::NotifyClassMetric( const std::string& module, unsigned int classes, unsigned int abstactClasses )
{
    modules_.insert( module );
    ClassMetrics& metrics = classMetrics_[ module ];
    metrics.classes_ = classes;
    metrics.abstract_ = abstactClasses;
}
