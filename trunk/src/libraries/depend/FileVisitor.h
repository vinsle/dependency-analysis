//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_FileVisitor_h
#define depend_FileVisitor_h

#include "Subject.h"

namespace depend
{
    class FileObserver_ABC;

// =============================================================================
/** @class  FileVisitor
    @brief  File visitor
*/
// Created: SLI 2010-08-16
// =============================================================================
class FileVisitor : public Subject< FileObserver_ABC >
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Extensions;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FileVisitor( const T_Extensions& extensions );
    virtual ~FileVisitor();
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const std::string& path );
    //@}

private:
    //! @name Member data
    //@{
    const T_Extensions extensions_;
    //@}
};

}

#endif // depend_FileVisitor_h
