//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "UnitSerializer.h"
#include "Filter_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: UnitSerializer constructor
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
UnitSerializer::UnitSerializer( Subject< UnitObserver_ABC >& subject )
    : Observer< UnitObserver_ABC >( subject )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitSerializer destructor
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
UnitSerializer::~UnitSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitSerializer::Serialize
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
void UnitSerializer::Serialize( xml::xostream& xos, const Filter_ABC& filter ) const
{
    xos << xml::start( "units" );
    BOOST_FOREACH( const std::string& module, units_ )
        if( filter.Check( module ) )
        {
            xos << xml::start( "unit" )
                    << module
                    << xml::attribute( "core", filter.CheckCore( module ) )
                << xml::end;
        }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UnitSerializer::NotifyUnit
// Created: SLI 2010-08-24
// -----------------------------------------------------------------------------
void UnitSerializer::NotifyUnit( const std::string& unit )
{
    units_.push_back( unit );
}
