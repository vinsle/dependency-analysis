//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleResolver_h
#define depend_ModuleResolver_h

#include "ExternalModuleResolver_ABC.h"
#include <vector>
#include <string>

namespace xml
{
    class xisubstream;
}

namespace depend
{
    class Finder_ABC;
    class Log_ABC;

// =============================================================================
/** @class  ExternalModuleResolver
    @brief  External module resolver
*/
// Created: SLI 2010-09-09
// =============================================================================
class ExternalModuleResolver : public ExternalModuleResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ExternalModuleResolver( xml::xisubstream xis, const Finder_ABC& finder, const Log_ABC& log );
    virtual ~ExternalModuleResolver();
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( const std::string& include ) const;
    virtual bool IsExcluded( const std::string& include ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Directories;
    typedef std::pair< std::string, std::string > T_NamedDirectory;
    typedef std::vector< T_NamedDirectory > T_NamedDirectories;
    //@}

private:
    //! @name Member data
    //@{
    const T_NamedDirectories directories_;
    const T_Directories excludes_;
    const Finder_ABC& finder_;
    //@}
};

}

#endif // depend_ModuleResolver_h
