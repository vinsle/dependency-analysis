//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "IncludeVisitor.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: IncludeVisitor constructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
IncludeVisitor::IncludeVisitor( LineVisitor_ABC& visitor )
    : visitor_( visitor )
{
    visitor_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: IncludeVisitor destructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
IncludeVisitor::~IncludeVisitor()
{
    visitor_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: IncludeVisitor::Visit
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void IncludeVisitor::Visit( std::istream& stream )
{
    visitor_.Visit( stream );
}

// -----------------------------------------------------------------------------
// Name: IncludeVisitor::Register
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void IncludeVisitor::Register( LineObserver_ABC& observer )
{
    observers_.push_back( &observer );
}

// -----------------------------------------------------------------------------
// Name: IncludeVisitor::Unregister
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void IncludeVisitor::Unregister( LineObserver_ABC& observer )
{
    observers_.erase( std::remove( observers_.begin(), observers_.end(), &observer ), observers_.end() );
}

// -----------------------------------------------------------------------------
// Name: IncludeVisitor::Notify
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void IncludeVisitor::Notify( const std::string& line )
{
    if( boost::algorithm::contains( line, "#include" ) )
        BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
            observer->Notify( line );
}
