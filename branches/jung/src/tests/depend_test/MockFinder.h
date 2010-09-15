//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockFinder_h
#define depend_test_MockFinder_h

#include "depend/Finder_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockFinder
    @brief  Mock finder
*/
// Created: SLI 2010-09-09
// =============================================================================
MOCK_BASE_CLASS( MockFinder, Finder_ABC )
{
    MOCK_METHOD( Find, 1 )
};

}

#endif // depend_test_MockFinder_h
