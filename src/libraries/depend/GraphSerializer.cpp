//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "GraphSerializer.h"
#include <graphviz/gvc.h>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: GraphSerializer constructor
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
GraphSerializer::GraphSerializer( const std::string& layout, const std::string& format,
                                  const T_Options& graph, const T_Options& node, const T_Options& edge )
    : layout_( layout )
    , format_( format )
    , graph_ ( graph )
    , node_  ( node )
    , edge_  ( edge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GraphSerializer destructor
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
GraphSerializer::~GraphSerializer()
{
    // NOTHING
}

namespace
{
    template< typename T, typename U >
    void SetAttributes( const T& options, const U& fun, Agraph_t* graph )
    {
        BOOST_FOREACH( const T::value_type& option, options )
        {
            Agsym_t* attribute = fun( graph, const_cast< char* >( option.first.c_str() ), const_cast< char* >( option.second.c_str() ) );
            agset( graph, attribute->name, attribute->value );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GraphSerializer::Serialize
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
void GraphSerializer::Serialize( const std::string& dot, const std::string& filename ) const
{
    std::string buffer = dot;
    GVC_t* context = gvContext();
    Agraph_t* graph = agmemread( const_cast< char* >( buffer.c_str() ) );
    SetAttributes( graph_, boost::bind( &agraphattr, _1, _2, _3 ), graph );
    SetAttributes( node_,  boost::bind( &agnodeattr, _1, _2, _3 ), graph );
    SetAttributes( edge_,  boost::bind( &agedgeattr, _1, _2, _3 ), graph );
    gvLayout( context, graph, layout_.c_str() );
    gvRenderFilename( context, graph, format_.c_str(), filename.c_str() );
    gvFreeLayout( context, graph );
    gvFreeContext( context );
}
