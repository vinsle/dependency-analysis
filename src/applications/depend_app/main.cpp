//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend/Facade.h"
#pragma warning( push, 0 )
#pragma warning( disable: 4512 4996 )
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <iostream>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifndef BUILD_VERSION
#   error BUILD_VERSION undefined !
#endif
#ifndef BUILD_TIME
#   error BUILD_TIME undefined !
#endif

namespace bpo = boost::program_options;

namespace
{
    static const std::string version = TOSTRING( BUILD_VERSION );
    static const std::string time = TOSTRING( BUILD_TIME );

    void CheckOptions( const bpo::variables_map& vm, const bpo::options_description& cmdline )
    {
        if( vm.count( "help" ) )
            std::cout << "Usage: depend_app [options] path1 path2..." << std::endl
                      << cmdline << std::endl;
        else if( vm.count( "version" ) )
            std::cout << "depend " << version << " (built " << time << ")" << std::endl << std::endl
                      << "Copyright Silvin Lubecki 2010" << std::endl
                      << "Distributed under the Boost Software License, Version 1.0. (See" << std::endl
                      << "accompanying file LICENSE_1_0.txt or copy at" << std::endl
                      << "http://www.boost.org/LICENSE_1_0.txt)" << std::endl
                      << "See http://code.google.com/p/dependency-analysis for more informations" << std::endl;
        else if( ! vm.count( "path" ) )
            throw std::invalid_argument( "Invalid application option argument: missing directory for analysis" );
        else if( vm.count( "stage" ) && vm[ "stage" ].as< std::string >() != "xml" && vm[ "stage" ].as< std::string >() != "dot" && vm[ "stage" ].as< std::string >() != "graph" )
            throw std::invalid_argument( "Invalid application option argument: stage '" + vm[ "stage" ].as< std::string >() + "' is not supported" );
        else if( vm.count( "stage" ) && vm[ "stage" ].as< std::string >() == "graph" && vm[ "output" ].as< std::string >().empty() )
            throw std::invalid_argument( "Invalid application option argument: output argument must be filled with 'graph' renderer" );
    }

    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description cmdline( "Allowed options" );
        bpo::options_description options( "Generic options" );
        options.add_options()
            ( "help,h"                                                      , "produce help message" )
            ( "version,v"                                                   , "produce version message" )
            ( "path" , bpo::value< std::vector< std::string > >()           , "add a directory containing modules for analysis" )
            ( "output", bpo::value< std::string >()->default_value( "" )    , "set output file" )
            ( "filter", bpo::value< std::vector< std::string > >()          , "select only modules in filter and their afferent and efferent modules" )
            ( "include,I", bpo::value< std::vector< std::string > >()       , "add an include directory path for external dependency (dependency name can be forced with following syntax: --include=\"directory,name\")" )
            ( "exclude,E", bpo::value< std::vector< std::string > >()       , "add an include directory path excluded from the dependencies and warnings" )
            ( "warning"                                                     , "enable warnings" )
            ( "stage", bpo::value< std::string >()->default_value( "graph" ), "set analysis stage for output (xml => dot => graph)" )
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
        p.add( "path", -1 );
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
        xos << xml::end;
    }
    void MakeExtensions( xml::xostream& xos )
    {
        xos << xml::start( "extensions" )
                << xml::content( "extension", ".h" )
                << xml::content( "extension", ".hh" )
                << xml::content( "extension", ".hpp" )
                << xml::content( "extension", ".hxx" )
                << xml::content( "extension", ".inl" )
                << xml::content( "extension", ".ipp" )
                << xml::content( "extension", ".cxx" )
                << xml::content( "extension", ".c" )
                << xml::content( "extension", ".cc" )
                << xml::content( "extension", ".cpp" )
            << xml::end;
    }
    void Serialize( xml::xostream& xos, const std::string& node, const std::vector< std::string >& options )
    {
        BOOST_FOREACH( const std::string& option, options )
            xos << xml::content( node, option );
    }
    std::auto_ptr< xml::xobufferstream > Translate( const bpo::variables_map& vm )
    {
        std::auto_ptr< xml::xobufferstream > xobs( new xml::xobufferstream() );
        *xobs << xml::start( "configuration" );
        MakeExtensions( *xobs );
        *xobs   << xml::content( "dependencies", vm[ "dependencies" ].as< std::string >() )
                << xml::content( "warning", vm.count( "warning" ) > 0 )
                << xml::content( "extend", vm.count( "extend" ) > 0 )
                << xml::start( "external" )
                    << xml::start( "includes" );
        if( vm.count( "include" ) )
            Serialize( *xobs, "directory", vm[ "include" ].as< std::vector< std::string > >() );
        *xobs       << xml::end
                    << xml::start( "excludes" );
        if( vm.count( "exclude" ) )
            Serialize( *xobs, "directory", vm[ "exclude" ].as< std::vector< std::string > >() );
        *xobs       << xml::end
                << xml::end
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
        depend::Facade facade( *xobs );
        BOOST_FOREACH( const std::string& path, vm[ "path" ].as< std::vector< std::string > >() )
            facade.Visit( path );
        facade.Serialize( vm[ "stage" ].as< std::string >(), vm[ "output" ].as< std::string >(), vm.count( "all" ) > 0 );
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
