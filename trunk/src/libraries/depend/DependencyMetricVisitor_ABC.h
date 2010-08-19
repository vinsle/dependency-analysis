//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_DependencyMetricVisitor_ABC_h
#define depend_DependencyMetricVisitor_ABC_h

#include <vector>

namespace depend
{
// =============================================================================
/** @class  DependencyMetricVisitor_ABC
    @brief  Dependency metric visitor definition
*/
// Created: SLI 2010-08-19
// =============================================================================
class DependencyMetricVisitor_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Dependencies;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DependencyMetricVisitor_ABC() {}
    virtual ~DependencyMetricVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyDependencyMetric( const std::string& module, const T_Dependencies& internalDependencies, const T_Dependencies& externalDependencies ) = 0;
    //@}
};

}

#endif // depend_DependencyMetricVisitor_ABC_h
