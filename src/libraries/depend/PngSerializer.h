//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_PngSerializer_h
#define depend_PngSerializer_h

#include <string>

namespace depend
{
// =============================================================================
/** @class  PngSerializer
    @brief  Png serializer
*/
// Created: SLI 2010-08-27
// =============================================================================
class PngSerializer
{
public:
    //! @name Constructors/Destructor
    //@{
             PngSerializer();
    virtual ~PngSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( const std::string& dot, const std::string& filename ) const;
    //@}
};

}

#endif // depend_PngSerializer_h
