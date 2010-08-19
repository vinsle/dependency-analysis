//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "UncommentedLineVisitor.h"
#include "UncommentedLineObserver_ABC.h"
#include <boost/foreach.hpp>
#pragma warning( push, 0 )
#pragma warning( disable: 4996 )
#include <boost/xpressive/xpressive.hpp>
#pragma warning( pop )

using namespace depend;
using namespace boost::xpressive;

// -----------------------------------------------------------------------------
// Name: UncommentedLineVisitor constructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
UncommentedLineVisitor::UncommentedLineVisitor( Subject< LineObserver_ABC >& visitor )
    : visitor_( visitor )
{
    visitor_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UncommentedLineVisitor destructor
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
UncommentedLineVisitor::~UncommentedLineVisitor()
{
    visitor_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UncommentedLineVisitor::NotifyLine
// Created: SLI 2010-08-19
// -----------------------------------------------------------------------------
void UncommentedLineVisitor::NotifyLine( const std::string& line )
{
    const sregex statement = *_;
    const sregex comment = ( as_xpr( "/*" ) | as_xpr( "//" ) ) >> statement >> eos;
    std::string output = regex_replace( line, comment, "" );
    if( !output.empty() )
        BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
            observer->NotifyUncommentedLine( output );
}
