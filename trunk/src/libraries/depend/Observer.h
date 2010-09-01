//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Observer_h
#define depend_Observer_h

#include <boost/noncopyable.hpp>
#include "Subject.h"

namespace depend
{
// =============================================================================
/** @class  Observer
    @brief  Observer
*/
// Created: SLI 2010-09-01
// =============================================================================
template< typename T >
class Observer : public T
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Observer( Subject< T >& subject )
        : subject_( subject )
    {
        subject_.Register( *this );
    }
    virtual ~Observer()
    {
        subject_.Unregister( *this );
    }
    //@}

private:
    //! @name Member data
    //@{
    Subject< T >& subject_;
    //@}
};

}

#endif // depend_Observer_h
