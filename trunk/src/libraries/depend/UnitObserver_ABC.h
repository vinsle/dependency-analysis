//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_UnitObserver_ABC_h
#define depend_UnitObserver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  UnitObserver_ABC
    @brief  Unit observer definition
*/
// Created: SLI 2010-08-17
// =============================================================================
class UnitObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitObserver_ABC() {}
    virtual ~UnitObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context ) = 0;
    //@}
};

}

#endif // depend_UnitObserver_ABC_h
