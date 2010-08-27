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
#include <boost/foreach.hpp>

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

    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help,h"                                           , "produce help message" )
            ( "path" , bpo::value< std::vector< std::string > >(), "add a directory containing modules for analysis" )
            ( "output", bpo::value< std::string >()              , "set output file" )
            ( "format", bpo::value< std::string >()              , "set output format (xml|dot|png)" )
            ( "version,v"                                        , "produce version message" );
        bpo::positional_options_description p;
        p.add( "path", -1 );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( desc ).positional( p ).run(), vm );
        bpo::notify( vm );
        if( vm.count( "help" ) )
            std::cout << "Usage: depend_app [options] path1 path2..." << std::endl
                      << desc << std::endl;
        else if( vm.count( "version" ) )
            std::cout << "depend " << version << " (built " << time << ")" << std::endl << std::endl
                      << "Copyright Silvin Lubecki 2010" << std::endl
                      << "Distributed under the Boost Software License, Version 1.0. (See" << std::endl
                      << "accompanying file LICENSE_1_0.txt or copy at" << std::endl
                      << "http://www.boost.org/LICENSE_1_0.txt)" << std::endl
                      << "See http://code.google.com/p/dependency-analysis for more informations" << std::endl;
        else if( ! vm.count( "path" ) )
            throw std::invalid_argument( "Invalid application option argument: missing directory for analysis" );
        else if( vm.count( "format" ) && vm[ "format" ].as< std::string >() != "xml" && vm[ "format" ].as< std::string >() != "dot" && vm[ "format" ].as< std::string >() != "png" )
            throw std::invalid_argument( "Invalid application option argument: format '" + vm[ "format" ].as< std::string >() + "' is not supported" );
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
        depend::Facade facade;
        BOOST_FOREACH( const std::string& path, vm[ "path" ].as< std::vector< std::string > >() )
            facade.Visit( path );
        if( vm.count( "format" ) && vm[ "format" ].as< std::string >() == "png" )
        {
            if( !vm.count( "output" ) )
                throw std::invalid_argument( "Invalid application option argument: output argument must be filled with 'png' format" );
            facade.Serialize( vm[ "output" ].as< std::string >() );
            return EXIT_SUCCESS;
        }
        const bool isDotFormat = vm.count( "format" ) && vm[ "format" ].as< std::string >() == "dot";
        std::ostream* out = &std::cout;
        if( vm.count( "output" ) )
            out = new std::ofstream( vm[ "output" ].as< std::string >().c_str() );
        if( isDotFormat )
            facade.Serialize( *out );
        else
        {
            xml::xostringstream xos;
            facade.Serialize( xos );
            *out << xos.str();
        }
        if( vm.count( "output" ) )
            delete out;
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
