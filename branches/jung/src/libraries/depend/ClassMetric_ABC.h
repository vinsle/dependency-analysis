//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassMetric_ABC_h
#define depend_ClassMetric_ABC_h

#include <boost/noncopyable.hpp>

namespace depend
{
    class ClassMetricVisitor_ABC;

// =============================================================================
/** @class  ClassMetric_ABC
    @brief  Class metric definition
*/
// Created: SLI 2010-08-20
// =============================================================================
class ClassMetric_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassMetric_ABC() {}
    virtual ~ClassMetric_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( ClassMetricVisitor_ABC& visitor ) const = 0;
    //@}
};

}

#endif // depend_ClassMetric_ABC_h
