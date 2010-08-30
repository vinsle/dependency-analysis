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
    template< typename T >
    bool Check( const T& filter, const std::string& module )
    {
        if( filter.empty() )
            return true;
        return std::find( filter.begin(), filter.end(), module ) != filter.end();
    }
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
    template< typename T, typename U >
    void SerializeDependency( xml::xostream& xos, const std::string& module, const T& dependencies, const U& filter )
    {
        T::const_iterator it = dependencies.find( module );
        if( it == dependencies.end() )
            return;
        BOOST_FOREACH( const T_Number& number, it->second )
            if( Check( filter, number.first ) )
                xos << xml::start( "dependency" )
                        << xml::attribute( "name", number.first )
                        << xml::attribute( "number", number.second )
                    << xml::end;
    }
    void SerializeMetrics( xml::xostream& xos, unsigned int classes, unsigned int abstractClasses, unsigned int ce, unsigned int ca )
    {
        const int abstractness = classes == 0u ? 0u : ( 100u * abstractClasses ) / classes;
        const int instability = ce + ca == 0u ? 0u : ( 100u * ce ) / ( ce + ca );
        const int distance = std::abs( abstractness + instability - 100 );
        xos << xml::start( "metrics" )
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
void MetricSerializer::Serialize( xml::xostream& xos, const T_Filter& filter ) const
{
    xos << xml::start( "categories" );
    BOOST_FOREACH( const std::string& module, modules_ )
    {
        if( Check( filter, module ) )
        {
            xos << xml::start( "category" )
                    << xml::attribute( "name", module )
                    << xml::start( "efferent-dependencies" )
                        << xml::attribute( "Ce", Sum( module, efferent_ ) );
            SerializeDependency( xos, module, efferent_, filter );
            xos     << xml::end
                    << xml::start( "afferent-dependencies" )
                        << xml::attribute( "Ca", Sum( module, afferent_ ) );
            SerializeDependency( xos, module, afferent_, filter );
            xos     << xml::end
                    << xml::start( "external-dependencies" )
                        << xml::attribute( "Ce", Sum( module, external_ ) );
            SerializeDependency( xos, module, external_, T_Filter() );
            xos     << xml::end;
            SerializeMetrics( xos, FindClass( module ).classes_, FindClass( module ).abstract_, Sum( module, efferent_ ), Sum( module, afferent_ ) );
            xos << xml::end;
        }
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer::NotifyInternalDependency
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void MetricSerializer::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*include*/ )
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
void MetricSerializer::NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*include*/ )
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
