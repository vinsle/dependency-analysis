//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockModuleResolver_h
#define depend_test_MockModuleResolver_h

#include "depend/ModuleResolver_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockModuleResolver
    @brief  Mock filter
*/
// Created: SLI 2010-09-09
// =============================================================================
MOCK_BASE_CLASS( MockModuleResolver, ModuleResolver_ABC )
{
    MOCK_METHOD( Resolve, 1 )
    MOCK_METHOD( IsExcluded, 1 )
};

}

#endif // depend_test_MockModuleResolver_h