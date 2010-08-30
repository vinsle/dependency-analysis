//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ModuleSerializer.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleSerializer constructor
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
ModuleSerializer::ModuleSerializer( Subject< ModuleObserver_ABC >& subject )
    : subject_( subject )
{
    subject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ModuleSerializer destructor
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
ModuleSerializer::~ModuleSerializer()
{
    subject_.Unregister( *this );
}

namespace
{
    template< typename T >
    bool Check( const T& filter, const std::string& module )
    {
        if( filter.empty() )
            return true;
        return std::find( filter.begin(), filter.end(), module ) != filter.end();
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleSerializer::Serialize
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
void ModuleSerializer::Serialize( xml::xostream& xos, const T_Filter& filter ) const
{
    xos << xml::start( "dependencies" );
    BOOST_FOREACH( const std::string& module, modules_ )
        if( Check( filter, module ) )
            xos << xml::start( "dependency" )
                    << xml::attribute( "name", module )
                << xml::end;
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
