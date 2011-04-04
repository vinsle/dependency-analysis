//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassLoader_h
#define depend_ClassLoader_h

#include "Subject.h"

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace depend
{
    class UnitObserver_ABC;
    class ClassObserver_ABC;

// =============================================================================
/** @class  ClassLoader
    @brief  Class loader
*/
// Created: SLI 2011-04-04
// =============================================================================
class ClassLoader : public Subject< UnitObserver_ABC >
                  , public Subject< ClassObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassLoader();
    virtual ~ClassLoader();
    //@}

    //! @name Operations
    //@{
    void Process( xml::xisubstream xis );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadNode( xml::xistream& xis );
    void ReadClass( xml::xistream& xis );
    //@}
};

}

#endif // depend_ClassLoader_h
