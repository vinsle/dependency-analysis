//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Log_h
#define depend_Log_h

#include "Log_ABC.h"
#include <set>
#include <memory>

namespace xml
{
    class xisubstream;
}

namespace depend
{
// =============================================================================
/** @class  Log
    @brief  Log
*/
// Created: SLI 2010-09-09
// =============================================================================
class Log : public Log_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Log( xml::xisubstream xis );
    virtual ~Log();
    //@}

    //! @name Operations
    //@{
    virtual void Warn( const std::string& message ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Messages;
    //@}

private:
    //! @name Member data
    //@{
    const bool enabled_;
    std::auto_ptr< T_Messages > messages_;
    //@}
};

}

#endif // depend_Log_h 
