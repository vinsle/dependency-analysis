//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Finder_ABC_h
#define depend_Finder_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  Finder_ABC
    @brief  Finder definition
*/
// Created: SLI 2010-09-09
// =============================================================================
class Finder_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Finder_ABC() {}
    virtual ~Finder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Find( const std::string& filename ) const = 0;
    //@}
};

}

#endif // depend_Finder_ABC_h
