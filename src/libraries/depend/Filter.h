//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Filter_h
#define depend_Filter_h

#include "Filter_ABC.h"
#include <vector>
#include <set>

namespace depend
{
// =============================================================================
/** @class  Filter
    @brief  Filter
*/
// Created: SLI 2010-08-30
// =============================================================================
class Filter : public Filter_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Modules;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Filter( const T_Modules& modules = T_Modules() );
    virtual ~Filter();
    //@}

    //! @name Operations
    //@{
    virtual bool Check( const std::string& module ) const;
    virtual bool CheckCore( const std::string& module ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::set< std::string > modules_;
    //@}
};

}

#endif // depend_Filter_h
