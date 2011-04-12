//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_MetricSerializer_h
#define depend_MetricSerializer_h

#include "MetricsVisitor_ABC.h"
#include <map>

namespace xml
{
    class xostream;
}

namespace depend
{
    class Filter_ABC;
    template< typename T > class Visitable;

// =============================================================================
/** @class  MetricSerializer
    @brief  Metric serializer
*/
// Created: SLI 2010-08-20
// =============================================================================
class MetricSerializer : private MetricsVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MetricSerializer( const Visitable< MetricsVisitor_ABC >& metrics );
    virtual ~MetricSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos, const Filter_ABC& filter ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyMetrics( const std::string& module, unsigned int afferent, unsigned int efferent,
                                unsigned int external, unsigned int classes, unsigned int abstractClasses,
                                unsigned int abstractness, unsigned int instability, unsigned int distance );
    //@}

private:
    //! @name Types
    //@{
    struct T_Metric
    {
    public:
        T_Metric()
        {}
        unsigned int afferent_;
        unsigned int efferent_;
        unsigned int external_;
        unsigned int classes_;
        unsigned int abstract_;
        unsigned int abstractness_;
        unsigned int instability_;
        unsigned int distance_;
    };
    typedef std::map< std::string, T_Metric > T_UnitMetrics;
    //@}

private:
    //! @name Member data
    //@{
    T_UnitMetrics metrics_;
    //@}
};

}

#endif // depend_MetricSerializer_h
