//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend/Facade.h"
#pragma warning( push, 0 )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <iostream>
#include <xeumeuleu/xml.hpp>

namespace bpo = boost::program_options;

namespace
{
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help"                               , "produce help message" )
            ( "path"  , bpo::value< std::string >(), "set path directory" )
            ( "output", bpo::value< std::string >(), "set output file" );
        bpo::positional_options_description p;
        p.add( "path", 1 );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( desc ).positional( p ).run(), vm );
        bpo::notify( vm );
        if( vm.count( "help" ) )
            std::cout << desc << std::endl;
        else if( ! vm.count( "path" ) )
            throw std::invalid_argument( "Invalid application option argument: missing path file" );
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
        const std::string path = vm[ "path" ].as< std::string >();
        depend::Facade facade;
        facade.Visit( path );
        if( !vm.count( "output" ) )
        {
            xml::xostringstream xos;
            facade.Serialize( xos );
            std::cout << xos.str();
        }
        else
        {
            xml::xofstream xos( vm[ "output" ].as< std::string >() );
            facade.Serialize( xos );
        }
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