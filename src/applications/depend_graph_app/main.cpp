//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "application/Version.h"
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable: 4512 4996 )
#endif
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

namespace bpo = boost::program_options;

namespace
{
    void CheckOptions( const bpo::variables_map& vm, const bpo::options_description& cmdline )
    {
        if( vm.count( "help" ) )
            std::cout << "Usage: depend_graph [options] --output=output graph.xml" << std::endl
                      << cmdline << std::endl;
        else if( vm.count( "version" ) )
            application::Version().Serialize( std::cout, "depend_graph" );
        else if( ! vm.count( "input" ) )
            throw std::invalid_argument( "Invalid application option argument: missing input graph for analysis" );
        else if( ! vm.count( "output" ) )
            throw std::invalid_argument( "Invalid application option argument: missing output file" );
    }
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description cmdline( "Allowed options" );
        bpo::options_description options( "Generic options" );
        options.add_options()
            ( "help,h"                                                      , "produce help message" )
            ( "version,v"                                                   , "produce version message" )
            ( "input" , bpo::value< std::string >()                         , "set graph input file for analysis" )
            ( "output", bpo::value< std::string >()                         , "set output file" )
            ( "filter", bpo::value< std::vector< std::string > >()          , "select only modules in filter and their afferent and efferent modules" )
            ( "extend"                                                      , "extend to all reachable modules if filter is enabled" )
            ( "all"                                                         , "render a graph centered on each node" );
        bpo::options_description graph( "Graph options (only for graph stage)" );
        graph.add_options()
            ( "layout", bpo::value< std::string >()->default_value( "dot" )           , "set layout algorithm (dot|neato)" )
            ( "format", bpo::value< std::string >()->default_value( "png" )           , "set graph format (png|jpg|svg|pdf)" )
            ( "graph,g", bpo::value< std::vector< std::string > >()                   , "set graph options (see http://www.graphviz.org/doc/info/attrs.html)" )
            ( "node,n", bpo::value< std::vector< std::string > >()                    , "set node options (see http://www.graphviz.org/doc/info/attrs.html)" )
            ( "edge,e", bpo::value< std::vector< std::string > >()                    , "set edge options (see http://www.graphviz.org/doc/info/attrs.html)" )
            ( "dependencies", bpo::value< std::string >()->default_value( "internal" ), "set optional external dependencies drawing (internal|external|both)" );
        cmdline.add( options ).add( graph );
        bpo::positional_options_description p;
        p.add( "input", -1 );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( cmdline ).positional( p ).run(), vm );
        bpo::notify( vm );
        CheckOptions( vm, cmdline );
        return vm;
    }
    void SerializeGraphOptions( xml::xostream& xos, const bpo::variables_map& vm, const std::string& option )
    {
        xos << xml::start( option );
        if( vm.count( option ) )
        {
            BOOST_FOREACH( const std::string& option, vm[ option ].as< std::vector< std::string > >() )
            {
                std::vector< std::string > buffer;
                boost::algorithm::split( buffer, option, boost::is_any_of( "=" ) );
                if( buffer.size() != 2 )
                    throw std::invalid_argument( "Invalid application graph argument: '" + option + "' is malformed" );
                xos << xml::start( "option" )
                        << xml::attribute( "name", buffer.at( 0 ) )
                        << xml::attribute( "value", buffer.at( 1 ) )
                    << xml::end;
            }
        }
        xos << xml::end;
    }
    void Serialize( xml::xostream& xos, const std::string& node, const std::vector< std::string >& options )
    {
        BOOST_FOREACH( const std::string& option, options )
            xos << xml::content( node, option );
    }
    std::auto_ptr< xml::xobufferstream > Translate( const bpo::variables_map& vm )
    {
        std::auto_ptr< xml::xobufferstream > xobs( new xml::xobufferstream() );
        *xobs << xml::start( "configuration" )
                << xml::content( "dependencies", vm[ "dependencies" ].as< std::string >() )
                << xml::content( "output", vm[ "output" ].as< std::string >() )
                << xml::content( "extend", vm.count( "extend" ) )
                << xml::content( "all", vm.count( "all" ) )
                << xml::content( "input", vm[ "input" ].as< std::string >() )
                << xml::start( "filters" );
        if( vm.count( "filter" ) )
            Serialize( *xobs, "filter", vm[ "filter" ].as< std::vector< std::string > >() );
        *xobs   << xml::end
                << xml::start( "graph-options" )
                    << xml::content( "layout", vm[ "layout" ].as< std::string >() )
                    << xml::content( "format", vm[ "format" ].as< std::string >() );
        SerializeGraphOptions( *xobs, vm, "graph" );
        SerializeGraphOptions( *xobs, vm, "node" );
        SerializeGraphOptions( *xobs, vm, "edge" );
        *xobs   << xml::end
             << xml::end;
        return xobs;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        const bpo::variables_map vm = ParseCommandLine( argc, argv );
        if( vm.count( "help" ) || vm.count( "version" ) )
            return EXIT_SUCCESS;
        std::auto_ptr< xml::xobufferstream > xobs = Translate( vm );
        Facade facade( *xobs );
        facade.Process( *xobs );
        return EXIT_SUCCESS;
    }
    catch( std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl
                  << "Use -h or --help for help on allowed options" << std::endl;
    }
    catch( ... )
    {
        std::cerr << "Unknown exception caught" << std::endl;
    }
    return EXIT_FAILURE;
}
