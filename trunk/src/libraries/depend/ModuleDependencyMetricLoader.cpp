//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ModuleDependencyMetricLoader.h"
#include "DependencyMetricVisitor_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetricLoader constructor
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
ModuleDependencyMetricLoader::ModuleDependencyMetricLoader( xml::xisubstream xis )
{
    xis >> xml::start( "graph" )
            >> xml::start( "nodes" )
                >> xml::list( "node", *this, &ModuleDependencyMetricLoader::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetricLoader destructor
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
ModuleDependencyMetricLoader::~ModuleDependencyMetricLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetricLoader::Apply
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
void ModuleDependencyMetricLoader::Apply( DependencyMetricVisitor_ABC& visitor ) const
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
// Name: ModuleDependencyMetricLoader::ReadNode
// Created: SLI 2011-02-18
// -----------------------------------------------------------------------------
void ModuleDependencyMetricLoader::ReadNode( xml::xistream& xis )
{
    T_Node node;
    xis >> xml::attribute( "name", node.from_ )
        >> xml::start( "efferent-dependencies" )
            >> xml::list( "dependency", *this, &ModuleDependencyMetricLoader::ReadDependency, node.internals_ )
        >> xml::end
        >> xml::start( "external-dependencies" )
            >> xml::list( "dependency", *this, &ModuleDependencyMetricLoader::ReadDependency, node.externals_ );
    nodes_.push_back( node );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetricLoader::ReadDependency
// Created: SLI 2011-02-21
// -----------------------------------------------------------------------------
void ModuleDependencyMetricLoader::ReadDependency( xml::xistream& xis, T_Dependencies& dependencies ) const
{
    std::string name;
    T_Contexts contexts;
    xis >> xml::attribute( "name", name )
        >> xml::list( "context", *this, &ModuleDependencyMetricLoader::ReadContext, contexts );
    dependencies.push_back( std::make_pair( name, contexts ) );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyMetricLoader::ReadContext
// Created: SLI 2011-02-21
// -----------------------------------------------------------------------------
void ModuleDependencyMetricLoader::ReadContext( xml::xistream& xis, T_Contexts& contexts ) const
{
    std::string context;
    xis >> context;
    contexts.push_back( context );
}
