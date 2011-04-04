//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ClassSerializer.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ClassSerializer constructor
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
ClassSerializer::ClassSerializer( Subject< UnitObserver_ABC >& unitSubject, Subject< ClassObserver_ABC >& classObserver )
    : Observer< UnitObserver_ABC >( unitSubject )
    , Observer< ClassObserver_ABC >( classObserver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassSerializer destructor
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
ClassSerializer::~ClassSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassSerializer::Serialize
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void ClassSerializer::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "classes" );
    BOOST_FOREACH( const T_Unit& unit, units_ )
    {
        xos << xml::start( "node" )
                << xml::attribute( "name", unit.first );
        BOOST_FOREACH( const T_Class& itClass, unit.second )
            xos << xml::start( "class" )
                    << xml::attribute( "abstract", itClass.abstractness_ )
                    << xml::attribute( "context", itClass.context_ )
                    << xml::attribute( "name", itClass.name_ )
                << xml::end;
        xos << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ClassSerializer::NotifyUnit
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void ClassSerializer::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    units_.push_back( std::make_pair( unit, T_Classes() ) );
}

// -----------------------------------------------------------------------------
// Name: ClassSerializer::NotifyClass
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void ClassSerializer::NotifyClass( const std::string& name, const std::string& context )
{
    if( units_.empty() )
        throw std::runtime_error( "Unknown class '" + name + "' out of module. " + context );
    units_.back().second.push_back( T_Class( name, context ) );
}

// -----------------------------------------------------------------------------
// Name: ClassSerializer::NotifyAbstractness
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void ClassSerializer::NotifyAbstractness()
{
    if( units_.empty() )
        throw std::runtime_error( "Abstractness detected without associated class" );
    if( units_.back().second.empty() )
        throw std::runtime_error( "Abstractness detected without associated class" );
    units_.back().second.back().abstractness_ = "true";
}
