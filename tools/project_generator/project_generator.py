import os
import os.path
import re
import sys
import time

from optparse import OptionParser

import asf.helper

from asf.archiver import Archiver
from asf.database import *
from asf.configuration import ConfigurationHandler
from asf.junit_report import *
from asf.runtime import Runtime


class ProjectGeneratorRuntime(Runtime):
	junit_suite_package = 'Atmel Software Framework'
	junit_suite_name    = 'Project generation'
	junit_class_name    = 'ASF Project Generation'

	def __init__(self, template_dir, configuration):
		super(ProjectGeneratorRuntime, self).__init__(template_dir, configuration)
		self.junit_report = None
		self.project_type_to_find = None
		self.project_list_file = None
		self.do_dry_run = False

	def set_project_list_file(self, list_file):
		"""
		Set the file object to write project IDs to.
		"""
		self.project_list_file = list_file

	def set_do_dry_run(self, do_dry_run):
		"""
		Set whether or not the generators should actually be run.
		"""
		self.do_dry_run = do_dry_run

	def set_junit_report(self, report):
		"""
		Set the JUnit report in which the project generator shall insert
		its suites and results.
		"""
		self.junit_report = report

	def create_new_junit_suite(self):
		"""
		Make and set a new JUnit suite if a report has been set, then
		return it. If a report has not been set, a dummy suite is
		created.
		"""
		if self.junit_report:
			suite = self.junit_report.get_new_suite(self.junit_suite_name, self.junit_suite_package)
		else:
			suite = JUnitNullSuite()

		self.junit_suite = suite
		return suite

	def set_project_type_to_find(self, type_to_find):
		"""
		Set the type of project to find.
		Valid types are: 'normal', 'template', 'unit-test', None

		If set to any other value than None, the generator will _only_
		generate projects of the specified type. This means that if any
		IDs are specified as arguments, they will be filtered for
		projects of this type.

		If set to None, the project generator will not differentiate
		between project types.
		"""
		if type_to_find and type_to_find not in Project.valid_types:
			raise Exception("Unknown project type `%s' was specified" % type_to_find)

		self.project_type_to_find = type_to_find

	def verify_project(self, generator):
		"""
		Function for running sanity checks before running generator
		"""
		# Make sure we have 1 and only 1 application in our project
		modules = generator._get_application_modules()
		application_modules = [m for m in modules if m.type == 'application']
		nr_of_app_modules = len(application_modules)
		if nr_of_app_modules > 1:
			self.log.warning("Expected 1 application module in project %s, found %d." % (generator.project.id, nr_of_app_modules ))
			for appm in application_modules:
				self.log.warning("\t%s" %appm.id)

		# Make sure we have 1 and only 1 board defined in our project
		modules = generator._get_board_modules()
		board_modules = [m for m in modules if m.type == 'board']
		nr_of_board_modules = len(board_modules)
		if nr_of_board_modules != 1:
			self.log.warning("Expected 1 board module in project %s, found %d." % (generator.project.id, nr_of_board_modules ))
			for board in board_modules:
				self.log.warning("\t%s" %board.id)

	def load_project(self, project_id):
		"""
		Load a project by the given project id

		Return:
		Project if successful.
		None if project cannot be found or there is an error with the project.
		"""
		try:
			project = self.db.lookup_by_id(project_id)
			return project
		except Exception as e:
			self.log.error("Unable to load module with id: "+project_id+" "+str(e))
			return None

	def _run_generator(self, generator):
		if not self.do_dry_run:
			generator.write()

		proj_id = generator.project.id
		if proj_id not in self.generated_project_ids:
			self.generated_project_ids.append(proj_id)

			try:
				self.project_list_file.write(proj_id + '\n')
			except:
				pass

	def _report_result(self, projects_total, projects_loaded, try_to_archive, projects_archived, generator_count, generator_num, generator_ok):
		print "Loaded %d of %d projects/modules successfully" % (projects_loaded, projects_total)
		print "Successfully ran %d of %d generators in %d projects/modules:" % (generator_ok, generator_num, projects_loaded)
		# Print the number of generators that have been run
		for name, count in generator_count.items():
			print "%s%s : %4d" % (' '*5, name.ljust(16), count)
		if self.archiver:
			print "Archived %d of %d projects successfully" % (projects_archived, try_to_archive)

	def get_interval(self):
		"""
		Get the number of elapsed seconds since last time this function
		was last called. For the first call, 0 is returned.
		"""
		current_time = time.time()

		try:
			split_time = self.split_time
		except AttributeError:
			elapsed_time = 0
		else:
			elapsed_time = current_time - split_time

		self.split_time = current_time

		return  elapsed_time

	def report_success_case(self, name):
		"""
		Add a success case to the current JUnit suite, with the
		default class and specified test name. The time (duration) of
		the case is determined with get_interval(), which should be
		called at the start of each case to reset the timing.
		"""
		self.junit_suite.add_success_case(self.junit_class_name, name, self.get_interval())

	def report_failure_case(self, name, message):
		"""
		Adds a failure case to the current JUnit suite, with the
		default class, specified name and failure message. The time
		(duration) of the case is determined with get_interval(), which
		should be called at the start of each case to reset the timing.
		"""
		self.junit_suite.add_failure_case(self.junit_class_name, name, self.get_interval(), message)

	def generate_project(self, project):
		"""
		Generate project for all project generators for the specified project.

		Return A dictionary with the generator name as key and the number of times it has been run as value.
		       A value higher than 1 indicates a mistake in the XML file.
		Return False if generation fails
		"""

		is_verified = False
		num_gens = 0
		ok_gens = 0
		generator_count = dict()
		available_generators = project.get_generator_list()
		if self.generators == ["all"]:
			project_generators = available_generators
		else:
			project_generators = self.generators

		case_log = ''

		for generator_name in project_generators:
			if generator_name not in available_generators:
				self.log.info("Info: Skipping generation of `%s' for project `%s`'" % (generator_name, project.id))
				continue
			the_class = self.generator_tag_to_class(generator_name)
			if the_class is None:
				self.log.warning("Warning: Unknown generator `%s' for project `%s`'" % (generator_name, project.id))
				continue

			num_gens += 1

			try:
				generator = the_class(project, self.db, self)

				if not is_verified and isinstance(project, Project):
					# Let's do some project sanity checking first
					self.verify_project(generator)
					is_verified = True

				project.visualize_requirements(self.log.debug)

				self._run_generator(generator)
			except Exception as e:
				case_log += '--GENERATOR : %s--\n%s\n\n' % (generator_name, str(e))
				try:
					raise
				except GeneratorError as e:
					self.log.error("Generator. Message: %s [%s]" % (str(e), project.id))
					continue
				except (NoSelectionError, NotFoundError) as e:
					self.log.error("Dependency. Message: %s [%s]" % (str(e), project.id))
					# Dependency issues won't go away for other generators, so we
					# break to avoid multiple error messages for the same issue.
					break
				except ConfigError as e:
					self.log.error("Config. Message: %s [%s]" % (str(e), project.id))
					continue
				except DbError as e:
					self.log.error("Db. Message: %s [%s]" % (str(e), project.id))
					continue
				except IOError as e:
					self.log.error("I/O. Message: %s [%s]" % (str(e), project.id))
					continue

			ok_gens += 1
			generator_count[generator_name] = generator_count.get(generator_name, 0) + 1

		return generator_count, num_gens, ok_gens, case_log

	def _run_init(self):
		self.generated_project_ids = []

	def run(self):
		self._run_init()
		projects_total = 0
		projects_loaded = 0
		projects_archived = 0
		generator_count = dict()
		generator_num = 0
		generator_ok = 0
		try_to_archive = 0
		proj_type = {Project.type_attrib : self.project_type_to_find}

		# Create a new suite for the JUnit report.
		self.create_new_junit_suite()

		if len(self.args) > 0:
			project_id_list = self.args

			# Check if some wildcards needs to be expanded
			expanded_project_list = []
			for project_wildcard in project_id_list:
				if '*' in project_wildcard:
					self.log.debug("Expanding wildcard from commandline: " + project_wildcard)

					# Look up projects of a specified type, or all projects/modules?
					if self.project_type_to_find:
						ids = self.db.wildcard_find(project_wildcard, Project, proj_type)
					else:
						ids = self.db.wildcard_find(project_wildcard, Project) + self.db.wildcard_find(project_wildcard, Module)

					expanded_project_list.extend(ids)
				else:
					# Check if specific ID actually exists
					try:
						db_object = self.db.lookup_by_id(project_wildcard)
					except:
						self.log.error("ID %s was not found in database" % project_wildcard)
					else:
						# Check that it is a project of a specified type?
						if self.project_type_to_find:
							if db_object.tag == 'project' and db_object.type == self.project_type_to_find:
									expanded_project_list.append(project_wildcard)
							else:
								self.log.error("ID %s is not a project of type `%s'" % (project_wildcard, self.project_type_to_find))
						else:
							expanded_project_list.append(project_wildcard)

			project_id_list = expanded_project_list
			self.log.debug("Generate projects from commandline: " + ' '.join(project_id_list))
		else:
			if not self.project_type_to_find:
				project_list = self.db.find_components(Project) + self.db.find_components(Module)
			else:
				project_list = self.db.find_components(Project, proj_type)

			project_id_list = [elem.attrib["id"] for elem in project_list]
			self.log.debug("Iterate all projects:")

		for project_id in project_id_list:
			# Reset the interval timing
			self.get_interval()

			projects_total += 1
			self.log.info("MODULE: " + project_id)
			project = self.load_project(project_id)

			if project is not None:
				projects_loaded += 1

				result, num, ok, case_log = self.generate_project(project)
				generator_num += num
				generator_ok += ok
				if result:

					for generator_name in result:
						generator_count[generator_name] = generator_count.get(generator_name, 0) + 1

					if self.archiver and isinstance(project, Project):
						try_to_archive += 1
						try:
							if self.archiver.create(project, self.db):
								projects_archived += 1
						except IOError as e:
							self.log.error("I/O. Message: %s [%s]" % (str(e), project.id))
							case_log += '--ARCHIVER--\n%s' % str(e)

				if case_log:
					self.report_failure_case(project_id, case_log)
				else:
					self.report_success_case(project_id)

		if self.archiver:
			self.archiver.xml_finalize()

		self._report_result(projects_total, projects_loaded, try_to_archive, projects_archived, generator_count, generator_num, generator_ok)


class ProjectGeneratorCleanupRuntime(ProjectGeneratorRuntime):

	def _run_init(self):
		# Disable archiver for now, since this is not supported.
		self.archiver = None

	def _report_result(self, projects_total, projects_loaded, try_to_archive, projects_archived, generator_count, generator_num, generator_ok):
		print "Cleaned %d projects" % (projects_loaded)

	def _run_generator(self, generator):
		for file_to_clean in generator.get_generated_files_list():
			self.log.info("Remove "+file_to_clean)
			if os.path.exists(file_to_clean):
				try:
					os.remove(file_to_clean)
				except Exception as e:
					self.log.info("Unable to remove file "+file_to_clean)

		for dir in generator.get_generated_dirs_list():
			self.log.info("Remove dir"+dir)
			if os.path.exists(dir):
				try:
					os.rmdir(dir)
				except Exception as e:
					self.log.info("Unable to remove folder "+dir)


if __name__ == "__main__":
	class ReturnCode:
		SUCCESS       = 0
		ERROR         = 1
		# Invalid user input
		ERROR_INVALID = 2

	class Mode:
		ProjectGenerator = 0
		ProjectGeneratorCleanup = 3

	start_time = time.clock()
	parser = OptionParser(usage=
		"""<options> <args>.
		The arguments specifies the ids of the projects to generate.
		The wildcard '*' can be used in the id to match several ids.
		Ex: 'avr32* common*' will match all avr32 and common projects.
		If no ids are given all projects are generated""")
	# General otions
	parser.add_option("-b", "--basedir", dest="basedir", help="Set root directory of ASF installation")
	parser.add_option("-s", "--set-config", action="append", dest="config", help="Set a config value: --set-config <name>=<value>")
	parser.add_option("-c","--cached", action="store_true", dest="cached", default=False, help="Do not rescan all directories, but use the list of XML files from last run")

	parser.add_option("-a","--archive-dir", dest="archive_dir", help="Create archive of specified project(s) in given directory")
	parser.add_option("-z","--archive-zip", action="store_true", dest="archive_zip", default=False, help="Create zip archives")
	parser.add_option("-7","--archive-7zip", action="store_true", dest="archive_7zip", default=False, help="Create 7-zip archives")
	parser.add_option("-x","--archive-doxygen", action="store_true", dest="archive_doxygen", default=False, help="Generate doxygen documentation in archives")
	parser.add_option("-1","--archive-flat", action="store_true", dest="archive_flat", default=False, help="Archive all projects in the same output folder. (Warning: does not clean folders before starting) Does not work with -7 or -z option.")
	parser.add_option("-p","--version-postfix", dest="version_postfix", help="Set postfix for version number for export (typically SVN revision). If not specified, the generator will try to find the SVN revision")
	parser.add_option("-g","--generator", dest="generators", default="all", help="Select generator(s) to run on given projects. If more than one, separate them by comma. Use 'all' to use all available generators. Example of possible generators: iarewarm,iarew32,iarew,armgcc,avr32gcc,avrgcc,as5_arm,as5_32,as5_8,as5_32_template,as5_8_template,doxygen,doxygen-module")
	parser.add_option("","--document-version", dest="document_version", default=None, help="Set version part of documentation URL in project files. If not specified, current version number will be used")
	parser.add_option("","--no-as5-zip", action="store_true", dest="no_zip", default=False, help="Do not zip Atmel Studio project source files")
	parser.add_option("-f","--input-file", dest="input_file", default=None, help="File which contains ID(s) of project(s) to generate. One ID per line. Overrides ID(s) given as argument")

	# Mode options
	parser.add_option("","--clean", action="store_const", dest="mode", const=Mode.ProjectGeneratorCleanup, help="Clean/remove all generated files from the project generator. It does not clean generated archives.")
	parser.add_option("", "--project-type", dest="project_type", default=None, help="Only generate specified type of projects, filtering any ID which is specified as argument. Available types: " + str(Project.valid_types))
	parser.add_option("", "--list-ids", dest="project_list_file", default=None, help="Do a dry run and output to file a plaintext list of IDs that would be generated: --list-ids <listfile>")

	# Debug options
	parser.add_option("-d","--debug", action="store_true", dest="debug", default=False, help="Enable debugging. Sets 'level' to debug if not otherwise specified.")
	parser.add_option("-j", "--junit", action="store_true", dest="make_junit_report", default=False, help="Make a JUnit report with generator results.")
	parser.add_option("", "--junit-file", dest="junit_report_file", default=None, help="Specify the filename (with path) of the JUnit report. Default: ./testReport.xml")
	parser.add_option("-l","--level", dest="level", default=False, help="Log output level, from most detailed to least detailed: debug, info, warning or error")
	parser.add_option("-t","--timestamp", action="store_true", dest="timestamp", default=False, help="Timestamp log")
	parser.add_option("-v","--verbose", action="store_const", dest="level", const="info", help="Verbose output")

	(options, args) = parser.parse_args()

	script_path = sys.path[0]

	basedir = options.basedir
	if basedir is None:
		basedir = "../../"

	# (Try to) load input file
	if options.input_file:
		try:
			f = open(options.input_file, "r")
			lines = f.readlines()
			f.close()
		except IOError:
			print "ERROR: Unable to read content from file: " + options.input_file
			parser.print_help()
			sys.exit(ReturnCode.ERROR_INVALID)

		args = []
		for line in lines:
			line = line.strip()
			if len(line) > 0:
				args.append(line)

	# Remove arguments if one of the args is '*'
	for arg in args:
		if arg ==  '*':
			args = []
			break

	# Find paths for input files in same folder as this script
	templatedir = os.path.join(script_path, "templates")
	guidir = os.path.join(script_path, "plugins")
	cachedir = os.path.join(script_path, "cache")
	xml_schema_path = os.path.join(script_path, "asf.xsd")

	# Expand any relative archive path from the current dir
	if options.archive_dir:
		options.archive_dir = os.path.abspath(options.archive_dir)

	# Create a JUnit report and get its location
	junit_report = JUnitReport()

	if options.junit_report_file:
		junit_filename = os.path.abspath(options.junit_report_file)
	else:
		junit_filename = os.path.join(script_path, 'testReport.xml')

	try:
		proj_list_file = open(options.project_list_file, 'w')
	except IOError:
		print "ERROR: Unable to open file: " + options.project_list_file
		parser.print_help()
		sys.exit(ReturnCode.ERROR_INVALID)
	except TypeError:
		# User did not enable the option, do nothing
		proj_list_file = None

	# Change directory to the ASF root folder
	os.chdir(basedir)

	# Read command line configuration
	try:
		configuration = ConfigurationHandler.generate_from_option_parser(options)
	except Exception as e:
		print "Error with configuration", e
		parser.print_help()
		sys.exit(ReturnCode.ERROR_INVALID)

	if options.mode == Mode.ProjectGeneratorCleanup:
		runtime = ProjectGeneratorCleanupRuntime(templatedir, configuration)
	else:
		runtime = ProjectGeneratorRuntime(templatedir, configuration)

	runtime.create_and_set_cache_dir(cachedir)

	runtime.set_debug(options.debug)
	runtime.set_junit_report(junit_report)
	runtime.set_use_file_list_cache(options.cached)
	runtime.set_xml_schema_path(xml_schema_path)
	runtime.set_commandline_args(args)
	runtime.set_generator_string(options.generators)
	runtime.set_document_version(options.document_version)
	runtime.set_project_type_to_find(options.project_type)
	runtime.set_no_zip(options.no_zip)

	if proj_list_file:
		runtime.set_do_dry_run(True)
		runtime.set_project_list_file(proj_list_file)

	version_postfix = options.version_postfix
	if not version_postfix:
		version_postfix = asf.helper.find_svn_revision() + "dev"
	runtime.set_version_postfix(version_postfix)

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

	if options.archive_dir:
		runtime.set_archiver(Archiver(runtime,
			options.archive_dir,
			options.archive_zip,
			options.archive_7zip,
			options.archive_doxygen,
			options.archive_flat))

	runtime.log.debug("Starting")

	# Assume script success for now
	return_code = ReturnCode.SUCCESS

	try:
		# Read XML files
		runtime.load_db()

		# Sanity check of database
		version = runtime.db.get_framework_version()
		runtime.log.info("Finished loading ASF version: " + version)

		if options.cached:
			runtime.log.info("Skipping DB sanity check in cached mode")
		else:
			runtime.log.info("Running DB sanity check")
			runtime.database_sanity_check()

		runtime.run()
	except Exception as e:
		runtime.log.critical("Project generation failed: "+str(e))
		return_code = ReturnCode.ERROR

		# Add the exception to the last suite that was added to the JUnit report
		suite = junit_report.get_last_suite() or JUnitNullSuite()
		suite.add_system_error(str(e))

		# If debugging we raise the exception to get the backtrace
		if options.debug:
			raise
	else:
		# If no unhandled exceptions occurred, check the results and set
		# the return code accordingly.
		metrics = junit_report.get_total_metrics()
		if metrics['failures'] or metrics['errors']:
			return_code = ReturnCode.ERROR
	finally:
		# Write the JUnit report to file, if this option was selected
		if options.make_junit_report:
			runtime.log.info("Writing JUnit report to " + junit_filename)
			junit_report.write(junit_filename)

	# Store debug information (do this last, since it modifies the XML tree)
	if options.debug:
		outputfile = os.path.join(cachedir, "db.xml")
		runtime.log.info("Writing database to " + outputfile)
		runtime.write_xml_tree(outputfile)

	# Temporary code for cleaning up everyones repository, before changing
	# location of stored files
	try:
		os.remove("db.xml")
	except OSError:
		pass
	try:
		os.remove("asf_filelist.cache")
	except OSError:
		pass

	runtime.log.info("Execution finished in %.2f seconds"  %(time.clock() - start_time))

	sys.exit(return_code)
