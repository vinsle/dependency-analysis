//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassObserver_ABC_h
#define depend_ClassObserver_ABC_h

#include <boost/noncopyable.hpp>

namespace depend
{
// =============================================================================
/** @class  ClassObserver_ABC
    @brief  Class observer definition
*/
// Created: SLI 2010-08-17
// =============================================================================
class ClassObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassObserver_ABC() {}
    virtual ~ClassObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyClass( const std::string& name ) = 0;
    virtual void NotifyAbstractness() = 0;
    //@}
};

}

#endif // depend_ClassObserver_ABC_h
