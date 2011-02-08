//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ModuleDependencyGuard.h"
#include "DependencyGuardVisitor_ABC.h"
#include "DependencyMetric_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard constructor
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
ModuleDependencyGuard::ModuleDependencyGuard( xml::xisubstream xis, const DependencyMetric_ABC& dependencies )
{
    xis >> xml::start( "dependencies" )
            >> xml::list( "module", *this, &ModuleDependencyGuard::ReadModule );
    dependencies.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard destructor
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
ModuleDependencyGuard::~ModuleDependencyGuard()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::Apply
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::Apply( DependencyGuardVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const T_Failure& failure, failures_ )
        visitor.NotifyDependencyFailure( failure.first, failure.second );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::NotifyInternalDependency
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule )
{
    if( modules_[ fromModule ].find( toModule ) == modules_[ fromModule ].end() )
        if( knownFailures_.find( fromModule + toModule ) == knownFailures_.end() )
        {
            knownFailures_.insert( fromModule + toModule );
            failures_.push_back( std::make_pair( fromModule, toModule ) );
        }
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::NotifyExternalDependency
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::ReadModule
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::ReadModule( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "name", name )
        >> xml::list( "dependency", *this, &ModuleDependencyGuard::ReadDependency, name );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::ReadDependency
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::ReadDependency( xml::xistream& xis, const std::string& module )
{
    std::string dependency;
    xis >> dependency;
    modules_[ module ].insert( dependency );
}
