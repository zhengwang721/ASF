import os
import os.path
import sys
import time

from optparse import OptionParser

from asf.findrebuild import *


if __name__ == "__main__":
	class ReturnCode:
		SUCCESS       = 0
		ERROR         = 1
		# Invalid user input
		ERROR_INVALID = 2

	start_time = time.clock()
	parser = OptionParser(usage=
		"""<options> input_file_list
		Finds projects and modules (services, drivers, components, libraries) that needs to be recompiled based on the files given by the input_file_list argument.

		The output is stored in two separate files, one containing the affected projects ids, one containing affected module ids. One id for each line in the output.
		If all projects need to be rebuild the result is an '*'.

		Example usage:
		python find_rebuild_list.py file_with_list_of_changed_files.txt
		""")
	# General otions
	parser.add_option("-b", "--basedir", dest="basedir", help="Set root directory of ASF installation")
	parser.add_option("", "--check-device-support", dest="device_support", action="store_true", default=False, help="Check device support and eliminate projects that do not have device support")
	parser.add_option("", "--show-not-affected", dest="not_affected", action="store_true", default=False, help="Show all projects not affected by the change to debug log (useful for debugging)")
	default_outfile_project = "find_rebuild_list_result.txt"
	default_outfile_module = "find_rebuild_list_modules.txt"
	parser.add_option("-o", "--outfile", dest="outfile_projects", action="store", default=default_outfile_project, help="Filename to store projects. Default: " + default_outfile_project)
	parser.add_option("-m", "--outfile_modules", dest="outfile_modules", action="store", default=default_outfile_module, help="Filename to store modules. Default: " + default_outfile_module)
	parser.add_option("", "--load-cache", dest="cache_file", action="store", default=None, help="Use this given database cache instead of parsing asf.xml")

	# Debug options
	parser.add_option("-d","--debug", action="store_true", dest="debug", default=False, help="Enable debugging. Sets 'level' to debug if not otherwise specified.")
	parser.add_option("-l","--level", dest="level", default=False, help="Log output level, from most detailed to least detailed: debug, info, warning or error")
	parser.add_option("-t","--timestamp", action="store_true", dest="timestamp", default=False, help="Timestamp log")
	parser.add_option("-v","--verbose", action="store_const", dest="level", const="info", help="Verbose output")

	(options, args) = parser.parse_args()

	if len(args) < 1:
		parser.error("Incorrect number of arguments or usage")

	script_path = sys.path[0]

	basedir = options.basedir
	if basedir is None:
		if options.cache_file:
			basedir = "."
		else:
			basedir = "../../"

	# Find paths for input files in same folder as this script
	guidir = os.path.join(script_path, "plugins")
	cachedir = os.path.join(script_path, "cache")
	xml_schema_path = os.path.join(script_path, "asf.xsd")

	# Find absolute paths
	outfile_project_path = os.path.abspath(options.outfile_projects)
	outfile_module_path = os.path.abspath(options.outfile_modules)
	current_folder = os.getcwd()

	# Change directory to the ASF root folder
	os.chdir(basedir)

	runtime = FindRebuildRuntime()

	runtime.create_and_set_cache_dir(cachedir)

	runtime.set_debug(options.debug)
	runtime.set_xml_schema_path(xml_schema_path)
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
		# Read XML files
		runtime.load_db(filename=options.cache_file)

		# Load framework version
		version = runtime.db.get_framework_version()
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
