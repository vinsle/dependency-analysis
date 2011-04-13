//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef application_Version_h
#define application_Version_h

#include <ostream>
#include <string>

namespace application
{
// =============================================================================
/** @class  Version
    @brief  Version
*/
// Created: SLI 2011-04-13
// =============================================================================
class Version
{
public:
    //! @name Constructors/Destructor
    //@{
             Version();
    virtual ~Version();
    //@}

    //! @name Operations
    //@{
    void Serialize( std::ostream& os, const std::string& application ) const;
    //@}
};

}

#endif // application_Version_h
