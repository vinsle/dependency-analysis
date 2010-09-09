//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "Finder.h"
#include <boost/filesystem.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Finder constructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
Finder::Finder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Finder destructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
Finder::~Finder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Finder::Find
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
bool Finder::Find( const std::string& filename ) const
{
    return boost::filesystem::exists( filename );
}
