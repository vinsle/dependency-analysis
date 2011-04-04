//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ClassSerializer_h
#define depend_ClassSerializer_h

#include "UnitObserver_ABC.h"
#include "ClassObserver_ABC.h"
#include "Observer.h"
#include <vector>

namespace xml
{
    class xostream;
}

namespace depend
{
    template< typename T > class Subject;

// =============================================================================
/** @class  ClassSerializer
    @brief  Class serializer
*/
// Created: SLI 2011-04-04
// =============================================================================
class ClassSerializer : private Observer< UnitObserver_ABC >
                      , private Observer< ClassObserver_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassSerializer( Subject< UnitObserver_ABC >& unitSubject, Subject< ClassObserver_ABC >& classObserver );
    virtual ~ClassSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    virtual void NotifyClass( const std::string& name, const std::string& context );
    virtual void NotifyAbstractness();
    //@}

private:
    //! @name Types
    //@{
    struct T_Class
    {
    public:
        T_Class(){}
        T_Class( const std::string& name, const std::string& context )
            : name_        ( name )
            , context_     ( context )
            , abstractness_( "false" )
        {}
        std::string name_;
        std::string context_;
        std::string abstractness_;
    };
    typedef std::vector< T_Class > T_Classes;
    typedef std::pair< std::string, T_Classes > T_Unit;
    typedef std::vector< T_Unit > T_Units;
    //@}

private:
    //! @name Member data
    //@{
    T_Units units_;
    //@}
};

}

#endif // depend_ClassSerializer_h
