//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "TransitiveReductionFilter.h"
#include "DependencyMetric_ABC.h"
#include <boost/foreach.hpp>
#pragma warning( push, 0 )
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_utility.hpp>
#pragma warning( pop )

using namespace depend;

// -----------------------------------------------------------------------------
// Name: TransitiveReductionFilter::TransitiveReductionFilter
// Created: SLI 2010-09-08
// -----------------------------------------------------------------------------
TransitiveReductionFilter::TransitiveReductionFilter( const DependencyMetric_ABC& metric, const Filter_ABC& filter )
    : filter_( filter )
{
    metric.Apply( *this );
    colormap_.resize( labels_.size(), boost::white_color );
    BOOST_FOREACH( const T_Labels::value_type& label, labels_ )
    {
        if( filter_.Check( label.second ) )
        {
            boost::bfs_visitor< boost::null_visitor > visitor;
            boost::queue< T_Graph::vertex_descriptor > buffer;
            boost::iterator_property_map< T_ColorMap::iterator, boost::identity_property_map > propertymap( colormap_.begin(), boost::identity_property_map() );
            boost::breadth_first_visit( graph_.graph(), graph_.vertex( label.second ), buffer, visitor, propertymap );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TransitiveReductionFilter destructor
// Created: SLI 2010-09-08
// -----------------------------------------------------------------------------
TransitiveReductionFilter::~TransitiveReductionFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransitiveReductionFilter::Check
// Created: SLI 2010-09-08
// -----------------------------------------------------------------------------
bool TransitiveReductionFilter::Check( const std::string& module ) const
{
    return filter_.Check( module ) || CheckMap( module );
}

// -----------------------------------------------------------------------------
// Name: TransitiveReductionFilter::CheckMap
// Created: SLI 2010-09-08
// -----------------------------------------------------------------------------
bool TransitiveReductionFilter::CheckMap( const std::string& module ) const
{
    const T_Graph::vertex_descriptor v = graph_.vertex( module );
    if( v >= colormap_.size() )
        return false;
    return colormap_[ v ] == boost::black_color || colormap_[ v ] == boost::gray_color;
}

// -----------------------------------------------------------------------------
// Name: TransitiveReductionFilter::NotifyInternalDependency
// Created: SLI 2010-09-08
// -----------------------------------------------------------------------------
void TransitiveReductionFilter::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule )
{
    labels_[ boost::add_vertex( fromModule, graph_ ) ] = fromModule;
    labels_[ boost::add_vertex( toModule, graph_ ) ] = toModule;
    boost::add_edge_by_label( fromModule, toModule, graph_ );
}
// -----------------------------------------------------------------------------
// Name: TransitiveReductionFilter::NotifyExternalDependency
// Created: SLI 2010-09-08
// -----------------------------------------------------------------------------
void TransitiveReductionFilter::NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/ )
{
    // NOTHING
}