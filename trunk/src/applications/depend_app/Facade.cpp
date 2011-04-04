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
#include "depend/ClassVisitor.h"
#include "depend/ModuleDependencyMetric.h"
#include "depend/EdgeSerializer.h"
#include "depend/Log.h"
#include "depend/Finder.h"
#include "depend/ExternalModuleResolver.h"
#include "depend/InternalModuleResolver.h"
#include "depend/ProxyModuleResolver.h"
#include "depend/Filter_ABC.h"
#include "depend/UnitCache.h"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <xeumeuleu/xml.hpp>
#include <iostream>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( xml::xisubstream xis )
    : log_                   ( new Log( xis >> xml::start( "configuration" ) ) )
    , finder_                ( new Finder() )
    , externalResolver_      ( new ExternalModuleResolver( xis, *finder_, *log_ ) )
    , proxy_                 ( new ProxyModuleResolver( *externalResolver_ ) )
    , moduleVisitor_         ( new ModuleVisitor() )
    , unitCache_             ( new UnitCache( *moduleVisitor_ ) )
    , fileVisitor_           ( new FileVisitor( xis ) )
    , lineVisitor_           ( new LineVisitor() )
    , uncommentedLineVisitor_( new UncommentedLineVisitor( *lineVisitor_ ) )
    , includeVisitor_        ( new IncludeVisitor( *uncommentedLineVisitor_ ) )
    , classVisitor_          ( new ClassVisitor( *uncommentedLineVisitor_ ) )
    , internalResolver_      ( new InternalModuleResolver( xis, *finder_, *moduleVisitor_ ) )
    , dependencyMetric_      ( new ModuleDependencyMetric( *moduleVisitor_, *fileVisitor_, *includeVisitor_, *proxy_, *internalResolver_, *log_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Facade::Process
// Created: SLI 2010-09-10
// -----------------------------------------------------------------------------
void Facade::Process( xml::xisubstream xis )
{
    xis >> xml::start( "configuration" )
            >> xml::start( "paths" )
                >> xml::list( "path", *this, &Facade::Visit )
            >> xml::end;
    Serialize( xis );
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
        virtual void NotifyFile( const std::string& /*path*/, std::istream& stream, const std::string& context )
        {
            lineVisitor_.Visit( stream, context );
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
        virtual void NotifyUnit( const std::string& unit, const std::string& context )
        {
            FileObserver observer( fileVisitor_, lineVisitor_ );
            fileVisitor_.Visit( path_ + "/" + unit, context );
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
void Facade::Visit( xml::xistream& xis )
{
    const std::string path = xis.value< std::string >();
    ModuleObserver observer( *moduleVisitor_, *fileVisitor_, *lineVisitor_, path );
    moduleVisitor_->Visit( path, path );
}

namespace
{
    void Noop() {}
    class SimpleFilter : public Filter_ABC
    {
    public:
        virtual bool Check( const std::string& /*module*/ ) const
        {
            return true;
        }
        virtual bool CheckCore( const std::string& /*module*/ ) const
        {
            return true;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::Serialize
// Created: SLI 2010-09-03
// -----------------------------------------------------------------------------
void Facade::Serialize( xml::xistream& xis )
{
    const std::string output = xis.content< std::string >( "output" );
    boost::shared_ptr< std::ostream > out( &std::cout, boost::bind( &Noop ) );
    if( !output.empty() )
        out.reset( new std::ofstream( output.c_str() ) );
    xml::xostringstream xos;
    EdgeSerializer( *dependencyMetric_, *unitCache_ ).Serialize( xos, SimpleFilter() );
    *out << xos.str();
}
