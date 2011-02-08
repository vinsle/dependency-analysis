//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_DependencyGuardVisitor_ABC_h
#define depend_DependencyGuardVisitor_ABC_h

#include <boost/noncopyable.hpp>

namespace depend
{
// =============================================================================
/** @class  DependencyGuardVisitor_ABC
    @brief   dependency guard visitor definition
*/
// Created: SLI 2011-02-08
// =============================================================================
class DependencyGuardVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DependencyGuardVisitor_ABC() {}
    virtual ~DependencyGuardVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyDependencyFailure( const std::string& from, const std::string& to ) = 0;
    virtual void NotifyUncheckedDependency( const std::string& from, const std::string& to ) = 0;
    //@}
};

}

#endif // depend_DependencyGuardVisitor_ABC_h
