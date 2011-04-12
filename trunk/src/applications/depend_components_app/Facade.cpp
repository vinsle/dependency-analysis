//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "depend/Filter_ABC.h"
#include "depend/UnitDependencyLoader.h"
#include "depend/StronglyConnectedComponents.h"
#include "depend/StronglyConnectedComponentsVisitor_ABC.h"
#include "depend/StronglyConnectedComponentsSerializer.h"
#include "depend/ComponentVisitor_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <set>

using namespace depend;

namespace
{
    void Noop() {}
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

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( xml::xisubstream xis, bool warning )
    : warning_         ( warning )
    , dependencyMetric_( new UnitDependencyLoader( xis ) )
    , filter_          ( new SimpleFilter() )
    , components_      ( new StronglyConnectedComponents( *dependencyMetric_, *filter_ ) )
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
    class ComponentChecker : private StronglyConnectedComponentsVisitor_ABC
                           , private ComponentVisitor_ABC
    {
    public:
        ComponentChecker( const Visitable< StronglyConnectedComponentsVisitor_ABC >& components, bool warning )
            : warning_   ( warning )
            , components_( 0u )
        {
            components.Apply( *this );
        }
        bool Check() const
        {
            return components_ == 0u;
        }
    private:
        virtual void NotifyComponent( const Visitable< ComponentVisitor_ABC >& component )
        {
            ++components_;
            if( warning_ )
                std::cout << "Strongly connected component detected:" << std::endl;
            component.Apply( *this );
        }
        virtual void NotifyUnit( const std::string& unit )
        {
            if( warning_ )
                std::cout << " - " << unit << std::endl;
        }
        unsigned int components_;
        bool warning_;
    };
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
    ComponentChecker checker( *components_, warning_ );
    StronglyConnectedComponentsSerializer( *components_ ).Serialize( xos );
    *out << xos.str();
    return checker.Check();
}
