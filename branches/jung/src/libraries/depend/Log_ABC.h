//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Log_ABC_h
#define depend_Log_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  Log_ABC
    @brief  Log definition
*/
// Created: SLI 2010-09-09
// =============================================================================
class Log_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Log_ABC() {}
    virtual ~Log_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Warn( const std::string& message ) const = 0;
    //@}
};

}

#endif // depend_Log_ABC_h
