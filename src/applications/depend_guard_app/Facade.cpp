//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "depend/ModuleDependencyMetricLoader.h"
#include "depend/Log.h"
#include "depend/ModuleDependencyGuard.h"
#include "depend/DependencyGuardVisitor_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <iostream>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SLI 2010-08-18
// -----------------------------------------------------------------------------
Facade::Facade( xml::xisubstream xis )
    : log_             ( new Log( xml::xistringstream( "<root warning='true'/>" ) >> xml::start( "root" ) ) )
    , dependencyMetric_( new ModuleDependencyMetricLoader( xis ) )
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
    class FailuresChecker : public DependencyGuardVisitor_ABC
    {
    public:
        FailuresChecker()
            : failure_( false )
        {}
        virtual void NotifyDependencyFailure( const std::string& fromModule, const std::string& toModule, const std::string& context )
        {
            std::cerr << context
                      << ": error : dependency from module '" << fromModule
                      << "' to module '" << toModule << "' is forbidden." << std::endl;
            failure_ = true;
        }
        virtual void NotifyUncheckedDependency( const std::string& from, const std::string& to )
        {
            std::cerr << "Warning: no dependency found from module '" << from << "' to module '" << to << "', you probably should remove it from your dependency file." << std::endl;
        }
        virtual void NotifyObsoleteDependency( const std::string& from, const std::string& to, const std::string& context )
        {
            std::cerr << context
                      << ": warning: obsolete dependency has been found from module '" << from << "' to module '" << to << "'." << std::endl;
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
