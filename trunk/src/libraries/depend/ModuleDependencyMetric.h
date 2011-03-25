//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleDependencyMetric_h
#define depend_ModuleDependencyMetric_h

#include "UnitObserver_ABC.h"
#include "FileObserver_ABC.h"
#include "IncludeObserver_ABC.h"
#include "Observer.h"
#include "Visitable.h"
#include <set>

namespace depend
{
    class DependencyMetricVisitor_ABC;
    class ExternalModuleResolver_ABC;
    class InternalModuleResolver_ABC;
    class Log_ABC;

// =============================================================================
/** @class  ModuleDependencyMetric
    @brief  Class metric
*/
// Created: SLI 2010-08-19
// =============================================================================
class ModuleDependencyMetric : public Visitable< DependencyMetricVisitor_ABC >
                             , private Observer< UnitObserver_ABC >
                             , private Observer< FileObserver_ABC >
                             , private Observer< IncludeObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleDependencyMetric( Subject< UnitObserver_ABC >& unitObserver, Subject< FileObserver_ABC >& fileObserver,
                                     Subject< IncludeObserver_ABC >& includeObserver_, const ExternalModuleResolver_ABC& externalResolver,
                                     const InternalModuleResolver_ABC& internalResolver, const Log_ABC& log );
    virtual ~ModuleDependencyMetric();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( DependencyMetricVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    virtual void NotifyFile( const std::string& path, std::istream& stream, const std::string& context );
    virtual void NotifyInternalInclude( const std::string& include, const std::string& context );
    virtual void NotifyExternalInclude( const std::string& include, const std::string& context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Units;
    typedef std::vector< std::string > T_Contexts;
    struct T_Dependency
    {
    public:
        T_Dependency( const std::string& include, const std::string& file )
            : include_( include )
            , file_   ( file )
        {}
        bool operator<( const T_Dependency& dependency ) const
        {
            return include_ < dependency.include_;
        }
        std::string include_;
        std::string file_;
        T_Contexts contexts_;
    };
    typedef std::set< T_Dependency > T_Dependencies;
    struct Metric
    {
        std::string unit_;
        T_Dependencies internal_;
        T_Dependencies external_;
        T_Dependencies files_;
    };
    typedef std::vector< Metric > T_Metrics;
    //@}

private:
    //! @name Member data
    //@{
    const ExternalModuleResolver_ABC& externalResolver_;
    const InternalModuleResolver_ABC& internalResolver_;
    const Log_ABC& log_;
    T_Metrics metrics_;
    T_Units units_;
    std::string lastFile_;
    //@}
};

}

#endif // depend_ModuleDependencyMetric_h
