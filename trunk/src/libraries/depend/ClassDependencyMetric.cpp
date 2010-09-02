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
ClassDependencyMetric::ClassDependencyMetric( Subject< FileObserver_ABC >& fileVisitor, Subject< ClassObserver_ABC >& classVisitor,
                                              Subject< IncludeObserver_ABC >& includeVisitor )
    : Observer< FileObserver_ABC >( fileVisitor )
    , Observer< ClassObserver_ABC >( classVisitor )
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
        if( !file.second.name_.empty() )
            BOOST_FOREACH( const std::string& dependency, file.second.includes_ )
                if( file.second.name_ != dependency )
                    visitor.NotifyInternalDependency( file.second.name_, dependency, dependency );
}

namespace
{
    std::string GetFilename( const std::string& path )
    {
        const size_t position = path.find_last_of( '/' );
        return path.substr( position == std::string::npos ? 0 : position + 1, std::string::npos );
    }
    std::string RemoveExtension( const std::string& path )
    {
        return path.substr( 0, path.find_last_of( '.' ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric::NotifyFile
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyFile( const std::string& path, std::istream& /*stream*/ )
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
// Name: ClassDependencyMetric::NotifyClass
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyClass( const std::string& name )
{
    if( files_.empty() )
        throw std::runtime_error( "unknown class '" + name + "' out of file" );
    if( GetFilename( files_.back().first ) == name )
        files_.back().second.name_ = name;
}

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric::NotifyAbstractness
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyAbstractness()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric::NotifyInternalInclude
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyInternalInclude( const std::string& file )
{
    if( files_.empty() )
        throw std::runtime_error( "unknown include '" + file + "' out of file" );
    files_.back().second.includes_.insert( RemoveExtension( GetFilename( file ) ) );
}

// -----------------------------------------------------------------------------
// Name: ClassDependencyMetric::NotifyExternalInclude
// Created: SLI 2010-09-01
// -----------------------------------------------------------------------------
void ClassDependencyMetric::NotifyExternalInclude( const std::string& file )
{

}
