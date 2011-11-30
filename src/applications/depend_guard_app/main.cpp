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
#ifdef _MSC_VER
#   pragma warning( pop )
#endif
#include <boost/foreach.hpp>
#include <iostream>
#include <xeumeuleu/xml.hpp>

namespace bpo = boost::program_options;

namespace
{
    void CheckOptions( const bpo::variables_map& vm, const bpo::options_description& cmdline )
    {
        if( vm.count( "help" ) )
            std::cout << "Usage: depend_guard --dependencies=input.xml --graph=graph.xml" << std::endl
                      << cmdline << std::endl;
        else if( vm.count( "version" ) )
            application::Version().Serialize( std::cout, "depend_guard" );
        else if( ! vm.count( "graph" ) )
            throw std::invalid_argument( "Invalid application option argument: missing input graph for analysis" );
        else if( ! vm.count( "dependencies" ) )
            throw std::invalid_argument( "Invalid application option argument: missing input for dependency guard check" );
    }
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description cmdline( "Allowed options" );
        cmdline.add_options()
            ( "help,h"                                   , "produce help message" )
            ( "version,v"                                , "produce version message" )
            ( "graph", bpo::value< std::string >()       , "set graph input file for analysis" )
            ( "dependencies", bpo::value< std::string >(), "set input xml file for dependency guard check" );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( cmdline ).run(), vm );
        bpo::notify( vm );
        CheckOptions( vm, cmdline );
        return vm;
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
        xml::xifstream xisDependencies( vm[ "dependencies" ].as< std::string >() );
        const Facade facade( xisGraph );
        if( facade.Process( xisDependencies ) )
            return EXIT_FAILURE;
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
