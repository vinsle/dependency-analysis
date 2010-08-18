//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "LineVisitor.h"
#include "LineObserver_ABC.h"
#include <boost/foreach.hpp>
#include <string>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: LineVisitor constructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
LineVisitor::LineVisitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LineVisitor destructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
LineVisitor::~LineVisitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LineVisitor::Visit
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void LineVisitor::Visit( std::istream& stream )
{
    std::string line;
    while( std::getline( stream, line ) )
        BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
            observer->Notify( line );
}
