//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockStronglyConnectedComponentsVisitor_h
#define depend_test_MockStronglyConnectedComponentsVisitor_h

#include "depend/StronglyConnectedComponentsVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockStronglyConnectedComponentsVisitor
    @brief  Mock strongly connected component visitor
*/
// Created: SLI 2010-08-16
// =============================================================================
MOCK_BASE_CLASS( MockStronglyConnectedComponentsVisitor, StronglyConnectedComponentsVisitor_ABC )
{
    MOCK_METHOD( NotifyComponent, 1 )
};

}

#endif // depend_test_MockStronglyConnectedComponentsVisitor_h
