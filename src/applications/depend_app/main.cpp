//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend/ModuleVisitor.h"
#include "depend/ModuleObserver_ABC.h"
#include "depend/FileVisitor.h"
#include "depend/FileObserver_ABC.h"
#include <boost/assign.hpp>
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
            ( "help"                             , "produce help message" )
            ( "path", bpo::value< std::string >(), "set path directory" );
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
    class FileObserver : private depend::FileObserver_ABC
    {
    public:
        FileObserver( depend::FileVisitor& visitor, const std::string& module )
            : visitor_( visitor )
            , module_ ( module )
        {
            visitor_.Register( *this );
        }
        ~FileObserver()
        {
            visitor_.Unregister( *this );
        }
    private:
        virtual void Notify( const std::string& path )
        {
            std::cout << module_ << ":" << path << std::endl;
        }
    private:
        depend::FileVisitor& visitor_;
        const std::string module_;
    };
    class ModuleObserver : private depend::ModuleObserver_ABC
    {
    public:
        ModuleObserver( depend::ModuleVisitor& visitor, const std::string& path )
            : visitor_( visitor )
            , path_   ( path )
        {
            visitor_.Register( *this );
        }
        ~ModuleObserver()
        {
            visitor_.Unregister( *this );
        }
    private:
        virtual void Notify( const std::string& module )
        {
            const std::vector< std::string > extensions = boost::assign::list_of( ".h" )( ".hh" )( ".hpp" )( ".hxx" )
                                                                                ( ".inl" )( ".ipp" )( ".cxx" )
                                                                                ( ".c" )( ".cc" )( ".cpp" );
            depend::FileVisitor visitor( extensions );
            FileObserver observer( visitor, module );
            visitor.Visit( path_ + "/" + module );
        }
    private:
        depend::ModuleVisitor& visitor_;
        const std::string path_;
    };
}

int main( int argc, char* argv[] )
{
    try
    {
        const bpo::variables_map vm = ParseCommandLine( argc, argv );
        if( vm.count( "help" ) )
            return EXIT_SUCCESS;
        const std::string path = vm[ "path" ].as< std::string >();
        depend::ModuleVisitor visitor;
        ModuleObserver observer( visitor, path );
        visitor.Visit( path );
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
