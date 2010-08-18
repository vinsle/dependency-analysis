//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ModuleVisitor.h"
#include "ModuleObserver_ABC.h"
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleVisitor constructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
ModuleVisitor::ModuleVisitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleVisitor destructor
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
ModuleVisitor::~ModuleVisitor()
{
    // NOTHING
}

namespace
{
    bool IsDirectory( const boost::filesystem::path& path )
    {
        return boost::filesystem::is_directory( path ) && !boost::algorithm::starts_with( path.filename(), "." );
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleVisitor::Visit
// Created: SLI 2010-08-17
// -----------------------------------------------------------------------------
void ModuleVisitor::Visit( const std::string& filename )
{
    const boost::filesystem::path path( filename + "/" );
    for( boost::filesystem::directory_iterator it( path ); it != boost::filesystem::directory_iterator(); ++it )
        if( IsDirectory( *it ) )
            BOOST_FOREACH( T_Observers::value_type& observer, observers_ )
                observer->Notify( it->filename() );
}
