General notes
=============

The Python scripts in this directory operate on extensions. An extension consists of a content database (source files and meta-data) and a definition of the extension's identity, origin, external dependencies, documentation locations and other meta-data. The content meta-data is stored in asf.xml files, while the extension definition is stored in extension.xml. An extension can only have one extension.xml file, and this file marks the root directory of the extension.

The projects and modules in one extension can depend on modules in another extension. For the scripts to be able to operate on these projects and modules, the depending extensions must be located in a common parent directory as the main extension that the scripts are working on. For example, the directory structure could look like this:

	my_extensions/
		ASF/
		SomeExtension/
		YetAnExtension/

When invoking the scripts to operate with several extensions, the extension root directory must be specified. If none is specified, the scripts assume that the extension directory is two parent directories up (`..\..`) from the script's location. If the extension to work on is located elsewhere, the root directory must also be specified.

Further, the scripts assume they are to operate on the ASF and will by default attempt to load the latest version ASF extension. If one wishes to operate on a different extension, the main extension must be specified via its UUID (universally unique ID). If several versions of an extension are found, the latest is selected unless the user specifies otherwise.

See the various scripts' help documentation (`-h` switch) for details on specifying the extension root directory, as well as the main extension's UUID and version.

More information on the XML format for content meta-data (asf.xml) is found in the application note AVR4011.
The schemas and corresponding documentation, in PDF format, for both extension and content XML is available in the `schemas/` sub directory.

Technical Requirements
----------------------

- Python 2.6 or later. Python 3.x not supported. ( http://python.org/ )
- lxml 2.3 or later ( http://pypi.python.org/pypi/lxml/2.3 )


Project Generator
=================

About
-----

The project generator attempts to generate all the projects that are defined in the main extension's database, for all their supported toolchains. The user can override this behavior by explicitly stating which IDs and toolchains to generate projects for.

By default, the generated project files are put in sub directories under the location of the asf.xml that describes the respective projects, but it is also possible to export the files to a different location and even to .ZIP them.


Running the script
------------------

Run the script from command line with the `-h` parameter to get a printout of the available options for the script:

    $ python project_generator.py -h


Example usage
-------------

1: Generate specific project(s):

    $ python project_generator.py xmega.drivers.adc.example1.xplain avr32.drivers.adc.example.evk1104


2: Set a configuration value -- this will override configuration values set in the ASF XML:

    $ python project_generator.py -s config.compiler.optimization=high xmega.drivers.adc.example1.xplain avr32.drivers.adc.example.evk1104

*Note that generic compiler config will be overridden by toolchain-specific config if it is set ASF XML, e.g.,
   `config.compiler.optimization` will be overridden by `config.compiler.avrgcc.optimization` for the AVRGCC toolchain.*


3: Archive project(s) to .ZIP with Doxygen output:

    $ python project_generator.py -a zips -z -x xmega.drivers.adc.example1.xplain avr32.drivers.adc.example.evk1104

*Note that the directory (specified with `-a`) is relative to the location of `project_generator.py` and must be created in advance.*


4: Generate all unit test projects for 8- and 32-bit AVR GCC for a specific driver:

    $ python project_generator.py -g avrgcc,avr32gcc --project-type=unit-test common.services.fifo.*


5: Generate all projects in another extension than ASF, located elsewhere:

    $ python project_generator.py -b ../../../my_extensions --main-ext-uuid cfb7f948-d25a-4640-9cbb-d4f057e11c70


Console
=======

About
-----

The console is an application for querying the main extension's database and is intended to aid developers in checking that the information, dependencies and device support in the database are correct.

The look-up commands are limited to the main database, but the console is still capable of showing dependencies from other extensions.


Example usage
-------------

1: Start the console:

    $ python console.py

*A list of available commands will show as soon as the script is done loading the main extension's database.*


2: Print dependency tree for a project in another extension than ASF, resolved for a specific device:

    $ python console.py -b ../../../my_extensions --main-ext-uuid cfb7f948-d25a-4640-9cbb-d4f057e11c70 l some.project.id atxmega128a1

*For dependencies that are located in other extensions, the org, shortname and version of the relevant extensions will be listed.*

*To save the dependency tree to file add: `> output.txt`*


3: Look up all modules that require one module:

    $ python console.py r xmega.drivers.adc

*This reverse look-up will only show modules in the main extension's database that require the specified module.*


Atmel Studio Help Link Checker
==============================

About
-----

The link checker script is used to verify that the doxygen help and quick start guide links from the main extension's database are valid, ensuring that Atmel Studio will link correctly for the users. The results are output to a JUnit report compatible XML file, showing which URLs work and which do not.

For pre-release testing of the links, it is possible to override the base URL of the links. This allows one to test the links against a local server instead of having to put the documentation on the final documentation server, then test against it.


Example usage
-------------

1: Test links against a custom server:

    $ python check_atmelstudio6_help_links.py -u http://test-server.internal.my-corp.com/docs/latest/


Rebuild List Finder
===================

About
-----

The rebuild list finder script is used to identify the project IDs of an extension that are affected by changes to files. The purpose of this is to minimize the amount of projects that need to be tested when validating a branch before merging it into the trunk.

A list of changed files must be supplied to the script, for example based on commit logs for the relevant branch. It is vital that the paths in the supplied list are relative to the extension's root directory, i.e., where its extension.xml is located.


Example usage
-------------

1: Check another extension for projects to rebuild:

    $ python find_rebuild_list.py changed_files.txt -b ../../../my_extensions/ --main-ext-uuid cfb7f948-d25a-4640-9cbb-d4f057e11c70


Master Doxygen Generator
===================

About
-----

The master doxygen generator script generates the .doxygen and .html files needed to create the documentation package for an extension. Note that it does not invoke doxygen to generate the documentation from the .doxygen files -- this must be done afterwards.

By default, the script will write its output to `master_doxygen/` under the current directory, but the location can be overridden by the user.

The script will generate documentation for all components (projects and modules) for all devices unless the user specifies otherwise. The user can specify which device documentation groups (architecture, family or series) to include as well as which directories to exclude, including all their sub directories. The device documentation groups can be found in the file `device_maps/atmel.xml` -- look for the doc-arch attribute.

To reduce the size of the documentation, graphics and source code inclusion are disabled by default. However, the user can enable these if wanted.


Example usage
-------------

1: Generate documentation for the XMEGA A family, staying out of common/:

    $ python generate_master_doxygen.py -a xmegaa -b common


2: Generate documentation for all devices, with graphics and sources included, and output to a custom directory:

    $ python generate_master_doxygen.py -d -s -o ../../../my_extension_docs/


Unit Test Runner
================

About
-----

The unit test runner scripts are used to run a battery of unit tests for the Python scripts. These tests are necessary to check that the behavior and output of the core scripts are not inadvertently changed or broken, since this could cause all the generated output to be unusable.

The script will run all tests unless the user explicitly specifies which ones to run. All of the unit tests are defined in asf/testcases.py.


Example usage
-------------

1: Run all tests, stopping on the first failure:

    $ python run_tests.py -f


2: Run all tests for the FdkExtension class, with verbose output:

    $ python run_tests.py -v FdkExtensionTestCase


Module Build Tester
===================

About
-----

The module build tester script is used to build test modules which are user-selectable in Atmel Studio, on the available boards and for any devices that they support. The purpose of this is to ensure that a project will not break if a user adds f.ex. a graphics driver to a project template. The user can specify a list of modules to test (on command line or in a file), boards to test them with, and which MCUs _not_ to test with. Further, the script can create a JUnit compatible report -- highly recommended if many builds will be done.

The modules which are not user-selectable have explicitly been flagged to be hidden from GUIs in their asf.xml. This is done to hide modules which are not useful on their own, but which are required for the operation of other modules. Such modules will often not build correctly unless added to the project along with their dependant module, and so they are exempt from the build tests by default. Note that they will still be build tested by association if they have a non-hidden, dependant module.

The script is based on the use of the Atmel Studio template projects in ASF (`common.applications.user_application*` in `common/applications/user_application/`). Using these as templates, one \*GCC project is created per board and device combination that a module is to be tested with, with the module under test and any dependencies added to it. The projects are then attempted built, and any failures reported back to user. Note that the dependency on the template projects means that if there is no template project that covers a device, the modules will not be build tested for that device. A template project will support several devices if its board has an unspecified MCU, like the user boards do (see `board.user_board.*` in `common/boards/user_board/`).

By default, the script will only test with the user board project templates since they give good device coverage and have no special board definitions. To test with other boards, they must be explicitly specified.

The generated GCC projects, the build output and log are written to `regression_tests/<board ID>/<device name>/` under the home directory of each tested module unless a custom output directory is specified. In this case, the output directory structure becomes `<module ID>/<board ID>/<device name>/` in the output directory.


Example usage
-------------

1: Run all tests for user boards in a custom directory, outputting the results to a JUnit report:

    $ python run_tests.py -j -o ../../../build_tests


2: Run tests for ADC, EBI and DAC for the XMEGA-A1 Xplained:

    $ python test_modules.py -t board.xmega_a1_xplained -m xmega.drivers.adc#*,xmega.drivers.dac,xmega.drivers.ebi

*The ADC driver is listed with `xmega.drivers.adc#*` because `xmega.drivers.adc` is not a module, but a module selector.*
