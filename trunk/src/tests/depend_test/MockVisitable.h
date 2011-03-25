//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockVisitable_h
#define depend_test_MockVisitable_h

#include "depend/Visitable.h"

namespace depend
{
// =============================================================================
/** @class  MockVisitable
    @brief  Mock visitable
*/
// Created: SLI 2010-03-25
// =============================================================================
template< typename T >
MOCK_BASE_CLASS( MockVisitable, Visitable< T > )
{
    MOCK_METHOD_EXT_TPL( Apply, 1, void( T& ), Apply )
};

}

#endif // depend_test_MockVisitable_h
