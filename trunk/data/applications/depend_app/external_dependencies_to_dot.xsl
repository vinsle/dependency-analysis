<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text"/>
    
    <xsl:template match="/report">
        digraph G {
        <xsl:apply-templates select="dependencies/dependency"/>
        <xsl:apply-templates select="categories/category"/>
        }
    </xsl:template>

    <xsl:template match="dependencies/dependency">
        &quot;<xsl:value-of select="@name"/>&quot;[shape=rectangle,style=filled];
    </xsl:template>

    <xsl:template match="categories/category">
        <xsl:variable name="name" select="@name"/>
        <xsl:apply-templates select="external-dependencies/dependency">
            <xsl:with-param name="parent" select="$name"/>
        </xsl:apply-templates>
    </xsl:template>

    <xsl:template match="external-dependencies/dependency">
        <xsl:param name="parent"/>
        &quot;<xsl:value-of select="$parent"/>&quot;->&quot;<xsl:value-of select="@name"/>&quot;[label="<xsl:value-of select="@number"/>"];
    </xsl:template>

</xsl:stylesheet>