//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "InternalModuleResolver.h"
#include "Finder_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;
using namespace depend;

namespace
{
    void AddPath( xml::xistream& xis, std::vector< std::string >& paths )
    {
        std::string path;
        xis >> path;
        paths.push_back( path );
    }
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver constructor
// Created: SLI 2011-02-16
// -----------------------------------------------------------------------------
InternalModuleResolver::InternalModuleResolver( xml::xisubstream xis, const Finder_ABC& finder, Subject< UnitObserver_ABC >& unitObserver )
    : Observer< UnitObserver_ABC >( unitObserver )
    , finder_( finder )
{
    xis >> xml::start( "paths" )
            >> xml::list( "path", boost::bind( &AddPath, _1, boost::ref( paths_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver destructor
// Created: SLI 2011-02-16
// -----------------------------------------------------------------------------
InternalModuleResolver::~InternalModuleResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver::Resolve
// Created: SLI 2011-02-17
// -----------------------------------------------------------------------------
std::string InternalModuleResolver::Resolve( const std::string& module, const std::string& file, const std::string& include ) const
{
    bfs::path fileContext = bfs::path( file ).parent_path();
    bfs::path includePath( include );
    BOOST_FOREACH( const std::string& path, paths_ )
    {
        if( finder_.Find( ( bfs::path( path ) / module / fileContext / includePath ).string() ) )
            return module;
    }
    const size_t position = include.find_first_of( '/' );
    const std::string unit = include.substr( 0, position );
    if( position != std::string::npos && units_.find( unit ) != units_.end() )
    {
        BOOST_FOREACH( const std::string& path, paths_ )
        {
            if( finder_.Find( ( bfs::path( path ) / includePath ).string() ) )
                return unit;
        }
    }
    return "";
}

// -----------------------------------------------------------------------------
// Name: InternalModuleResolver::NotifyUnit
// Created: SLI 2011-02-17
// -----------------------------------------------------------------------------
void InternalModuleResolver::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    units_.insert( unit );
}
