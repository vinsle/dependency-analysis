//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ExternalModuleResolver_ABC_h
#define depend_ExternalModuleResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  ExternalModuleResolver_ABC
    @brief  External module resolver definition
*/
// Created: SLI 2010-09-09
// =============================================================================
class ExternalModuleResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ExternalModuleResolver_ABC() {}
    virtual ~ExternalModuleResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( const std::string& include ) const = 0;
    virtual bool IsExcluded( const std::string& include ) const = 0;
    //@}
};

}

#endif // depend_ExternalModuleResolver_ABC_h
