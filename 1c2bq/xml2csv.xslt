<?xml version="1.0" encoding="windows-1251"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="�������">
  <xsl:variable name='source' select='.'/>
  <xsl:text>1;</xsl:text>
  <xsl:value-of select="@�������������"/>
  <xsl:text>;</xsl:text>
  <xsl:variable name="formdate" select="@����������������"/>
  <xsl:variable name="year" select="substring($formdate,1,4)"/>
  <xsl:variable name="month" select="substring($formdate,6,2)"/>
  <xsl:variable name="day" select="substring($formdate,9,2)"/>
  <xsl:value-of select="concat($day,'/',$month,'/',$year)"/>
  <xsl:text>;</xsl:text>
  <xsl:text>3;</xsl:text>
  
  <!-- This loops through the branch when a sibling meets a condition. -->
  <xsl:for-each select="����������������">
    <xsl:value-of select="����������"/>
    <xsl:text>&#10;</xsl:text>
  </xsl:for-each>

  <xsl:for-each select="������������������/���������">
    <xsl:text>2;</xsl:text>
    <xsl:value-of select="@���"/>
    <xsl:text>;</xsl:text>
    <xsl:value-of select="�������"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="���"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="��������"/>
    <xsl:text>;</xsl:text>
    <xsl:value-of select="�����������"/>
    <xsl:text>;</xsl:text>
    <xsl:value-of select="�����"/>
    <xsl:text>&#10;</xsl:text>
  </xsl:for-each>

</xsl:template>
</xsl:stylesheet>
