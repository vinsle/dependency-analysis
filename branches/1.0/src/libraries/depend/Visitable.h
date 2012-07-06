//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Visitable_h
#define depend_Visitable_h

#include <boost/noncopyable.hpp>

namespace depend
{
// =============================================================================
/** @class  Visitable
    @brief  Visitable
*/
// Created: SLI 2011-03-25
// =============================================================================
template< typename T >
class Visitable : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Visitable() {}
    virtual ~Visitable() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( T& visitor ) const = 0;
    //@}
};

}

#endif // depend_Visitable_h
