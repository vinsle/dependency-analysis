//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_FileVisitor_h
#define depend_FileVisitor_h

#include "FileVisitor_ABC.h"
#include <vector>

namespace depend
{

// =============================================================================
/** @class  FileVisitor
    @brief  File visitor
*/
// Created: SLI 2010-08-16
// =============================================================================
class FileVisitor : public FileVisitor_ABC
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
    virtual void Visit( const std::string& filename );
    virtual void Register( FileObserver_ABC& observer );
    virtual void Unregister( FileObserver_ABC& observer );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< FileObserver_ABC* > T_Observers;
    //@}

private:
    //! @name Member data
    //@{
    const T_Extensions extensions_;
    T_Observers observers_;
    //@}
};

}

#endif // depend_FileVisitor_h
