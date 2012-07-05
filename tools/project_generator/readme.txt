ASF Project Generator
=====================

About
-----

The Atmel Software Framework (ASF) project generator works with the XML files in an ASF tree. When the script starts it will parse all XML files in the specified base folder and sub folders, build a database with all the information, resolve all dependencies between modules and then generate project files for each project it is able to find, unless otherwise specified.

More information on the XML format is found in the application note AVR4011.


Requirements
------------

Python 2.6 or later. Python 3.x not supported. ( http://python.org/ )
lxml 2.3 or later ( http://pypi.python.org/pypi/lxml/2.3 )


Running the script
------------------

Run the script from command line with the -h parameter to get a printout of the available options for the script:

$ python project_generator.py -h


Example usage
-------------

1: Generate specific project(s):

$ python project_generator.py xmega.drivers.adc.example1.xplain avr32.drivers.adc.example.evk1104


2: Set a configuration value -- this will override configuration values set in the ASF XML:

$ python project_generator.py -s config.compiler.optimization=high xmega.drivers.adc.example1.xplain avr32.drivers.adc.example.evk1104

-- Note that generic compiler config will be overridden by toolchain-specific config if it is set ASF XML, e.g.,
   config.compiler.optimization will be overridden by config.compiler.avrgcc.optimization for the AVRGCC toolchain.


3: Archive project(s) to .ZIP with Doxygen output:

$ python project_generator.py -a zips -z -x xmega.drivers.adc.example1.xplain avr32.drivers.adc.example.evk1104

-- Note that the directory (specified with -a) is relative to the location of project_generator.py and must be created in advance.


4: Generate all unit test projects for 8- and 32-bit AVR GCC for a specific driver:

$ python project_generator.py -g avrgcc,avr32gcc --project-type=unit-test common.services.fifo.*


ASF Console
===========

About
-----

The ASF console is an application for querying the ASF database and is intended to aid developers in checking that the information, dependencies and device support in the database are correct.


1: Start the console:

$ python console.py

-- A list of available commands will show as soon as the script is done creating the database.


2: Print dependency tree for a project, resolved for a specific device:

$ python console.py l common.services.basic.fifo.unit_tests.xplain atxmega128a1

-- To save the dependency tree to file add: > output.txt


3: Look up all modules that require one module:

$ python console.py r xmega.drivers.adc
