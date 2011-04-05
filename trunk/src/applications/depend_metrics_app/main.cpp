//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#pragma warning( push, 0 )
#pragma warning( disable: 4512 4996 )
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#pragma warning( pop )
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
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
            std::cout << "Usage: depend_metrics_app [options] graph.xml" << std::endl
                      << cmdline << std::endl;
        else if( vm.count( "version" ) )
            std::cout << "depend " << version << " (built " << time << ")" << std::endl << std::endl
                      << "Copyright Silvin Lubecki 2011" << std::endl
                      << "Distributed under the Boost Software License, Version 1.0. (See" << std::endl
                      << "accompanying file LICENSE_1_0.txt or copy at" << std::endl
                      << "http://www.boost.org/LICENSE_1_0.txt)" << std::endl
                      << "See http://code.google.com/p/dependency-analysis for more informations" << std::endl;
        else if( ! vm.count( "graph" ) )
            throw std::invalid_argument( "Invalid application option argument: missing input graph for analysis" );
    }
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description cmdline( "Allowed options" );
        cmdline.add_options()
            ( "help,h"                                            , "produce help message" )
            ( "version,v"                                         , "produce version message" )
            ( "graph", bpo::value< std::string >()                , "set graph input file for analysis" )
            ( "output", bpo::value< std::string >()               , "set output metrics file" )
            ( "filter", bpo::value< std::vector< std::string > >(), "select only modules in filter and their afferent and efferent modules" )
            ( "extend"                                            , "extend to all reachable modules if filter is enabled" );
        bpo::variables_map vm;
        bpo::positional_options_description p;
        p.add( "graph", -1 );
        bpo::store( bpo::command_line_parser( argc, argv ).options( cmdline ).positional( p ).run(), vm );
        bpo::notify( vm );
        CheckOptions( vm, cmdline );
        return vm;
    }
    std::auto_ptr< xml::xobufferstream > Translate( const bpo::variables_map& vm )
    {
        std::auto_ptr< xml::xobufferstream > xobs( new xml::xobufferstream() );
        *xobs << xml::start( "configuration" )
                  << xml::content( "output", vm[ "output" ].as< std::string >() )
                  << xml::content( "extend", vm.count( "extend" ) )
                  << xml::start( "filters" );
        if( vm.count( "filter" ) )
            BOOST_FOREACH( const std::string& filter, vm[ "filter" ].as< std::vector< std::string > >() )
                *xobs << xml::content( "filter", filter );
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
        xml::xifstream xisGraph( vm[ "graph" ].as< std::string >() );
        Facade facade( xisGraph );
        std::auto_ptr< xml::xobufferstream > xisConfiguration = Translate( vm );
        facade.Process( *xisConfiguration );
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
