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
#include "istream_guard.h"
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace depend;

namespace
{
    typedef std::vector< std::string > T_Extensions;
    void ReadExtension( xml::xistream& xis, T_Extensions& extensions )
    {
        extensions.push_back( xis.value< std::string >() );
    }
    T_Extensions ReadExtensions( xml::xisubstream xis )
    {
        T_Extensions result;
        xis >> xml::start( "extensions" )
                >> xml::list( "extension", boost::bind( &ReadExtension, _1, boost::ref( result ) ) );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: FileVisitor constructor
// Created: SLI 2010-08-16
// -----------------------------------------------------------------------------
FileVisitor::FileVisitor( xml::xisubstream xis )
    : extensions_( ReadExtensions( xis ) )
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
        return extensions.empty() || std::find( extensions.begin(), extensions.end(), path.extension() ) != extensions.end();
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
                    {
                        std::ifstream ifs( it->string().c_str() );
                        if( !ifs )
                            throw std::runtime_error( "could not open file '" + it->string() + "'" );
                        BOOST_FOREACH( T::value_type& observer, observers )
                        {
                            istream_guard guard( ifs );
                            observer->NotifyFile( Relative( root, *it ), ifs );
                        }
                    }
    }
}

// -----------------------------------------------------------------------------
// Name: FileVisitor::Visit
// Created: SLI 2010-08-16
// -----------------------------------------------------------------------------
void FileVisitor::Visit( const std::string& path )
{
    const boost::filesystem::path root( path + "/" );
    ::Visit( root, root, observers_, extensions_ );
}
