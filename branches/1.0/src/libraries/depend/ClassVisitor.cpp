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

using namespace depend;
using namespace boost::xpressive;

// -----------------------------------------------------------------------------
// Name: ClassVisitor constructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
ClassVisitor::ClassVisitor( Subject< UncommentedLineObserver_ABC >& visitor )
    : Observer< UncommentedLineObserver_ABC >( visitor )
    , insideClass_( false )
    , abstract_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassVisitor destructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
ClassVisitor::~ClassVisitor()
{
    // NOTHING
}

namespace
{
    template< typename T >
    bool DetectClass( const std::string& line, T& observers, const std::string& context )
    {
        const mark_tag class_tag( 1 );
        const sregex spaces = *space;
        const sregex identifier = *( alnum | as_xpr( '_' ) );
        const sregex templates = as_xpr( '<' ) >> *_ >> as_xpr( '>' );
        const sregex classKeyword = as_xpr( "class" ) | as_xpr( "struct" );
        const sregex rule = classKeyword >> spaces >> *templates >> spaces >> ( class_tag = ( identifier ) ) >> spaces >> ( ( set = ':', '{' ) | eos );
        sregex_iterator it( line.begin(), line.end(), rule );
        sregex_iterator end;
        if( it != end )
        {
            BOOST_FOREACH( typename T::value_type& observer, observers )
                observer->NotifyClass( (*it)[ class_tag ], context );
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
// Name: ClassVisitor::NotifyUncommentedLine
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void ClassVisitor::NotifyUncommentedLine( const std::string& line, const std::string& context )
{
    if( DetectClass( line, observers_, context ) )
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
