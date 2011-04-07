//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ComponentVisitor_ABC_h
#define depend_ComponentVisitor_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  ComponentVisitor_ABC
    @brief  Component visitor definition
*/
// Created: SLI 2011-04-06
// =============================================================================
class ComponentVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ComponentVisitor_ABC() {}
    virtual ~ComponentVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit ) = 0;
    //@}

};

}

#endif // depend_ComponentVisitor_ABC_h
