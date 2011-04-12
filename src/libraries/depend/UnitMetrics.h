//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_UnitMetrics_h
#define depend_UnitMetrics_h

#include "Visitable.h"
#include "UnitObserver_ABC.h"
#include "DependencyVisitor_ABC.h"
#include "ClassMetricVisitor_ABC.h"
#include <set>
#include <map>
#include <vector>

namespace xml
{
    class xostream;
}

namespace depend
{
    class Filter_ABC;
    class MetricsVisitor_ABC;

// =============================================================================
/** @class  MetricSerializer
    @brief  Metric serializer
*/
// Created: SLI 2011-04-12
// =============================================================================
class UnitMetrics : public Visitable< MetricsVisitor_ABC >
                  , private UnitObserver_ABC
                  , private DependencyVisitor_ABC
                  , private ClassMetricVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitMetrics( const Visitable< UnitObserver_ABC >& units, const Visitable< DependencyVisitor_ABC >& dependencies, const Visitable< ClassMetricVisitor_ABC >& classes );
    virtual ~UnitMetrics();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( MetricsVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    virtual void NotifyClassMetric( const std::string& module, unsigned int classes, unsigned int abstactClasses );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Modules;
    struct ClassMetrics
    {
    public:
        ClassMetrics()
            : classes_ ( 0u )
            , abstract_( 0u )
        {}
        unsigned int classes_;
        unsigned int abstract_;
    };
    typedef std::map< std::string, ClassMetrics > T_ClassMetrics;
    typedef T_ClassMetrics::const_iterator      CIT_ClassMetrics;
    typedef std::map< std::string, unsigned int > T_Dependency;
    typedef std::map< std::string, T_Dependency > T_Dependencies;
    //@}

private:
    //! @name Helpers
    //@{
    const UnitMetrics::ClassMetrics FindClass( const std::string& module ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_Modules modules_;
    T_ClassMetrics classMetrics_;
    T_Dependencies afferent_;
    T_Dependencies efferent_;
    T_Dependencies external_;
    //@}
};

}

#endif // depend_UnitMetrics_h
