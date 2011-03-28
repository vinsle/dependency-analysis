//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "UnitCache.h"
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: UnitCache constructor
// Created: SLI 2011-03-28
// -----------------------------------------------------------------------------
UnitCache::UnitCache( Subject< UnitObserver_ABC >& subject )
    : Observer< UnitObserver_ABC >( subject )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitCache destructor
// Created: SLI 2011-03-28
// -----------------------------------------------------------------------------
UnitCache::~UnitCache()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitCache::Apply
// Created: SLI 2011-03-28
// -----------------------------------------------------------------------------
void UnitCache::Apply( UnitObserver_ABC& visitor ) const
{
    BOOST_FOREACH( const T_Unit& unit, units_ )
        visitor.NotifyUnit( unit.first, unit.second );
}

// -----------------------------------------------------------------------------
// Name: UnitCache::NotifyUnit
// Created: SLI 2011-03-28
// -----------------------------------------------------------------------------
void UnitCache::NotifyUnit( const std::string& unit, const std::string& context )
{
    units_.push_back( std::make_pair( unit, context ) );
}
