//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_LineVisitor_h
#define depend_LineVisitor_h

#include "LineVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  LineVisitor
    @brief  Line visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class LineVisitor : public LineVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LineVisitor();
    virtual ~LineVisitor();
    //@}

    //! @name Operations
    //@{
    virtual void Visit( std::istream& stream );
    //@}
};

}

#endif // depend_LineVisitor_h
