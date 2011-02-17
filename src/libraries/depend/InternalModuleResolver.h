//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_InternalModuleResolver_h
#define depend_InternalModuleResolver_h

#include "InternalModuleResolver_ABC.h"
#include "Observer.h"
#include "UnitObserver_ABC.h"
#include <vector>
#include <set>

namespace xml
{
    class xisubstream;
}

namespace depend
{
    class Finder_ABC;

// =============================================================================
/** @class  InternalModuleResolver
    @brief  Internal module resolver
*/
// Created: SLI 2011-02-16
// =============================================================================
class InternalModuleResolver : public InternalModuleResolver_ABC
                             , private Observer< UnitObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InternalModuleResolver( xml::xisubstream xis, const Finder_ABC& finder, Subject< UnitObserver_ABC >& unitObserver );
    virtual ~InternalModuleResolver();
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( const std::string& module, const std::string& file, const std::string& include ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Paths;
    typedef std::set< std::string > T_Units;
    //@}

private:
    //! @name Member data
    //@{
    const Finder_ABC& finder_;
    T_Paths paths_;
    T_Units units_;
    //@}
};

}

#endif // depend_InternalModuleResolver_h
