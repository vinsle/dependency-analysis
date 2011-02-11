//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockLog_h
#define depend_test_MockLog_h

#include "depend/Log_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockLog
    @brief  Mock log
*/
// Created: SLI 2010-08-30
// =============================================================================
MOCK_BASE_CLASS( MockLog, Log_ABC )
{
    MOCK_METHOD( Warn, 2 )
};

}

#endif // depend_test_MockLog_h
