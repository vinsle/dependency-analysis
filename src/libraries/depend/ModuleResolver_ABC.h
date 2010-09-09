//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleResolver_ABC_h
#define depend_ModuleResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  ModuleResolver_ABC
    @brief  Module resolver definition
*/
// Created: SLI 2010-09-09
// =============================================================================
class ModuleResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModuleResolver_ABC() {}
    virtual ~ModuleResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( const std::string& include ) const = 0;
    //@}
};

}

#endif // depend_ModuleResolver_ABC_h
