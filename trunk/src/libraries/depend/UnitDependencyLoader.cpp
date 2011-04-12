//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "UnitDependencyLoader.h"
#include "DependencyVisitor_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: UnitDependencyLoader constructor
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
UnitDependencyLoader::UnitDependencyLoader( xml::xisubstream xis )
{
    xis >> xml::start( "graph" )
            >> xml::start( "nodes" )
                >> xml::list( "node", *this, &UnitDependencyLoader::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: UnitDependencyLoader destructor
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
UnitDependencyLoader::~UnitDependencyLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitDependencyLoader::Apply
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
void UnitDependencyLoader::Apply( DependencyVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const T_Node& node, nodes_ )
    {
        BOOST_FOREACH( const T_Dependency& dependency, node.internals_ )
            BOOST_FOREACH( const std::string& context, dependency.second )
                visitor.NotifyInternalDependency( node.from_, dependency.first, context );
        BOOST_FOREACH( const T_Dependency& dependency, node.externals_ )
            BOOST_FOREACH( const std::string& context, dependency.second )
                visitor.NotifyExternalDependency( node.from_, dependency.first, context );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitDependencyLoader::ReadNode
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
void UnitDependencyLoader::ReadNode( xml::xistream& xis )
{
    T_Node node;
    xis >> xml::attribute( "name", node.from_ )
        >> xml::start( "efferent-dependencies" )
            >> xml::list( "dependency", *this, &UnitDependencyLoader::ReadDependency, node.internals_ )
        >> xml::end
        >> xml::start( "external-dependencies" )
            >> xml::list( "dependency", *this, &UnitDependencyLoader::ReadDependency, node.externals_ );
    nodes_.push_back( node );
}

// -----------------------------------------------------------------------------
// Name: UnitDependencyLoader::ReadDependency
// Created: SLI 2011-02-21
// -----------------------------------------------------------------------------
void UnitDependencyLoader::ReadDependency( xml::xistream& xis, T_Dependencies& dependencies ) const
{
    std::string name;
    T_Contexts contexts;
    xis >> xml::attribute( "name", name )
        >> xml::list( "context", *this, &UnitDependencyLoader::ReadContext, contexts );
    dependencies.push_back( std::make_pair( name, contexts ) );
}

// -----------------------------------------------------------------------------
// Name: UnitDependencyLoader::ReadContext
// Created: SLI 2011-02-21
// -----------------------------------------------------------------------------
void UnitDependencyLoader::ReadContext( xml::xistream& xis, T_Contexts& contexts ) const
{
    std::string context;
    xis >> context;
    contexts.push_back( context );
}
