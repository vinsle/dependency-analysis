//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_EdgeSerializer_h
#define depend_EdgeSerializer_h

#include "DependencyMetricVisitor_ABC.h"
#include <set>
#include <map>

namespace xml
{
    class xostream;
}

namespace depend
{
    class DependencyMetric_ABC;
    class Filter_ABC;

// =============================================================================
/** @class  EdgeSerializer
    @brief  Edge serializer
*/
// Created: SLI 2010-09-07
// =============================================================================
class EdgeSerializer : private DependencyMetricVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EdgeSerializer( const DependencyMetric_ABC& dependencies );
    virtual ~EdgeSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos, const Filter_ABC& filter ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule );
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Modules;
    typedef std::map< std::string, T_Modules > T_Dependencies;
    //@}

private:
    //! @name Member data
    //@{
    T_Modules modules_;
    T_Dependencies afferent_;
    T_Dependencies efferent_;
    T_Dependencies external_;
    //@}
};

}

#endif // depend_EdgeSerializer_h
