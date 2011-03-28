//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_UnitCache_h
#define depend_UnitCache_h

#include "Visitable.h"
#include "Observer.h"
#include "UnitObserver_ABC.h"
#include <vector>

namespace depend
{
    template< typename T > class Subject;

// =============================================================================
/** @class  UnitCache
    @brief  Unit cache
*/
// Created: SLI 2011-03-28
// =============================================================================
class UnitCache : public Visitable< UnitObserver_ABC >
                , private Observer< UnitObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UnitCache( Subject< UnitObserver_ABC >& subject );
    virtual ~UnitCache();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( UnitObserver_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string > T_Unit;
    typedef std::vector< T_Unit > T_Units;
    //@}

private:
    //! @name Member data
    //@{
    T_Units units_;
    //@}
};

}

#endif // depend_UnitCache_h
