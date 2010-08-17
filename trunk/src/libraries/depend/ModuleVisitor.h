//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleVisitor_h
#define depend_ModuleVisitor_h

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace depend
{
    class ModuleObserver_ABC;

// =============================================================================
/** @class  ModuleVisitor
    @brief  Module visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
class ModuleVisitor : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleVisitor();
    virtual ~ModuleVisitor();
    //@}

    //! @name Operations
    //@{
    void Visit( const std::string& filename );
    void Register( ModuleObserver_ABC& observer );
    void Unregister( ModuleObserver_ABC& observer );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ModuleObserver_ABC* > T_Observers;
    //@}

private:
    //! @name Member data
    //@{
    T_Observers observers_;
    //@}
};

}

#endif // depend_ModuleVisitor_h
