//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "Filter.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace depend;

namespace
{
    typedef std::set< std::string > T_Modules;
    void ReadModule( xml::xistream& xis, T_Modules& modules )
    {
        modules.insert( xis.value< std::string >() );
    }
    T_Modules ReadModules( xml::xisubstream xis )
    {
        T_Modules result;
        xis >> xml::start( "filters" )
                >> xml::list( "filter", boost::bind( &ReadModule, _1, boost::ref( result ) ) );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Filter constructor
// Created: SLI 2010-08-30
// -----------------------------------------------------------------------------
Filter::Filter( xml::xisubstream xis )
    : modules_( ReadModules( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter destructor
// Created: SLI 2010-08-30
// -----------------------------------------------------------------------------
Filter::~Filter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter::Check
// Created: SLI 2010-08-30
// -----------------------------------------------------------------------------
bool Filter::Check( const std::string& module ) const
{
    return CheckCore( module );
}

// -----------------------------------------------------------------------------
// Name: Filter::CheckCore
// Created: SLI 2010-09-10
// -----------------------------------------------------------------------------
bool Filter::CheckCore( const std::string& module ) const
{
    return modules_.empty() ? true : modules_.find( module ) != modules_.end();
}
