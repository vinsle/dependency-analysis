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
#pragma warning( push, 0 )
#pragma warning( disable: 4996 )
#include <boost/xpressive/xpressive.hpp>
#pragma warning( pop )

using namespace depend;
using namespace boost::xpressive;

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
    const mark_tag include( 1 );
    const sregex spaces = *space;
    const sregex opening_include = ( set = '<', '\"' );
    const sregex closing_include = ( set = '>', '\"' );
    const sregex file = opening_include >> *_ >> closing_include;
    const sregex keyword = "#" >> spaces >> "include";
    const sregex rule = bos >> spaces >> keyword >> spaces >> ( include = file );
    sregex_iterator it( line.begin(), line.end(), rule );
    sregex_iterator end;
    if( it != end )
        BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
            observer->Notify( (*it)[ include ] );
}
