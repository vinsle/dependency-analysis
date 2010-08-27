//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "PngSerializer.h"
#include <graphviz/gvc.h>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: PngSerializer constructor
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
PngSerializer::PngSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PngSerializer destructor
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
PngSerializer::~PngSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PngSerializer::Serialize
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
void PngSerializer::Serialize( const std::string& dot, const std::string& filename ) const
{
    std::string buffer = dot;
    GVC_t* gvc = gvContext();
    Agraph_t* G = agmemread( const_cast< char* >( buffer.c_str() ) );
    gvLayout( gvc, G, "dot" );
    gvRenderFilename( gvc, G, "png", filename.c_str() );
    gvFreeLayout( gvc, G );
    gvFreeContext( gvc );
}
