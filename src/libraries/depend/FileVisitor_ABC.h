// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef depend_FileVisitor_ABC_h
#define depend_FileVisitor_ABC_h

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
class FileVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FileVisitor_ABC() {}
    virtual ~FileVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const std::string& filename ) = 0;
    virtual void Register( FileObserver_ABC& observer ) = 0;
    virtual void Unregister( FileObserver_ABC& observer ) = 0;
    //@}
};

}

#endif // depend_FileVisitor_ABC_h
