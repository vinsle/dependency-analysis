//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockDependencyGuardVisitor_h
#define depend_test_MockDependencyGuardVisitor_h

#include "depend/DependencyGuardVisitor_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockDependencyGuardVisitor
    @brief  Mock dependency guard visitor
*/
// Created: SLI 2011-02-08
// =============================================================================
MOCK_BASE_CLASS( MockDependencyGuardVisitor, DependencyGuardVisitor_ABC )
{
    MOCK_METHOD( NotifyDependencyFailure, 2 )
};

}

#endif // depend_test_MockDependencyGuardVisitor_h
