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
    class Filter_ABC;
    class StronglyConnectedComponentsVisitor_ABC;
    class DependencyVisitor_ABC;
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
             Facade( xml::xisubstream xis, bool warning );
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
    const bool warning_;
    std::auto_ptr< depend::Visitable< depend::DependencyVisitor_ABC > > dependencyMetric_;
    std::auto_ptr< depend::Filter_ABC > filter_;
    std::auto_ptr< depend::Visitable< depend::StronglyConnectedComponentsVisitor_ABC > > components_;
    //@}
};

#endif // depend_metrics_app_Facade_h
