//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "MetricSerializer.h"
#include "Filter_ABC.h"
#include "Visitable.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: MetricSerializer constructor
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
MetricSerializer::MetricSerializer( const Visitable< MetricsVisitor_ABC >& metrics )
{
    metrics.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer destructor
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
MetricSerializer::~MetricSerializer()
{
    // NOTHING
}

namespace
{
    template< typename T >
    void SerializeMetric( xml::xostream& xos, const std::string& module, const T& metric )
    {
        xos << xml::start( "metric" )
                << xml::attribute( "name", module )
                << xml::content( "afferent", metric.afferent_ )
                << xml::content( "efferent", metric.efferent_ )
                << xml::content( "external", metric.external_ )
                << xml::content( "classes", metric.classes_ )
                << xml::content( "abstract-classes", metric.abstract_ )
                << xml::content( "abstractness", metric.abstractness_ )
                << xml::content( "instability", metric.instability_ )
                << xml::content( "distance", metric.distance_ )
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
    BOOST_FOREACH( const T_UnitMetrics::value_type& metric, metrics_ )
        if( filter.Check( metric.first ) )
            SerializeMetric( xos, metric.first, metric.second );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MetricSerializer::NotifyMetrics
// Created: SLI 2011-04-12
// -----------------------------------------------------------------------------
void MetricSerializer::NotifyMetrics( const std::string& module, unsigned int afferent, unsigned int efferent,
                                      unsigned int external, unsigned int classes, unsigned int abstractClasses,
                                      unsigned int abstractness, unsigned int instability, unsigned int distance )
{
    T_Metric& metric = metrics_[ module ];
    metric.afferent_ = afferent;
    metric.efferent_ = efferent;
    metric.external_ = external;
    metric.classes_ = classes;
    metric.abstract_ = abstractClasses;
    metric.abstractness_ = abstractness;
    metric.instability_ = instability;
    metric.distance_ = distance;
}
