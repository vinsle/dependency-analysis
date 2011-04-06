//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "depend/Filter_ABC.h"
#include "depend/ModuleDependencyMetricLoader.h"
#include "depend/StronglyConnectedComponents.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <set>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( xml::xisubstream xis )
    : dependencyMetric_( new ModuleDependencyMetricLoader( xis ) )
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
    void Noop() {}
namespace
{
    class SimpleFilter : public Filter_ABC
    {
    public:
        SimpleFilter()
        {}
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
}

// -----------------------------------------------------------------------------
// Name: Facade::Process
// Created: SLI 2010-09-10
// -----------------------------------------------------------------------------
bool Facade::Process( const std::string& output )
{
    boost::shared_ptr< std::ostream > out( &std::cout, boost::bind( &Noop ) );
    if( !output.empty() )
        out.reset( new std::ofstream( output.c_str() ) );
    xml::xostringstream xos;
    const bool result = StronglyConnectedComponents( *dependencyMetric_ ).Serialize( xos, SimpleFilter() );
    *out << xos.str();
    return result;
}
