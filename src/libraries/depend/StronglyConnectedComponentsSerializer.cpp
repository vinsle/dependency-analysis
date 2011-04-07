//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "StronglyConnectedComponentsSerializer.h"
#include "Visitable.h"
#include "ComponentVisitor_ABC.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponentsSerializer constructor
// Created: SLI 2011-04-07
// -----------------------------------------------------------------------------
StronglyConnectedComponentsSerializer::StronglyConnectedComponentsSerializer( const Visitable< StronglyConnectedComponentsVisitor_ABC >& components )
{
    components.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponentsSerializer destructor
// Created: SLI 2011-04-07
// -----------------------------------------------------------------------------
StronglyConnectedComponentsSerializer::~StronglyConnectedComponentsSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponentsSerializer::Serialize
// Created: SLI 2011-04-07
// -----------------------------------------------------------------------------
void StronglyConnectedComponentsSerializer::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "strongly-connected-components" );
    BOOST_FOREACH( const T_Component& component, components_ )
    {
        xos << xml::start( "component" );
        BOOST_FOREACH( const std::string& unit, component )
            xos << xml::content( "node", unit );
        xos << xml::end;
    }
    xos << xml::end;
}

namespace
{
    class ComponentVisitor : public ComponentVisitor_ABC
    {
    public:
        explicit ComponentVisitor( std::vector< std::string >& component )
            : component_( component )
        {}
        void NotifyUnit( const std::string& unit )
        {
            component_.push_back( unit );
        }
    private:
        std::vector< std::string >& component_;
    };
}

// -----------------------------------------------------------------------------
// Name: StronglyConnectedComponentsSerializer::NotifyComponent
// Created: SLI 2011-04-07
// -----------------------------------------------------------------------------
void StronglyConnectedComponentsSerializer::NotifyComponent( const Visitable< ComponentVisitor_ABC >& component )
{
    components_.push_back( T_Component() );
    ComponentVisitor visitor( components_.back() );
    component.Apply( visitor );
}
