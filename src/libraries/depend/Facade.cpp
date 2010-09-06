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
#include "ModuleDependencyMetric.h"
#include "MetricSerializer.h"
#include "ModuleSerializer.h"
#include "StronglyConnectedComponents.h"
#include "DotSerializer.h"
#include "GraphSerializer.h"
#include "Filter.h"
#include <boost/foreach.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>
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
Facade::Facade( const T_Filter& filter, const std::string& layout, const std::string& format,
                const T_Options& graph, const T_Options& node, const T_Options& edge )
    : filter_                ( new Filter( filter ) )
    , moduleVisitor_         ( new ModuleVisitor() )
    , fileVisitor_           ( new FileVisitor( extensions ) )
    , lineVisitor_           ( new LineVisitor() )
    , uncommentedLineVisitor_( new UncommentedLineVisitor( *lineVisitor_ ) )
    , includeVisitor_        ( new IncludeVisitor( *uncommentedLineVisitor_ ) )
    , classVisitor_          ( new ClassVisitor( *uncommentedLineVisitor_ ) )
    , classMetric_           ( new ClassMetric( *moduleVisitor_, *classVisitor_ ) )
    , dependencyMetric_      ( new ModuleDependencyMetric( *moduleVisitor_, *fileVisitor_, *includeVisitor_ ) )
    , moduleSerializer_      ( new ModuleSerializer( *moduleVisitor_ ) )
    , graphSerializer_       ( new GraphSerializer( layout, format, graph, node, edge ) )
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

    class ModuleObserver : private Observer< ModuleObserver_ABC >
    {
    public:
        ModuleObserver( ModuleVisitor& moduleVisitor, FileVisitor& fileVisitor, LineVisitor& lineVisitor, const std::string& path, std::vector< std::string >& modules )
            : Observer< ModuleObserver_ABC >( moduleVisitor )
            , fileVisitor_  ( fileVisitor )
            , lineVisitor_  ( lineVisitor )
            , path_         ( path )
            , modules_      ( modules )
        {
             // NOTHING
        }
        virtual ~ModuleObserver()
        {
            // NOTHING
        }
    private:
        virtual void NotifyModule( const std::string& module )
        {
            FileObserver observer( fileVisitor_, lineVisitor_ );
            fileVisitor_.Visit( path_ + "/" + module );
            modules_.push_back( module );
        }
    private:
        FileVisitor& fileVisitor_;
        LineVisitor& lineVisitor_;
        const std::string path_;
        std::vector< std::string >& modules_;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade::Visit
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
void Facade::Visit( const std::string& path )
{
    ModuleObserver observer( *moduleVisitor_, *fileVisitor_, *lineVisitor_, path, modules_ );
    moduleVisitor_->Visit( path );
}

namespace
{
    void Noop() {}
}

// -----------------------------------------------------------------------------
// Name: Facade::Serialize
// Created: SLI 2010-09-03
// -----------------------------------------------------------------------------
void Facade::Serialize( const std::string& stage, const std::string& output, bool all )
{
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
        FilterExtender( const DependencyMetric_ABC& metric, const Filter_ABC& filter )
            : filter_( filter )
        {
            metric.Apply( *this );
        }
        virtual bool Check( const std::string& module ) const
        {
            return filter_.Check( module ) || extended_.find( module ) != extended_.end();
        }
    private:
        virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule )
        {
            if( filter_.Check( fromModule ) )
                extended_.insert( toModule );
            else if( filter_.Check( toModule ) )
                extended_.insert( fromModule );
        }
        virtual void NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/ )
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
    FilterExtender filter( *dependencyMetric_, *filter_ );
    moduleSerializer_->Serialize( xos, filter );
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
    Serialize( xos );
    DotSerializer().Serialize( xos, os );
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

// -----------------------------------------------------------------------------
// Name: Facade::SerializeAll
// Created: SLI 2010-08-31
// -----------------------------------------------------------------------------
void Facade::SerializeAll( const std::string& filename )
{
    const std::string name = filename.substr( 0, filename.find_last_of( '.' ) );
    const std::string extension = filename.substr( filename.find_last_of( '.' ), std::string::npos );
    BOOST_FOREACH( const std::string& module, modules_ )
    {
        const std::vector< std::string > filter = boost::assign::list_of( module );
        filter_.reset( new Filter( filter ) ); // $$$$ _RC_ SLI 2010-09-06: not that great
        Serialize( name + "-" + module + "." + extension );
    }
}
