//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockIncludeObserver_h
#define depend_test_MockIncludeObserver_h

#include "depend/UncommentedLineObserver_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockIUncommentedLineObserver
    @brief  Mock uncommented line observer
*/
// Created: SLI 2010-08-19
// =============================================================================
MOCK_BASE_CLASS( MockUncommentedLineObserver, UncommentedLineObserver_ABC )
{
    MOCK_METHOD( NotifyUncommentedLine, 1 )
};

}

#endif // depend_test_MockIncludeObserver_h
