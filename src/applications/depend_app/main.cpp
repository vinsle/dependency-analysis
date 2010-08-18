//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend/ModuleObserver_ABC.h"
#include "depend/FileObserver_ABC.h"
#include "depend/IncludeObserver_ABC.h"
#include "depend/LineObserver_ABC.h"
#include "depend/ClassObserver_ABC.h"
#include "depend/Facade.h"
#pragma warning( push, 0 )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <iostream>

namespace bpo = boost::program_options;

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
    class ClassObserver : private depend::ClassObserver_ABC
    {
    public:
        explicit ClassObserver( depend::Subject< depend::ClassObserver_ABC >& facade )
            : facade_( facade )
        {
            facade_.Register( *this );
        }
        virtual ~ClassObserver()
        {
            facade_.Unregister( *this );
        }
    private:
        virtual void NotifyClass( const std::string& name )
        {
            std::cout <<  "class " << name << std::endl;
        }
        virtual void NotifyAbstractness()
        {
            std::cout <<  "abstract" << std::endl;
        }
    private:
        depend::Subject< depend::ClassObserver_ABC >& facade_;
    };
    class IncludeObserver : private depend::IncludeObserver_ABC
    {
    public:
        explicit IncludeObserver( depend::Subject< depend::IncludeObserver_ABC >& facade )
            : facade_( facade )
        {
            facade_.Register( *this );
        }
        virtual ~IncludeObserver()
        {
            facade_.Unregister( *this );
        }
    private:
        virtual void NotifyInternalInclude( const std::string& file )
        {
            std::cout <<  "internal: " << file << std::endl;
        }
        virtual void NotifyExternalInclude( const std::string& file )
        {
            std::cout << "external: " << file << std::endl;
        }
    private:
        depend::Subject< depend::IncludeObserver_ABC >& facade_;
    };
    class FileObserver : private depend::FileObserver_ABC
    {
    public:
        FileObserver( depend::Subject< depend::FileObserver_ABC >& facade )
            : facade_( facade )
        {
            facade_.Register( *this );
        }
        virtual ~FileObserver()
        {
            facade_.Unregister( *this );
        }
    private:
        virtual void NotifyFile( const std::string& path, std::istream& /*stream*/ )
        {
            std::cout << "file: " << path << std::endl;
        }
    private:
        depend::Subject< depend::FileObserver_ABC >& facade_;
    };
    class ModuleObserver : private depend::ModuleObserver_ABC
    {
    public:
        ModuleObserver( depend::Subject< depend::ModuleObserver_ABC >& facade )
            : facade_( facade )
        {
            facade_.Register( *this );
        }
        virtual ~ModuleObserver()
        {
            facade_.Unregister( *this );
        }
    private:
        virtual void NotifyModule( const std::string& module )
        {
            std::cout << "module: " << module << std::endl;
        }
    private:
        depend::Subject< depend::ModuleObserver_ABC >& facade_;
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
        depend::Facade facade;
        ModuleObserver moduleObserver( facade );
        FileObserver fileObserver( facade );
        IncludeObserver includeObserver( facade );
        ClassObserver classObserver( facade );
        facade.Visit( path );
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
