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
#include <boost/assign.hpp>
#include <xeumeuleu/xml.hpp>

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
Facade::Facade()
    : moduleVisitor_         ( new ModuleVisitor() )
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

// -----------------------------------------------------------------------------
// Name: Facade::Serialize
// Created: SLI 2010-08-20
// -----------------------------------------------------------------------------
void Facade::Serialize( xml::xostream& xos )
{
    xos << xml::start( "report" );
    moduleSerializer_->Serialize( xos );
    MetricSerializer( *dependencyMetric_, *classMetric_ ).Serialize( xos );
    StronglyConnectedComponents( *dependencyMetric_ ).Serialize( xos );
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
    const DotSerializer().Serialize( xos, os );
}
