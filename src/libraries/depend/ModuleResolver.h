//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleResolver_h
#define depend_ModuleResolver_h

#include <boost/noncopyable.hpp>
#include <vector>
#include <string>

namespace depend
{
    class Finder_ABC;

// =============================================================================
/** @class  ModuleResolver
    @brief  Module resolver
*/
// Created: SLI 2010-09-09
// =============================================================================
class ModuleResolver : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Directories;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ModuleResolver( const T_Directories& roots, const Finder_ABC& finder );
    virtual ~ModuleResolver();
    //@}

    //! @name Operations
    //@{
    std::string Resolve( const std::string& include ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string > T_NamedDirectory;
    typedef std::vector< T_NamedDirectory > T_NamedDirectories;
    //@}

private:
    //! @name Member data
    //@{
    const T_NamedDirectories directories_;
    const Finder_ABC& finder_;
    //@}
};

}

#endif // depend_ModuleResolver_h
