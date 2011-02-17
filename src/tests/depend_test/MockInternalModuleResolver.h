//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockInternalModuleResolver_h
#define depend_test_MockInternalModuleResolver_h

#include "depend/InternalModuleResolver_ABC.h"

namespace depend
{
// =============================================================================
/** @class  MockInternalModuleResolver
    @brief  Mock internal module resolver
*/
// Created: SLI 2011-02-17
// =============================================================================
MOCK_BASE_CLASS( MockInternalModuleResolver, InternalModuleResolver_ABC )
{
    MOCK_METHOD( Resolve, 3 )
};

}

#endif // depend_test_MockInternalModuleResolver_h