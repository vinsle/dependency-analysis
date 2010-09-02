//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "depend_pch.h"
#include "DotSerializer.h"
#include <sstream>
#include <xeuseuleu/xsl.hpp>

using namespace depend;

// -----------------------------------------------------------------------------
// Name: DotSerializer constructor
// Created: SLI 2010-08-26
// -----------------------------------------------------------------------------
DotSerializer::DotSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotSerializer destructor
// Created: SLI 2010-08-26
// -----------------------------------------------------------------------------
DotSerializer::~DotSerializer()
{
    // NOTHING
}

namespace
{
    static const std::string stylesheet =
        "<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>"
            "<xsl:output method='text'/>"
            ""
            "<xsl:template match='/report'>"
                "digraph G {"
                "<xsl:apply-templates select='dependencies/dependency'/>"
                "<xsl:apply-templates select='categories/category'/>"
                "}"
            "</xsl:template>"
            ""
            "<xsl:template match='dependencies/dependency'>"
                "<xsl:variable name='name' select='@name'/>"
                "<xsl:variable name='instability' select='/report/categories/category[@name=$name]/metrics/instability'/>"
                "<xsl:variable name='abstractness' select='/report/categories/category[@name=$name]/metrics/abstractness'/>"
                "<xsl:variable name='distance' select='/report/categories/category[@name=$name]/metrics/distance'/>"
                "<xsl:variable name='color' select='( 100 - $distance ) div 360'/>"
                "&quot;<xsl:value-of select='@name'/>&quot;[label=\"\\N\\nI=<xsl:value-of select='$instability'/> A=<xsl:value-of select='$abstractness'/> D=<xsl:value-of select='$distance'/>\",shape=rectangle,style=filled,color=\"<xsl:value-of select='$color'/> 1.0 1.0\"];"
            "</xsl:template>"
            ""
            "<xsl:template match='categories/category'>"
                "<xsl:variable name='name' select='@name'/>"
                "<xsl:apply-templates select='efferent-dependencies/dependency'>"
                    "<xsl:with-param name='parent' select='$name'/>"
                "</xsl:apply-templates>"
            "</xsl:template>"
            ""
            "<xsl:template match='efferent-dependencies/dependency'>"
                "<xsl:param name='parent'/>"
                "<xsl:choose>"
                    "<xsl:when test='(/report/strongly-connected-components/component/module=$parent) and (/report/strongly-connected-components/component/module=@name)'>"
                        "<xsl:apply-templates select='/report/strongly-connected-components/component'>"
                            "<xsl:with-param name='from' select='$parent'/>"
                            "<xsl:with-param name='to' select='@name'/>"
                            "<xsl:with-param name='number' select='@number'/>"
                        "</xsl:apply-templates>"
                    "</xsl:when>"
                    "<xsl:otherwise>"
                        "&quot;<xsl:value-of select='$parent'/>&quot;->&quot;<xsl:value-of select='@name'/>&quot;;"
                    "</xsl:otherwise>"
                "</xsl:choose>"
            "</xsl:template>"
            ""
            "<xsl:template match='strongly-connected-components/component'>"
                "<xsl:param name='from'/>"
                "<xsl:param name='to'/>"
                "<xsl:param name='number'/>"
                "<xsl:choose>"
                    "<xsl:when test='( module=$from ) and ( module=$to )'>"
                        "<xsl:variable name='color' select='position() div last()'/>"
                        "&quot;<xsl:value-of select='$from'/>&quot;->&quot;<xsl:value-of select='$to'/>&quot;[color=\"<xsl:value-of select='$color'/> 1.0 1.0\"];"
                    "</xsl:when>"
                    "<xsl:otherwise>"
                        "<xsl:if test='( module=$from )'>"
                            "&quot;<xsl:value-of select='$from'/>&quot;->&quot;<xsl:value-of select='$to'/>&quot;;"
                        "</xsl:if>"
                    "</xsl:otherwise>"
                "</xsl:choose>"
            "</xsl:template>"
            ""
        "</xsl:stylesheet>";
}

// -----------------------------------------------------------------------------
// Name: DotSerializer::Serialize
// Created: SLI 2010-08-26
// -----------------------------------------------------------------------------
void DotSerializer::Serialize( xml::xistream& xis, std::ostream& os ) const
{
    std::istringstream ss( stylesheet );
    xsl::xstringtransform xst( ss );
    xst << xis;
    os << xst.str() << std::endl;
}
