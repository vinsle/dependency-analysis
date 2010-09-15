//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_LineObserver_ABC_h
#define depend_LineObserver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  LineObserver_ABC
    @brief  Line observer definition
*/
// Created: SLI 2010-08-17
// =============================================================================
class LineObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LineObserver_ABC() {}
    virtual ~LineObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyLine( const std::string& line ) = 0;
    //@}
};

}

#endif // depend_LineObserver_ABC_h
