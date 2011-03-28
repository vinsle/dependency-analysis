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
#include <vector>
#include <set>

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace depend
{
    template< typename T > class Visitable;
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
             ModuleDependencyGuard( xml::xisubstream xis, const Visitable< DependencyMetricVisitor_ABC >& dependencies );
    virtual ~ModuleDependencyGuard();
    //@}

    //! @name Operations
    //@{
    void Apply( DependencyGuardVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadModule( xml::xistream& xis );
    void ReadDependency( xml::xistream& xis, const std::string& module );
    void ReadObsoleteDependency( xml::xistream& xis, const std::string& module );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Module;
    typedef std::map< std::string, T_Module > T_Modules;
    struct T_Failure
    {
    public:
        T_Failure( const std::string& from, const std::string& to, const std::string& context )
            : from_   ( from )
            , to_     ( to )
            , context_( context )
        {}
        std::string from_;
        std::string to_;
        std::string context_;
    };
    typedef std::vector< T_Failure > T_Failures;
    typedef std::set< std::string > T_Contexts;
    typedef std::map< std::string, T_Contexts > T_Dependencies;
    //@}

private:
    //! @name Member data
    //@{
    T_Modules modules_;
    T_Modules obsoletes_;
    T_Failures failures_;
    T_Dependencies checked_;
    //@}
};

}

#endif // depend_ModuleDependencyGuard_h
