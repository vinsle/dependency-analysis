//
//  Copyright Silvin Lubecki 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_GraphMLSerializer_h
#define depend_GraphMLSerializer_h

#include "DependencyVisitor_ABC.h"
#include "UnitObserver_ABC.h"
#include <set>
#include <map>

namespace xml
{
    class xostream;
}

namespace depend
{
    template< typename T > class Visitable;

// =============================================================================
/** @class  GraphMLSerializer
    @brief  Graph ML Serializer
*/
// Created: SLI 2012-07-09
// =============================================================================
class GraphMLSerializer : private DependencyVisitor_ABC
                        , private UnitObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GraphMLSerializer( const Visitable< DependencyVisitor_ABC >& dependencies, const Visitable< UnitObserver_ABC >& units );
    virtual ~GraphMLSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Dependency;
    typedef std::map< std::string, T_Dependency > T_Dependencies;
    typedef std::set< std::string > T_Modules;
    //@}

private:
    //! @name Member data
    //@{
    T_Modules modules_;
    T_Dependencies dependencies_;
    //@}
};

}

#endif // depend_GraphMLSerializer_h
