//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockDependencyMetric_h
#define depend_test_MockDependencyMetric_h

#include "depend/DependencyMetric_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockDependencyMetric
    @brief  Mock dependency metric
*/
// Created: SLI 2010-08-20
// =============================================================================
MOCK_BASE_CLASS( MockDependencyMetric, DependencyMetric_ABC )
{
    MOCK_METHOD( Apply, 1 )
};

}

#endif // depend_test_MockDependencyMetric_h
