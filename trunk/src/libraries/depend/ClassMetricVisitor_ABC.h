//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassMetricVisitor_ABC_h
#define depend_ClassMetricVisitor_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  ClassMetricVisitor_ABC
    @brief  Class metric visitor definition
*/
// Created: SLI 2010-08-19
// =============================================================================
class ClassMetricVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassMetricVisitor_ABC() {}
    virtual ~ClassMetricVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyClassMetric( const std::string& module, unsigned int classes, unsigned int abstactClasses ) = 0;
    //@}
};

}

#endif // depend_ClassMetricVisitor_ABC_h
