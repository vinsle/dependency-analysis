//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassDependencyMetric_h
#define depend_ClassDependencyMetric_h

#include "Observer.h"
#include "DependencyMetric_ABC.h"
#include "FileObserver_ABC.h"
#include "ClassObserver_ABC.h"
#include "IncludeObserver_ABC.h"
#include <vector>
#include <set>

namespace depend
{
// =============================================================================
/** @class  ClassDependencyMetric
    @brief  Class dependency metric
*/
// Created: SLI 2010-09-01
// =============================================================================
class ClassDependencyMetric : public DependencyMetric_ABC
                            , private Observer< FileObserver_ABC >
                            , private Observer< ClassObserver_ABC >
                            , private Observer< IncludeObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassDependencyMetric( Subject< FileObserver_ABC >& fileVisitor, Subject< ClassObserver_ABC >& classVisitor,
                                    Subject< IncludeObserver_ABC >& includeVisitor );
    virtual ~ClassDependencyMetric();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( DependencyMetricVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyFile( const std::string& path, std::istream& stream );
    virtual void NotifyClass( const std::string& name );
    virtual void NotifyAbstractness();
    virtual void NotifyInternalInclude( const std::string& file );
    virtual void NotifyExternalInclude( const std::string& file );
    //@}

private:
    //! @name Types
    //@{
    struct T_Class
    {
        std::string name_;
        std::set< std::string > includes_;
    };
    typedef std::pair< std::string, T_Class > T_File;
    typedef std::vector< T_File > T_Files;
    //@}

private:
    //! @name Member data
    //@{
    T_Files files_;
    //@}
};

}

#endif // depend_ClassDependencyMetric_h
