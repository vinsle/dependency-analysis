//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassMetric_h
#define depend_ClassMetric_h

#include "Visitable.h"
#include "ClassObserver_ABC.h"
#include "UnitObserver_ABC.h"
#include "Observer.h"
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
class ClassMetric : public Visitable< ClassMetricVisitor_ABC >
                  , private Observer< UnitObserver_ABC >
                  , private Observer< ClassObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassMetric( Subject< UnitObserver_ABC >& unitObserver, Subject< ClassObserver_ABC >& classObserver );
    virtual ~ClassMetric();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( ClassMetricVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    virtual void NotifyClass( const std::string& name, const std::string& context );
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
        std::string unit_;
        unsigned int classes_;
        unsigned int abstract_;
    };
    typedef std::vector< Metric > T_Metrics;
    //@}

private:
    //! @name Member data
    //@{
    T_Metrics metrics_;
    //@}
};

}

#endif // depend_ClassMetric_h
