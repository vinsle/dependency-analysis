//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "Facade.h"
#include "ModuleVisitor.h"
#include "FileVisitor.h"
#include "LineVisitor.h"
#include "UncommentedLineVisitor.h"
#include "IncludeVisitor.h"
#include "ClassVisitor.h"
#include "ClassMetric.h"
#include "DependencyMetric.h"
#include "MetricSerializer.h"
#include "ModuleSerializer.h"
#include "StronglyConnectedComponents.h"
#include "DotSerializer.h"
#include "PngSerializer.h"
#include <boost/assign.hpp>
#include <xeumeuleu/xml.hpp>
#include <set>

using namespace depend;

namespace
{
    const std::vector< std::string > extensions = boost::assign::list_of( ".h" )( ".hh" )( ".hpp" )( ".hxx" )
                                                                        ( ".inl" )( ".ipp" )( ".cxx" )
                                                                        ( ".c" )( ".cc" )( ".cpp" );
}

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( const T_Filter& filter )
    : filter_                ( filter.begin(), filter.end() )
    , moduleVisitor_         ( new ModuleVisitor() )
    , fileVisitor_           ( new FileVisitor( extensions ) )
    , lineVisitor_           ( new LineVisitor() )
    , uncommentedLineVisitor_( new UncommentedLineVisitor( *lineVisitor_ ) )
    , includeVisitor_        ( new IncludeVisitor( *uncommentedLineVisitor_ ) )
    , classVisitor_          ( new ClassVisitor( *uncommentedLineVisitor_ ) )
    , classMetric_           ( new ClassMetric( *moduleVisitor_, *classVisitor_ ) )
    , dependencyMetric_      ( new DependencyMetric( *moduleVisitor_, *fileVisitor_, *includeVisitor_ ) )
    , moduleSerializer_      ( new ModuleSerializer( *moduleVisitor_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Facade destructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::~Facade()
{
    // NOTHING
}

namespace
{
    class FileObserver : private FileObserver_ABC
    {
    public:
        FileObserver( FileVisitor& fileVisitor, LineVisitor& lineVisitor )
            : fileVisitor_( fileVisitor )
            , lineVisitor_( lineVisitor )
        {
            fileVisitor_.Register( *this );
        }
        virtual ~FileObserver()
        {
            fileVisitor_.Unregister( *this );
        }
    private:
        virtual void NotifyFile( const std::string& /*path*/, std::istream& stream )
        {
            lineVisitor_.Visit( stream );
        }
    private:
        FileVisitor& fileVisitor_;
        LineVisitor& lineVisitor_;
    };

    class ModuleObserver : private ModuleObserver_ABC
    {
    public:
        ModuleObserver( ModuleVisitor& moduleVisitor, FileVisitor& fileVisitor, LineVisitor& lineVisitor, const std::string& path )
            : moduleVisitor_( moduleVisitor )
            , fileVisitor_  ( fileVisitor )
            , lineVisitor_  ( lineVisitor )
            , path_         ( path )
        {
            moduleVisitor_.Register( *this );
        }
        virtual ~ModuleObserver()
        {
            moduleVisitor_.Unregister( *this );
        }
    private:
        virtual void NotifyModule( const std::string& module )
        {
            FileObserver observer( fileVisitor_, lineVisitor_ );
            fileVisitor_.Visit( path_ + "/" + module );
        }
    private:
        ModuleVisitor& moduleVisitor_;
        FileVisitor& fileVisitor_;
        LineVisitor& lineVisitor_;
        const std::string path_;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::Visit
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Visit( const std::string& path )
{
    ModuleObserver observer( *moduleVisitor_, *fileVisitor_, *lineVisitor_, path );
    moduleVisitor_->Visit( path );
}

namespace
{
    template< typename T >
    bool Check( const T& filter, const std::string& module )
    {
        if( filter.empty() )
            return true;
        return std::find( filter.begin(), filter.end(), module ) != filter.end();
    }
    class FilterExtender : private DependencyMetricVisitor_ABC
    {
    public:
        FilterExtender( const DependencyMetric_ABC& metric, Facade::T_Filter& filter )
            : filter_( filter )
        {
            metric.Apply( *this );
            filter.insert( filter.end(), extended_.begin(), extended_.end() );
        }

    private:
        virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*include*/ )
        {
            if( Check( filter_, fromModule ) )
                extended_.insert( toModule );
            else if( Check( filter_, toModule ) )
                extended_.insert( fromModule );
        }
        virtual void NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/, const std::string& /*include*/ )
        {
            // NOTHING
        }

        Facade::T_Filter& filter_;
        std::set< std::string > extended_;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::Serialize
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void Facade::Serialize( xml::xostream& xos )
{
    xos << xml::start( "report" );
    FilterExtender( *dependencyMetric_, filter_ );
    moduleSerializer_->Serialize( xos, filter_ );
    MetricSerializer( *dependencyMetric_, *classMetric_ ).Serialize( xos, filter_ );
    StronglyConnectedComponents( *dependencyMetric_ ).Serialize( xos, filter_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Facade::Serialize
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
void Facade::Serialize( std::ostream& os )
{
    xml::xobufferstream xos;
    Serialize( xos );
    DotSerializer().Serialize( xos, os );
}

// -----------------------------------------------------------------------------
// Name: Facade::SerializePng
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
void Facade::Serialize( const std::string& filename )
{
    std::ostringstream buffer;
    Serialize( buffer );
    PngSerializer().Serialize( buffer.str(), filename );
}
