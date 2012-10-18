from __future__ import division

import copy
import os
import os.path
import re
import subprocess
import sys
import time

from optparse import OptionParser

from asf.configuration import ConfigurationHandler
from asf.database import *
from asf.exception import *
from asf.extensionmanager import *
from asf.junit_report import *
from asf.runtime import Runtime
from asf.toolchain import *


class ModuleListerRuntime(Runtime):
	default_module_ids     = ['*']
	# Do not list meta or application
	module_types_to_list   = [
			'component',
			'driver',
			'library',
			'service',
		]
	module_list_file = None

	def __init__(self, template_dir, configuration, module_ids):
		super(ModuleListerRuntime, self).__init__(template_dir, configuration)

		if not module_ids:
			module_ids = self.default_module_ids
		self.module_ids = module_ids

		self.list_hidden_modules = False
		self.module_list_file = None

	def set_module_list_file(self, list_file):
		"""
		Set the file object to write module IDs to.
		"""
		self.module_list_file = list_file

	def set_test_hidden_modules(self, test_hidden):
		"""
		Set whether or not modules with the 'hidden' GUI flag should
		be listed: True to list, False to skip.
		"""
		self.list_hidden_modules = test_hidden

	def _resolve_ids(self, id_list, type):
		"""
		Resolve/check the supplied IDs, resolving any wildcard ID,
		removing any ID that does not exist and any duplicates.
		"""
		user_wildcard_ids = [id for id in id_list if '*' in id]
		user_specific_ids = [id for id in id_list if '*' not in id]

		specific_ids = []

		# Check that the specific IDs exist
		for id in user_specific_ids:
			try:
				db_obj = self.db.lookup_by_id(id)
			except KeyError:
				self.log.error("ID `%s' was not found in database" % id)
			else:
				if db_obj.tag != type.tag:
					self.log.error("ID `%s' is not a %s but a %s" % (id, type.tag, db_obj.tag))
				else:
					specific_ids.append(id)

		# Look for the wildcard IDs
		for id in user_wildcard_ids:
			new_ids = self.db.wildcard_find(id, type)
			specific_ids.extend(new_ids)
			if not new_ids:
				self.log.error("Wildcard ID `%s' does not match any %s in database" % (id, type.tag))

		# Remove duplicates
		specific_ids = list(set(specific_ids))
		specific_ids.sort()

		return specific_ids

	def run(self):
		"""
		List all module IDs
		"""
		def iterate_specified_modules(module_id_list):
			for module_id in module_id_list:
					yield self.db.lookup_by_id(module_id)

		# Resolve module ids, as they may be incorrect and/or contain wildcards
		list_module_ids = self._resolve_ids(self.module_ids, Module)
		self.log.debug('Resolved module IDs:')
		for module_id in list_module_ids:
			self.log.debug('\t' + module_id)

		module_id_set = set()

		for module in iterate_specified_modules(list_module_ids):
			if module.type not in self.module_types_to_list:
				continue

			if 'hidden' in module.get_gui_flag_list() and not self.list_hidden_modules:
				self.log.debug("Skipping hidden module `%s'." % module.id)
				continue

			# Strip hashes -- we want top-level module-selector if there is one
			module_id_set.add(module.id.split("#")[0])

		module_id_list = list(module_id_set)
		module_id_list.sort()

		for module_id in module_id_list:
			self.module_list_file.write(module_id + '\n')

		nr_of_listed_modules = len(module_id_list)

		# Return failure if no modules were listed
		if nr_of_listed_modules == 0:
			self.log.error("Did not list any modules!")
			return 1
		else:
			self.log.info("Listed %s module and module-selector IDs to file" % nr_of_listed_modules)
			return 0


class ModuleTesterRuntime(Runtime):
	output_subdir          = 'regression_tests'
	test_application_id    = 'common.applications.user_application'
	tick_length            = 5 * 60
	default_board_ids      = ['board.user_board.*']
	default_module_ids     = ['*']
	unspecified_mcu_regexp = re.compile('^unspecified-')
	mcu_map_id             = generic.GenericElement.mcu_map_id
	junit_name             = 'ASF Module Build Tests'
	junit_package          = 'ASF Module Build Tests'
	junit_class            = 'ASF Module Build Test'
	module_types_to_test   = [
			'component',
			'driver',
			'library',
			'service',
		]


	def __init__(self, template_dir, configuration, board_ids, module_ids, defines):
		super(ModuleTesterRuntime, self).__init__(template_dir, configuration)

		if not board_ids:
			board_ids  = self.default_board_ids
		self.board_ids  = board_ids

		if not module_ids:
			module_ids = self.default_module_ids
		self.module_ids = module_ids

		self.defines = defines

		self.mcu_blacklist = []
		self.output_dir = None
		self.test_hidden_modules = False
		self.junit_report = None
		self.junit_suite = None
		self.start_time = 0
		self.current_tick = 0


	def set_mcu_blacklist(self, mcu_list):
		self.mcu_blacklist = mcu_list


	def set_output_directory(self, directory):
		self.output_dir = directory


	def set_test_hidden_modules(self, test_hidden):
		self.test_hidden_modules = test_hidden


	def set_junit_report(self, report):
		self.junit_report = report


	def _give_life_sign(self):
		"""
		Prints a string with the current time every time a "tick" has
		passed and this function is called. The length of a tick is
		specified in seconds.

		The purpose of this function is to occasionally give some
		indication that the script is still running, without flooding
		the output with irrelevant data.
		"""
		current_tick = int((time.time() - self.start_time) / self.tick_length)

		if current_tick > self.current_tick:
			self.current_tick = current_tick

			now = time.localtime()
			print '--MARK @ %02d:%02d:%02d--' % (now.tm_hour, now.tm_min, now.tm_sec)


	def _resolve_ids(self, id_list, type):
		"""
		Resolve/check the supplied IDs, resolving any wildcard ID,
		removing any ID that does not exist and any duplicates.
		"""
		user_wildcard_ids = [id for id in id_list if '*' in id]
		user_specific_ids = [id for id in id_list if '*' not in id]

		specific_ids = []

		# Check that the specific IDs exist
		for id in user_specific_ids:
			try:
				db_obj = self.db.lookup_by_id(id)
			except NotFoundError:
				self.log.error("ID `%s' was not found in database" % id)
			else:
				if db_obj.tag != type.tag:
					self.log.error("ID `%s' is not a %s but a %s" % (id, type.tag, db_obj.tag))
				else:
					specific_ids.append(id)

		# Look for the wildcard IDs
		for id in user_wildcard_ids:
			new_ids = self.db.wildcard_find(id, type)
			specific_ids.extend(new_ids)
			if not new_ids:
				self.log.error("Wildcard ID `%s' does not match any %s in database" % (id, type.tag))

		# Remove duplicates
		specific_ids = list(set(specific_ids))
		specific_ids.sort()

		return specific_ids


	def _get_project_output_directory(self, board, mcu, module):
		"""
		Return the output directory to use for projects.

		If an output directory has been set, the path becomes:
		<output dir>/<module id>/<board id>/<mcu name>

		If no output directory has been set, the path becomes:
		<module dir>/<test subdir>/<board id>/<mcu name>
		"""
		if self.output_dir:
			root_dir = self.output_dir
			subdir = module.id
		else:
			root_dir = os.path.dirname(module.fromfile)
			subdir = self.output_subdir

		base_dir = os.path.join(root_dir, subdir)

		output_dir = os.path.join(base_dir, board.id, mcu.name)
		output_dir = output_dir.replace('#', '_')

		return output_dir


	def _get_supported_project_mcus(self, project):
		"""
		Return a list with all MCU objects that the specified project
		supports. The list will contain multiple MCUs if the project
		has an unspecified MCU, e.g., 'unspecified-xmegaa' or its
		device-support specifies groups.
		"""
		def mcu_name_is_specific(mcu_name):
			if self.unspecified_mcu_regexp.match(mcu_name):
				return False
			else:
				return True

		def get_mcu_group(mcu_name):
			mcu_group = self.unspecified_mcu_regexp.sub('', mcu_name)
			return mcu_group

		# List the name of specific MCUs from the project's device support list
		supported_mcu_names = []
		for dev_supp in project.get_device_support():
			mcu_names = self.device_map.get_mcu_list(dev_supp, check_group_is_mcu=True)
			specific_mcu_names = [mcu_name for mcu_name in mcu_names if mcu_name_is_specific(mcu_name)]
			supported_mcu_names.extend(specific_mcu_names)

		# Instantiate MCU objects from the names
		supported_mcus = [MCU(mcu_name, self.db) for mcu_name in supported_mcu_names]

		return supported_mcus


	def _get_modules_for_mcu(self, mcu, module_id_list):
		"""
		Return a list with all module objects, based on the supplied
		list of modules to test, which have support for the specified
		MCU.
		"""
		def iterate_modules(module_id_list):
			for module_id in module_id_list:
					yield self.db.lookup_by_id(module_id)


		mcu_modules       = []
		hidden_module_ids = []

		# Iterate through all modules in the database
		for module in iterate_modules(module_id_list):
			# Skip to next module if wrong module type
			if module.type not in self.module_types_to_test:
				continue

			# Skip hidden modules if this has been explicitly set
			if 'hidden' in module.get_gui_flag_list() and not self.test_hidden_modules:
				self.log.debug("Skipping hidden module `%s'." % module.id)
				hidden_module_ids.append(module.id)
				continue

			# Check if mcu is supported by the module
			for support_item in module.get_device_support():
				mcu_list = self.device_map.get_mcu_list(support_item, check_group_is_mcu=True)

				if mcu.name in mcu_list:
					mcu_modules.append(module)
					break

		if not mcu_modules:
			self.log.info("No modules with support for MCU `%s' were found" % mcu.name)

		# Remove the modules which were hidden, to avoid looking them up again
		for id_to_remove in hidden_module_ids:
			module_id_list.remove(id_to_remove)

		return mcu_modules


	def _get_generator_class(self, project):
		"""
		Return the generator class to use for a specified project:
		- AVRGCC for 8-bit AS5 template projects
		- AVR32GCC for 32-bit AS5 template projects
		- None for projects without any AS5 template generators
		"""
		# Get the generator class to use
		generator_value_to_class = {
				avrstudio5.AVRStudio5ProjectTemplate.generator_tag   : avrgcc.AVRGCCProject,
				avrstudio5.AVRStudio5Project32Template.generator_tag : avrgcc.AVR32GCCProject,
				avrstudio5.AVRStudio5ProjectARMTemplate.generator_tag : armgcc.ARMGCCProject,
			}

		generator_list = project.get_generator_list()

		for g_value, g_class in generator_value_to_class.items():
			if g_value in generator_list:
				return g_class

		return None


	def _get_template_projects_for_boards(self, board_id_list):
		"""
		Return a list of tuples with the template projects and their
		corresponding generator classes for the boards that are in the
		supplied list of IDs.

		The projects are meant to be used as templates, and must be
		deep copied.
		"""
		def add_define(project_e, define_set):
			build_e = ET.SubElement(project_e, 'build')
			build_e.set('type', 'define')
			build_e.set('name', define_set[0])
			build_e.set('value', define_set[1])

		project_and_generator_tuples = []

		# Find all projects for the test application
		template_project_ids = self.db.wildcard_find(self.test_application_id, Project)

		added_board_ids = set()

		# Look through the user application template projects for our boards
		for project_id in template_project_ids:
			project = self.db.lookup_by_id(project_id)

			# Add the project if it uses any of the specified boards
			prereq_ids = project.get_prerequisite_ids(recursive=False)

			add_project = False
			for prereq_id in prereq_ids:
				if prereq_id in board_id_list:
					added_board_ids.add(prereq_id)
					add_project = True
					break

			# Move on to next project if this one did not require any of the specified boards
			if not add_project:
				continue

			# Get the generator class to use
			project_generator = self._get_generator_class(project)
			if not project_generator:
				self.log.error("Could not determine generator for project template `%s'" % project.id)
				continue

			# Add the test defines to the project XML
			for define in self.defines:
				add_define(project.element, define)

			# Now add a tuple with the project and generator to the list
			project_and_generator_tuples.append((project, project_generator))

		# If template projects are missing for any of the specified boards, give a warning
		remaining_board_ids = set(board_id_list) - added_board_ids
		if remaining_board_ids:
			error_log = "Could not find project template for board(s):"
			for id in remaining_board_ids:
				error_log += "\n\t%s" % id

			self.log.error(error_log)
			if self.junit_suite:
				case_name = '%s' % (project_id)
				self.junit_suite.add_error_case(self.junit_class, case_name, 0, error_log)

		return project_and_generator_tuples


	def _make_test_projects(self, project_and_generator_tuples, module_id_list):
		"""
		Iterate through the supplied lists of template projects and
		module IDs, and write projects for all combinations of modules
		and supported MCUs.

		Return a tuple with the number of projects which were attempted
		written, and a list with the directories of written projects.
		"""
		attempted_projects = 0
		project_dirs       = []

		# For every template project..
		for project, generator in project_and_generator_tuples:
			# Get supported MCUs which are not blacklisted
			project_mcus = [mcu for mcu in self._get_supported_project_mcus(project) if not mcu.name in self.mcu_blacklist]

			# For every MCU supported by the project..
			for mcu in project_mcus:
				# Find all relevant modules supported by it.
				mcu_modules = self._get_modules_for_mcu(mcu, module_id_list)

				attempted_projects += len(mcu_modules)

				# And for every module..
				for module in mcu_modules:
					# Try to write a test project.
					new_project_dir = self._write_test_project(project, generator, mcu, module)

					if new_project_dir:
						project_dirs.append(new_project_dir)

				self._give_life_sign()

		return (attempted_projects, project_dirs)


	def _write_test_project(self, project_template, generator_class, mcu, module):
		"""
		Write the test project for the specified combination of board,
		MCU and module.
		"""
		def add_requirement(project_e, idref):
			require_e = ET.SubElement(project_e, 'require')
			require_e.attrib['idref'] = idref


		def add_header_file(project_e, file):
			build_e = ET.SubElement(project_e, 'build')
			build_e.set('type', 'header-file')
			build_e.set('value', file)


		def add_include_path(project_e, path):
			build_e = ET.SubElement(project_e, 'build')
			build_e.set('type', 'include-path')
			build_e.set('value', path)


		board = project_template._board

		# Since the template project can be used multiple times, we must work on a
		# copy so its element does not accumulate the changes
		new_project_e = copy.deepcopy(project_template.element)

		# Find the output directory for this project -- it will be in the module
		# under test's directory, unless the user specifies a custom directory
		output_directory = self._get_project_output_directory(board, mcu, module)

		# Trick the project generator into outputting to our directory and to use the
		# specified MCU
		new_project_e.set(ConfigDB.basedir_tag, os.path.relpath(output_directory, '.'))
		new_project_e.set(Project.mcu_attrib, mcu.name)

		# Instantiate the project
		project = Project(new_project_e, self.db)

		# Create configuration to use
		our_configuration = ConfigurationHandler()
		# Merge project configuration with command line configuration (prioritizing the latter)
		our_configuration.config = dict(project._project_board_configuration.config, **self.configuration.config)

		# Resolve dependencies
		project.resolve_all_selections(our_configuration, mcu)

		# Find the application module, add the module under test as a prerequisite
		for prereq in project.get_prerequisites(recursive=True):
			if isinstance(prereq, Module) and (prereq.type == 'application'):
				app_module = prereq
				break

		# Copy the current prereq. list because it MUST be restored
		# before this function returns to avoid accumulating prereqs
		old_prereqs = copy.copy(app_module.prerequisites)
		app_module.prerequisites.append(module)

		# Try to resolve selections again -- this may fail due to, e.g.,
		# missing device support deeper down in the "prerequisite tree"
		try:
			project.resolve_all_selections(our_configuration, mcu)
		except NoSelectionError as e:
			error_log = "Could not resolve selections for module `%s' with MCU `%s':\n%s" % (module.id, mcu.name, str(e))
			self.log.error(error_log)
			if self.junit_suite:
				case_name = '%s with %s' % (module.id, mcu.name)
				self.junit_suite.add_error_case(self.junit_class, case_name, 0, error_log)
			app_module.prerequisites = old_prereqs
			return []

		# Add the default config headers for the relevant modules in the project
		# -- it is safe to do this after the Project has been instantiated
		project_modules = project.get_prerequisites(recursive=True)
		for m in project_modules:
			# Ignore board definitions
			if m.fromfile == board.fromfile:
				continue

			module_config_headers = m.get_build(BuildModuleConfigRequiredHeaderFile, generator_class.toolchain, recursive=False)

			if module_config_headers:
				try:
					module_config_path = m.get_build(BuildModuleConfigPath, generator_class.toolchain, recursive=False).pop()
				except IndexError:
					error_log = "%s with ID `%s' does not provide path(s) to default config header files" % (m.tag, m.id)
					self.log.error(error_log)
					if self.junit_suite:
						case_name = '%s with %s on %s' % (module.id, mcu.name, board.id)
						self.junit_suite.add_error_case(self.junit_class, case_name, 0, error_log)
					app_module.prerequisites = old_prereqs
					return []
				else:
					for header in module_config_headers:
						header_path = os.path.join(module_config_path, header)
						add_header_file(project.element, header_path)
						add_include_path(project.element, module_config_path)

		# Now instantiate the project generator and start it
		generator = generator_class(project, self.db, self)
		try:
			generator.write()
		except Exception as e:
			error_log = "Could not write project for module `%s' on board `%s' with MCU `%s':\n%s" % (module.id, board.id, mcu.name, str(e))
			self.log.error(error_log)
			if self.junit_suite:
				case_name = '%s with %s on %s' % (module.id, mcu.name, board.id)
				self.junit_suite.add_error_case(self.junit_class, case_name, 0, error_log)
			app_module.prerequisites = old_prereqs
			return []
		else:
			generator_directories = generator.get_generated_dirs_list()
			generator_directory = generator_directories.pop()
			self.log.info("Test project for module `%s' on board `%s' with MCU `%s' written to %s" % (module.id, board.id, mcu.name, generator_directory))
			app_module.prerequisites = old_prereqs
			return generator_directory


	def _perform_test_builds(self, make_dir_list):
		"""
		Run make in the specified directories, saving build logs for the
		individual builds.

		Log all failed builds to the error log, with complete log output
		"""
		failure_dirs   = []

		for dir in make_dir_list:
			self.log.info("Commencing test build in %s" % dir)

			failure  = False
			log_file = open(os.path.join(dir, 'build.log'), 'w')

			start_time = time.time()

			try:
				process = subprocess.Popen('make -C "%s"' % dir, stderr=subprocess.STDOUT, stdout=log_file, shell=True)
				process.wait()
			except:
				failure = True
				log_file.write("Something went wrong while building the project.")
			else:
				if process.returncode != 0:
					failure = True

			test_time = time.time() - start_time

			log_file.close()
			self.log.info("--- Done")

			if failure:
				failure_dirs.append(dir)
				log_file = open(log_file.name, 'r')
				fail_log = log_file.read()
				log_file.close()

				self.log.error("Build failed in %s with log output:\n%s" % (dir, fail_log))

				if self.junit_suite:
					self.junit_suite.add_failure_case(self.junit_class, dir, test_time, fail_log)
			else:
				if self.junit_report:
					self.junit_suite.add_success_case(self.junit_class, dir, test_time)

			self._give_life_sign()

		# Output a list of all failed build dirs
		if failure_dirs:
			self.log.error("Builds failed in:")
			for dir in failure_dirs:
				self.log.error("\t%s" % dir)

		build_successes = len(make_dir_list) - len(failure_dirs)
		return build_successes


	def run(self):
		"""
		Run regression tests of modules.

		The regression tests are essentially build testing of projects
		which include the individual modules.

		The projects are are based on the user application template, and
		will be generated for the specified boards, i.e., a project
		template must exist for each board.

		If a board supports several MCUs, i.e., like a user board,
		regression tests will be run for all MCUs on that board.
		"""
		self.start_time = time.time()

		if self.junit_report:
			self.junit_suite = self.junit_report.get_new_suite(self.junit_name, self.junit_package)

		# Get device map, as it is used in several functions
		self.device_map = self.db.get_device_map()

		# Resolve board and module ids, as they may be incorrect and/or contain wildcards
		test_board_ids  = self._resolve_ids(self.board_ids, Board)
		self.log.debug('Resolved board IDs:')
		for board_id in test_board_ids:
			self.log.debug('\t' + board_id)

		test_module_ids = self._resolve_ids(self.module_ids, Module)
		self.log.debug('Resolved module IDs:')
		for module_id in test_module_ids:
			self.log.debug('\t' + module_id)

		# For the specified boards, get a list of the template projects and corresponding generator classes
		project_and_generator_tuples = self._get_template_projects_for_boards(test_board_ids)

		# Iterate through the boards' supported MCUs, and generate
		# test projects for every module which supports one of them
		# -- one test project is generated per MCU supported.
		(attempted_projects, project_dirs) = self._make_test_projects(project_and_generator_tuples, test_module_ids)

		if self.junit_report:
			self.junit_suite.set_time(time.time() - self.start_time)

		if not project_dirs:
			self.log.error("No projects were generated")
			return 1

		project_dirs.sort()
		build_successes = self._perform_test_builds(project_dirs)

		# Output some stats about the projects
		generated_projects = len(project_dirs)

		if generated_projects:
			self.log.info("Successfully generated %s projects" % generated_projects)

		if generated_projects != attempted_projects:
			failed_projects = attempted_projects - generated_projects
			failure_ratio = failed_projects / attempted_projects
			self.log.error("Could not generate %s out of %s (%.1f %%) possible projects" % (failed_projects, attempted_projects, 100 * failure_ratio))

		# Start build-testing and output some stats
		success_ratio = build_successes / generated_projects
		if build_successes:
			self.log.info("Build succeeded for %s out of %s (%.1f %%) generated projects" % (build_successes, generated_projects, 100 * success_ratio))

		if build_successes != generated_projects:
			build_failures = generated_projects - build_successes
			failure_ratio = 1 - success_ratio
			self.log.error("Build failed for %s out of %s (%.1f %%) generated projects" % (build_failures,  generated_projects, 100 * failure_ratio))

		# Return failure if any build failed, or not all projects could
		# be written.
		if build_successes != attempted_projects:
			return 1
		else:
			return 0


def get_list_entries(string_list, separator=','):
	"""
	Return a list of entries from a supplied list of strings which may each
	contain several entries themselves, separated with a specifiable
	separator.

	The default separator is comma.
	"""
	entries = []

	if string_list:
		for string in string_list:
			entries.extend(string.split(separator))

	return entries


if __name__ == "__main__":
	start_time = time.time()

	home_directory     = sys.path[0]
	template_directory = os.path.join(home_directory, 'templates')
	xml_schema_dir     = os.path.join(home_directory, "schemas")
	device_map         = os.path.join(home_directory, "device_maps", "atmel.xml")
	report_directory   = home_directory

	parser = OptionParser(usage =
		"""
		Run regression tests on all or only specified ASF modules,
		using the default test boards or the specified list of boards.
		A user application template project must exist for the board(s)
		to test -- refer to
		common/applications/user_application/ for these.

		Tests are run for all supported MCUs on a board, meaning boards
		with unspecified MCUs can be used to run tests for a group of
		MCUs.

		Note that although it is possible to set config values, there
		is no point in doing this to control select-by-config elements,
		since all modules are tested anyway.


		Alternatively, list all module IDs to a file by specifing the
		list filename with --list-ids. In this case, the options
		--output-dir, --test-board, --not-mcu, --set-config and --junit
		will have no effect.
		""")
	# parser.add_option("","--ext-root", dest="ext_root", default=os.path.join(home_directory, '..', '..'), help="FDK extension root directory")
	parser.add_option("-b","--base-dir", dest="ext_root", default=os.path.join(home_directory, '..', '..'), help="FDK extension root directory")
	parser.add_option("","--main-ext-uuid", dest="main_ext_uuid", default="Atmel.ASF", help="Main FDK extension UUID")
	parser.add_option("","--main-ext-version", dest="main_ext_version", help="Main FDK extension version")

	parser.add_option("-c","--cached", action="store_true", dest="cached", default=False, help="Do not rescan all directories, but use the list of XML files from last run")
	parser.add_option("-o", "--output-dir", dest="out_dir", help="Set output directory for test projects")
	parser.add_option("-t", "--test-board", action="append", dest="test_boards", help="Set ID of board(s) to test with, as comma-separated list w/ wildcard (*), overriding the default " + str(ModuleTesterRuntime.default_board_ids))
	parser.add_option("-n", "--not-mcu", action="append", dest="mcu_blacklist", help="Set MCUs _not_ to test with, as comma-separated list")
	parser.add_option("-m", "--test-module", action="append", dest="test_modules", help="Set ID of module(s) to test, as comma-separated list w/ wildcard (*), overriding the default " + str(ModuleTesterRuntime.default_module_ids))
	parser.add_option("-s", "--set-config", action="append", dest="config", help="Set a config value: --set-config <name>=<value>")
	parser.add_option("-l", "--set-level", dest="log_level", default="warning", help="Set level of logging to output: debug, info, warning, error (default: warning)")
	parser.add_option("", "--set-define", action="append", dest="defines", help="Set a define symbol: --set-define <name>[=<value>]")
	parser.add_option("", "--no-hidden", action="store_true", dest="test_hidden", default=False, help="Ignore the 'hidden' GUI flag in ASF XML and test/list _all_ modules")
	parser.add_option("-j", "--junit", action="store_true", dest="write_junit", default=False, help="Write a test report in JUnit-compatible XML")
	parser.add_option("", "--list-ids", dest="module_list_file", default=None, help="Do a dry run and output to file a plaintext list of module IDs: --list-ids <listfile>")
	parser.add_option("-f","--input-file", dest="input_file", default=None, help="File which contains ID(s) of module(s) to test. One ID per line. Overrides ID(s) given as argument")

	# Parse command line options and arguments
	(options, arguments) = parser.parse_args()

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

		arguments = []
		for line in lines:
			line = line.strip()
			if len(line) > 0:
				arguments.append(line)

	# Remove arguments if one of the arguments is '*'
	for arg in arguments:
		if arg ==  '*':
			arguments = []
			break

	output_directory = None
	if options.out_dir:
		output_directory = os.path.abspath(options.out_dir)
		report_directory = output_directory

	test_boards   = get_list_entries(options.test_boards)
	mcu_blacklist = get_list_entries(options.mcu_blacklist)
	test_modules  = get_list_entries(options.test_modules)
	test_defines = []
	for define in options.defines or []:
		(name, sep, value) = define.partition('=')
		new_define = (name, value)
		test_defines.append(new_define)

	log_level_text = options.log_level

	try:
		configuration = ConfigurationHandler.generate_from_option_parser(options)
	except Exception as e:
		print "Error with configuration", e
		parser.print_help()
		sys.exit(2)

	junit_report = None
	if options.write_junit:
		junit_filename = os.path.join(report_directory, 'testReport.xml')
		junit_report = JUnitReport()

	mod_list_file_opt = options.module_list_file
	if mod_list_file_opt:
		runtime = ModuleListerRuntime(template_directory, configuration, test_modules)
	else:
		runtime = ModuleTesterRuntime(template_directory, configuration, test_boards, test_modules, test_defines)

	try:
		log_level = runtime.get_log_level_from_text(log_level_text)
	except:
		print "Unknown log level `%s'" % log_level_text
		parser.print_help()
		sys.exit(2)

	# Set mode-specific options.
	list_file = None
	if mod_list_file_opt:
		try:
			list_file = open(mod_list_file_opt, 'w')
		except IOError:
			print "Unable to open module list file: " + mod_list_file_opt
			parser.print_help()
			sys.exit(2)

		runtime.set_module_list_file(list_file)
	else:
		runtime.set_junit_report(junit_report)
		runtime.set_mcu_blacklist(mcu_blacklist)
		runtime.set_output_directory(output_directory)

	# Set common options
	runtime.set_commandline_args(arguments)
	runtime.set_test_hidden_modules(options.test_hidden)
	runtime.setup_log(log_level)

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

		# Sanity check of database?
		if options.cached:
			runtime.log.info("Skipping DB sanity check in cached mode")
		else:
			runtime.log.info("Running DB sanity check")
			main_db.sanity_check()

		# Set the database for the runtime class
		runtime.set_db(main_db)

	except Exception as e:
		runtime.log.error("Exception during ASF database creation: " + str(e))
		raise
	else:
		asf_version = runtime.db.get_framework_version()
		runtime.log.info("Commencing regression tests on ASF " + asf_version)
		exit_code = runtime.run()

	if list_file:
		list_file.close()

	if junit_report:
		junit_report.write(junit_filename)

	runtime.log.info("Regression tests finished in %.2f seconds" % (time.time() - start_time))
	sys.exit(exit_code)
