//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_LineVisitor_h
#define depend_LineVisitor_h

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <istream>

namespace depend
{
    class LineObserver_ABC;

// =============================================================================
/** @class  LineVisitor
    @brief  Line visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class LineVisitor : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LineVisitor();
    virtual ~LineVisitor();
    //@}

    //! @name Operations
    //@{
    void Visit( std::istream& stream );
    void Register( LineObserver_ABC& observer );
    void Unregister( LineObserver_ABC& observer );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< LineObserver_ABC* > T_Observers;
    //@}

private:
    //! @name Member data
    //@{
    T_Observers observers_;
    //@}
};

}

#endif // depend_LineVisitor_h
