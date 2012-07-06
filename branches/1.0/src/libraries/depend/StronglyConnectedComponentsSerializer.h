//
//  Copyright Silvin Lubecki 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_StronglyConnectedComponentsSerializer_h
#define depend_StronglyConnectedComponentsSerializer_h

#include "StronglyConnectedComponentsVisitor_ABC.h"
#include <vector>
#include <string>

namespace xml
{
    class xostream;
}

namespace depend
{
    template< typename T > class Visitable;

// =============================================================================
/** @class  StronglyConnectedComponentsSerializer
    @brief  Strongly connected components serializer
*/
// Created: SLI 2011-04-07
// =============================================================================
class StronglyConnectedComponentsSerializer : private StronglyConnectedComponentsVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StronglyConnectedComponentsSerializer( const Visitable< StronglyConnectedComponentsVisitor_ABC >& components );
    virtual ~StronglyConnectedComponentsSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyComponent( const Visitable< ComponentVisitor_ABC >& component );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Component;
    typedef std::vector< T_Component > T_Components;
    //@}

private:
    //! @name Member data
    //@{
    T_Components components_;
    //@}
};

}

#endif // depend_StronglyConnectedComponentsSerializer_h
