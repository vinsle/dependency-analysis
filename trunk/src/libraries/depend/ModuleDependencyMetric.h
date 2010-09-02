//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleDependencyMetric_h
#define depend_ModuleDependencyMetric_h

#include "DependencyMetric_ABC.h"
#include "ModuleObserver_ABC.h"
#include "FileObserver_ABC.h"
#include "IncludeObserver_ABC.h"
#include "Observer.h"
#include <set>

namespace depend
{
    class DependencyMetricVisitor_ABC;

// =============================================================================
/** @class  ModuleDependencyMetric
    @brief  Class metric
*/
// Created: SLI 2010-08-19
// =============================================================================
class ModuleDependencyMetric : public DependencyMetric_ABC
                             , private Observer< ModuleObserver_ABC >
                             , private Observer< FileObserver_ABC >
                             , private Observer< IncludeObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleDependencyMetric( Subject< ModuleObserver_ABC >& moduleObserver, Subject< FileObserver_ABC >& fileObserver,
                               Subject< IncludeObserver_ABC >& includeObserver_ );
    virtual ~ModuleDependencyMetric();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( DependencyMetricVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyModule( const std::string& module );
    virtual void NotifyFile( const std::string& path, std::istream& stream );
    virtual void NotifyInternalInclude( const std::string& file );
    virtual void NotifyExternalInclude( const std::string& file );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Dependencies;
    struct Metric
    {
        std::string module_;
        T_Dependencies internal_;
        T_Dependencies external_;
        T_Dependencies files_;
    };
    typedef std::vector< Metric > T_Metrics;
    //@}

private:
    //! @name Member data
    //@{
    T_Metrics metrics_;
    T_Dependencies modules_;
    //@}
};

}

#endif // depend_ModuleDependencyMetric_h
