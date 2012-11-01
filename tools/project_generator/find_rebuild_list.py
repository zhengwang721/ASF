import os
import os.path
import re
import sys
import time

from optparse import OptionParser
import sys

import asf.helper

from asf.archiver import Archiver
from asf.database import *
from asf.configuration import ConfigurationHandler
from asf.runtime import Runtime


class ProjectGeneratorRuntime(Runtime):

	require_tag = "require"
	require_ref = "idref"
	id_attrib = "id"
	type_attrib ="type"
	build_tag = "build"
	build_value = "value"
	module_tag = "module"
	project_tag = "project"
	module_types_to_add = ["driver", "service", "component", "library"]

	def __init__(self, template_dir, configuration):
		super(ProjectGeneratorRuntime, self).__init__(template_dir, configuration)
		self.check_device_support = False
		self.show_not_affected = False
		self.outfilename = "outfile.txt"
		self.outfile = None
		self.start_folder = "."

	def set_check_device_support(self, value):
		self.check_device_support = value

	def set_show_not_affected(self, value):
		self.show_not_affected = value

	def set_outfiles(self, ppath, mpath):
		self.outfilename_p = ppath
		self.outfilename_m = mpath

	def set_start_folder(self, value):
		self.start_folder = value

	def find_parent_element_with_specific_tag(self, el, tag):
		current = el
		while current is not None:
			if current.attrib.has_key(tag):
				return current
			current = current.getparent()
		return None

	def find_projects_and_modules_for_id(self, id, level=0):
		result_projects = set()
		result_modules = set()
		prefix = "  "*level

		# On first level, check if we should add the id itself
		if level == 0:
			# Check if this is a module.
			element = self.db.tree.find(".//%s[@%s='%s']" % (self.module_tag, self.id_attrib, id))
			if element is not None:
				# This is a module, check if it's the right kind and then add it
				if element.attrib[self.type_attrib] in self.module_types_to_add:
					result_modules.add(id)
			else:
				# No module, is this a project?
				element = self.db.tree.find(".//%s[@%s='%s']" % (self.project_tag, self.id_attrib, id))
				if element is not None:
					# Yes, project. Add to list of projects.
					result_projects.add(id)

		# Search for parent module if a module selector is selected
		if id.find("#") > 0:
			id = id.partition("#")[0]

		for related_el in self.db.tree.findall(".//%s[@%s='%s']" % (self.require_tag, self.require_ref, id)):
			rel = self.find_parent_element_with_specific_tag(related_el, self.id_attrib)
			rid = rel.attrib[self.id_attrib]

			type = rel.tag
			# Do we have a project or do we need to search further?
			if type == "project":
				result_projects.add(rid)
			else:
				if type == "module":
					if rel.attrib[self.type_attrib] in self.module_types_to_add:
						result_modules.add(rid)

				# Search for parent module if a module selector is selected
				if rid.find("#") > 0:
					rid = rid.partition("#")[0]

				if rid in self._cache:
					# Cached result
					pp, mm = self._cache[rid]
				else:
					# Search modules recursively
					pp, mm = self.find_projects_and_modules_for_id(rid, level+1)
					self._cache[rid] = [pp, mm];

				result_projects.update(pp)
				result_modules.update(mm)

		return result_projects, result_modules

	def find_all_project_ids(self):
		idset = set()
		for id in self.db.tree.findall(".//%s" % (Project.tag)):
			idset.add(id.attrib[self.id_attrib])
		return idset

	def search_modules_from_asf_xml(self, filename):
		self.log.debug("  Finding module for asf.xml file '%s'" %(filename))

		# Format filename the same way as the XML:
		xml_filename = os.path.join(".",filename)

		# Find all elements that have fromfile attribute set to this xml file
		fromfile_elements = self.db.tree.findall(".//%s[@%s='%s']" % ("*", ConfigDB.fromfile_tag, xml_filename))

		affected_ids = set()
		for xml_element in fromfile_elements:
			# Add current node if it has the "id" attribute
			if xml_element.attrib.has_key(self.id_attrib):
				affected_ids.add(xml_element.attrib[self.id_attrib])

			# Find all sub-elements that contain "id" attribute from XML tree
			for id_el in xml_element.findall(".//%s[@%s]" % ("*", self.id_attrib)):
				affected_ids.add(id_el.attrib[self.id_attrib])

		if len(affected_ids) == 0:
			# Unable to find anything, safest to rebuild all
			return None

		# Return set of ids from a specific XML file
		return affected_ids

	def find_module_config_file(self, build_element, filename):
		name = os.path.basename(filename)
		# Find parent module to the build element
		parent = self.find_parent_element_with_specific_tag(build_element, self.id_attrib)
		parent_id = parent.attrib[self.id_attrib]

		self.log.debug("   Searching for '%s'" % (name))
		header_file_element = parent.find(".//%s[@%s='%s']" % (self.build_tag, "value", name))
		if header_file_element is not None:
			if header_file_element.attrib.get("type", "")=="module-config" and header_file_element.attrib.get("subtype", "")=="required-header-file":
				self.log.debug("   Found module_config file '%s' in '%s'" % (filename, parent_id))
				return parent_id

		self.log.debug("   Cannot find module_config file '%s' in module '%s'" % (filename, parent_id))
		return None

	def search_modules_for_file_extended(self, filename):
		path = os.path.dirname(filename)
		self.log.debug("  Extended search for file '%s' by searching for '%s'" %(filename, path))
		# Find all build elements that contain this file
		build_elements = self.db.tree.findall(".//%s[@%s='%s']" % (self.build_tag, self.build_value, path))

		if len(build_elements) == 0:
			# Path does not exist in xml, safest to rebuild all
			return None

		affected_ids = set()
		for element in build_elements:
			type = element.attrib.get("type", False)
			subtype = element.attrib.get("subtype", False)

			if type == "module-config" and subtype == "path":
				module_id = self.find_module_config_file(element, filename)
				if module_id is None:
					# Does not exist in XML, safest to rebuild all
					return None
				else:
					# Found module config, continue processing
					continue

			if type == "distribute" and subtype == "path":
				self.log.debug("   Found distribute file '%s', in path '%s' but don't know how to handle" % (filename, path))
				# Don't know how to handle distribute files, rebuilding all
				return None

		return affected_ids

	def search_modules_for_file(self, filename):
		self.log.debug("  Finding module for file '%s'" %(filename))
		# Find all build elements that contain this file
		build_elements = self.db.tree.findall(".//%s[@%s='%s']" % (self.build_tag, self.build_value, filename))
		if len(build_elements) == 0:
			# Nothing found, safest to rebuild all
			return None

		affected_ids = set()
		for el in build_elements:
			current_el = self.find_parent_element_with_specific_tag(el, self.id_attrib)
			current_id = current_el.attrib[self.id_attrib]
			self.log.debug("   Found '%s'" % (current_id))
			affected_ids.add(current_id)

		return affected_ids

	def eliminate_unsupported_devices(self, original_module_id, project_id_set):
		"""
		Remove project entries from project_id_set that do not match the <device-support> given by original_module_id
		"""
		original_module = self.db.lookup_by_id(original_module_id)

		new_set = set()
		for project_id in project_id_set:
			project = self.db.lookup_by_id(project_id)
			mcu = project.mcu
			mcu_search_map = mcu.get_group_map()

			not_supported_list = original_module.resolve_device_support(mcu_search_map, recursive=False)
			supported = len(not_supported_list) == 0
			if supported:
				new_set.add(project_id)

		return new_set

	def report_line_p(self, str):
		self.outfile_p.write(str + "\n")

	def report_line_m(self, str):
		self.outfile_m.write(str + "\n")

	def report_line(self, str):
		self.report_line_m(str + "\n")
		self.report_line_p(str + "\n")

	def report_rebuild_none(self):
		# Return an invalid ID if nothing should be build
		# This will cause the project generator to not generate any projects
		self.log.info("Rebuild nothing")
		self.report_line("nothing")

	def report_rebuild_all(self):
		# Return "*" for all projects needing to be rebuilt
		self.log.info("Rebuild all")
		self.report_line("*")


	def run(self):
		self._cache = dict()
		self.outfile_p = open(self.outfilename_p, "w")
		self.outfile_m = open(self.outfilename_m, "w")

		# Files that always require recompilation of everything
		# Top level asf.xml require everything to be rebuilt:
		always_build_list = ["asf.xml"]

		# Find what files to check
		input_filename = os.path.join(self.start_folder, self.args[0])
		try:
			f = open(input_filename, "r")
		except Exception as e:
			print "Error opening file '%s'. Error %s" % (input_filename, str(e))
			self.report_rebuild_none()
			sys.exit(1)

		filename_list = f.readlines()
		f.close()
		self.log.debug("Opened input file '%s' for processing" % (input_filename))

		# Convert slashes to correct format for this script
		filename_list = [os.path.normpath(x.strip()) for x in filename_list]

		# Find modules that contain the files specified
		self.log.info("Finding modules for file input")
		affected_ids_set = set()
		for filename in filename_list:
			if filename in always_build_list:
				self.log.debug("File '%s' is in always rebuild list, need to rebuild everything" % (filename))
				self.report_rebuild_all()
				return

			if os.path.isdir(filename):
				# Skip folders
				self.log.debug("  Skipping directory '%s'" % (filename))
				continue

			if os.path.basename(filename) == "asf.xml":
				# Find modules affected by asf.xml change
				affected = self.search_modules_from_asf_xml(filename)
				if affected is None:
					self.log.debug("Unable to find ids in asf.xml file '%s', need to rebuild everything" % (filename))
					self.report_rebuild_all()
					return
				affected_ids_set.update(affected)
			else:
				# Find modules affected by file change
				affected = self.search_modules_for_file(filename)
				if affected is None:
					# Not found in XML, try searching for module configs
					affected = self.search_modules_for_file_extended(filename)
					if affected is None:
						self.log.debug("Unable to find file '%s' in asf.xml, need to rebuild everything" % (filename))
						self.report_rebuild_all()
						return
				affected_ids_set.update(affected)

		# Finding projects for the modules that have changed
		self.log.info("Finding projects for affected modules")
		project_id_set = set()
		module_id_set = set()
		for id in affected_ids_set:
			pids, mids = self.find_projects_and_modules_for_id(id)
			if self.check_device_support:
				pids = self.eliminate_unsupported_devices(id, pids)
			project_id_set.update(pids)
			module_id_set.update(mids)

		everything_set = self.find_all_project_ids()
		unmodified_set = everything_set.difference(project_id_set)

		if len(project_id_set) == 0:
			# If the file was found in the XML but no module we should compile everything
			# since this is probably a linkerscript or something else used by the project generator
			self.report_rebuild_all()
		elif len(project_id_set) == len(everything_set):
			# Rebuilding everything, so no need to return id for each module
			self.report_rebuild_all()
		else:
			# Report projects that need to be rebuilt
			for the_id in project_id_set:
				self.report_line_p(the_id)

			if self.show_not_affected:
				for unmodified in unmodified_set:
					self.log.debug("Not affected: " + unmodified)

			# Report modules that need to be tested
			for the_id in module_id_set:
				self.report_line_m(the_id)

		self.outfile_p.close()
		self.outfile_m.close()
		self.log.info("Affected projects stored in '%s'" % (self.outfilename_p))
		self.log.info("%d projects need rebuild. %d projects not affected" % (len(project_id_set), len(unmodified_set)))
		self.log.info("Affected modules stored in '%s'" % (self.outfilename_m))
		self.log.info("%d modules affected." % (len(module_id_set)))


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

	# Debug options
	parser.add_option("-d","--debug", action="store_true", dest="debug", default=False, help="Enable debugging. Sets 'level' to debug if not otherwise specified.")
	parser.add_option("-l","--level", dest="level", default=False, help="Log output level, from most detailed to least detailed: debug, info, warning or error")
	parser.add_option("-t","--timestamp", action="store_true", dest="timestamp", default=False, help="Timestamp log")
	parser.add_option("-v","--verbose", action="store_const", dest="level", const="info", help="Verbose output")
	parser.add_option("", "--load-cache", dest="cache_file", action="store", default=None, help="Use this given database cache instead of parsing asf.xml")

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
	templatedir = os.path.join(script_path, "templates")
	guidir = os.path.join(script_path, "plugins")
	cachedir = os.path.join(script_path, "cache")
	xml_schema_path = os.path.join(script_path, "asf.xsd")

	# Find absolute paths
	outfile_project_path = os.path.abspath(options.outfile_projects)
	outfile_module_path = os.path.abspath(options.outfile_modules)
	current_folder = os.getcwd()

	# Change directory to the ASF root folder
	os.chdir(basedir)

	configuration = ConfigurationHandler()

	runtime = ProjectGeneratorRuntime(templatedir, configuration)

	runtime.create_and_set_cache_dir(cachedir)

	runtime.set_debug(options.debug)
	runtime.set_xml_schema_path(xml_schema_path)
	runtime.set_commandline_args(args)
	runtime.set_check_device_support(options.device_support)
	runtime.set_show_not_affected(options.not_affected)
	runtime.set_outfiles(outfile_project_path, outfile_module_path)
	runtime.set_start_folder(current_folder)

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
