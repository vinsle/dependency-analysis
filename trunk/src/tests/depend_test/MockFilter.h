//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockFilter_h
#define depend_test_MockFilter_h

#include "depend/Filter_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockFilter
    @brief  Mock filter
*/
// Created: SLI 2010-08-30
// =============================================================================
MOCK_BASE_CLASS( MockFilter, Filter_ABC )
{
    MOCK_METHOD( Check, 1 )
    MOCK_METHOD( CheckCore, 1 )
};

}

#endif // depend_test_MockFilter_h
