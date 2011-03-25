//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "StronglyConnectedComponents.h"
#include "DependencyMetric_ABC.h"
#include "Filter_ABC.h"
#include "Visitable.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/graph/strong_components.hpp>
#pragma warning( pop )
#include <boost/foreach.hpp>
#include <map>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponents constructor
// Created: SLI 2010-08-23
// -----------------------------------------------------------------------------
StronglyConnectedComponents::StronglyConnectedComponents( const Visitable< DependencyMetricVisitor_ABC >& metric )
{
    metric.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponents destructor
// Created: SLI 2010-08-23
// -----------------------------------------------------------------------------
StronglyConnectedComponents::~StronglyConnectedComponents()
{
    // NOTHING
}

namespace
{
    template< typename T, typename U >
    void Serialize( T& components, const U& labels, const Filter_ABC& filter, xml::xostream& xos )
    {
        typedef std::vector< std::string > T_Dependencies;
        typedef std::map< T::key_type, T_Dependencies > T_Components;
        T_Components sorted_components;
        BOOST_FOREACH( const T::value_type& component, components )
        {
            U::const_iterator it = labels.find( component.first );
            if( filter.Check( it->second ) )
                sorted_components[ component.second ].push_back( it->second );
        }
        BOOST_FOREACH( const T_Components::value_type& component, sorted_components )
        {
            if( component.second.size() > 1 )
            {
                xos << xml::start( "component" );
                BOOST_FOREACH( const std::string& module, component.second )
                    xos << xml::content( "node", module );
                xos << xml::end;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponents::Serialize
// Created: SLI 2010-08-23
// -----------------------------------------------------------------------------
void StronglyConnectedComponents::Serialize( xml::xostream& xos, const Filter_ABC& filter ) const
{
    xos << xml::start( "strongly-connected-components" );
    typedef std::map< T_Graph::vertex_descriptor, T_Graph::vertices_size_type > T_Map;
    typedef boost::associative_property_map< T_Map > T_PropertyMap;
    T_Map mymap;
    T_PropertyMap pmap( mymap );
    boost::strong_components( graph_.graph(), pmap );
    ::Serialize( mymap, labels_, filter, xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponents::NotifyInternalDependency
// Created: SLI 2010-08-23
// -----------------------------------------------------------------------------
void StronglyConnectedComponents::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*context*/ )
{
    labels_[ boost::add_vertex( fromModule, graph_ ) ] = fromModule;
    labels_[ boost::add_vertex( toModule, graph_ ) ] = toModule;
    boost::add_edge_by_label( fromModule, toModule, graph_ );
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponents::NotifyExternalDependency
// Created: SLI 2010-08-23
// -----------------------------------------------------------------------------
void StronglyConnectedComponents::NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/, const std::string& /*context*/ )
{
    // NOTHING
}
