//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ClassVisitor.h"
#include "LineVisitor_ABC.h"
#include "ClassObserver_ABC.h"
#include <algorithm>
#include <boost/foreach.hpp>
#pragma warning( push, 0 )
#pragma warning( disable: 4996 )
#include <boost/xpressive/xpressive.hpp>
#pragma warning( pop )

using namespace depend;
using namespace boost::xpressive;

// -----------------------------------------------------------------------------
// Name: ClassVisitor constructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
ClassVisitor::ClassVisitor( LineVisitor_ABC& visitor )
    : visitor_    ( visitor )
    , insideClass_( false )
    , abstract_   ( false )
{
    visitor_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ClassVisitor destructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
ClassVisitor::~ClassVisitor()
{
    visitor_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ClassVisitor::Register
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void ClassVisitor::Register( ClassObserver_ABC& observer )
{
    observers_.push_back( &observer );
}

// -----------------------------------------------------------------------------
// Name: ClassVisitor::Unregister
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void ClassVisitor::Unregister( ClassObserver_ABC& observer )
{
    observers_.erase( std::remove( observers_.begin(), observers_.end(), &observer ), observers_.end() );
}

namespace
{
    template< typename T >
    bool DetectClass( const std::string& line, T& observers )
    {
        const mark_tag class_tag( 1 );
        const sregex spaces = *space;
        const sregex rule = bos >> spaces >> "class" >> spaces >> ( class_tag = ( *alnum ) ) >> spaces >> *( set = ':' );
        sregex_iterator it( line.begin(), line.end(), rule );
        sregex_iterator end;
        if( it != end )
        {
            BOOST_FOREACH( T::value_type& observer, observers )
                observer->NotifyClass( (*it)[ class_tag ] );
            return true;
        }
        return false;
    }
    bool DetectAbstractness( const std::string& line )
    {
        const sregex spaces = *space;
        const sregex rule = ')' >> spaces >> repeat< 0, 1 >( "const" ) >> spaces >> '=' >> spaces >> '0' >> spaces >> *( set = ';' ) >> spaces >> eos;
        sregex_iterator it( line.begin(), line.end(), rule );
        sregex_iterator end;
        return  it != end;
    }
}

// -----------------------------------------------------------------------------
// Name: ClassVisitor::Notify
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void ClassVisitor::Notify( const std::string& line )
{
    if( DetectClass( line, observers_ ) )
    {
        insideClass_ = true;
        abstract_ = false;
    }
    else if( insideClass_ && !abstract_ && DetectAbstractness( line ) )
    {
        abstract_ = true;
        BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
            observer->NotifyAbstractness();
    }
}
