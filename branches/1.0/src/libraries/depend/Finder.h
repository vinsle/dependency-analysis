//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Finder_h
#define depend_Finder_h

#include "Finder_ABC.h"

namespace depend
{
// =============================================================================
/** @class  Finder
    @brief  Finder
*/
// Created: SLI 2010-09-09
// =============================================================================
class Finder : public Finder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Finder();
    virtual ~Finder();
    //@}

    //! @name Operations
    //@{
    virtual bool Find( const std::string& filename ) const;
    //@}
};

}

#endif // depend_Finder_h
