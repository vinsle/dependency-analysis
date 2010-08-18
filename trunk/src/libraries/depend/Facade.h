//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Facade_h
#define depend_Facade_h

#include "Subject.h"
#include "ModuleObserver_ABC.h"
#include "FileObserver_ABC.h"
#include "LineObserver_ABC.h"
#include "IncludeObserver_ABC.h"

namespace depend
{
    class ModuleVisitor;
    class FileVisitor;
    class LineVisitor;
    class IncludeVisitor;

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SLI 2010-08-18
// =============================================================================
class Facade : public Subject< ModuleObserver_ABC >
             , public Subject< FileObserver_ABC >
             , public Subject< LineObserver_ABC >
             , public Subject< IncludeObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Facade();
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void Visit( const std::string& path );
    //@}

    //! @name Register
    //@{
    virtual void Register( ModuleObserver_ABC& observer );
    virtual void Register( FileObserver_ABC& observer );
    virtual void Register( LineObserver_ABC& observer );
    virtual void Register( IncludeObserver_ABC& observer );
    virtual void Unregister( ModuleObserver_ABC& observer );
    virtual void Unregister( FileObserver_ABC& observer );
    virtual void Unregister( LineObserver_ABC& observer );
    virtual void Unregister( IncludeObserver_ABC& observer );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ModuleVisitor > moduleVisitor_;
    std::auto_ptr< FileVisitor > fileVisitor_;
    std::auto_ptr< LineVisitor > lineVisitor_;
    std::auto_ptr< IncludeVisitor > includeVisitor_;
    //@}
};

}

#endif // depend_Facade_h
