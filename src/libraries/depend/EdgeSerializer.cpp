//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "EdgeSerializer.h"
#include "Filter.h"
#include "Visitable.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: EdgeSerializer constructor
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
EdgeSerializer::EdgeSerializer( const Visitable< DependencyVisitor_ABC >& dependencies, const Visitable< UnitObserver_ABC >& units )
{
    units.Apply( *this );
    dependencies.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: EdgeSerializer destructor
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
EdgeSerializer::~EdgeSerializer()
{
    // NOTHING
}

namespace
{
    template< typename T, typename U >
    void SerializeDependency( xml::xostream& xos, const std::string& module, const T& dependencies, const Filter_ABC& filter )
    {
        T::const_iterator it = dependencies.find( module );
        if( it == dependencies.end() )
            return;
        BOOST_FOREACH( const U::value_type& to, it->second )
            if( filter.Check( to.first ) )
            {
                xos << xml::start( "dependency" )
                        << xml::attribute( "name", to.first );
                BOOST_FOREACH( const std::string& context, to.second )
                    xos << xml::content( "context", context );
                xos << xml::end;
            }
    }
    class NullFilter : public Filter_ABC
    {
    public:
        NullFilter() {}
        virtual bool Check( const std::string& /*module*/ ) const
        {
            return true;
        }
        virtual bool CheckCore( const std::string& /*module*/ ) const
        {
            return true;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: EdgeSerializer::Serialize
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void EdgeSerializer::Serialize( xml::xostream& xos, const Filter_ABC& filter ) const
{
    xos << xml::start( "nodes" );
    BOOST_FOREACH( const std::string& module, modules_ )
    {
        if( filter.Check( module ) )
        {
            xos << xml::start( "node" )
                    << xml::attribute( "name", module )
                    << xml::start( "efferent-dependencies" );
            SerializeDependency< T_Dependencies, T_Dependency >( xos, module, efferent_, filter ); // $$$$ _RC_ SLI 2011-03-21: remove templates
            xos     << xml::end
                    << xml::start( "external-dependencies" );
            SerializeDependency< T_Dependencies, T_Dependency >( xos, module, external_, NullFilter() );
            xos     << xml::end
                << xml::end;
        }
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: EdgeSerializer::NotifyInternalDependency
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void EdgeSerializer::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context )
{
    modules_.insert( fromModule );
    modules_.insert( toModule );
    efferent_[ fromModule ][ toModule ].push_back( context );
}

// -----------------------------------------------------------------------------
// Name: EdgeSerializer::NotifyExternalDependency
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void EdgeSerializer::NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context )
{
    modules_.insert( fromModule );
    external_[ fromModule ][ toModule ].push_back( context );
}

// -----------------------------------------------------------------------------
// Name: EdgeSerializer::NotifyUnit
// Created: SLI 2011-04-04
// -----------------------------------------------------------------------------
void EdgeSerializer::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    modules_.insert( unit );
}
