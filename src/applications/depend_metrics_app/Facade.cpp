//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "depend/ClassLoader.h"
#include "depend/ClassMetric.h"
#include "depend/UnitDependencyLoader.h"
#include "depend/MetricSerializer.h"
#include "depend/Filter.h"
#include "depend/TransitiveReductionFilter.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <set>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( xml::xisubstream xis )
    : classLoader_     ( new ClassLoader() )
    , classMetric_     ( new ClassMetric( *classLoader_, *classLoader_ ) )
    , dependencyMetric_( new UnitDependencyLoader( xis ) )
{
    classLoader_->Process( xis );
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
    void Noop() {}
    class FilterExtender : public Filter_ABC, private DependencyVisitor_ABC
    {
    public:
        FilterExtender( const Visitable< DependencyVisitor_ABC >& metric, const Filter_ABC& filter )
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
// Name: Facade::Process
// Created: SLI 2010-09-10
// -----------------------------------------------------------------------------
void Facade::Process( xml::xisubstream xis )
{
    xis >> xml::start( "configuration" );
    const std::string output = xis.content< std::string >( "output" );
    boost::shared_ptr< std::ostream > out( &std::cout, boost::bind( &Noop ) );
    if( !output.empty() )
        out.reset( new std::ofstream( output.c_str() ) );
    const Filter filter( xis );
    TransitiveReductionFilter transitive( *dependencyMetric_, filter );
    FilterExtender extension( *dependencyMetric_, filter );
    Filter_ABC& finalFilter = xis.content< bool >( "extend" ) ? static_cast< Filter_ABC& >( transitive ) : static_cast< Filter_ABC& >( extension );
    xml::xostringstream xos;
    MetricSerializer( *dependencyMetric_, *classMetric_ ).Serialize( xos, finalFilter );
    *out << xos.str();
}
