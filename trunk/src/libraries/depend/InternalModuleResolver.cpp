//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "InternalModuleResolver.h"

using namespace depend;

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver constructor
// Created: SLI 2011-02-16
// -----------------------------------------------------------------------------
InternalModuleResolver::InternalModuleResolver( Subject< UnitObserver_ABC >& unitObserver )
    : Observer< UnitObserver_ABC >( unitObserver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver destructor
// Created: SLI 2011-02-16
// -----------------------------------------------------------------------------
InternalModuleResolver::~InternalModuleResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver::Resolve
// Created: SLI 2011-02-16
// -----------------------------------------------------------------------------
std::string InternalModuleResolver::Resolve( const std::string& include ) const
{
    const size_t position = include.find_first_of( '/' );
    const std::string unit = include.substr( 0, position );
    if( position == std::string::npos || units_.find( unit ) == units_.end() )
        return "";
    return unit;
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver::IsExcluded
// Created: SLI 2011-02-16
// -----------------------------------------------------------------------------
bool InternalModuleResolver::IsExcluded( const std::string& /*include*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver::NotifyUnit
// Created: SLI 2011-02-16
// -----------------------------------------------------------------------------
void InternalModuleResolver::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    units_.insert( unit );
}
