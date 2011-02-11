//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ClassDependencyMetric.h"
#include "DependencyMetricVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric constructor
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
ClassDependencyMetric::ClassDependencyMetric( Subject< FileObserver_ABC >& fileVisitor, Subject< IncludeObserver_ABC >& includeVisitor )
    : Observer< FileObserver_ABC >( fileVisitor )
    , Observer< IncludeObserver_ABC >( includeVisitor )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric destructor
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
ClassDependencyMetric::~ClassDependencyMetric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric::Apply
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::Apply( DependencyMetricVisitor_ABC& visitor ) const
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
// Name: ClassDependencyMetric::NotifyFile
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyFile( const std::string& path, std::istream& /*stream*/, const std::string& /*context*/ )
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
// Name: ClassDependencyMetric::NotifyInternalInclude
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyInternalInclude( const std::string& file, const std::string& context )
{
    if( files_.empty() )
        throw std::runtime_error( "unknown include '" + file + "' out of file" );
    files_.back().second.insert( std::make_pair( RemoveExtension( file ), context ) );
}

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric::NotifyExternalInclude
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyExternalInclude( const std::string& /*file*/, const std::string& /*context*/ )
{
    // NOTHING
}
