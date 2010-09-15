//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_DotSerializer_h
#define depend_DotSerializer_h

#include <ostream>
#include "DotOption.h"

namespace xml
{
    class xistream;
}

namespace depend
{
// =============================================================================
/** @class  DotSerializer
    @brief  DotSerializer
*/
// Created: SLI 2010-08-26
// =============================================================================
class DotSerializer
{
public:
    //! @name Constructors/Destructor
    //@{
             DotSerializer();
    virtual ~DotSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xistream& xis, std::ostream& os, DotOption option ) const;
    //@}
};

}

#endif // depend_DotSerializer_h
