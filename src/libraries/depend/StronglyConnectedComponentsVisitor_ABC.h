//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_StronglyConnectedComponentsVisitor_ABC_h
#define depend_StronglyConnectedComponentsVisitor_ABC_h

#include <boost/noncopyable.hpp>

namespace depend
{
    class ComponentVisitor_ABC;
    template< typename T > class Visitable;

// =============================================================================
/** @class  StronglyConnectedComponentsVisitor_ABC
    @brief  Strongly connected components visitor definition
*/
// Created: SLI 2011-04-06
// =============================================================================
class StronglyConnectedComponentsVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             StronglyConnectedComponentsVisitor_ABC() {}
    virtual ~StronglyConnectedComponentsVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyComponent( const Visitable< ComponentVisitor_ABC >& component ) = 0;
    //@}
};

}

#endif // depend_StronglyConnectedComponentsVisitor_ABC_h
