//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "DotSerializer.h"
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <xeumeuleu/xml.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: DotSerializer constructor
// Created: SLI 2010-08-26
// -----------------------------------------------------------------------------
DotSerializer::DotSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotSerializer destructor
// Created: SLI 2010-08-26
// -----------------------------------------------------------------------------
DotSerializer::~DotSerializer()
{
    // NOTHING
}

namespace
{
    struct T_Metric 
    {
        float abstractness;
        float instability;
        float distance;
    };
    typedef std::map< std::string, T_Metric > T_Metrics;
    void ReadMetric( xml::xistream& xis, T_Metrics& metrics )
    {
        std::string name;
        T_Metric metric;
        xis >> xml::attribute( "name", name )
            >> xml::content( "abstractness", metric.abstractness )
            >> xml::content( "instability", metric.instability )
            >> xml::content( "distance", metric.distance );
        metrics[ name ] = metric;
    }
    typedef std::set< std::string > T_Component;
    typedef std::vector< T_Component > T_Components;

    void ReadComponent( xml::xistream& xis, T_Component& component )
    {
        std::string node;
        xis >> node;
        component.insert( node );
    }
    void ReadComponents( xml::xistream& xis, T_Components& components )
    {
        T_Component component;
        xis >> xml::list( "node", boost::bind( &ReadComponent, _1, boost::ref( component ) ) );
        components.push_back( component );
    }
    void ReadUnit( xml::xistream& xis, const T_Metrics& metrics, std::ostream& os )
    {
        std::string unit;
        xis >> unit;
        T_Metrics::const_iterator cit = metrics.find( unit );
        const float color = ( 100.f - cit->second.distance ) / 360.f;
        os << "\"" << unit << "\"" << "[label=\"\\N\\nI=" <<  cit->second.instability
                                   << " A=" << cit->second.abstractness
                                   << " D=" << cit->second.distance
                                   << "\",shape=rectangle,style=filled,color=\""
                                   << color << " 1.0 1.0\"];"
                                   << std::endl;
    }
    void ReadDependency( xml::xistream& xis, const T_Components& components, std::ostream& os, const std::string& from )
    {
        std::string to;
        xis >> xml::attribute( "name", to );
        boost::function< T_Component::const_iterator( const T_Component&, const std::string& ) > find = boost::bind< T_Component::const_iterator >( &T_Component::find, _1, _2 );
        boost::function< T_Component::const_iterator( const T_Component& ) > end = boost::bind< T_Component::const_iterator >( &T_Component::end, _1 );
        boost::function< bool( const T_Component&, const std::string& ) > predicate = boost::bind( std::not_equal_to< T_Component::const_iterator >(), boost::bind( find, _1, _2 ), boost::bind( end, _1 ) );
        const size_t componentFrom = std::distance( components.begin(), std::find_if( components.begin(), components.end(), boost::bind( predicate, _1, boost::cref( from ) ) ) );
        const size_t componentTo = std::distance( components.begin(), std::find_if( components.begin(), components.end(), boost::bind( predicate, _1, boost::cref( to ) ) ) );
        os << "\"" << from << "\"->\"" << to << "\"";
        if( componentFrom != components.size() && componentTo != components.size() && componentFrom == componentTo )
            os << "[color=\"" << static_cast< float >( componentFrom + 1 ) / static_cast< float >( components.size() ) << " 1.0 1.0\"]";
        os << ";" << std::endl;
    }
    void ReadNode( xml::xistream& xis, const T_Components& components, std::ostream& os )
    {
        std::string from;
        xis >> xml::attribute( "name", from )
            >> xml::start( "efferent-dependencies" )
                >> xml::list( "dependency", boost::bind( &ReadDependency, _1, boost::cref( components ), boost::ref( os ), boost::cref( from ) ) )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: DotSerializer::Serialize
// Created: SLI 2010-08-26
// -----------------------------------------------------------------------------
void DotSerializer::Serialize( xml::xistream& xis, std::ostream& os ) const
{
    T_Metrics metrics;
    T_Components components;
    os << "digraph G {" << std::endl;
    xis >> xml::start( "report" )
            >> xml::start( "metrics" )
                >> xml::list( "metric", boost::bind( &ReadMetric, _1, boost::ref( metrics ) ) )
            >> xml::end
            >> xml::start( "strongly-connected-components" )
                >> xml::list( "component", boost::bind( &ReadComponents, _1, boost::ref( components ) ) )
            >> xml::end
            >> xml::start( "units" )
                >> xml::list( "unit", boost::bind( &ReadUnit, _1, boost::cref( metrics ), boost::ref( os ) ) )
            >> xml::end
            >> xml::start( "graph" )
                >> xml::list( "node", boost::bind( &ReadNode, _1, boost::cref( components ), boost::ref( os ) ) )
            >> xml::end
        >> xml::end;
    os << "}" << std::endl;
}
