//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_ExternalSerializer_h
#define depend_ExternalSerializer_h

#include "DependencyMetricVisitor_ABC.h"
#include <set>

namespace xml
{
    class xostream;
}

namespace depend
{
    class Filter_ABC;
    template< typename T > class Visitable;

// =============================================================================
/** @class  ExternalSerializer
    @brief  ExternalSerializer
*/
// Created: SLI 2010-09-07
// =============================================================================
class ExternalSerializer : private DependencyMetricVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ExternalSerializer( const Visitable< DependencyMetricVisitor_ABC >& metric, const Filter_ABC& filter );
    virtual ~ExternalSerializer();
    //@}

public:
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

private:
    //! @name Member data
    //@{
    const Filter_ABC& filter_;
    std::set< std::string > externals_;
    //@}
};

}

#endif // depend_ExternalSerializer_h
