//
//  Copyright Silvin Lubecki 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "GraphMLSerializer.h"
#include "Visitable.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: GraphMLSerializer constructor
// Created: SLI 2012-07-09
// -----------------------------------------------------------------------------
GraphMLSerializer::GraphMLSerializer( const Visitable< DependencyVisitor_ABC >& dependencies, const Visitable< UnitObserver_ABC >& units )
{
    units.Apply( *this );
    dependencies.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: GraphMLSerializer destructor
// Created: SLI 2012-07-09
// -----------------------------------------------------------------------------
GraphMLSerializer::~GraphMLSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GraphMLSerializer::Serialize
// Created: SLI 2012-07-09
// -----------------------------------------------------------------------------
void GraphMLSerializer::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "graphml" )
            << xml::start( "graph" )
                << xml::attribute( "id", "G" )
                << xml::attribute( "edgedefault", "directed" );
    BOOST_FOREACH( const std::string& module, modules_ )
        xos     << xml::start( "node" )
                    << xml::attribute( "id", module )
                << xml::end;
    BOOST_FOREACH( const T_Dependencies::value_type& from, dependencies_ )
        BOOST_FOREACH( const std::string& to, from.second )
            xos << xml::start( "edge" )
                    << xml::attribute( "source", from.first )
                    << xml::attribute( "target", to )
                << xml::end;
    xos     << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: GraphMLSerializer::NotifyInternalDependency
// Created: SLI 2012-07-09
// -----------------------------------------------------------------------------
void GraphMLSerializer::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*context*/ )
{
    modules_.insert( fromModule );
    modules_.insert( toModule );
    dependencies_[ fromModule ].insert( toModule );
}

// -----------------------------------------------------------------------------
// Name: GraphMLSerializer::NotifyExternalDependency
// Created: SLI 2012-07-09
// -----------------------------------------------------------------------------
void GraphMLSerializer::NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*context*/ )
{
    modules_.insert( fromModule );
    modules_.insert( toModule );
    dependencies_[ fromModule ].insert( toModule );
}

// -----------------------------------------------------------------------------
// Name: GraphMLSerializer::NotifyUnit
// Created: SLI 2012-07-09
// -----------------------------------------------------------------------------
void GraphMLSerializer::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    modules_.insert( unit );
}
