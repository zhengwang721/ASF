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