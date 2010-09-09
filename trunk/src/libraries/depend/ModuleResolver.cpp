//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ModuleResolver.h"
#include "Finder_ABC.h"
#include <boost/foreach.hpp>
#pragma warning( push, 0 )
#pragma warning( disable: 4512 4996 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace depend;

namespace
{
    template< typename T, typename U >
    U Parse( const T& directories )
    {
        U result;
        BOOST_FOREACH( const std::string& input, directories )
        {
            if( input.empty() )
                throw std::invalid_argument( "empty directory definition is invalid" );
            std::vector< std::string > buffer;
            boost::algorithm::split( buffer, input, boost::algorithm::is_any_of( "," ) );
            if( buffer.size() > 2 )
                throw std::invalid_argument( "multiple module name definition is invalid '" + input + "'" );
            const std::string directory = boost::algorithm::ends_with( buffer.front(), "/" ) ?
                                          boost::algorithm::erase_last_copy( buffer.front(), "/" ) :
                                          buffer.front();
            const std::string forced = buffer.size() > 1 ? buffer.at( 1 ) : "";
            result.push_back( std::make_pair( directory, forced ) );
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleResolver constructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
ModuleResolver::ModuleResolver( const T_Directories& directories, const Finder_ABC& finder )
    : directories_( Parse< T_Directories, T_NamedDirectories >( directories ) )
    , finder_     ( finder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleResolver destructor
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
ModuleResolver::~ModuleResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleResolver::Resolve
// Created: SLI 2010-09-09
// -----------------------------------------------------------------------------
std::string ModuleResolver::Resolve( const std::string& include ) const
{
    BOOST_FOREACH( const T_NamedDirectory& directory, directories_ )
        if( finder_.Find( directory.first + "/" + include ) )
        {
            if( !directory.second.empty() )
                return directory.second;
            const size_t position = include.find_first_of( '/' );
            if( position != std::string::npos )
                return include.substr( 0, position );
            const size_t pos = directory.first.find_last_of( '/' );
            return pos == std::string::npos ? directory.first : directory.first.substr( pos + 1, std::string::npos );
        }
    return "";
}
