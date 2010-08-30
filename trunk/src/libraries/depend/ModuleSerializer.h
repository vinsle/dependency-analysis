//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ModuleSerializer_h
#define depend_ModuleSerializer_h

#include "ModuleObserver_ABC.h"
#include "Subject.h"
#include <vector>

namespace xml
{
    class xostream;
}

namespace depend
{
// =============================================================================
/** @class  ModuleSerializer
    @brief  Module serializer
*/
// Created: SLI 2010-08-24
// =============================================================================
class ModuleSerializer : private ModuleObserver_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Filter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModuleSerializer( Subject< ModuleObserver_ABC >& subject );
    virtual ~ModuleSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos, const T_Filter& filter ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyModule( const std::string& module );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Modules;
    //@}

private:
    //! @name Member data
    //@{
    Subject< ModuleObserver_ABC >& subject_;
    T_Modules modules_;
    //@}
};

}

#endif // depend_ModuleSerializer_h
