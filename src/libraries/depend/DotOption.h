//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_DotOption_h
#define depend_DotOption_h

#include <string>
#include <stdexcept>

namespace depend
{
// =============================================================================
/** @enum   DotOption
    @brief  Dot option
*/
// Created: SLI 2010-09-07
// =============================================================================
enum DotOption
{
    Internal = 0,
    External,
    Both
};

inline std::istream& operator>>( std::istream& is, DotOption& option )
{
    std::string s;
    is >> s;
    option = s == "internal"  ? Internal :
             s == "external"  ? External :
                                Both;
    if( option == Both && s != "both" )
        throw std::bad_cast( std::string( "DotOption '" + s + "' is unknown" ).c_str() );
    return is;
}

inline std::ostream& operator<<( std::ostream& os, DotOption option )
{
    return os << ( option == Internal ? "internal" :
                   option == External ? "external" :
                                        "both" );
}

}

#endif // depend_DotOption_h
