//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_DependencyVisitor_ABC_h
#define depend_DependencyVisitor_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  DependencyVisitor_ABC
    @brief  Dependency metric visitor definition
*/
// Created: SLI 2010-08-19
// =============================================================================
class DependencyVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DependencyVisitor_ABC() {}
    virtual ~DependencyVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context ) = 0;
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context ) = 0;
    //@}
};

}

#endif // depend_DependencyVisitor_ABC_h
