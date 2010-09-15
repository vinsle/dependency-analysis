//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_test_MockSubject_h
#define depend_test_MockSubject_h

#include "depend/Subject.h"

namespace depend
{
// =============================================================================
/** @class  MockSubject
    @brief  Mock subject
*/
// Created: SLI 2010-08-17
// =============================================================================
template< typename T >
MOCK_BASE_CLASS( MockSubject, Subject< T > )
{
    MOCK_METHOD_EXT_TPL( Register, 1, void( T& ), Register )
    MOCK_METHOD_EXT_TPL( Unregister, 1, void( T& ), Unregister )
};

}

#endif // depend_test_MockSubject_h