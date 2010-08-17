//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassVisitor_h
#define depend_ClassVisitor_h

#include "LineObserver_ABC.h"
#include <vector>

namespace depend
{
    class LineVisitor_ABC;
    class ClassObserver_ABC;

// =============================================================================
/** @class  ClassVisitor
    @brief  Class visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class ClassVisitor : private LineObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ClassVisitor( LineVisitor_ABC& visitor );
    virtual ~ClassVisitor();
    //@}

    //! @name Operations
    //@{
    virtual void Register( ClassObserver_ABC& observer );
    virtual void Unregister( ClassObserver_ABC& observer );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const std::string& line );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ClassObserver_ABC* > T_Observers;
    //@}

private:
    //! @name Member data
    //@{
    LineVisitor_ABC& visitor_;
    T_Observers observers_;
    bool insideClass_;
    bool abstract_;
    //@}
};

}

#endif // depend_ClassVisitor_h
