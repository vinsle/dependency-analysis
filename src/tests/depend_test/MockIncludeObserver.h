//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockIncludeObserver_h
#define depend_test_MockIncludeObserver_h

#include "depend/IncludeObserver_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockIncludeObserver
    @brief  Mock include observer
*/
// Created: SLI 2010-08-17
// =============================================================================
MOCK_BASE_CLASS( MockIncludeObserver, IncludeObserver_ABC )
{
    MOCK_METHOD( NotifyInternal, 1 )
    MOCK_METHOD( NotifyExternal, 1 )
};

}

#endif // depend_test_MockIncludeObserver_h
