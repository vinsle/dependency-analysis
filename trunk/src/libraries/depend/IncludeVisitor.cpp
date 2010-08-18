//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "IncludeVisitor.h"
#include "IncludeObserver_ABC.h"
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
IncludeVisitor::IncludeVisitor( Subject< LineObserver_ABC >& visitor )
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
// Name: IncludeVisitor::Notify
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void IncludeVisitor::Notify( const std::string& line )
{
    const mark_tag internal_tag( 1 );
    const mark_tag external_tag( 2 );
    const sregex spaces = *space;
    const sregex keyword = "#" >> spaces >> "include";
    const sregex rule = bos >> spaces >> keyword >> spaces >>
                        ( ( '\"' >> ( internal_tag = ( *_ ) ) >> '\"' )
                        | ( '<'  >> ( external_tag = ( *_ ) ) >> '>' ) );
    sregex_iterator it( line.begin(), line.end(), rule );
    sregex_iterator end;
    if( it != end )
        BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
            if( !std::string( (*it)[ internal_tag ] ).empty() )
                observer->NotifyInternal( (*it)[ internal_tag ] );
            else
                observer->NotifyExternal( (*it)[ external_tag ] );
}
