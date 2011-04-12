//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockMetricsVisitor_h
#define depend_test_MockMetricsVisitor_h

#include "depend/MetricsVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockMetricsVisitor
    @brief  Mock metrics visitor
*/
// Created: SLI 2011-04-12
// =============================================================================
MOCK_BASE_CLASS( MockMetricsVisitor, MetricsVisitor_ABC )
{
    MOCK_METHOD( NotifyMetrics, 9 )
};

}

#endif // depend_test_MockMetricsVisitor_h
