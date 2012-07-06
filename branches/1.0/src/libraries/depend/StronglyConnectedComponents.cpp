//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "StronglyConnectedComponents.h"
#include "Filter_ABC.h"
#include "StronglyConnectedComponentsVisitor_ABC.h"
#include "ComponentVisitor_ABC.h"
#include <boost/foreach.hpp>
#include <map>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponents constructor
// Created: SLI 2010-08-23
// -----------------------------------------------------------------------------
StronglyConnectedComponents::StronglyConnectedComponents( const Visitable< DependencyVisitor_ABC >& metric, const Filter_ABC& filter )
    : filter_( filter )
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
    class Component : public Visitable< ComponentVisitor_ABC >
    {
    public:
        Component( const std::vector< std::string >& units )
            : units_( units )
        {}
        virtual void Apply( ComponentVisitor_ABC& visitor ) const
        {
            BOOST_FOREACH( const std::string& unit, units_ )
                visitor.NotifyUnit( unit );
        }
    private:
        const std::vector< std::string >& units_;
    };
    template< typename T, typename U >
    void Visit( T& components, const U& labels, const Filter_ABC& filter, StronglyConnectedComponentsVisitor_ABC& visitor )
    {
        typedef std::vector< std::string > T_Dependencies;
        typedef std::map< typename T::key_type, T_Dependencies > T_Components;
        T_Components sorted_components;
        BOOST_FOREACH( const typename T::value_type& component, components )
        {
            typename U::const_iterator it = labels.find( component.first );
            if( filter.Check( it->second ) )
                sorted_components[ component.second ].push_back( it->second );
        }
        BOOST_FOREACH( const typename T_Components::value_type& component, sorted_components )
            if( component.second.size() > 1 )
                visitor.NotifyComponent( Component( component.second ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponents::Apply
// Created: SLI 2011-04-06
// -----------------------------------------------------------------------------
void StronglyConnectedComponents::Apply( StronglyConnectedComponentsVisitor_ABC& visitor ) const
{
    typedef std::map< T_Graph::vertex_descriptor, T_Graph::vertices_size_type > T_Map;
    typedef boost::associative_property_map< T_Map > T_PropertyMap;
    T_Map mymap;
    T_PropertyMap pmap( mymap );
    boost::strong_components( graph_.graph(), pmap );
    ::Visit( mymap, labels_, filter_, visitor );
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
