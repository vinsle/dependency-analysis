//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "GraphSerializer.h"
#include "depend/ClassMetric.h"
#include "depend/ClassLoader.h"
#include "depend/ModuleDependencyMetricLoader.h"
#include "depend/EdgeSerializer.h"
#include "depend/MetricSerializer.h"
#include "depend/UnitSerializer.h"
#include "depend/ExternalSerializer.h"
#include "depend/StronglyConnectedComponents.h"
#include "depend/StronglyConnectedComponentsSerializer.h"
#include "depend/UnitCache.h"
#include "depend/DotSerializer.h"
#include "depend/Filter.h"
#include "depend/TransitiveReductionFilter.h"
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
    : option_          ( ( xis >> xml::start( "configuration" ) ).content< std::string >( "dependencies" ) )
    , extend_          ( xis.content< bool >( "extend" ) )
    , filter_          ( new Filter( xis ) )
    , classLoader_     ( new ClassLoader() )
    , unitCache_       ( new UnitCache( *classLoader_ ) )
    , dependencyMetric_( new ModuleDependencyMetricLoader( xml::xifstream( xis.content< std::string >( "input" ) ) ) )
    , classMetric_     ( new ClassMetric( *classLoader_, *classLoader_ ) )
    , unitSerializer_  ( new UnitSerializer( *classLoader_ ) )
    , graphSerializer_ ( new GraphSerializer( xis ) )
{
    classLoader_->Subject< UnitObserver_ABC >::Register( *this );
    classLoader_->Process( xml::xifstream( xis.content< std::string >( "input" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Facade destructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::~Facade()
{
    classLoader_->Subject< UnitObserver_ABC >::Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Facade::Process
// Created: SLI 2010-09-10
// -----------------------------------------------------------------------------
void Facade::Process( xml::xisubstream xis )
{
    std::string output;
    xis >> xml::start( "configuration" )
            >> xml::content( "output", output );
    if( xis.content< bool >( "all" ) )
        SerializeAll( output );
    else
        Serialize( output );
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
    StronglyConnectedComponents components( *dependencyMetric_, filter );
    StronglyConnectedComponentsSerializer( components ).Serialize( xos );
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
    const std::string path = filename.substr( 0, filename.find_last_of( '.' ) );
    BOOST_FOREACH( const std::string& module, modules_ )
    {
        filter_.reset( new SimpleFilter( module ) ); // $$$$ _RC_ SLI 2010-09-06: not that great
        Serialize( path + "_" + module + "." + extension );
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::NotifyUnit
// Created: SLI 2011-04-08
// -----------------------------------------------------------------------------
void Facade::NotifyUnit( const std::string& unit, const std::string& /*context*/ )
{
    modules_.push_back( unit );
}
