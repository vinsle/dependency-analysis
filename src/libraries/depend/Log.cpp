//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "Log.h"
#include <iostream>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Log constructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
Log::Log( bool enabled )
    : enabled_ ( enabled )
    , messages_( new T_Messages() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Log destructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
Log::~Log()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Log::Warn
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
void Log::Warn( const std::string& message ) const
{
    if( enabled_ && messages_->find( message ) == messages_->end() )
    {
        std::cout << message << std::endl;
        messages_->insert( message );
    }
}
