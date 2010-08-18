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
#include "IncludeVisitor.h"
#include "ModuleObserver_ABC.h"
#include "FileObserver_ABC.h"
#include "LineObserver_ABC.h"
#include "IncludeObserver_ABC.h"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

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
    : moduleVisitor_ ( new ModuleVisitor() )
    , fileVisitor_   ( new FileVisitor( extensions ) )
    , lineVisitor_   ( new LineVisitor() )
    , includeVisitor_( new IncludeVisitor( *lineVisitor_ ) )
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
            //IncludeVisitor includeVisitor( visitor );
            //IncludeObserver observer( includeVisitor );
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
// Name: Facade::Register
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Register( ModuleObserver_ABC& observer )
{
    moduleVisitor_->Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Facade::Register
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Register( FileObserver_ABC& observer )
{
    fileVisitor_->Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Facade::Register
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Register( LineObserver_ABC& observer )
{
    lineVisitor_->Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Facade::Register
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Register( IncludeObserver_ABC& observer )
{
    includeVisitor_->Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Facade::Unregister
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Unregister( ModuleObserver_ABC& observer )
{
    moduleVisitor_->Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: Facade::Unregister
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Unregister( FileObserver_ABC& observer )
{
    fileVisitor_->Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: Facade::Unregister
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Unregister( LineObserver_ABC& observer )
{
    lineVisitor_->Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: Facade::Unregister
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Unregister( IncludeObserver_ABC& observer )
{
    includeVisitor_->Unregister( observer );
}
