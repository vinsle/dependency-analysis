//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_IncludeObserver_ABC_h
#define depend_IncludeObserver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  IncludeObserver_ABC
    @brief  Include observer definition
*/
// Created: SLI 2010-08-17
// =============================================================================
class IncludeObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IncludeObserver_ABC() {}
    virtual ~IncludeObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyInternalInclude( const std::string& file ) = 0;
    virtual void NotifyExternalInclude( const std::string& file ) = 0;
    //@}
};

}

#endif // depend_IncludeObserver_ABC_h
