//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "FileVisitor.h"
#include "FileObserver_ABC.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: FileVisitor constructor
// Created: SLI 2010-08-16
// -----------------------------------------------------------------------------
FileVisitor::FileVisitor( const T_Extensions& extensions )
    : extensions_( extensions )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileVisitor destructor
// Created: SLI 2010-08-16
// -----------------------------------------------------------------------------
FileVisitor::~FileVisitor()
{
    // NOTHING
}

namespace
{
    std::string Relative( const boost::filesystem::path& parent, const boost::filesystem::path& current )
    {
        return boost::erase_all_copy( current.string(), parent.string() );
    }
    bool IsValid( const boost::filesystem::path& path )
    {
        return !boost::algorithm::starts_with( path.filename(), "." );
    }
    template< typename T >
    bool CheckExtension( const boost::filesystem::path& path, const T& extensions )
    {
        return std::find( extensions.begin(), extensions.end(), path.extension() ) != extensions.end();
    }
    template< typename T, typename U >
    void Visit( const boost::filesystem::path& root, const boost::filesystem::path& current, T& observers, const U& extensions )
    {
        for( boost::filesystem::directory_iterator it( current ); it != boost::filesystem::directory_iterator(); ++it )
            if( IsValid( *it ) )
                if( boost::filesystem::is_directory( *it ) )
                    Visit( root, *it, observers, extensions );
                else
                    if( CheckExtension( *it, extensions ) )
                        BOOST_FOREACH( T::value_type& observer, observers )
                            observer->Notify( Relative( root, *it ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FileVisitor::Visit
// Created: SLI 2010-08-16
// -----------------------------------------------------------------------------
void FileVisitor::Visit( const std::string& filename )
{
    const boost::filesystem::path root( filename );
    ::Visit( root, root, observers_, extensions_ );
}

// -----------------------------------------------------------------------------
// Name: FileVisitor::Register
// Created: SLI 2010-08-16
// -----------------------------------------------------------------------------
void FileVisitor::Register( FileObserver_ABC& oberver )
{
    observers_.push_back( &oberver );
}

// -----------------------------------------------------------------------------
// Name: FileVisitor::Unregister
// Created: SLI 2010-08-16
// -----------------------------------------------------------------------------
void FileVisitor::Unregister( FileObserver_ABC& observer )
{
    observers_.erase( std::remove( observers_.begin(), observers_.end(), &observer ), observers_.end() );
}
