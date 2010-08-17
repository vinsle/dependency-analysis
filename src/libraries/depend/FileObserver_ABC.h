// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef depend_FileObserver_ABC_h
#define depend_FileObserver_ABC_h

#include <boost/noncopyable.hpp>

namespace depend
{
// =============================================================================
/** @class  FileObserver_ABC
    @brief  File observer definition
*/
// Created: SLI 2010-08-16
// =============================================================================
class FileObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FileObserver_ABC() {}
    virtual ~FileObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const std::string& path ) = 0;
    //@}
};

}

#endif // depend_FileObserver_ABC_h
