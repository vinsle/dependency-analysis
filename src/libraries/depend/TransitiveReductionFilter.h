//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_TransitiveReductionFilter_h
#define depend_TransitiveReductionFilter_h

#include "Filter_ABC.h"
#include "DependencyVisitor_ABC.h"
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable: 4701 4702 4996 )
#elif defined __GNUC__
#   pragma GCC system_header
#endif
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/properties.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif
#include <vector>

namespace depend
{
    template< typename T > class Visitable;

// =============================================================================
/** @class  TransitiveReductionFilter
    @brief  Transitive reduction filter
*/
// Created: SLI 2010-09-08
// =============================================================================
class TransitiveReductionFilter : public Filter_ABC, private DependencyVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TransitiveReductionFilter( const Visitable< DependencyVisitor_ABC >& metric, const Filter_ABC& filter );
    virtual ~TransitiveReductionFilter();
    //@}
    
    //! @name Operations
    //@{
    virtual bool Check( const std::string& module ) const;
    virtual bool CheckCore( const std::string& module ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyInternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    virtual void NotifyExternalDependency( const std::string& fromModule, const std::string& toModule, const std::string& context );
    //@}

    //! @name Helpers
    //@{
    bool CheckMap( const std::string& module ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::adjacency_list<> T_AdjacencyGraph;
    typedef boost::labeled_graph< T_AdjacencyGraph, std::string > T_Graph;
    typedef std::map< T_Graph::vertex_descriptor, std::string > T_Labels;
    typedef std::vector< boost::default_color_type > T_ColorMap;
    //@}

private:
    //! @name Member data
    //@{
    const Filter_ABC& filter_;
    T_Graph graph_;
    T_Labels labels_;
    T_ColorMap colormap_;
    //@}
};


}

#endif // __TransitiveReductionFilter_h_
