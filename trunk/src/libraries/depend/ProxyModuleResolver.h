//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ProxyModuleResolver_h
#define depend_ProxyModuleResolver_h

#include "ModuleResolver_ABC.h"
#include <map>
#include <memory>

namespace depend
{
// =============================================================================
/** @class  ProxyModuleResolver
    @brief  ProxyModuleResolver
*/
// Created: SLI 2010-09-09
// =============================================================================
class ProxyModuleResolver : public ModuleResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ProxyModuleResolver( ModuleResolver_ABC& resolver );
    virtual ~ProxyModuleResolver();
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( const std::string& include ) const;
    virtual bool IsExcluded( const std::string& include ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Results;
    typedef std::map< std::string, bool > T_Excludes;
    typedef T_Excludes::const_iterator  CIT_Excludes;
    //@}

private:
    //! @name Member data
    //@{
    ModuleResolver_ABC& resolver_;
    std::auto_ptr< T_Results > results_;
    std::auto_ptr< T_Excludes > excludes_;
    //@}
};

}

#endif // depend_ProxyModuleResolver_h
