//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockDependencyVisitor_h
#define depend_test_MockDependencyVisitor_h

#include "depend/DependencyVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockDependencyVisitor
    @brief  Mock class metric visitor
*/
// Created: SLI 2010-08-16
// =============================================================================
MOCK_BASE_CLASS( MockDependencyVisitor, DependencyVisitor_ABC )
{
    MOCK_METHOD( NotifyInternalDependency, 3 )
    MOCK_METHOD( NotifyExternalDependency, 3 )
};

}

#endif // depend_test_MockDependencyVisitor_h
