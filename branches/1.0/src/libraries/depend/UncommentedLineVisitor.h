//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_UncommentedLineVisitor_h
#define depend_UncommentedLineVisitor_h

#include "Observer.h"
#include "LineObserver_ABC.h"

namespace depend
{
    class UncommentedLineObserver_ABC;

// =============================================================================
/** @class  UncommentedLineVisitor
    @brief  UncommentedLine visitor
*/
// Created: SLI 2010-08-19
// =============================================================================
class UncommentedLineVisitor : public Subject< UncommentedLineObserver_ABC >, private Observer< LineObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UncommentedLineVisitor( Subject< LineObserver_ABC >& visitor );
    virtual ~UncommentedLineVisitor();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyLine( const std::string& line, const std::string& context );
    //@}
};

}

#endif // depend_UncommentedLineVisitor_h
