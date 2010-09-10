//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_test_pch.h"
#include "depend/ModuleResolver.h"
#include "MockFinder.h"
#include <xeumeuleu/xml.hpp>

using namespace depend;


BOOST_AUTO_TEST_CASE( empty_resolver_returns_empty_module )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes/>"
        "   <excludes/>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( "module/file.h" ) );
}

BOOST_AUTO_TEST_CASE( resolver_finds_include_in_its_directory )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes>"
        "       <directory>include</directory>"
        "   </includes>"
        "   <excludes/>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    MOCK_EXPECT( finder, Find ).once().with( "include/module/file.h" ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module/file.h" ) );
}

BOOST_AUTO_TEST_CASE( resolver_checks_modules_in_all_its_directories )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes>"
        "       <directory>include</directory>"
        "       <directory>other</directory>"
        "   </includes>"
        "   <excludes/>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    mock::sequence s;
    MOCK_EXPECT( finder, Find ).once().with( "include/module/file.h" ).in( s ).returns( false );
    MOCK_EXPECT( finder, Find ).once().with( "other/module/file.h" ).in( s ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module/file.h" ) );
}

BOOST_AUTO_TEST_CASE( module_can_be_the_root_directory )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes>"
        "       <directory>include</directory>"
        "   </includes>"
        "   <excludes/>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    mock::sequence s;
    MOCK_EXPECT( finder, Find ).once().with( "include/file.h" ).in( s ).returns( true );
    BOOST_CHECK_EQUAL( "include", resolver.Resolve( "file.h" ) );
}

BOOST_AUTO_TEST_CASE( module_name_resolution_can_be_forced )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes>"
        "       <directory>include</directory>"
        "       <directory>include/sub-directory,module</directory>"
        "   </includes>"
        "   <excludes/>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    MOCK_EXPECT( finder, Find ).once().with( "include/file.h" ).returns( false );
    MOCK_EXPECT( finder, Find ).once().with( "include/sub-directory/file.h" ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "file.h" ) );
}

BOOST_AUTO_TEST_CASE( resolver_returns_empty_module_if_not_found )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes>"
        "       <directory>include</directory>"
        "   </includes>"
        "   <excludes/>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    MOCK_EXPECT( finder, Find ).once().with( "include/map" ).returns( false );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( "map" ) );
}

BOOST_AUTO_TEST_CASE( resolver_can_exclude_include_directories )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes/>"
        "   <excludes>"
        "       <directory>exclude</directory>"
        "   </excludes>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    MOCK_EXPECT( finder, Find ).once().with( "exclude/map" ).returns( true );
    BOOST_CHECK( resolver.IsExcluded( "map" ) );
    MOCK_EXPECT( finder, Find ).once().with( "exclude/other" ).returns( false );
    BOOST_CHECK( !resolver.IsExcluded( "other" ) );
}

BOOST_AUTO_TEST_CASE( resolver_throws_if_empty_directory_is_found )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes>"
        "       <directory/>"
        "   </includes>"
        "   <excludes/>"
        "</external>" );
    BOOST_CHECK_THROW( ModuleResolver( xis, finder ), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( resolver_cleans_directories_definitions )
{
    MockFinder finder;
    xml::xistringstream xis( 
        "<external>"
        "   <includes>"
        "       <directory>include/</directory>"
        "   </includes>"
        "   <excludes/>"
        "</external>" );
    ModuleResolver resolver( xis, finder );
    MOCK_EXPECT( finder, Find ).with( "include/module/file.h" ).returns( true );
    BOOST_CHECK_EQUAL( "module", resolver.Resolve( "module/file.h" ) );
}
