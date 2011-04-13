//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "Facade.h"
#include "application/Version.h"
#pragma warning( push, 0 )
#pragma warning( disable: 4512 4996 )
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

namespace bpo = boost::program_options;

namespace
{
    void CheckOptions( const bpo::variables_map& vm, const bpo::options_description& cmdline )
    {
        const application::Version version;
        if( vm.count( "help" ) )
            std::cout << "Usage: depend [options] path1 path2..." << std::endl
                      << cmdline << std::endl;
        else if( vm.count( "version" ) )
            application::Version().Serialize( std::cout, "depend" );
        else if( ! vm.count( "path" ) )
            throw std::invalid_argument( "Invalid application option argument: missing directory for analysis" );
    }
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description cmdline( "Allowed options" );
        cmdline.add_options()
            ( "help,h"                                               , "produce help message" )
            ( "version,v"                                            , "produce version message" )
            ( "path" , bpo::value< std::vector< std::string > >()    , "add a directory containing modules for analysis" )
            ( "output", bpo::value< std::string >()                  , "set output file" )
            ( "include,I", bpo::value< std::vector< std::string > >(), "add an include directory path for external dependency (dependency name can be forced with following syntax: --include=\"directory,name\")" )
            ( "exclude,E", bpo::value< std::vector< std::string > >(), "add an include directory path excluded from the dependencies and warnings" )
            ( "warning"                                              , "enable warnings" );
        bpo::positional_options_description p;
        p.add( "path", -1 );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( cmdline ).positional( p ).run(), vm );
        bpo::notify( vm );
        CheckOptions( vm, cmdline );
        return vm;
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
        *xobs   << xml::content( "output", vm[ "output" ].as< std::string >() )
                << xml::content( "warning", vm.count( "warning" ) )
                << xml::start( "paths" );
        Serialize( *xobs, "path", vm[ "path" ].as< std::vector< std::string > >() );
        *xobs   << xml::end
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
