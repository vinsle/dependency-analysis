//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ClassVisitor.h"
#include "ClassObserver_ABC.h"
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
ClassVisitor::ClassVisitor( Subject< LineObserver_ABC >& visitor )
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

namespace
{
    template< typename T >
    bool DetectClass( const std::string& line, T& observers )
    {
        const mark_tag class_tag( 1 );
        const sregex spaces = *space;
        const sregex rule = bos >> spaces >> ( as_xpr( "class" ) | as_xpr( "struct" ) ) >> spaces >> ( class_tag = ( *( alnum | as_xpr( '_' ) ) ) ) >> spaces >> !( set = ':' )
            >> *_ >> ( ( '{' >> *_ >> '}' >> spaces >> ';' ) | ( spaces >> ~after( ';' ) ) ) >> spaces >> eos;
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
        const sregex rule = ')' >> spaces >> !as_xpr( "const" ) >> spaces >> '=' >> spaces >> '0' >> spaces >> *( set = ';' ) >> spaces >> eos;
        sregex_iterator it( line.begin(), line.end(), rule );
        sregex_iterator end;
        return  it != end;
    }
}

// -----------------------------------------------------------------------------
// Name: ClassVisitor::NotifyLine
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void ClassVisitor::NotifyLine( const std::string& line )
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
