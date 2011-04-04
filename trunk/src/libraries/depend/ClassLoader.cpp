//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ClassLoader.h"
#include "UnitObserver_ABC.h"
#include "ClassObserver_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ClassLoader constructor
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
ClassLoader::ClassLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassLoader destructor
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
ClassLoader::~ClassLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassLoader::Process
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void ClassLoader::Process( xml::xisubstream xis )
{
    xis >> xml::start( "classes" )
            >> xml::list( "node", *this, &ClassLoader::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: ClassLoader::ReadNode
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void ClassLoader::ReadNode( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name" );
    BOOST_FOREACH( Subject< UnitObserver_ABC >::T_Observers::value_type& observer, Subject< UnitObserver_ABC >::observers_ )
        observer->NotifyUnit( name, "" );
    xis >> xml::list( "class", *this, &ClassLoader::ReadClass );
}

// -----------------------------------------------------------------------------
// Name: ClassLoader::ReadClass
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void ClassLoader::ReadClass( xml::xistream& xis )
{
    std::string name, context, abstractness;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "context", context )
        >> xml::attribute( "abstract", abstractness );
    BOOST_FOREACH( Subject< ClassObserver_ABC >::T_Observers::value_type& observer, Subject< ClassObserver_ABC >::observers_ )
        observer->NotifyClass( name, context );
    if( abstractness == "true" )
        BOOST_FOREACH( Subject< ClassObserver_ABC >::T_Observers::value_type& observer, Subject< ClassObserver_ABC >::observers_ )
            observer->NotifyAbstractness();
}
