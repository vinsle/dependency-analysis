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
#include "EdgeSerializer.h"
#include "MetricSerializer.h"
#include "UnitSerializer.h"
#include "ExternalSerializer.h"
#include "StronglyConnectedComponents.h"
#include "DotSerializer.h"
#include "GraphSerializer.h"
#include "Filter.h"
#include <boost/foreach.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
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
Facade::Facade( const T_Filter& filter, const std::string& layout, const std::string& format, const std::string& option,
                const T_Options& graph, const T_Options& node, const T_Options& edge )
    : option_                ( option )
    , filter_                ( new Filter( filter ) )
    , moduleVisitor_         ( new ModuleVisitor() )
    , fileVisitor_           ( new FileVisitor( extensions ) )
    , lineVisitor_           ( new LineVisitor() )
    , uncommentedLineVisitor_( new UncommentedLineVisitor( *lineVisitor_ ) )
    , includeVisitor_        ( new IncludeVisitor( *uncommentedLineVisitor_ ) )
    , classVisitor_          ( new ClassVisitor( *uncommentedLineVisitor_ ) )
    , classMetric_           ( new ClassMetric( *moduleVisitor_, *classVisitor_ ) )
    , dependencyMetric_      ( new ModuleDependencyMetric( *moduleVisitor_, *fileVisitor_, *includeVisitor_ ) )
    , unitSerializer_        ( new UnitSerializer( *moduleVisitor_ ) )
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
        virtual void NotifyUnit( const std::string& unit )
        {
            FileObserver observer( fileVisitor_, lineVisitor_ );
            fileVisitor_.Visit( path_ + "/" + unit );
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
    FilterExtender extender( *dependencyMetric_, *filter_ );
    Filter_ABC& filter = boost::lexical_cast< DotOption >( option_ ) == External ? *filter_ : extender;
    unitSerializer_->Serialize( xos, filter );
    ExternalSerializer( *dependencyMetric_, filter ).Serialize( xos );
    EdgeSerializer( *dependencyMetric_ ).Serialize( xos, filter );
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

// -----------------------------------------------------------------------------
// Name: Facade::SerializeAll
// Created: SLI 2010-08-31
// -----------------------------------------------------------------------------
void Facade::SerializeAll( const std::string& filename )
{
    const std::string extension = filename.substr( filename.find_last_of( '.' ) + 1, std::string::npos );
    BOOST_FOREACH( const std::string& module, modules_ )
    {
        const std::vector< std::string > filter = boost::assign::list_of( module );
        filter_.reset( new Filter( filter ) ); // $$$$ _RC_ SLI 2010-09-06: not that great
        Serialize( module + "." + extension );
    }
}
