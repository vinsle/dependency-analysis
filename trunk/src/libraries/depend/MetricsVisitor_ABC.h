//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_MetricsVisitor_ABC_h
#define depend_MetricsVisitor_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  MetricsVisitor_ABC
    @brief  Metrics visitor definition
*/
// Created: SLI 2011-04-12
// =============================================================================
class MetricsVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MetricsVisitor_ABC() {}
    virtual ~MetricsVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyMetrics( const std::string& module, unsigned int afferent, unsigned int efferent,
                                unsigned int external, unsigned int classes, unsigned int abstractClasses,
                                unsigned int abstractness, unsigned int instability, unsigned int distance ) = 0;
    //@}
};

}

#endif // depend_MetricsVisitor_ABC_h
