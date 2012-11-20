<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" indent="yes"/>
  <xsl:variable name="figure" select="1"/>

  <xsl:template match="/doxygen">
    <book>
      <title>
        <xsl:value-of select="compounddef/title"/>
      </title>
      <subtitle>
        <xsl:text>ASF PROGRAMMERS MANUAL</xsl:text>
      </subtitle>
      <preface id="{compounddef[@kind ='group'][1]/@id}">
        <title>
          <xsl:value-of select="compounddef/title"/>
        </title>
          <xsl:apply-templates select="compounddef[@kind ='group'][1]/detaileddescription/para"/>
      </preface>
      <xsl:for-each select="compounddef[@kind ='group']">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
      <xsl:for-each select="compounddef[@kind ='page']">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
    </book>
  </xsl:template>

  <xsl:template match="compounddef[@kind ='group']">
    <chapter id="{@id}" xreflabel="{title}">
      <title>
        <xsl:value-of select="title"/>
      </title>
      <xsl:apply-templates select="detaileddescription"/>
    </chapter>
  </xsl:template>

  <xsl:template match="compounddef[@kind ='group'][1]">
    <xsl:for-each select="detaileddescription">
      <xsl:call-template name="detaileddescription_special"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="detaileddescription_special">
    <xsl:for-each select="sect1">
      <xsl:call-template name="sect1_special"/>
    </xsl:for-each>
  </xsl:template>

<xsl:template name="sect1_special">
    <xsl:choose>
      <xsl:when test="title='API Overview'">
        <xsl:call-template name="API-Overview"/>
      </xsl:when>
      <xsl:otherwise>
        <chapter id="{@id}" xreflabel="{title}">
          <title>
            <xsl:value-of select="title"/>
          </title>
          <xsl:for-each select="para">
            <xsl:apply-templates select="current()"/>
          </xsl:for-each>
          <xsl:for-each select="sect2">
            <xsl:apply-templates select="current()"/>
          </xsl:for-each>
        </chapter>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template match="compounddef[@kind ='page']">
    <xsl:choose>
      <xsl:when test="@id='indexpage'">
      </xsl:when>
      <!--<xsl:when test="@id='quickstart'">
      </xsl:when>-->
      <xsl:otherwise>
      <chapter id="{@id}" xreflabel="{title}">
        <xsl:choose>
          <xsl:when test="@id='_license'">
            <title>
              <xsl:value-of select="compoundname"/>
            </title>
          </xsl:when>
          <xsl:otherwise>
            <title>
              <xsl:value-of select="title"/>
            </title>
          </xsl:otherwise>
        </xsl:choose>

        <!-- Add all the other documentation -->
        <xsl:apply-templates select="detaileddescription"/>
      </chapter>
    </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Used for making struct look nice in the documentation -->
  <xsl:template match="//compounddef[@kind ='struct']">
    <section id="{@id}" xreflabel="{compoundname}">
      <title> struct <xsl:value-of select="compoundname"/>
      </title>
      <xsl:apply-templates select="detaileddescription"/>
      <xsl:for-each select="sectiondef[@kind='public-attrib']">
        <informaltable tabstyle="striped">
          <title>
            <xsl:value-of select="../compoundname"/>
          </title>
          <tgroup cols="3">
            <thead>
              <row>
                <entry>Data Fields</entry>
              </row>
            </thead>
            <tbody>
              <xsl:for-each select="memberdef">
                <row id="{@id}" xreflabel="{name}">
                  <entry>
                    <xsl:apply-templates select="type"/>
                  </entry>
                  <entry>
                    <xsl:value-of select="name"/>
                  </entry>
                  <entry>
                    <xsl:apply-templates select="detaileddescription"/>
                  </entry>
                </row>
              </xsl:for-each>
            </tbody>
          </tgroup>
        </informaltable>
      </xsl:for-each>
    </section>
  </xsl:template>

  <xsl:template match="compounddef">
    <!-- For any other compounddef -->
  </xsl:template>

  <xsl:template match="type">
    <xsl:for-each select="node()">
      <xsl:apply-templates select="."/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="sectiondef">
    <para>
      <xsl:value-of select="description"/>
    </para>
    <xsl:for-each select="memberdef">
      <xsl:apply-templates select="."/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="//sectiondef[@kind='var']">
    <informaltable tabstyle="striped">
      <tgroup cols="2">
        <tbody>
          <xsl:for-each select="memberdef">
            <row>
              <entry align="right">
                <xsl:apply-templates select="type"/>
              </entry>
              <entry>
                <xsl:value-of select="name"/>
              </entry>
            </row>
          </xsl:for-each>
        </tbody>
      </tgroup>
    </informaltable>
  </xsl:template>

  <xsl:template match="memberdef[@kind ='function']">
    <section id="{@id}" xreflabel="{name}">
      <title>
        <xsl:value-of select="name"/><xsl:text>()</xsl:text>
      </title>
      <emphasis role="italic">
        <xsl:value-of select="briefdescription"/>
      </emphasis>
      <programlisting language="c">
        <xsl:value-of select="definition"/>
        <xsl:text>&#10;</xsl:text>
        <xsl:text>&#9;</xsl:text>
        <xsl:text>&#9;</xsl:text>
        <xsl:apply-templates select="argsstring"/>
      </programlisting>
      <xsl:apply-templates select="detaileddescription"/>
    </section>
  </xsl:template>

  <xsl:template match="memberdef[@kind = 'enum']">
    <section id="{@id}" xreflabel="{name}">
      <title>enum <xsl:value-of select="name"/></title>
      <xsl:apply-templates select="detaileddescription"/>
      <informaltable tabstyle="striped">
        <title>
          <xsl:text>enum </xsl:text><xsl:value-of select="name"/>
        </title>
        <tgroup cols="2">
          <thead>
            <row>
              <entry>enum <xsl:value-of select="name"/></entry>
              <entry>Description</entry>
            </row>
          </thead>
          <tbody>
            <xsl:for-each select="enumvalue">
              <row>
                <entry>
                  <para id="{@id}" xreflabel="{name}">
                    <xsl:value-of select="name"/>
                  </para>
                </entry>
                <entry>
                  <para>
                    <xsl:apply-templates select="detaileddescription"/>
                  </para>
                </entry>
              </row>
            </xsl:for-each>
          </tbody>
        </tgroup>
      </informaltable>
    </section>
  </xsl:template>

  <xsl:template match="memberdef[@kind = 'define']">
    <section id="{@id}" xreflabel="{name}">
      <title>
        <xsl:text>Macro </xsl:text><xsl:value-of select="name"/>
      </title>
      <informaltable tabstyle="striped">
        <tgroup cols="2">
          <thead>
            <row>
              <entry>Initializer</entry>
              <entry>Description</entry>
            </row>
          </thead>
          <tbody>
            <row>
              <entry>
                <para>
                  <xsl:value-of select="initializer"/>
                </para>
              </entry>
              <entry>
                <para>
                  <xsl:value-of select="detaileddescription"/>
                </para>
              </entry>
            </row>
          </tbody>
        </tgroup>
      </informaltable>
    </section>
  </xsl:template>

  <xsl:template match="simplesect">
    <note>
      <xsl:choose>
        <xsl:when test="@kind='warning'">
          <title>Warning</title>
        </xsl:when>
        <xsl:when test="@kind='note'">
          <title>Note</title>
        </xsl:when>
        <xsl:when test="@kind='return'">
          <title>Returns</title>
        </xsl:when>
        <xsl:when test="@kind='pre'">
          <title>Precondition</title>
        </xsl:when>
        <xsl:when test="@kind='par'">
          <title>
            <xsl:value-of select="title"/>
          </title>
        </xsl:when>
        <xsl:when test="@kind='see'">
          <title>See also</title>
        </xsl:when>
        <xsl:otherwise>
          <title>UNHANDLELED TITLE</title>
        </xsl:otherwise>
      </xsl:choose>
      <para>
        <xsl:value-of select="."/>
      </para>
    </note>
  </xsl:template>

  <xsl:template match="parameterlist">
      <xsl:choose>
        <xsl:when test="@kind='param'">
          <emphasis role="bold">Parameters</emphasis>
          <informaltable tabstyle="striped">
            <tgroup cols="3">
              <thead>
                <row>
                  <entry>Data direction</entry>
                  <entry>Parameter name</entry>
                  <entry>Description</entry>
                </row>
              </thead>
              <tbody>
                <xsl:for-each select="parameteritem">
                  <row>
                    <xsl:apply-templates select="."/>
                  </row>
                </xsl:for-each>
              </tbody>
            </tgroup>
          </informaltable>
        </xsl:when>
        <xsl:when test="@kind='retval'">
          <emphasis role="bold">Return</emphasis>
          <informaltable tabstyle="striped">
            <tgroup cols="2">
              <thead>
                <row>
                  <entry>Values</entry>
                  <entry>Description</entry>
                </row>
              </thead>
              <tbody>
                <xsl:for-each select="parameteritem">
                  <row>
                    <xsl:apply-templates select="."/>
                  </row>
                </xsl:for-each>
              </tbody>
            </tgroup>
          </informaltable>
        </xsl:when>
      </xsl:choose>

  </xsl:template>

  <xsl:template match="parameteritem">
    <xsl:if test="parameternamelist/parametername/@direction!=''">
      <entry>
        <para>
          <emphasis role="bold"> [<xsl:value-of select="parameternamelist/parametername/@direction"
            />] </emphasis>
        </para>
      </entry>
    </xsl:if>
    <entry>
      <para>
        <xsl:value-of select="parameternamelist/parametername"/>
      </para>
    </entry>
    <entry>
      <xsl:for-each select="parameterdescription">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
    </entry>
  </xsl:template>

  <xsl:template match="detaileddescription">
    <xsl:for-each select="para">
      <xsl:apply-templates select="."/>
    </xsl:for-each>
    <xsl:for-each select="sect1">
      <xsl:apply-templates select="current()"/>
    </xsl:for-each>
  </xsl:template>

  <!-- Generate the API Overview chapter -->
  <xsl:template name="API-Overview">
    <chapter>
      <title>
        <xsl:value-of select="title"/>
      </title>

      <!-- Show all variable information -->
      <xsl:if test="count(../../sectiondef[@kind='var'])>0">
        <section>
          <title>Variables</title>
          <xsl:apply-templates select="//sectiondef[@kind='var']"/>
        </section>
      </xsl:if>

      <!-- Show all the struct information -->
      <xsl:if test="count(../../innerclass)>0">
        <section>
          <title>Data Structure Documentation</title>
          <xsl:for-each select="../../innerclass">
            <xsl:apply-templates select="//compounddef[@id=current()/@refid]"/>
          </xsl:for-each>
        </section>
      </xsl:if>

      <!-- Show all the macro information -->
      <xsl:if test="count(../../sectiondef[memberdef/@kind='define' or @kind='define'])>0">
        <section>
          <title>Macro definitions</title>
          <xsl:for-each select="../../sectiondef[memberdef/@kind='define' or @kind='define']">
            <section>
              <title>
                <xsl:value-of select="header"/>
                </title>
              <xsl:apply-templates select="."/>
            </section>
          </xsl:for-each>
        </section>
      </xsl:if>

      <!-- Show all the function information -->
      <xsl:if test="count(../../sectiondef[memberdef/@kind='function' or @kind='func'])>0">
        <section>
          <title>Function calls</title>
          <xsl:for-each select="../../sectiondef[memberdef/@kind='function' or @kind='func']">
            <section>
              <title>
                <xsl:value-of select="header"/>
              </title>
              <xsl:apply-templates select="."/>
            </section>
          </xsl:for-each>
        </section>
      </xsl:if>

      <!-- Show all the enum information -->
      <xsl:if test="count(../../sectiondef[@kind ='enum'])>0">
        <section>
          <title>Enumeration Type Documentation</title>
          <xsl:for-each select="../../sectiondef[@kind ='enum']">
            <xsl:apply-templates select="."/>
          </xsl:for-each>
        </section>
      </xsl:if>
    </chapter>
  </xsl:template>


  <xsl:template match="sect1">
    <xsl:choose>
      <xsl:when test="title='API Overview'">
        <xsl:call-template name="API-Overview"/>
      </xsl:when>
      <xsl:otherwise>
        <section id="{@id}" xreflabel="{title}">
          <title>
            <xsl:value-of select="title"/>
          </title>
          <xsl:for-each select="para">
            <xsl:apply-templates select="current()"/>
          </xsl:for-each>
          <xsl:for-each select="sect2">
            <xsl:apply-templates select="current()"/>
          </xsl:for-each>
        </section>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="sect2">
    <section id="{@id}" xreflabel="{title}">
      <title>
        <xsl:value-of select="title"/>
      </title>
      <xsl:for-each select="para">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
      <xsl:for-each select="sect3">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
    </section>
  </xsl:template>

  <xsl:template match="sect3">
    <section id="{@id}" xreflabel="{title}">
      <title>
        <xsl:value-of select="title"/>
      </title>
      <xsl:for-each select="para">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
    </section>
  </xsl:template>

  <xsl:template match="//para">
    <para>
      <!--<xsl:text>(HERE PARA START)</xsl:text>-->
      <xsl:for-each select="node()">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
      <!--<xsl:text>(HERE PARA END)</xsl:text>-->
    </para>
  </xsl:template>

  <xsl:template match="//bold">
    <emphasis role="bold">
      <xsl:value-of select="."/>
    </emphasis>
  </xsl:template>

  <xsl:template match="//text()">
    <!--<xsl:text>(TS)</xsl:text>-->
    <xsl:value-of select="."/>
    <!--<xsl:text>(TE)</xsl:text>-->
  </xsl:template>

  <xsl:template match="listitem//text()">
    <!--<xsl:text>(LTS)</xsl:text>-->
    <xsl:value-of select="."/>
    <!--<xsl:text>(LTE)</xsl:text>-->
  </xsl:template>

  <xsl:template match="table">
    <informaltable tabstyle="striped">
      <tgroup cols="{@cols}">
        <thead>
          <xsl:for-each select="row[1]">
            <row>
              <xsl:for-each select="entry">
                <entry>
                  <xsl:value-of select="."/>
                </entry>
              </xsl:for-each>
            </row>
          </xsl:for-each>
        </thead>
        <tbody>
          <xsl:for-each select="row[position() != 1]">
            <row>
              <xsl:for-each select="entry">
                <entry>
                  <xsl:value-of select="."/>
                </entry>
              </xsl:for-each>
            </row>
          </xsl:for-each>
        </tbody>
      </tgroup>
    </informaltable>
  </xsl:template>

  <xsl:template match="itemizedlist">
    <itemizedlist>
      <xsl:for-each select="listitem">
        <listitem>
          <xsl:apply-templates select="."/>
        </listitem>
      </xsl:for-each>
    </itemizedlist>
  </xsl:template>

  <xsl:template match="orderedlist">
    <orderedlist>
      <xsl:for-each select="listitem">
        <listitem>
          <xsl:apply-templates select="."/>
        </listitem>
      </xsl:for-each>
    </orderedlist>
  </xsl:template>

  <xsl:template match="programlisting">
    <programlisting language="c">
    <xsl:for-each select="codeline">
      <xsl:apply-templates select="."/>
      <xsl:text>&#10;</xsl:text>
    </xsl:for-each>
    </programlisting>
  </xsl:template>

  <xsl:template match="dot">
    <xsl:variable name="pos" select="position()"/>
    <figure>
      <title>
        <xsl:value-of select="../../title"/>
      </title>
      <!--      <graphic>
        <xsl:attribute name="fileref">html/dot_inline_dotgraph_<xsl:number format="1" level="any" count="dot"/>.svg</xsl:attribute>
      </graphic>-->
      <mediaobject>
        <imageobject>
          <imagedata scalefit="1" width="100%" contentdepth="100%">
            <xsl:attribute name="fileref">html/dot_inline_dotgraph_<xsl:number format="1"
                level="any" count="dot"/>.svg</xsl:attribute>
          </imagedata>
        </imageobject>
      </mediaobject>
    </figure>
  </xsl:template>

  <xsl:template match="ref">
    <xref linkend="{@refid}" xrefstyle="nopage"/>
  </xsl:template>

  <xsl:template match="highlight//text()">
    <xsl:value-of select="."/>
  </xsl:template>
  <xsl:template match="highlight//sp">
    <xsl:text>&#32;</xsl:text>
  </xsl:template>

  <xsl:template match="//formula">
    <equation>
    <latex align="center" xmlns="http://forge.scilab.org/p/jlatexmath">
      <xsl:value-of select="."/>
    </latex>
    </equation>
  </xsl:template>

  <xsl:template name="splitargs" match="argsstring">
    <xsl:param name="text" select="."/>
    <xsl:value-of select="substring-before(concat($text,','),',')" />
    <xsl:if test="contains($text,',')">
      <xsl:text>,&#10;&#09; </xsl:text>
        <xsl:call-template name="splitargs">
          <xsl:with-param name="text" select="substring-after($text,',')" />
      </xsl:call-template>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>
