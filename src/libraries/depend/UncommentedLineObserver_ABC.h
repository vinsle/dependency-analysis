//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_UncommentedLineObserver_ABC_h
#define depend_UncommentedLineObserver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
// =============================================================================
/** @class  UncommentedLineObserver_ABC
    @brief  Uncommented line observer definition
*/
// Created: SLI 2010-08-19
// =============================================================================
class UncommentedLineObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UncommentedLineObserver_ABC() {}
    virtual ~UncommentedLineObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUncommentedLine( const std::string& line, const std::string& context ) = 0;
    //@}
};

}

#endif // depend_UncommentedLineObserver_ABC_h
