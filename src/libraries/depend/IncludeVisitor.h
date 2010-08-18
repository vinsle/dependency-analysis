//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_IncludeVisitor_h
#define depend_IncludeVisitor_h

#include "Subject.h"
#include "LineObserver_ABC.h"

namespace depend
{
    class LineVisitor_ABC;
    class IncludeObserver_ABC;

// =============================================================================
/** @class  IncludeVisitor
    @brief  Include visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class IncludeVisitor : public Subject< IncludeObserver_ABC >, private LineObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit IncludeVisitor( LineVisitor_ABC& visitor );
    virtual ~IncludeVisitor();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const std::string& line );
    //@}

private:
    //! @name Member data
    //@{
    LineVisitor_ABC& visitor_;
    //@}
};

}

#endif // depend_IncludeVisitor_h
