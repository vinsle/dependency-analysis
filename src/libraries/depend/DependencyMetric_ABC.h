//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_DependencyMetric_ABC_h
#define depend_DependencyMetric_ABC_h

#include <boost/noncopyable.hpp>

namespace depend
{
    class DependencyMetricVisitor_ABC;

// =============================================================================
/** @class  DependencyMetric_ABC
    @brief  Dependency metric definition
*/
// Created: SLI 2010-08-20
// =============================================================================
class DependencyMetric_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DependencyMetric_ABC() {}
    virtual ~DependencyMetric_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( DependencyMetricVisitor_ABC& visitor ) const = 0;
    //@}
};

}

#endif // depend_DependencyMetric_ABC_h
