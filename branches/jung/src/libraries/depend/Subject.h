//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Subject_h
#define depend_Subject_h

#include <boost/noncopyable.hpp>
#include <vector>
#include <algorithm>

namespace depend
{
// =============================================================================
/** @class  Subject
    @brief  Subject
*/
// Created: SLI 2010-08-18
// =============================================================================
template< typename T >
class Subject : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Subject() {}
    virtual ~Subject() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( T& observer )
    {
        observers_.push_back( &observer );
    }
    virtual void Unregister( T& observer )
    {
        observers_.erase( std::remove( observers_.begin(), observers_.end(), &observer ), observers_.end() );
    }
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< T* > T_Observers;
    //@}

protected:
    //! @name Member data
    //@{
    T_Observers observers_;
    //@}
};

}

#endif // depend_Subject_h
