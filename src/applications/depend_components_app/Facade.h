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
#include <string>

namespace xml
{
    class xisubstream;
}

namespace depend
{
    class DependencyMetricVisitor_ABC;
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
    bool Process( const std::string& output );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< depend::Visitable< depend::DependencyMetricVisitor_ABC > > dependencyMetric_;
    //@}
};

#endif // depend_metrics_app_Facade_h
