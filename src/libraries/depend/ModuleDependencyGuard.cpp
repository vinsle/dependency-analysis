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
#include "Visitable.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard constructor
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
ModuleDependencyGuard::ModuleDependencyGuard( xml::xisubstream xis, const Visitable< DependencyVisitor_ABC >& dependencies )
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
        visitor.NotifyDependencyFailure( failure.from_, failure.to_, failure.context_ );
    BOOST_FOREACH( const T_Modules::value_type& module, modules_ )
        BOOST_FOREACH( const std::string& dependency, module.second )
            if( checked_.find( module.first + dependency ) == checked_.end() )
                visitor.NotifyUncheckedDependency( module.first, dependency );
    BOOST_FOREACH( const T_Modules::value_type& module, obsoletes_ )
        BOOST_FOREACH( const std::string& dependency, module.second )
            if( checked_.find( module.first + dependency ) != checked_.end() )
            {
                BOOST_FOREACH( const std::string& context, checked_.find( module.first + dependency )->second )
                    visitor.NotifyObsoleteDependency( module.first, dependency, context );
            }
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::NotifyInternalDependency
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context )
{
    const std::string identifier = fromModule + toModule;
    if( modules_[ fromModule ].find( toModule ) == modules_[ fromModule ].end() )
        failures_.push_back( T_Failure( fromModule, toModule, context ) );
    else
        checked_[ identifier ].insert( context );
}

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::NotifyExternalDependency
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::NotifyExternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/, const std::string& /*context*/ )
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
        >> xml::list( "dependency", *this, &ModuleDependencyGuard::ReadDependency, name )
        >> xml::list( "obsolete-dependency", *this, &ModuleDependencyGuard::ReadObsoleteDependency, name );
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

// -----------------------------------------------------------------------------
// Name: ModuleDependencyGuard::ReadObsoleteDependency
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void ModuleDependencyGuard::ReadObsoleteDependency( xml::xistream& xis, const std::string& module )
{
    std::string dependency;
    xis >> dependency;
    modules_[ module ].insert( dependency );
    obsoletes_[ module ].insert( dependency );
}
