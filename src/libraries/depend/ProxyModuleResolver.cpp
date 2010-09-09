//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ProxyModuleResolver.h"

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ProxyModuleResolver constructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
ProxyModuleResolver::ProxyModuleResolver( ModuleResolver_ABC& resolver )
    : resolver_( resolver )
    , results_ ( new T_Results() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProxyModuleResolver destructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
ProxyModuleResolver::~ProxyModuleResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProxyModuleResolver::Resolve
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
std::string ProxyModuleResolver::Resolve( const std::string& include ) const
{
    std::string& result = (*results_)[ include ];
    if( result.empty() )
        result = resolver_.Resolve( include );
    return result;
}
