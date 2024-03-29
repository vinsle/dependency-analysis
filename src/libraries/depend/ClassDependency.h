//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassDependency_h
#define depend_ClassDependency_h

#include "Observer.h"
#include "Visitable.h"
#include "FileObserver_ABC.h"
#include "IncludeObserver_ABC.h"
#include <vector>
#include <set>

namespace depend
{
    class DependencyVisitor_ABC;

// =============================================================================
/** @class  ClassDependency
    @brief  Class dependency metric
*/
// Created: SLI 2010-09-01
// =============================================================================
class ClassDependency : public Visitable< DependencyVisitor_ABC >
                            , private Observer< FileObserver_ABC >
                            , private Observer< IncludeObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassDependency( Subject< FileObserver_ABC >& fileVisitor,
                                    Subject< IncludeObserver_ABC >& includeVisitor );
    virtual ~ClassDependency();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( DependencyVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyFile( const std::string& path, std::istream& stream, const std::string& context );
    virtual void NotifyInternalInclude( const std::string& file, const std::string& context );
    virtual void NotifyExternalInclude( const std::string& file, const std::string& context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::pair< std::string, std::string > > T_Includes;
    typedef std::pair< std::string, T_Includes > T_File;
    typedef std::vector< T_File > T_Files;
    //@}

private:
    //! @name Member data
    //@{
    T_Files files_;
    //@}
};

}

#endif // depend_ClassDependency_h
