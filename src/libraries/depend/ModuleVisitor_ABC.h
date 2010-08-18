//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleVisitor_ABC_h
#define depend_ModuleVisitor_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
    class ModuleObserver_ABC;

// =============================================================================
/** @class  ModuleVisitor_ABC
    @brief  Module visitor definition
*/
// Created: SLI 2010-08-18
// =============================================================================
class ModuleVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleVisitor_ABC() {}
    virtual ~ModuleVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const std::string& filename ) = 0;
    virtual void Register( ModuleObserver_ABC& observer ) = 0;
    virtual void Unregister( ModuleObserver_ABC& observer ) = 0;
    //@}

};

}

#endif // depend_ModuleVisitor_ABC_h
