//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_DependencyMetric_h
#define depend_DependencyMetric_h

#include "ModuleObserver_ABC.h"
#include "FileObserver_ABC.h"
#include "IncludeObserver_ABC.h"
#include "Subject.h"
#include <set>

namespace depend
{
    class DependencyMetricVisitor_ABC;

// =============================================================================
/** @class  DependencyMetric
    @brief  Class metric
*/
// Created: SLI 2010-08-19
// =============================================================================
class DependencyMetric : private ModuleObserver_ABC
                       , private FileObserver_ABC
                       , private IncludeObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DependencyMetric( Subject< ModuleObserver_ABC >& moduleObserver, Subject< FileObserver_ABC >& fileObserver,
                               Subject< IncludeObserver_ABC >& includeObserver_ );
    virtual ~DependencyMetric();
    //@}

    //! @name Operations
    //@{
    void Apply( DependencyMetricVisitor_ABC& visitor ) const;
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
    Subject< ModuleObserver_ABC >& moduleObserver_;
    Subject< FileObserver_ABC >& fileObserver_;
    Subject< IncludeObserver_ABC >& includeObserver_;
    T_Metrics metrics_;
    //@}
};

}

#endif // depend_DependencyMetric_h
