//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_InternalModuleResolver_ABC_h
#define depend_InternalModuleResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  InternalModuleResolver_ABC
    @brief  Internal module resolver definition
*/
// Created: SLI 2011-02-17
// =============================================================================
class InternalModuleResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InternalModuleResolver_ABC() {}
    virtual ~InternalModuleResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( const std::string& module, const std::string& file, const std::string& include ) const = 0;
    //@}
};

}

#endif // depend_InternalModuleResolver_ABC_h
