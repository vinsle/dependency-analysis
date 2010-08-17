//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockClassObserver_h
#define depend_test_MockClassObserver_h

#include "depend/ClassObserver_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockClassObserver
    @brief  Mock class observer
*/
// Created: SLI 2010-08-17
// =============================================================================
MOCK_BASE_CLASS( MockClassObserver, ClassObserver_ABC )
{
    MOCK_METHOD( NotifyClass, 1 )
    MOCK_METHOD( NotifyAbstractness, 0 )
};

}

#endif // depend_test_MockClassObserver_h
