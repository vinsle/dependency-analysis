//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockUnitObserver_h
#define depend_test_MockUnitObserver_h

#include "depend/UnitObserver_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockUnitObserver
    @brief  Mock unit observer
*/
// Created: SLI 2010-08-17
// =============================================================================
MOCK_BASE_CLASS( MockUnitObserver, UnitObserver_ABC )
{
    MOCK_METHOD( NotifyUnit, 2 )
};

}

#endif // depend_test_MockUnitObserver_h
