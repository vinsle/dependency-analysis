//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "ExternalSerializer.h"
#include "DependencyMetric_ABC.h"
#include "Filter_ABC.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: ExternalSerializer constructor
// Created: SLI 2010-09-07
// -----------------------------------------------------------------------------
ExternalSerializer::ExternalSerializer( DependencyMetric_ABC& metric, const Filter_ABC& filter )
    : filter_( filter )
{
    metric.Apply( *this );
}

// -----------------------------------------------------------------------------
// Name: ExternalSerializer destructor
// Created: SLI 2010-09-07
// -----------------------------------------------------------------------------
ExternalSerializer::~ExternalSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExternalSerializer::Serialize
// Created: SLI 2010-09-07
// -----------------------------------------------------------------------------
void ExternalSerializer::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "externals" );
    BOOST_FOREACH( const std::string& external, externals_ )
        xos << xml::content( "external", external );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ExternalSerializer::NotifyInternalDependency
// Created: SLI 2010-09-07
// -----------------------------------------------------------------------------
void ExternalSerializer::NotifyInternalDependency( const std::string& /*fromModule*/, const std::string& /*toModule*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExternalSerializer::NotifyExternalDependency
// Created: SLI 2010-09-07
// -----------------------------------------------------------------------------
void ExternalSerializer::NotifyExternalDependency( const std::string& fromModule, const std::string& toModule )
{
    if( filter_.Check( fromModule ) )
        externals_.insert( toModule );
}
