//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassVisitor_h
#define depend_ClassVisitor_h

#include "Observer.h"
#include "UncommentedLineObserver_ABC.h"

namespace depend
{
    class ClassObserver_ABC;

// =============================================================================
/** @class  ClassVisitor
    @brief  Class visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class ClassVisitor : public Subject< ClassObserver_ABC >, private Observer< UncommentedLineObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ClassVisitor( Subject< UncommentedLineObserver_ABC >& visitor );
    virtual ~ClassVisitor();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUncommentedLine( const std::string& line, const std::string& context );
    //@}

private:
    //! @name Member data
    //@{
    bool insideClass_;
    bool abstract_;
    //@}
};

}

#endif // depend_ClassVisitor_h
