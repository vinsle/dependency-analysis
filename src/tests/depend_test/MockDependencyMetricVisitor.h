//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockDependencyMetricVisitor_h
#define depend_test_MockDependencyMetricVisitor_h

#include "depend/DependencyMetricVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockDependencyMetricVisitor
    @brief  Mock class metric visitor
*/
// Created: SLI 2010-08-16
// =============================================================================
MOCK_BASE_CLASS( MockDependencyMetricVisitor, DependencyMetricVisitor_ABC )
{
    MOCK_METHOD( NotifyInternalDependency, 3 )
    MOCK_METHOD( NotifyExternalDependency, 3 )
};

}

#endif // depend_test_MockDependencyMetricVisitor_h
