//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockComponentVisitor_h
#define depend_test_MockComponentVisitor_h

#include "depend/ComponentVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockComponentVisitor
    @brief  Mock component visitor
*/
// Created: SLI 2010-08-16
// =============================================================================
MOCK_BASE_CLASS( MockComponentVisitor, ComponentVisitor_ABC )
{
    MOCK_METHOD( NotifyUnit, 1 )
};

}

#endif // depend_test_MockComponentVisitor_h
