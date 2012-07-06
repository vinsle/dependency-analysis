//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_StronglyConnectedComponents_h
#define depend_StronglyConnectedComponents_h

#include "DependencyVisitor_ABC.h"
#include "Visitable.h"
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#elif defined __GNUC__
#   pragma GCC system_header
#endif
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif
#include <vector>

namespace depend
{
    class Filter_ABC;
    class StronglyConnectedComponentsVisitor_ABC;
    template< typename T > class Visitable;

// =============================================================================
/** @class  StronglyConnectedComponents
    @brief  Strongly connected components
*/
// Created: SLI 2010-08-23
// =============================================================================
class StronglyConnectedComponents : public Visitable< StronglyConnectedComponentsVisitor_ABC >
                                  , private DependencyVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StronglyConnectedComponents( const Visitable< DependencyVisitor_ABC >& metric, const Filter_ABC& filter );
    virtual ~StronglyConnectedComponents();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( StronglyConnectedComponentsVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::adjacency_list<> T_AdjacencyGraph;
    typedef boost::labeled_graph< T_AdjacencyGraph, std::string > T_Graph;
    typedef std::map< T_Graph::vertex_descriptor, std::string > T_Labels;
    //@}

private:
    //! @name Member data
    //@{
    T_Graph graph_;
    T_Labels labels_;
    const Filter_ABC& filter_;
    //@}
};

}

#endif // depend_StronglyConnectedComponents_h
