//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleDependencyMetricLoader_h
#define depend_ModuleDependencyMetricLoader_h

#include "Visitable.h"
#include <vector>
#include <string>

namespace xml
{
    class xisubstream;
    class xistream;
}

namespace depend
{
    class DependencyMetricVisitor_ABC;

// =============================================================================
/** @class  ModuleDependencyMetricLoader
    @brief  Module dependency metric loader
*/
// Created: SLI 2011-02-18
// =============================================================================
class ModuleDependencyMetricLoader : public Visitable< DependencyMetricVisitor_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModuleDependencyMetricLoader( xml::xisubstream xis );
    virtual ~ModuleDependencyMetricLoader();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( DependencyMetricVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Contexts;
    typedef std::pair< std::string, T_Contexts > T_Dependency;
    typedef std::vector< T_Dependency > T_Dependencies;
    struct T_Node
    {
        std::string from_;
        T_Dependencies internals_;
        T_Dependencies externals_;
    };
    typedef std::vector< T_Node > T_Nodes;
    //@}

private:
    //! @name Helper
    //@{
    void ReadNode( xml::xistream& xis );
    void ReadDependency( xml::xistream& xis, T_Dependencies& dependencies ) const;
    void ReadContext( xml::xistream& xis, T_Contexts& contexts ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_Nodes nodes_;
    //@}
};

}

#endif // depend_ModuleDependencyMetricLoader_h
