//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_LineVisitor_ABC_h
#define depend_LineVisitor_ABC_h

#include "Subject.h"
#include <istream>

namespace depend
{
    class LineObserver_ABC;

// =============================================================================
/** @class  LineVisitor_ABC
    @brief  Line visitor definition
*/
// Created: SLI 2010-08-17
// =============================================================================
class LineVisitor_ABC : public Subject< LineObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LineVisitor_ABC() {}
    virtual ~LineVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( std::istream& stream ) = 0;
    //@}
};

}

#endif // depend_LineVisitor_ABC_h
