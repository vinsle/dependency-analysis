//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleObserver_ABC_h
#define depend_ModuleObserver_ABC_h

#include <boost/noncopyable.hpp>

namespace depend
{
// =============================================================================
/** @class  ModuleObserver_ABC
    @brief  Module observer definition
*/
// Created: SLI 2010-08-17
// =============================================================================
class ModuleObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleObserver_ABC() {}
    virtual ~ModuleObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const std::string& path ) = 0;
    //@}
};

}

#endif // depend_ModuleObserver_ABC_h
