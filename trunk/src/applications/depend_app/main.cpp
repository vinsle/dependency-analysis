//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <xeumeuleu/xml.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

namespace
{
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help"         , "produce help message" )
            ( "configuration", bpo::value< std::string >(), "set configuration file" );
        bpo::positional_options_description p;
        p.add( "configuration", 1 );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( desc ).positional( p ).run(), vm );
        bpo::notify( vm );
        if( vm.count( "help" ) )
            std::cout << desc << std::endl;
        else if( ! vm.count( "configuration" ) )
            throw std::invalid_argument( "Invalid application option argument: missing configuration file" );
        return vm;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        const bpo::variables_map vm = ParseCommandLine( argc, argv );
        if( vm.count( "help" ) )
            return EXIT_SUCCESS;
        const std::string configuration = vm[ "configuration" ].as< std::string >();
        xml::xifstream xis( configuration, xml::internal_grammar() );
        xis >> xml::start( "configuration" );
        return EXIT_SUCCESS;
    }
    catch( std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "Unknown exception caught" << std::endl;
    }
    return EXIT_FAILURE;
}
