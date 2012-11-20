import os
import os.path
import sys
import time

from optparse import OptionParser
from asf.extensionmanager import *
from asf.findrebuild import *


if __name__ == "__main__":
	class ReturnCode:
		SUCCESS       = 0
		ERROR         = 1
		# Invalid user input
		ERROR_INVALID = 2

	start_time = time.clock()

	script_path = sys.path[0]

	parser = OptionParser(usage=
		"""<options> input_file_list
		Finds projects and modules (services, drivers, components, libraries) that needs to be recompiled based on the files given by the input_file_list argument.

		The output is stored in two separate files, one containing the affected projects ids, one containing affected module ids. One id for each line in the output.
		If all projects need to be rebuild the result is an '*'.

		Example usage:
		python find_rebuild_list.py file_with_list_of_changed_files.txt
		""")
	# General otions
	# parser.add_option("","--ext-root", dest="ext_root", default=os.path.join(script_path, '..', '..'), help="FDK extension root directory")
	parser.add_option("-b","--basedir", dest="ext_root", default=os.path.join(script_path, '..', '..'), help="FDK extension root directory")
	parser.add_option("","--main-ext-uuid", dest="main_ext_uuid", default="Atmel.ASF", help="Main FDK extension UUID")
	parser.add_option("","--main-ext-version", dest="main_ext_version", help="Main FDK extension version")

	parser.add_option("-c","--cached", action="store_true", dest="cached", default=False, help="Do not rescan all directories, but use the list of XML files from last run")

	parser.add_option("", "--check-device-support", dest="device_support", action="store_true", default=False, help="Check device support and eliminate projects that do not have device support. Currently not supported for modules.")
	parser.add_option("", "--show-not-affected", dest="not_affected", action="store_true", default=False, help="Show all projects not affected by the change to debug log (useful for debugging)")
	default_outfile_project = "find_rebuild_list_result.txt"
	default_outfile_module = "find_rebuild_list_modules.txt"
	parser.add_option("-o", "--outfile", dest="outfile_projects", action="store", default=default_outfile_project, help="Filename to store projects. Default: " + default_outfile_project)
	parser.add_option("-m", "--outfile-modules", dest="outfile_modules", action="store", default=default_outfile_module, help="Filename to store modules. Default: " + default_outfile_module)

	# Debug options
	parser.add_option("-d","--debug", action="store_true", dest="debug", default=False, help="Enable debugging. Sets 'level' to debug if not otherwise specified.")
	parser.add_option("-l","--level", dest="level", default=False, help="Log output level, from most detailed to least detailed: debug, info, warning or error")
	parser.add_option("-t","--timestamp", action="store_true", dest="timestamp", default=False, help="Timestamp log")
	parser.add_option("-v","--verbose", action="store_const", dest="level", const="info", help="Verbose output")

	(options, args) = parser.parse_args()

	if len(args) < 1:
		parser.error("Incorrect number of arguments or usage")

	# Find paths for input files in same folder as this script
	templatedir = os.path.join(script_path, "templates")
	xml_schema_dir = os.path.join(script_path, "schemas")
	device_map = os.path.join(script_path, "device_maps", "atmel.xml")

	# Find absolute paths
	outfile_project_path = os.path.abspath(options.outfile_projects)
	outfile_module_path = os.path.abspath(options.outfile_modules)
	current_folder = os.getcwd()

	# Change directory to the ASF root folder
	runtime = FindRebuildRuntime()

	runtime.set_debug(options.debug)
	runtime.set_commandline_args(args)
	runtime.set_check_device_support(options.device_support)
	runtime.set_show_not_affected(options.not_affected)
	runtime.set_outfiles(outfile_project_path, outfile_module_path)
	runtime.set_start_folder(current_folder)
	runtime.set_input_file(args[0])

	# If debugging is enabled and not the log level, we set it to debug
	if options.debug and not options.level:
		options.level = "debug"

	if options.level:
		level = runtime.get_log_level_from_text(options.level)
		if not level:
			print "ERROR: Invalid debug level: " + options.level
			parser.print_help()
			sys.exit(ReturnCode.ERROR_INVALID)

		runtime.setup_log(level, options.timestamp)
	else:
		runtime.setup_log()


	runtime.log.debug("Starting")

	# Assume script success for now
	return_code = ReturnCode.SUCCESS

	try:
		# Create the extension manager
		ext_manager = FdkExtensionManager(runtime, options.ext_root, use_cache=options.cached)

		# Load prerequisites like XML schemas and device map
		ext_manager.load_schemas(xml_schema_dir)
		ext_manager.load_device_map(device_map)

		# Now scan for extensions and load them
		ext_paths = ext_manager.scan_for_extensions()
		ext_manager.load_and_register_extensions(ext_paths)

		# Get the database of the main extension
		main_ext = ext_manager.request_extension(options.main_ext_uuid, options.main_ext_version)
		main_db = main_ext.get_database()

		# Set the database for the runtime class
		runtime.set_db(main_db)

		# Load framework version
		version = main_db.get_framework_version()
		runtime.log.info("Finished loading ASF version: " + version)

		runtime.run()
	except Exception as e:
		runtime.log.critical("Project generation failed: "+str(e))
		return_code = ReturnCode.ERROR

		# If debugging we raise the exception to get the backtrace
		if options.debug:
			raise

	runtime.log.info("Execution finished in %.2f seconds"  %(time.clock() - start_time))

	sys.exit(return_code)
