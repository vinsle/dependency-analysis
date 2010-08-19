//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassMetric_h
#define depend_ClassMetric_h

#include "ClassObserver_ABC.h"
#include "ModuleObserver_ABC.h"
#include "Subject.h"
#include <vector>

namespace depend
{
    class ClassMetricVisitor_ABC;

// =============================================================================
/** @class  ClassMetric
    @brief  Class metric
*/
// Created: SLI 2010-08-19
// =============================================================================
class ClassMetric : private ModuleObserver_ABC, private ClassObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassMetric( Subject< ModuleObserver_ABC >& moduleObserver, Subject< ClassObserver_ABC >& classObserver );
    virtual ~ClassMetric();
    //@}

    //! @name Operations
    //@{
    void Apply( ClassMetricVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyModule( const std::string& module );
    virtual void NotifyClass( const std::string& name );
    virtual void NotifyAbstractness();
    //@}

private:
    //! @name Types
    //@{
    struct Metric
    {
    public:
        Metric()
            : classes_ ( 0u )
            , abstract_( 0u )
        {}
        std::string module_;
        unsigned int classes_;
        unsigned int abstract_;
    };
    typedef std::vector< Metric > T_Metrics;
    //@}

private:
    //! @name Member data
    //@{
    Subject< ModuleObserver_ABC >& moduleObserver_;
    Subject< ClassObserver_ABC >& classObserver_;
    T_Metrics metrics_;
    //@}
};

}

#endif // depend_ClassMetric_h
