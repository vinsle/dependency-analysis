//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/TransitiveReductionFilter.h"
#include "MockDependencyMetric.h"
#include "MockFilter.h"
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

using namespace depend;

namespace
{
    typedef std::pair< std::string, std::string > T_Edge;
    typedef std::vector< T_Edge > T_Edges;
    bool Notify( DependencyMetricVisitor_ABC& visitor, const T_Edges& edges )
    {
        BOOST_FOREACH( const T_Edge& edge, edges )
            visitor.NotifyInternalDependency( edge.first, edge.second );
        return true;
    }
    class Fixture
    {
    public:
        MockDependencyMetric metric;
        MockFilter mockFilter;
    };
}

BOOST_FIXTURE_TEST_CASE( unknown_node_is_filtered, Fixture )
{
    MOCK_EXPECT( metric, Apply );
    TransitiveReductionFilter filter( metric, mockFilter );
    MOCK_EXPECT( mockFilter, Check ).once().with( "unknown" ).returns( false );
    BOOST_CHECK( !filter.Check( "unknown" ) );
}

BOOST_FIXTURE_TEST_CASE( reduction_checks_all_root_nodes, Fixture )
{
    const T_Edges edges = boost::assign::map_list_of( "a", "b" )( "c", "b" );
    MOCK_EXPECT( metric, Apply ).once().with( boost::bind( &Notify, _1, boost::cref( edges ) ) );
    MOCK_EXPECT( mockFilter, Check ).once().with( "a" ).returns( false );
    MOCK_EXPECT( mockFilter, Check ).once().with( "b" ).returns( false );
    MOCK_EXPECT( mockFilter, Check ).once().with( "c" ).returns( false );
    TransitiveReductionFilter filter( metric, mockFilter );
}

BOOST_FIXTURE_TEST_CASE( reduction_on_node_filters_any_non_reachable_node, Fixture )
{
    const T_Edges edges = boost::assign::map_list_of( "a", "b" )( "c", "b" );
    MOCK_EXPECT( metric, Apply ).once().with( boost::bind( &Notify, _1, boost::cref( edges ) ) );
    MOCK_EXPECT( mockFilter, Check ).once().with( "a" ).returns( true );
    MOCK_EXPECT( mockFilter, Check ).once().with( "b" ).returns( false );
    MOCK_EXPECT( mockFilter, Check ).once().with( "c" ).returns( false );
    TransitiveReductionFilter filter( metric, mockFilter );
    mock::verify();
    MOCK_EXPECT( mockFilter, Check ).once().with( "a" ).returns( true );
    BOOST_CHECK( filter.Check( "a" ) );
    MOCK_EXPECT( mockFilter, Check ).once().with( "b" ).returns( false );
    BOOST_CHECK( filter.Check( "b" ) );
    MOCK_EXPECT( mockFilter, Check ).once().with( "c" ).returns( false );
    BOOST_CHECK( !filter.Check( "c" ) );
}

BOOST_FIXTURE_TEST_CASE( reduction_finds_every_reachable_nodes, Fixture )
{
    const T_Edges edges = boost::assign::map_list_of( "a", "b" )( "b", "c" );
    MOCK_EXPECT( metric, Apply ).once().with( boost::bind( &Notify, _1, boost::cref( edges ) ) );
    MOCK_EXPECT( mockFilter, Check ).once().with( "a" ).returns( true );
    MOCK_EXPECT( mockFilter, Check ).once().with( "b" ).returns( false );
    MOCK_EXPECT( mockFilter, Check ).once().with( "c" ).returns( false );
    TransitiveReductionFilter filter( metric, mockFilter );
    mock::verify();
    MOCK_EXPECT( mockFilter, Check ).once().with( "a" ).returns( true );
    BOOST_CHECK( filter.Check( "a" ) );
    MOCK_EXPECT( mockFilter, Check ).once().with( "b" ).returns( false );
    BOOST_CHECK( filter.Check( "b" ) );
    MOCK_EXPECT( mockFilter, Check ).once().with( "c" ).returns( false );
    BOOST_CHECK( filter.Check( "c" ) );
}
