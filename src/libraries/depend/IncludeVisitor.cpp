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

using namespace depend;
using namespace boost::xpressive;

// -----------------------------------------------------------------------------
// Name: IncludeVisitor constructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
IncludeVisitor::IncludeVisitor( Subject< UncommentedLineObserver_ABC >& visitor )
    : Observer< UncommentedLineObserver_ABC >( visitor )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IncludeVisitor destructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
IncludeVisitor::~IncludeVisitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IncludeVisitor::NotifyUncommentedLine
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void IncludeVisitor::NotifyUncommentedLine( const std::string& line, const std::string& context )
{
    const mark_tag internal_tag( 1 );
    const mark_tag external_tag( 2 );
    const sregex spaces = *space;
    const sregex keyword = "#" >> spaces >> "include";
    const sregex rule = keyword >> spaces >>
                        ( ( '\"' >> ( internal_tag = ( *_ ) ) >> '\"' )
                        | ( '<'  >> ( external_tag = ( *_ ) ) >> '>' ) );
    sregex_iterator it( line.begin(), line.end(), rule );
    sregex_iterator end;
    if( it != end )
    {
        BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
        {
            if( !std::string( (*it)[ internal_tag ] ).empty() )
                observer->NotifyInternalInclude( (*it)[ internal_tag ], context ); // $$$$ _RC_ SLI 2010-08-19: seperate in two different visitors
            else
                observer->NotifyExternalInclude( (*it)[ external_tag ], context );
        }
    }
}
