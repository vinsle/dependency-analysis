//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockLineObserver_h
#define depend_test_MockLineObserver_h

#include "depend/LineObserver_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockLineObserver
    @brief  Mock line observer
*/
// Created: SLI 2010-08-17
// =============================================================================
MOCK_BASE_CLASS( MockLineObserver, LineObserver_ABC )
{
    MOCK_METHOD( Notify, 1 )
};

}

#endif // depend_test_MockLineObserver_h
