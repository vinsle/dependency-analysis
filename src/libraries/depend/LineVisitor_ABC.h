//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_LineVisitor_ABC_h
#define depend_LineVisitor_ABC_h

#include <boost/noncopyable.hpp>
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
class LineVisitor_ABC : private boost::noncopyable
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
    virtual void Register( LineObserver_ABC& observer ) = 0;
    virtual void Unregister( LineObserver_ABC& observer ) = 0;
    //@}
};

}

#endif // depend_LineVisitor_ABC_h
