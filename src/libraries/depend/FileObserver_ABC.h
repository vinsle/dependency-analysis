//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_FileObserver_ABC_h
#define depend_FileObserver_ABC_h

#include <boost/noncopyable.hpp>
#include <istream>

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
    virtual void NotifyFile( const std::string& path, std::istream& stream, const std::string& context ) = 0;
    //@}
};

}

#endif // depend_FileObserver_ABC_h
