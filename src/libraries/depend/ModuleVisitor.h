//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleVisitor_h
#define depend_ModuleVisitor_h

#include "Subject.h"

namespace depend
{
    class ModuleObserver_ABC;

// =============================================================================
/** @class  ModuleVisitor
    @brief  Module visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class ModuleVisitor : public Subject< ModuleObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleVisitor();
    virtual ~ModuleVisitor();
    //@}

    //! @name Operations
    //@{
    void Visit( const std::string& filename );
    //@}
};

}

#endif // depend_ModuleVisitor_h
