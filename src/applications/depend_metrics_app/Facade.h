//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_metrics_app_Facade_h
#define depend_metrics_app_Facade_h

#include <boost/noncopyable.hpp>
#include <memory>

namespace xml
{
    class xisubstream;
}

namespace depend
{
    class ClassLoader;
    class ClassMetricVisitor_ABC;
    class DependencyVisitor_ABC;
    class UnitObserver_ABC;
    template< typename T > class Visitable;
}

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SLI 2010-08-18
// =============================================================================
class Facade : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Facade( xml::xisubstream xis );
    virtual ~Facade();
    //@}

public:
    //! @name Operations
    //@{
    void Process( xml::xisubstream xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< depend::ClassLoader > classLoader_;
    std::auto_ptr< depend::Visitable< depend::ClassMetricVisitor_ABC > > classMetric_;
    std::auto_ptr< depend::Visitable< depend::DependencyVisitor_ABC > > dependencyMetric_;
    std::auto_ptr< depend::Visitable< depend::UnitObserver_ABC > > units_;
    //@}
};

#endif // depend_metrics_app_Facade_h
