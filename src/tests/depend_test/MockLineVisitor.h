//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockLineVisitor_h
#define depend_test_MockLineVisitor_h

#include "depend/LineVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockLineVisitor
    @brief  Mock line visitor
*/
// Created: SLI 2010-08-17
// =============================================================================
MOCK_BASE_CLASS( MockLineVisitor, LineVisitor_ABC )
{
    MOCK_METHOD( Register, 1 )
    MOCK_METHOD( Unregister, 1 )
    MOCK_METHOD( Visit, 1 )
};

}

#endif // depend_test_MockLineVisitor_h
