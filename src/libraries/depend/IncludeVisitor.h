//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_IncludeVisitor_h
#define depend_IncludeVisitor_h

#include "Observer.h"
#include "UncommentedLineObserver_ABC.h"

namespace depend
{
    class IncludeObserver_ABC;

// =============================================================================
/** @class  IncludeVisitor
    @brief  Include visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class IncludeVisitor : public Subject< IncludeObserver_ABC >, private Observer< UncommentedLineObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit IncludeVisitor( Subject< UncommentedLineObserver_ABC >& visitor );
    virtual ~IncludeVisitor();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUncommentedLine( const std::string& line, const std::string& context );
    //@}
};

}

#endif // depend_IncludeVisitor_h
