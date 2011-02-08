//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "depend/ModuleVisitor.h"
#include "depend/FileVisitor.h"
#include "depend/LineVisitor.h"
#include "depend/UncommentedLineVisitor.h"
#include "depend/IncludeVisitor.h"
#include "depend/ModuleDependencyMetric.h"
#include "depend/Log.h"
#include "depend/Finder.h"
#include "depend/ModuleResolver.h"
#include "depend/ProxyModuleResolver.h"
#include "depend/ModuleDependencyGuard.h"
#include <xeumeuleu/xml.hpp>
#include <iostream>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( xml::xisubstream xis )
    : log_                   ( new Log( xis ) )
    , finder_                ( new Finder() )
    , resolver_              ( new ModuleResolver( xis, *finder_ ) )
    , proxy_                 ( new ProxyModuleResolver( *resolver_ ) )
    , moduleVisitor_         ( new ModuleVisitor() )
    , fileVisitor_           ( new FileVisitor( xis ) )
    , lineVisitor_           ( new LineVisitor() )
    , uncommentedLineVisitor_( new UncommentedLineVisitor( *lineVisitor_ ) )
    , includeVisitor_        ( new IncludeVisitor( *uncommentedLineVisitor_ ) )
    , dependencyMetric_      ( new ModuleDependencyMetric( *moduleVisitor_, *fileVisitor_, *includeVisitor_, *proxy_, *log_ ) )
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
    class FileObserver : private Observer< FileObserver_ABC >
    {
    public:
        FileObserver( FileVisitor& fileVisitor, LineVisitor& lineVisitor )
            : Observer< FileObserver_ABC >( fileVisitor )
            , lineVisitor_( lineVisitor )
        {
            // NOTHING
        }
        virtual ~FileObserver()
        {
            // NOTHING
        }
    private:
        virtual void NotifyFile( const std::string& /*path*/, std::istream& stream )
        {
            lineVisitor_.Visit( stream );
        }
    private:
        LineVisitor& lineVisitor_;
    };

    class ModuleObserver : private Observer< UnitObserver_ABC >
    {
    public:
        ModuleObserver( ModuleVisitor& unitVisitor, FileVisitor& fileVisitor, LineVisitor& lineVisitor, const std::string& path )
            : Observer< UnitObserver_ABC >( unitVisitor )
            , fileVisitor_( fileVisitor )
            , lineVisitor_( lineVisitor )
            , path_       ( path )
        {
             // NOTHING
        }
        virtual ~ModuleObserver()
        {
            // NOTHING
        }
    private:
        virtual void NotifyUnit( const std::string& unit )
        {
            FileObserver observer( fileVisitor_, lineVisitor_ );
            fileVisitor_.Visit( path_ + "/" + unit );
        }
    private:
        FileVisitor& fileVisitor_;
        LineVisitor& lineVisitor_;
        const std::string path_;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::Visit
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Visit( const std::string& path ) const
{
    ModuleObserver observer( *moduleVisitor_, *fileVisitor_, *lineVisitor_, path );
    moduleVisitor_->Visit( path );
}

namespace
{
    class FailuresChecker : public DependencyMetricVisitor_ABC
    {
    public:
        FailuresChecker()
            : failure_( false )
        {}
        virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule )
        {
            std::cerr << "Error: dependency from module '" << fromModule << "' to module '" << toModule << "' is forbidden." << std::endl;
            failure_ = true;
        }
        virtual void NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/ )
        {
            // NOTHING
        }
        bool failure_;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::Process
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
bool Facade::Process( xml::xisubstream xis ) const
{
    ModuleDependencyGuard guard( xis, *dependencyMetric_ );
    FailuresChecker checker;
    guard.Apply( checker );
    return checker.failure_;
}
