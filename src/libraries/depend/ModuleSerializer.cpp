//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ModuleSerializer.h"
#include "Filter_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleSerializer constructor
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
ModuleSerializer::ModuleSerializer( Subject< ModuleObserver_ABC >& subject )
    : Observer< ModuleObserver_ABC >( subject )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleSerializer destructor
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
ModuleSerializer::~ModuleSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleSerializer::Serialize
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
void ModuleSerializer::Serialize( xml::xostream& xos, const Filter_ABC& filter ) const
{
    xos << xml::start( "nodes" );
    BOOST_FOREACH( const std::string& module, modules_ )
        if( filter.Check( module ) )
            xos << xml::content( "node", module );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ModuleSerializer::NotifyModule
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
void ModuleSerializer::NotifyModule( const std::string& module )
{
    modules_.push_back( module );
}
