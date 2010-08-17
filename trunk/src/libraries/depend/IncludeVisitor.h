//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_IncludeVisitor_h
#define depend_IncludeVisitor_h

#include "LineObserver_ABC.h"
#include <vector>

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
class IncludeVisitor : private LineObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit IncludeVisitor( LineVisitor_ABC& visitor );
    virtual ~IncludeVisitor();
    //@}

    //! @name Operations
    //@{
    virtual void Register( IncludeObserver_ABC& observer );
    virtual void Unregister( IncludeObserver_ABC& observer );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const std::string& line );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< IncludeObserver_ABC* > T_Observers;
    //@}

private:
    //! @name Member data
    //@{
    LineVisitor_ABC& visitor_;
    T_Observers observers_;
    //@}
};

}

#endif // depend_IncludeVisitor_h
