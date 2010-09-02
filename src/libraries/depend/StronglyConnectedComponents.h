//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_StronglyConnectedComponents_h
#define depend_StronglyConnectedComponents_h

#include "DependencyMetricVisitor_ABC.h"
#pragma warning( push, 0 )
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#pragma warning( pop )
#include <vector>

namespace xml
{
    class xostream;
}

namespace depend
{
    class DependencyMetric_ABC;
    class Filter_ABC;

// =============================================================================
/** @class  StronglyConnectedComponents
    @brief  Strongly connected components
*/
// Created: SLI 2010-08-23
// =============================================================================
class StronglyConnectedComponents : private DependencyMetricVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StronglyConnectedComponents( const DependencyMetric_ABC& metric );
    virtual ~StronglyConnectedComponents();
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
    typedef boost::property< boost::vertex_name_t, std::string > T_Property;
    typedef boost::adjacency_list<> T_AdjacencyGraph;
    typedef boost::labeled_graph< T_AdjacencyGraph, std::string > T_Graph;
    typedef std::map< T_Graph::vertex_descriptor, std::string > T_Labels;
    //@}

private:
    //! @name Member data
    //@{
    T_Graph graph_;
    T_Labels labels_;
    //@}
};

}

#endif // depend_StronglyConnectedComponents_h
