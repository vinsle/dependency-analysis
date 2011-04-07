//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "GraphSerializer.h"
#include "depend/ModuleVisitor.h"
#include "depend/FileVisitor.h"
#include "depend/LineVisitor.h"
#include "depend/UncommentedLineVisitor.h"
#include "depend/IncludeVisitor.h"
#include "depend/ClassVisitor.h"
#include "depend/ClassMetric.h"
#include "depend/ModuleDependencyMetric.h"
#include "depend/EdgeSerializer.h"
#include "depend/MetricSerializer.h"
#include "depend/UnitSerializer.h"
#include "depend/ExternalSerializer.h"
#include "depend/StronglyConnectedComponents.h"
#include "depend/DotSerializer.h"
#include "depend/Filter.h"
#include "depend/Log.h"
#include "depend/Finder.h"
#include "depend/ExternalModuleResolver.h"
#include "depend/InternalModuleResolver.h"
#include "depend/ProxyModuleResolver.h"
#include "depend/TransitiveReductionFilter.h"
#include "depend/UnitCache.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#include <set>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( xml::xisubstream xis )
    : option_                ( ( xis >> xml::start( "configuration" ) ).content< std::string >( "dependencies" ) )
    , extend_                ( xis.content< bool >( "extend" ) )
    , log_                   ( new Log( xis ) )
    , filter_                ( new Filter( xis ) )
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
    , classMetric_           ( new ClassMetric( *moduleVisitor_, *classVisitor_ ) )
    , internalResolver_      ( new InternalModuleResolver( xis, *finder_, *moduleVisitor_ ) )
    , dependencyMetric_      ( new ModuleDependencyMetric( *moduleVisitor_, *fileVisitor_, *includeVisitor_, *proxy_, *internalResolver_, *log_ ) )
    , unitSerializer_        ( new UnitSerializer( *moduleVisitor_ ) )
    , graphSerializer_       ( new GraphSerializer( xis ) )
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
        ModuleObserver( ModuleVisitor& unitVisitor, FileVisitor& fileVisitor, LineVisitor& lineVisitor, const std::string& path, std::vector< std::string >& units )
            : Observer< UnitObserver_ABC >( unitVisitor )
            , fileVisitor_  ( fileVisitor )
            , lineVisitor_  ( lineVisitor )
            , path_         ( path )
            , units_        ( units )
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
            units_.push_back( unit );
        }
    private:
        FileVisitor& fileVisitor_;
        LineVisitor& lineVisitor_;
        const std::string path_;
        std::vector< std::string >& units_;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::Visit
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Visit( xml::xistream& xis )
{
    const std::string path = xis.value< std::string >();
    ModuleObserver observer( *moduleVisitor_, *fileVisitor_, *lineVisitor_, path, modules_ );
    moduleVisitor_->Visit( path, path );
}

namespace
{
    void Noop() {}
}

// -----------------------------------------------------------------------------
// Name: Facade::Serialize
// Created: SLI 2010-09-03
// -----------------------------------------------------------------------------
void Facade::Serialize( xml::xistream& xis )
{
    const std::string stage = xis.content< std::string >( "stage" );
    const std::string output = xis.content< std::string >( "output" );
    const bool all = xis.content< bool >( "all" );
    boost::shared_ptr< std::ostream > out( &std::cout, boost::bind( &Noop ) );
    if( !output.empty() )
        out.reset( new std::ofstream( output.c_str() ) );
    if( stage == "xml" )
    {
        xml::xostringstream xos;
        Serialize( xos );
        *out << xos.str();
    }
    else if( stage == "dot" )
        Serialize( *out );
    else if( stage == "graph" )
    {
        Serialize( output );
        if( all )
            SerializeAll( output );
    }
}

namespace
{
    class FilterExtender : public Filter_ABC, private DependencyMetricVisitor_ABC
    {
    public:
        FilterExtender( const Visitable< DependencyMetricVisitor_ABC >& metric, const Filter_ABC& filter )
            : filter_( filter )
        {
            metric.Apply( *this );
        }
        virtual bool Check( const std::string& module ) const
        {
            return CheckCore( module ) || extended_.find( module ) != extended_.end();
        }
        virtual bool CheckCore( const std::string& module ) const
        {
            return filter_.Check( module );
        }
    private:
        virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& /*context*/ )
        {
            if( filter_.Check( fromModule ) )
                extended_.insert( toModule );
            else if( filter_.Check( toModule ) )
                extended_.insert( fromModule );
        }
        virtual void NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/, const std::string& /*context*/ )
        {
            // NOTHING
        }
        const Filter_ABC& filter_;
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
    TransitiveReductionFilter transitive( *dependencyMetric_, *filter_ );
    FilterExtender extension( *dependencyMetric_, *filter_ );
    Filter_ABC& filter = extend_ ? static_cast< Filter_ABC& >( transitive ) : static_cast< Filter_ABC& >( extension );
    unitSerializer_->Serialize( xos, filter );
    ExternalSerializer( *dependencyMetric_, filter ).Serialize( xos );
    xos << xml::start( "graph" );
    EdgeSerializer( *dependencyMetric_, *unitCache_ ).Serialize( xos, filter );
    xos << xml::end;
    MetricSerializer( *dependencyMetric_, *classMetric_ ).Serialize( xos, filter );
    StronglyConnectedComponents( *dependencyMetric_ ).Serialize( xos, filter );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Facade::Serialize
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
void Facade::Serialize( std::ostream& os )
{
    xml::xobufferstream xos;
    Serialize( static_cast< xml::xostream& >( xos ) );
    DotSerializer().Serialize( xos, os, boost::lexical_cast< DotOption >( option_ ) );
}

// -----------------------------------------------------------------------------
// Name: Facade::SerializeGraph
// Created: SLI 2010-08-27
// -----------------------------------------------------------------------------
void Facade::Serialize( const std::string& filename )
{
    std::ostringstream buffer;
    Serialize( buffer );
    graphSerializer_->Serialize( buffer.str(), filename );
}

namespace
{
    class SimpleFilter : public Filter_ABC
    {
    public:
        explicit SimpleFilter( const std::string& module )
            : module_( module )
        {}
        virtual bool Check( const std::string& module ) const
        {
            return module == module_;
        }
        virtual bool CheckCore( const std::string& module ) const
        {
            return module == module_;
        }
    private:
        const std::string module_;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::SerializeAll
// Created: SLI 2010-08-31
// -----------------------------------------------------------------------------
void Facade::SerializeAll( const std::string& filename )
{
    const std::string extension = filename.substr( filename.find_last_of( '.' ) + 1, std::string::npos );
    BOOST_FOREACH( const std::string& module, modules_ )
    {
        filter_.reset( new SimpleFilter( module ) ); // $$$$ _RC_ SLI 2010-09-06: not that great
        Serialize( module + "." + extension );
    }
}