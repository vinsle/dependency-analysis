//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Version.h"

using namespace application;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifndef BUILD_VERSION
#   error BUILD_VERSION undefined !
#endif
#ifndef BUILD_TIME
#   error BUILD_TIME undefined !
#endif

namespace
{
    static const std::string version = TOSTRING( BUILD_VERSION );
    static const std::string time = TOSTRING( BUILD_TIME );
}

// -----------------------------------------------------------------------------
// Name: Version constructor
// Created: SLI 2011-04-13
// -----------------------------------------------------------------------------
Version::Version()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Version destructor
// Created: SLI 2011-04-13
// -----------------------------------------------------------------------------
Version::~Version()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Version::Serialize
// Created: SLI 2011-04-13
// -----------------------------------------------------------------------------
void Version::Serialize( std::ostream& os, const std::string& application ) const
{
    os << application << " (version:" << version << ", built " << time << ")" << std::endl << std::endl
       << "Copyright Silvin Lubecki 2010-2011" << std::endl
       << "Distributed under the Boost Software License, Version 1.0." << std::endl
       << "(See accompanying file LICENSE_1_0.txt or " << std::endl
       << "copy at http://www.boost.org/LICENSE_1_0.txt)" << std::endl
       << "See http://code.google.com/p/dependency-analysis for more informations" << std::endl;
}
