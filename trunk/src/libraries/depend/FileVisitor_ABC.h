//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_FileVisitor_ABC_h
#define depend_FileVisitor_ABC_h

#include "Subject.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace depend
{
    class FileObserver_ABC;

// =============================================================================
/** @class  FileVisitor_ABC
    @brief  File visitor definition
*/
// Created: SLI 2010-08-18
// =============================================================================
class FileVisitor_ABC : public Subject< FileObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             FileVisitor_ABC() {}
    virtual ~FileVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const std::string& path ) = 0;
    //@}
};

}

#endif // depend_FileVisitor_ABC_h
