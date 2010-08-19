//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockFileObserver_h
#define depend_test_MockFileObserver_h

#include "depend/ClassMetricVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockClassMetricVisitor
    @brief  Mock class metric visitor
*/
// Created: SLI 2010-08-16
// =============================================================================
MOCK_BASE_CLASS( MockClassMetricVisitor, ClassMetricVisitor_ABC )
{
    MOCK_METHOD( NotifyClassMetric, 3 )
};

}

#endif // depend_test_MockFileObserver_h
