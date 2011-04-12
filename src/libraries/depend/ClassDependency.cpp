//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ClassDependency.h"
#include "DependencyVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ClassDependency constructor
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
ClassDependency::ClassDependency( Subject< FileObserver_ABC >& fileVisitor, Subject< IncludeObserver_ABC >& includeVisitor )
    : Observer< FileObserver_ABC >( fileVisitor )
    , Observer< IncludeObserver_ABC >( includeVisitor )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassDependency destructor
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
ClassDependency::~ClassDependency()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassDependency::Apply
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependency::Apply( DependencyVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const T_File& file, files_ )
        BOOST_FOREACH( const T_Includes::value_type& dependency, file.second )
            if( file.first != dependency.first )
                visitor.NotifyInternalDependency( file.first, dependency.first, dependency.second );
}

namespace
{
    std::string RemoveExtension( const std::string& path )
    {
        return path.substr( 0, path.find_last_of( '.' ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ClassDependency::NotifyFile
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependency::NotifyFile( const std::string& path, std::istream& /*stream*/, const std::string& /*context*/ )
{
    const std::string cleaned = RemoveExtension( path );
    if( files_.empty() || cleaned != files_.back().first )
    {
        T_File file;
        file.first = cleaned;
        files_.push_back( file );
    }
}

// -----------------------------------------------------------------------------
// Name: ClassDependency::NotifyInternalInclude
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependency::NotifyInternalInclude( const std::string& file, const std::string& context )
{
    if( files_.empty() )
        throw std::runtime_error( "unknown include '" + file + "' out of file" );
    files_.back().second.insert( std::make_pair( RemoveExtension( file ), context ) );
}

// -----------------------------------------------------------------------------
// Name: ClassDependency::NotifyExternalInclude
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependency::NotifyExternalInclude( const std::string& /*file*/, const std::string& /*context*/ )
{
    // NOTHING
}
