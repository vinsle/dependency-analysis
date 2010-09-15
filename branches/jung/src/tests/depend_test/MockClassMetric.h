//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockClassMetric_h
#define depend_test_MockClassMetric_h

#include "depend/ClassMetric_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockClassMetric
    @brief  Mock class metric
*/
// Created: SLI 2010-08-20
// =============================================================================
MOCK_BASE_CLASS( MockClassMetric, ClassMetric_ABC )
{
    MOCK_METHOD( Apply, 1 )
};

}

#endif // depend_test_MockClassMetric_h
