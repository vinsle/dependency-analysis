//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Filter_ABC_h
#define depend_Filter_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  Filter_ABC
    @brief  Filter definition
*/
// Created: SLI 2010-08-30
// =============================================================================
class Filter_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Filter_ABC() {}
    virtual ~Filter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Check( const std::string& module ) const = 0;
    virtual bool CheckCore( const std::string& module ) const = 0;
    //@}
};

}

#endif // depend_Filter_ABC_h
