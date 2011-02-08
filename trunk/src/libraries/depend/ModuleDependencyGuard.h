//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleDependencyGuard_h
#define depend_ModuleDependencyGuard_h

#include "DependencyMetricVisitor_ABC.h"
#include <map>
#include <set>
#include <vector>

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace depend
{
    class DependencyMetric_ABC;
    class DependencyGuardVisitor_ABC;

// =============================================================================
/** @class  ModuleDependencyGuard
    @brief  Module dependency guard
*/
// Created: SLI 2011-02-08
// =============================================================================
class ModuleDependencyGuard : private DependencyMetricVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleDependencyGuard( xml::xisubstream xis, const DependencyMetric_ABC& dependencies );
    virtual ~ModuleDependencyGuard();
    //@}

    //! @name Operations
    //@{
    void Apply( DependencyGuardVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule );
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadModule( xml::xistream& xis );
    void ReadDependency( xml::xistream& xis, const std::string& module );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Module;
    typedef std::map< std::string, T_Module > T_Modules;
    typedef std::pair< std::string, std::string > T_Failure;
    typedef std::vector< T_Failure > T_Failures;
    typedef std::set< std::string > T_KnownFailures;
    //@}

private:
    //! @name Member data
    //@{
    T_Modules modules_;
    T_Failures failures_;
    T_KnownFailures knownFailures_;
    T_KnownFailures checked_;
    //@}
};

}

#endif // depend_ModuleDependencyGuard_h
