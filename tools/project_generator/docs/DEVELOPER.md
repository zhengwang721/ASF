# Introduction to extension XML

This section shows a skeleton extension.xml and documents which parts of it are
mandatory and which are optional, along with details about the tag attributes.
For further details on the XML, please refer to the PDF found in
`tools/project_generator/schemas/`.


## Skeleton XML:

    <fdk xml-version="2.0">
        <extension fullname="" org="" shortname="" uuid="" version="">
            <author email="" name="" website=""/>
            <description/>
            <icon-image path=""/>
            <preview-image path=""/>
    <!-- search-terms is optional -->
            <search-terms>
                <term/>
            </search-terms>
            <license caption="" path=""/>
            <release-notes caption="" url=""/>
            <online-help>
                <index-page url="" caption=""/>
    <!-- the two following tags are optional -->
                <!-- scheme can be "append" or "asf-docs" -->
                <module-help-page baseurl="" scheme=""/>
                <module-guide-page baseurl="" scheme=""/>
            </online-help>
    <!-- offline-help is optional -->
            <offline-help>
    <!-- the following three tags are optional -->
                <index-page path="" caption=""/>
                <!-- scheme can be "append" or "asf-docs" -->
                <module-help-page basepath="" scheme=""/>
                <module-guide-page basepath="" scheme=""/>
            </offline-help>
    <!-- dependencies is optional -->
            <dependencies>
                <fdk-extension eid="" uuidref="" version=""/>
            </dependencies>
        </extension>
    </fdk>


## Tag requirements

Unless otherwise noted, tags should be assumed to be required. If a tag is
optional, but can contain child tags, the child tags are by default required
if the parent tag exists. However, if all child tags are optional, at least
one of them must exist if the parent tag exists.


## Attribute requirements and details

All attributes of all tags are required.

For the `extension` tag, the `org` and `shortname` attributes are meant to be
path-safe, i.e., so that they can be used to name directories. The `version`
attribute cannot contain letters, only numbers separated by periods. The only
limitation on the number of periods is that there must be at least one.

The `caption` attributes are meant to contain the text with which the various
documentation elements can be presented to users.

The `path` attribute of the tags are meant to contain paths to files, relative
to the location of the extension.xml. Naturally, the `url` attribute of the tags
are meant to contain a URL.

The `basepath` and `baseurl` attributes are meant to contain a partial path or
URL that can be built upon to get the specific help pages for a database
element, e.g., a project or module.

The `scheme` attribute determines how the specific help page paths and URLs are
constructed from the base path and URL.

For the `fdk-dependencies` tag, the attribute `eid` can consist of lower capital
letters, numbers, periods, dashes and underlines. This is to ensure that they
look similar to the `id` attribute of modules. The attribute `version` can be
specified in several formats:
   1. a specific version number: `1.2.3.4`
   1. a version series using `+` as wildcard: `1.2.+`
   1. the latest available version: `latest`
   1. a range of allowable versions, for example:
      * `(, )` for any version -- same as `latest`.
      * `(, 1.2.3)` for any version sooner than 1.2.3, not inclusive.
      * `(0.1.2, 0.1.9]` for any version later than 0.1.2, not inclusive, but no
         later than 0.1.9, inclusive.
      * `[1.2.5, )` for any version later than 1.2.5, inclusive.


* * *


# Linking to extension documentation

This section documents and exemplifies how documentation for an extension can be
linked to the individual modules and projects.


## Location specifiers in extension.xml

Referring to the skeleton extension.xml above, the online and offline
documentation locations can be specified with the following tags:

    <online-help>
        <index-page url="..." caption="..."/>
        <module-help-page baseurl="..." scheme="..."/>
        <module-guide-page baseurl="..." scheme="..."/>
    </online-help>

    <offline-help>
        <index-page path="..." caption="..."/>
        <module-help-page basepath="..." scheme="..."/>
        <module-guide-page basepath="..." scheme="..."/>
    </offline-help>

The only tag that is mandatory for an extension.xml file, is the `online-help/
index-page` one. This means that an extension author must in the very least
supply a URL to where user-browsable documentation can be found.

The tags `module-help-page` and `module-guide-page` are respectively used to
specify the base location of the module API and project documentation pages, and
of the quick start guides. These behave in pretty much the same way for online
and offline help documentation. Note that the path and basepath for offline
documentation is relative to the containing extension.xml.

The attribute `scheme` specifies how the URL or path to the documentation for a
specific module or project shall be created from the base location. It can have
one of two values:
   1. `asf-docs`: this scheme assumes that the documentation is structured
      like the output of the script `generate_master_doxygen.py`, only requiring
      some build items in the asf.xml for generating parts of the URL or path.
   1. `append`: this scheme is a simpler approach which basically allows the
      developer to hardcode the URL or path per module or project. In this case,
      the common part (base location) is put in the extension.xml, while the
      asf.xml files contain the parts to append per module or project.

Under both schemes, the asf.xml must contain module- or project-specific
information about the location. The only exception is for projects under the
`asf-docs` scheme, in which case the file name is already known (index.html).

It is also possible to link to both online and offline documentation without
relying on any base location or scheme in the extension.xml. This is the
simplest way to link to documentation and is particularily useful if, e.g., the
documentation for a single module in the extension is located somewhere else
than the rest. The entire path and/or URL must then be hardcoded in the relevant
asf.xml file.

The XML needed for all three cases are shown in the next section.


## Location specifiers in asf.xml

For API and project documentation, and quick start guides, there are three
different ways in which one can specify the documentation's precise location,
depending on which scheme (if any) has been set for the documentation in the
extension.xml.

In the following subsections, these are shown by example. The examples are based
on an XMEGA ADC driver and assume we want to add both a quick start guide and
API documentation -- it is not actually required to add both of these.


### Hardcode entire location in asf.xml

Let's say we need to specify an online quick start guide URL and offline API
documentation path for the driver. The following XML snippet shows how to add
this with two build items.

    <module type="driver" id="xmega.drivers.adc#0"
            caption="ADC - XMEGA A/AU Implementation">
        ...
        <build type="online-help" subtype="module-guide-page"
                value="http://www.example.com/help/xmega_adc.html"/>
        <build type="offline-help" subtype="module-help-page"
                value="doc/xmega_adc.html"/>
        ...
    </module>

The URL to the online quick start guide is here
`http://www.example.com/help/xmega_adc.html`, while the path to the offline API
documentation is `doc/xmega_adc.html` relative to the location of the asf.xml.

In this case, it does not matter if documentation base locations or schemes have
been specified in the extension.xml since the hardcoded locations will take
priority.


### Specify location for asf-docs scheme

If the extension's documentation is written and structured as in ASF, it must be
defined in the extension.xml and two build items be added to the module. The XML
shown below is for the case where quick start guides are available online, and
API documentation is available offline.

In extension.xml:

    <online-help>
        <index-page url="..." caption="..."/>
        <module-guide-page baseurl="http://example.com/doc/" scheme="asf-docs"/>
    </online-help>

    <offline-help>
        <index-page path="..." caption="..."/>
        <module-help-page basepath="doc/" scheme="asf-docs"/>
    </offline-help>

Two things to note:
   1. for online documentation only, the version number will automatically be
      appended to the base URL.
   1. the base location of offline help is relative to the location of the
      extension.xml.

In asf.xml:

    <module type="driver" id="xmega.drivers.adc#0"
            caption="ADC - XMEGA A/AU Implementation">
        ...
        <build type="doxygen-entry-point" value="adc_group"/>
        <build type="doxygen-quick-start" value="adc_quickstart"/>
        <generator value="doxygen-module"
                default-mcu="unspecified-xmegaa1"/>
        ...
    </module>

The two build items respectively specify the name of the driver's doxygen group
and the name of the driver's quick start page. These names are translated to
filenames when the documentation URL or path are determined.

Be aware that the script `generate_master_doxygen.py` generates the
documentation for _all_ doc-arch MCU groups (see the device map) that contain a
device supported by the module.
The exact, generated documentation URL and path depend on which device it is
requested for. If requested for an XMEGA A device, the result could be
`http://example.com/doc/1.2.3/xmegaa/html/group__adc__group.html` for the API
documentation and `doc\xmegaa\html\adc_quickstart.html`, relative to the
location of the extension.xml, for the quick start guide.

For projects and application modules, there is no build item to add. However,
for the project documentation to be generated, the doxygen generator should be
added to its asf.xml:

    <project id="xmega.drivers.adc.example.xplain" ...>
        ...
        <generator value="doxygen"/>
    </project>

One final note: under the asf-docs scheme, there is no differentiation between
online and offline documentation via the build items. This means there is no way
to specify that, e.g., some quick start guides are available online and some are
available offline -- it is assumed that all the documentation is available at
the specified base locations.


### Specify location for append scheme

If the extension documentation is not generated by use of the
`generate_master_doxygen.py` script, and is available in a common location, the
append scheme should be used.

The XML shown below is for the case where all documentation is available both
online and offline, but not in the same format between the two.

In extension.xml:

    <online-help>
        <index-page url="..." caption="..."/>
        <module-help-page baseurl="http://example.com/help/" scheme="append"/>
        <module-guide-page baseurl="http://example.com/guides/"
                scheme="append"/>
    </online-help>

    <offline-help>
        <index-page path="..." caption="..."/>
        <module-help-page basepath="help/" scheme="append"/>
        <module-guide-page basepath="guides/" scheme="append"/>
    </offline-help>

In asf.xml: 

    <module type="driver" id="xmega.drivers.adc#0"
            caption="ADC - XMEGA A/AU Implementation">
        ...
        <build type="online-help" subtype="module-help-page-append"
                value="xmega/adc.html#api"/>
        <build type="online-help" subtype="module-guide-page-append"
                value="xmega/adc.html#intro"/>
        <build type="offline-help" subtype="module-help-page-append"
                value="xmega/adc.pdf"/>
        <build type="offline-help" subtype="module-guide-page-append"
                value="xmega/adc.pdf"/>
        <generator value="doxygen-module"
                default-mcu="unspecified-xmegaa1"/>


The generated URLs and paths to the documentation will in this case not depend
on which device it is requested for, but will always be the same. For the API
documentation, they will be `http://example.com/help/xmega/adc.html#api` and
`help\xmega\adc.pdf`, relative to the location of the extension.xml. For the
quick start guides, they will be
`http://example.com/guides/xmega/adc.html#intro` and `guides\xmega\adc.pdf`,
relative to the location of the extension.xml.

Note that the version number is not appended here, as it is for the asf-docs
scheme. The developer must therefore ensure that the base location is updated
accordingly when the version changes.