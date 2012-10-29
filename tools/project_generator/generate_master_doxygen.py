import logging
import os
import os.path
import shutil
import sys

from optparse import OptionParser

from asf.database import *
from asf.runtime import Runtime
from asf.toolchain.generic import *
from asf.junit_report import *
from asf.toolchain.doxygen import DoxygenProject

use_dot = 'NO'
include_sources = 'NO'
path_blacklist = []

def path_is_blacklisted(path):
	if not path_blacklist:
		return False

	for entry in path_blacklist:
		if os.path.join(entry, '') in path:
			return True

	return False


def get_mcu_part_defines(mcu_list):
	"""
	Define symbols for all the supported MCUs, for parts.h.
	These are added as avr-gcc-style defines.
	"""
	mcu_def = []

	for mcu in mcu_list:
		# Skip the unspecified parts.
		if re.match('unspecified.*', mcu):
			continue

		# Is this an XMEGA?
		xmega_match = re.match('(at)(xmega)([a-z0-9]*)', mcu)
		sam_match = re.match('(at)(sam)([a-z0-9]*)', mcu)
		if xmega_match:
			new_mcu_def = '__AVR_' \
					+ xmega_match.group(1).upper() \
					+ xmega_match.group(2) \
					+ xmega_match.group(3).upper() \
					+ '__'
		elif sam_match:
			new_mcu_def = '__SAM_' \
					+ sam_match.group(1).upper() \
					+ sam_match.group(2) \
					+ sam_match.group(3).upper() \
					+ '__'
		# Assume UC3 if no match for xmega nor sam.
		else:
			new_mcu_def = '__AVR32_' \
					+ mcu.upper().lstrip('AT32') \
					+ '__'

		mcu_def.append(new_mcu_def)

	return mcu_def

def get_doxygen_uri(doc_module, arch):
	"""
	Return the doxygen URI from the documentation module
	"""
	if doc_module["type"] in ["module"]:
		uri = doc_module["object"].get_help_uri()
		if uri:
			return arch + "/html/" + uri
		return ""
	elif doc_module["type"] in ["application", "example", "unit-test"]:
		return doc_module["object"].id + "/html/index.html"

	return ""

def get_quickstart_uri_list(doc_module, arch):
	"""
	Build and return the list of quickstart URI(s) associated with this
	module
	"""
	quickstart_list = []
	# Parse the associated Header-files.
	pattern_quickstart = re.compile("\\page\s+(\w+_quickstart)")
	pattern_use_case = re.compile("\\page\s+(\w+_use_case_[0-9]+)")
	for file in doc_module["object"].get_build(BuildHeader, None, False):

		# Open and read the content of the file
		h_file = open(file, "r")
		h_file_data = h_file.read()
		h_file.close()

		# Look for quick start page
		results = pattern_quickstart.findall(h_file_data)
		for result in results:
			url = arch + "/html/" + result + ".html"
			if not url in quickstart_list:
				quickstart_list.append(url)

		# Look for advanced use case page
		results = pattern_use_case.findall(h_file_data)
		for result in results:
			url = arch + "/html/" + result + ".html"
			if not url in quickstart_list:
				quickstart_list.append(url)

	# Return the list of quickstarts
	return quickstart_list

def get_board_name(module):
	"""
	Get the name of the board support by this module
	"""
	if not isinstance(module, Project):
		return ""

	board = db.lookup_by_id(module.board)
	board_name = re.sub("\s+(support|board)", "", board.caption)

	# Hacks
	# Change RZ600S to RZ600
	if re.match('RZ600S', board_name):
		board_name = 'RZ600'

	return board_name

def doc_module_merge(a, b):
	"""
	Merge 2 documentation modules together and return the merge
	"""
	# Merge its board attribute
	while b["board"]:
		board = b["board"].pop()
		if board not in a["board"]:
			a["board"].append(board)
	# Merge its MCU attribute
	while b["mcu"]:
		mcu = b["mcu"].pop()
		if mcu not in a["mcu"]:
			a["mcu"].append(mcu)
	# Merge its technology keywords attribute
	while b["technology"]:
		technology = b["technology"].pop()
		if technology not in a["technology"]:
			a["technology"].append(technology)
	# Merge its application notes attribute
	while b["appnote"]:
		appnote = b["appnote"].pop()
		if appnote not in a["appnote"]:
			a["appnote"].append(appnote)
	# Merge its dependencies
	while b["dep"]:
		dep = b["dep"].pop()
		if dep not in a["dep"]:
			a["dep"].append(dep)
	# Merge its doxygen entry point
	for key in b["doxygen"].keys():
		a["doxygen"][key] = b["doxygen"][key]
	# Merge its URI
	for key in b["uri"].keys():
		a["uri"][key] = b["uri"][key]
	# Merge its quickstart guides
	for key in b["quickstart"].keys():
		if not a["quickstart"].has_key(key):
			a["quickstart"][key] = []
		for quickstart in b["quickstart"][key]:
			if quickstart not in a["quickstart"][key]:
				a["quickstart"][key].append(quickstart)

	# Return the merge
	return a

def generate_doxygen(arch, suite):
	arch_mcu = None
	arch_modules = []
	required_meta_modules = []
	required_application_modules = []
	module_types_to_handle = ["driver", "component", "service"]
	project_types_to_handle = ["normal", "unit-test"]
	mainpage_template_name = "doxygen_master_mainpage.h"
	documentation_modules = []

	(doxy_title) = arch_to_dir_and_title[arch]

	# Get available MCUs for the architecture
	dev_map = db.lookup_by_id(GenericElement.mcu_map_id)
	arch_mcu = dev_map.get_mcu_list(arch)

	# If no MCUs for specified architecture exist, exit immediately.
	if not arch_mcu:
		return []

	def check_and_add_module(module):

		# Don't do anything if this is the wrong type, or its path contains a blacklisted element
		if module.type not in module_types_to_handle or path_is_blacklisted(module.fromfile):
			return

		# Check if any of the MCUs for this architecture are supported by the module
		module_mcu_list = module.get_device_support()

		for module_mcus in module_mcu_list:

			for module_mcu in dev_map.get_mcu_list(module_mcus):

				for this_mcu in arch_mcu:
					if this_mcu == module_mcu:

						# We have a match! Keep it.
						arch_modules.append(module)

						# Look for required meta modules
						for prereq in module.get_prerequisites(recursive=False):
							if prereq.type == 'meta':
								arch_modules.append(prereq)
								required_meta_modules.append(prereq)

	def check_and_add_project(project):

		# Don't do anything if this is the wrong type, or its path contains a blacklisted element
		if project.type not in project_types_to_handle or path_is_blacklisted(project.fromfile):
			return

		# Check if any of the MCUs for this architecture are supported by the module
		module_mcu_list = project.get_device_support()

		for module_mcu in module_mcu_list:
			for this_mcu in arch_mcu:
				if this_mcu == module_mcu:

					arch_modules.append(project)

					# Look for required application modules
					project_module_list = project.get_project_modules()
					for module in project_module_list:
						arch_modules.append(module)

	# Find all modules to handle
	for elem in db.find_components(Module):
		module = db.lookup_by_id(elem.attrib["id"], Module)

		check_and_add_module(module)

	# Find all projects to handle
	for elem in db.find_components(Project):
		project = db.lookup_by_id(elem.attrib["id"], Project)

		check_and_add_project(project)

	# Check requirements of the required meta modules
	while required_meta_modules:
		meta_module = required_meta_modules.pop()

		for prereq in meta_module.get_prerequisites(recursive=False):
			check_and_add_module(prereq)

	# Remove duplicates of the modules
	arch_modules = list(set(arch_modules))

	# ---- Build the documentation modules (doc_module) ----------------------------

	def update_documentation_modules(documentation_modules, module):
		"""
		Helper function to build heplfull documentation attributes for a module
		This function will update the documentation_modules list
		"""

		doc_module = {
			"type"        : "",
			"arch"        : doxy_title,
			"caption"     : module.caption,
			"description" : "",
			"technology"  : [],
			"appnote"     : [],
			"uri"         : {arch: ""},
			"doxygen"     : {arch: ""},
			"path"        : module.fromfile,
			"dep"         : [],
			"board"       : [],
			"mcu"         : [],
			"object"      : module,
			"quickstart"  : {arch: []},
		}

		# Update the description field
		description = module.get_description_summary_text()
		if description:
			doc_module["description"] = description

		# Deal with Modules
		if isinstance(module, Module):
			if module.type not in module_types_to_handle:
				return
			doc_module["type"] = "module"

			# Update the doxygen entry point
			doc_module["doxygen"][arch] = module.get_doxygen_entry_point()
			if not doc_module["doxygen"][arch]:
				suite.add_failure_case(module.__class__.__name__, module.id, 0, "Missing doxygen-entry-point tag")
				print "[ERROR] Module `" + module.id + "' is missing a doxygen-entry-point tag"

			# Check if this module has a getting started
			doc_refs_quickstart = module.get_all_doxygen_quick_start()
			if not doc_refs_quickstart:
				doc_module["quickstart"][arch] = get_quickstart_uri_list(doc_module, arch)
			else:
				for doc_ref_quickstart in doc_refs_quickstart:
					url = arch + "/html/" + doc_ref_quickstart + ".html"
					doc_module["quickstart"][arch].append(url)
					if not url in doc_module["quickstart"][arch]:
						doc_module["quickstart"][arch].append(url)

		# Deal with Projects (Applications, Examples and Unit-tests)
		elif isinstance(module, Project):
			if module.type == 'unit-test':
				doc_module["type"] = "unit-test"
			elif re.search("example", module.fromfile):
				doc_module["type"] = "example"
			else:
				doc_module["type"] = "application"

			# Discard some boards
			board_name = get_board_name(module)
			if re.match('Dummy / *', board_name) or re.match('User template / *', board_name):
				return False
			# Add the board support
			doc_module["board"].append(board_name)

			# Look for required modules to update the description and the keyword parameter
			project_module_list = module.get_project_modules()
			for project_module in project_module_list:
				# Update the description field
				if not doc_module["description"]:
					description = project_module.get_description_summary_text()
					if description:
						doc_module["description"] = description
				# Update the keyword
				keyword = project_module.get_keyword_dict()
				if "technology" in keyword:
					for technology in keyword["technology"]:
						doc_module["technology"].append(technology)

		# Update the mcu information
		doc_module["mcu"].extend(module.get_device_support())

		# Update the URI field
		doc_module["uri"][arch] = get_doxygen_uri(doc_module, arch)

		# Update the technology attribute
		keyword = module.get_keyword_dict()
		if "technology" in keyword:
			for technology in keyword["technology"]:
				doc_module["technology"].append(technology)

		# Update the Application Notes attribute
		appnote = module.get_appnote_list()
		for appnote_value in appnote:
			doc_module["appnote"].append(appnote_value)

		# Check if this documentation entry already exists in the list
		# only applicable for "module"s
		is_in = False
		if doc_module["type"] in ["module"]:
			for index, temp_module in enumerate(documentation_modules):
				if temp_module["doxygen"][arch] == doc_module["doxygen"][arch]:
					if temp_module["type"] == doc_module["type"]:
						is_in = True
						# If this entry exists, merge it with the existing one
						documentation_modules[index] = doc_module_merge(temp_module, doc_module)
						break
		if not is_in:
			# Add this entry if it is not already in
			documentation_modules.append(doc_module)

		# Use the "related-module" tag to build relationship between modules
		for type, id_list in module.get_related_modules_dict():
			if type in module_types_to_handle + project_types_to_handle:
				for id in id_list:
					related_module = db.lookup_by_id(id)
					related_doc_module = update_documentation_modules(documentation_modules, related_module)
					if related_doc_module:
						doc_module["dep"].append(related_doc_module)
						related_doc_module["dep"].append(doc_module)

		return doc_module

	# Create documentation modules from the module list
	for module in arch_modules:
		# Filter only the visible modules
		if 'hidden' in module.get_gui_flag_list():
			continue
		update_documentation_modules(documentation_modules, module)

	# Sort modules by caption
	documentation_modules.sort(key=lambda x: x["caption"].lower())

	# Build relation between modules
	for doc_module in documentation_modules:
		if doc_module["type"] in ["module"]:
			# Guess with the tree architecture
			module_dirname = os.path.dirname(doc_module["path"])
			for doc_project in documentation_modules:
				if doc_project["type"] in ["example", "unit-test"]:
					if os.path.commonprefix([doc_project["path"], module_dirname]) == module_dirname:
						doc_module["dep"].append(doc_project)
						doc_project["dep"].append(doc_module)

	# ---- Build the project doxygen documentation --------------------

	# Generate the documentation for all projects
	for module in documentation_modules:
		if module["type"] in ["example", "unit-test", "application"]:
			project = module["object"]
			doxygen_module = DoxygenProject(project, db, runtime)
			doxygen_module.set_custom_html_header("../doxygen_mainpage/doxygen_header.html")
			doxygen_module.set_custom_layout("../doxygen_mainpage/DoxygenLayout.xml")
			doxygen_module.set_custom_use_dot(use_dot)
			# Create and set the output directory
			our_output = os.path.join(output_dir, project.id)
			if not os.path.exists(our_output):
				os.makedirs(our_output)
			doxygen_module.doxyfile_dir = our_output
			print "Writing doxygen for " + arch + " project to master_doxygen/" + project.id + "/doxyfile.doxygen"
			doxygen_module.write()

	# ---- Build the architecture module documentation ----------------

	modules_only = []
	# Build a list with only the modules
	for module in arch_modules:
		if not isinstance(module, Module):
			continue
		modules_only.append(module)


	# Set output directory according to architecture
	our_output = os.path.join(output_dir, arch)
	if not os.path.exists(our_output):
		os.makedirs(our_output)

	# Find all files for all modules and add to doxygen file.
	doxygen_data = {
		"$ASF_DEFINES$"      : "",
		"$ASF_PROJECT$"      : "\"%s\"" % doxy_title,
		'$ASF_INCLUDE_PATH$' : "",
		'$ASF_INPUT$'        : mainpage_template_name,
		'$ASF_USE_DOT$'      : use_dot,
		'$ASF_INCLUDE_SOURCES$' : include_sources,
		'$ASF_IMAGE_PATH$'   : "",
		'$ASF_HTML_HEADER$'  : "../doxygen_mainpage/doxygen_header.html",
		'$ASF_CUSTOM_LAYOUT$' : "../doxygen_mainpage/DoxygenLayout.xml",
		'$ASF_HTML_FOOTER$'  : "",
		'$ASF_HTML_STYLESHEET$'  : "",
	}

	# Add build items non-recursively for the added modules
	for module in modules_only:
		# C- and H-files.
		for file in module.get_build(BuildC, None, False) + module.get_build(BuildHeader, None, False):
			# Path should relative to our output path
			file = os.path.relpath(file, our_output)
			doxygen_data['$ASF_INPUT$'] += " " + file

		# #defines
		defines = module.get_build(BuildDefine, None, False)
		define_text = ""
		if not defines is None:
			for definition, value in defines:
				if value is None:
					define_text += " " + definition
				else:
					define_text += " %s=%s" % (definition, value)

		doxygen_data["$ASF_DEFINES$"] += define_text

	# Add defines for the MCUs, for parts.h.
	mcu_part_defines = get_mcu_part_defines(arch_mcu)
	for define in mcu_part_defines:
		doxygen_data["$ASF_DEFINES$"] += " " + define

	# Add the image include path
	for module in modules_only:
		for path in module.get_build(BuildDoxygenImagePath, None, False):
			# Path should relative to our output path
			path = os.path.relpath(path, our_output)
			doxygen_data['$ASF_IMAGE_PATH$'] += " " + path

	# Fill template
	our_template = template
	for key, value in doxygen_data.items():
		our_template = our_template.replace(key, value)

	# Default values for the mainpage template
	mainpage_data = {
		"$DOXYGEN_BODY$"  : "",
		"$DOXYGEN_PROJECTS$" : "",
		"$DOXYGEN_UNIT_TESTS$" : "",
		"$DOXYGEN_EXTRA$" : "",
	}

	def print_list_start():
		return "\htmlonly <table class=\"memberdecls\"><tbody>"

	def print_list_end():
		return "</tbody></table> \endhtmlonly"

	def print_list_link(url, caption):
		return "<tr><td class=\"memItemLeft\"></td><td class=\"memItemRight\"><a class=\"el\" href=\"" + url + "\">" + caption + "</a></td></tr>"

	def print_list_description(description):
		return "<tr><td class=\"mdescLeft\"></td><td class=\"mdescRight\"><p>" + description + "</p></td></tr>"

	def print_list(title, module_list, arch, is_description):
		body = ""
		for doc_module in module_list:
			if not body:
				body += "\n\section section_" + str(print_list.section_id) + " " + title + "\n"
				print_list.section_id += 1
				body += print_list_start()
			body += print_list_link("../../" + doc_module["uri"][arch], doc_module["caption"])
			if is_description:
				body += print_list_description(doc_module["description"])
		if body:
			body += print_list_end()
		return body
	# Static variable attributed to the function print_list
	print_list.section_id = 0

	# Update the body of the mainpage template
	body = ""

	temp_module_list = []
	for doc_module in documentation_modules:
		if doc_module["type"] in ["module"]:
			temp_module_list.append(doc_module)
	body += print_list("API", temp_module_list, arch, False)

	mainpage_data["$DOXYGEN_BODY$"] = body

	# Update the application and examples group
	body_projects = ""

	temp_module_list = []
	for doc_module in documentation_modules:
		if doc_module["type"] in ["application"]:
			temp_module_list.append(doc_module)
	body_projects += print_list("Applications", temp_module_list, arch, False)

	temp_module_list = []
	for doc_module in documentation_modules:
		if doc_module["type"] in ["example"]:
			temp_module_list.append(doc_module)
	body_projects += print_list("Examples", temp_module_list, arch, False)

	mainpage_data["$DOXYGEN_PROJECTS$"] = body_projects

	# Update the unit tests group
	body_unit_tests = ""

	temp_module_list = []
	for doc_module in documentation_modules:
		if doc_module["type"] in ["unit-test"]:
			temp_module_list.append(doc_module)
	body_unit_tests += print_list("Unit-tests", temp_module_list, arch, False)

	mainpage_data["$DOXYGEN_UNIT_TESTS$"] = body_unit_tests

	# Update the extra section of the mainpage template
	extra = ""

	# Link the examples with their modules
	for doc_module in documentation_modules:
		if (doc_module["type"] in ["module"]) and doc_module["doxygen"]:
			# List all module dependencies
			temp_examples = []
			temp_unittests = []
			for module_dep in doc_module["dep"]:
				if module_dep["type"] in ["example"]:
					temp_examples.append(module_dep)
				elif module_dep["type"] in ["unit-test"]:
					temp_unittests.append(module_dep)

			# Create the container group "Related project(s)" if needed
			if len(temp_examples) or len(temp_unittests):
				extra += "\n\\defgroup auto_" + doc_module["doxygen"][arch] + "_related_projects Related Project(s)"
				extra += "\n\\ingroup " + doc_module["doxygen"][arch]
				extra += "\n\\{\n"
				extra += "\nIn this section you can find all the projects related to the \\ref " + doc_module["doxygen"][arch] + "\n"
				extra += print_list("Example(s)", temp_examples, arch, True)
				extra += print_list("Unit-test(s)", temp_unittests, arch, True)
				extra += "\n\\}\n"

			# Create the container group "Quick Start Guide(s)" if needed
			if len(doc_module["quickstart"][arch]):
				extra += "\n\\defgroup auto_" + doc_module["doxygen"][arch] + "_quickstart Quick Start Guide(s)"
				extra += "\n\\ingroup " + doc_module["doxygen"][arch]
				extra += "\n\\{\n"
				extra += "\nIn this section you can find a list of all Quick Start guides related to the \\ref " + doc_module["doxygen"][arch] + "\n"
				quickstart_module = []
				number = 0
				for quickstart in doc_module["quickstart"][arch]:
					caption = "Quick Start Guide"
					result = re.match('.*use_case_([0-9]+).*', quickstart)
					if result:
						caption = "Advanced Use Case #" + str(result.group(1))
					quickstart_module.append({
						"uri" : { arch : quickstart },
						"caption" : caption,
					})
				extra += print_list("Quick Start Guide(s)", quickstart_module, arch, False);
				extra += "\n\\}\n"

	mainpage_data["$DOXYGEN_EXTRA$"] = extra

	# Fill the mainpage template
	our_mainpage_template = mainpage_template
	for key, value in mainpage_data.items():
		our_mainpage_template = our_mainpage_template.replace(key, value)

	# Copy mainpage template into output directory
	mainpage_out = os.path.join(our_output, mainpage_template_name)
	f = open(mainpage_out, "wb")
	f.write(our_mainpage_template)
	f.close

	# Write output doxygen file
	outfile = arch + ".doxygen"
	outfile = os.path.join(our_output, outfile)
	f = open(outfile, "wb")
	f.write(our_template)
	f.close
	print "Writing doxygen for %s to %s" %(arch, os.path.relpath(outfile, script_path))

	suite.add_success_case(arch, "Doxygen documentation generated with success", 0)

	return documentation_modules

if __name__ == "__main__":
	parser = OptionParser(usage = """
	<options>
	""")

	parser.add_option("-o", "--output-dir", dest="output_dir", default="./master_doxygen", help="Set output base directory")
	parser.add_option("-d", "--use-dot", action="store_true", dest="use_dot", default=False, help="Use DOT, i.e., generate images in documentation")
	parser.add_option("-b", "--blacklist-path", action="append", dest="path_blacklist", help="Add path element to avoid processing of")
	parser.add_option("-s", "--include-sources", action="store_true", dest="inc_sources", default=False, help="Include sources, both inline and for browsing")
	parser.add_option("-a", "--build-arch", action="append", dest="arch_list", help="Generate master doxygen for specified architecture(s)")
	parser.add_option("-j", "--junit", action="store_true", dest="write_junit", default=False, help="Create Junit report file \"testReport.xml\" in the output directory")

	# Check the supplied arguments
	(options, args) = parser.parse_args()

	output_dir = os.path.abspath(options.output_dir)

	if options.use_dot:
		use_dot = 'YES'
	else:
		use_dot = 'NO'

	if options.inc_sources:
		include_sources = 'YES'
	else:
		include_sources = 'NO'

	path_blacklist = options.path_blacklist

	# Set everything up. Paths, runtime, database and so on
	script_path = sys.path[0]
	xml_schema_path = os.path.join(script_path, "asf.xsd")
	templatedir = os.path.join(script_path, "templates")

	runtime = Runtime(templatedir, ConfigurationHandler())
	runtime.setup_log(logging.WARNING)
	runtime.set_xml_schema_path(xml_schema_path)

	os.chdir(script_path + "/../..")

	db = ConfigDB(runtime)
	db.sanity_check()

	# Architecture list to generate help for
	arch_list = options.arch_list
	if not arch_list:
		dev_map = db.lookup_by_id(GenericElement.mcu_map_id)
		arch_list = dev_map.get_doc_arch_dict()

	# Translation table to get a friendly name from an architecture name
	arch_to_dir_and_title = {}
	dev_map = db.lookup_by_id(GenericElement.mcu_map_id)
	for arch in arch_list:
		arch_to_dir_and_title[arch] = ""
		for e in dev_map.get_sub_elements(tag=DeviceMap.group_tag, attribute_list=["name", DeviceMap.doc_group_attr], value_list=[arch]):
			arch_to_dir_and_title[arch] = e.attrib[DeviceMap.doc_group_attr]

	def print_json_mcu_group_arch():
		"""
		This function outputs in JSON format the mcu group architecture.
		It removes the unspecified parts from the list.
		"""
		def print_json_mcu_group_arch_rec(dev_map, group_name, path = ""):
			# Build the new path
			path += "/group[@name='%s']" % (group_name)
			# Search for the children group names
			node = dev_map.element.findall("./" + path)
			output = ":{"
			if len(node) == 1:
				node = node[0]

				# Search child elements for mcu and sub-groups
				for child_e in node:
					try:
						group_mcu = child_e.attrib['name']
					except KeyError:
						# If element does not have a name, it is an XML comment
						continue

					# Skip the unspecified parts
					if re.match('unspecified.*', group_mcu):
						continue

					# Add a comma only if the element is not the 1rst
					if not output.endswith("{"):
                                                output += ","
					output += "'" + group_mcu + "'"

					# Recursive iteration for sub-groups
					output += print_json_mcu_group_arch_rec(dev_map, group_mcu, path)
			output += "}"
			return output

		dev_map = db.lookup_by_id(GenericElement.mcu_map_id)
		output = "{"
		# Loop through the toplevel group names
		for root_element in dev_map.element.findall("group"):
			# Add a comma only if the element is not the 1rst
			if output[-1] != "{":
				output += ","
			output += "'" + root_element.attrib["name"] + "'"
			# Generate the JSON output for all group_name
			output += print_json_mcu_group_arch_rec(dev_map, root_element.attrib["name"])
		output += "}"
		return output

	# JUnit test
	report = JUnitReport()

	# Load the template
	template_file_path = os.path.join(templatedir, "doxyfile.doxygen")
	f = open(template_file_path, "r")
	template = f.read()
	f.close()

	# Load the mainpage template
	template_file_path = os.path.join(templatedir, "doxygen_master_mainpage.h")
	f = open(template_file_path, "r")
	mainpage_template = f.read()
	f.close()

	# Generate doxygen for all architectures
	doc_module_list = []
	for arch in arch_list:
		# Create a test suite for this architecture
		suite = report.get_new_suite(name=arch, package="ASF Documentation Test Suite")
		# Generate the doxygen
		arch_doc_module_list = generate_doxygen(arch, suite)
		# Add new entries to the main list and merge duplicates
		for arch_doc_module in arch_doc_module_list:
			arch_module_id = arch_doc_module["object"].id
			is_duplicate = False
			for index, doc_module in enumerate(doc_module_list):
				if arch_module_id == doc_module["object"].id:
					# If we have a duplicate, merge
					is_duplicate = True
					doc_module_list[index] = doc_module_merge(doc_module, arch_doc_module)
					break
			# If no duplicate, add this entry tothe list
			if not is_duplicate:
				doc_module_list.append(arch_doc_module)

	if options.write_junit:
		junit_filename = os.path.join(output_dir, 'testReport.xml')
		# Write the JUnit test report
		report.write(junit_filename)

	# Wrapper template helper function
	def merge_with_template_and_save(title, content, wrapper_data, filename):
		"""
		This function will copy the content and title into the template and save
		the resulting file to the output_dir with the name of "filename"
		"""
		# Load the mainpage html wrapper template
		template_file_path = os.path.join(templatedir, "documentation_template.html")
		f = open(template_file_path, "r")
		wrapper_template = f.read()
		f.close()
		# Assign the value to be filled into the template
		wrapper_data["$TEMPLATE_TITLE$"] = title
		wrapper_data["$TEMPLATE_CONTENT$"] = content
		# Fill the template
		for key, value in wrapper_data.items():
			wrapper_template = wrapper_template.replace(key, value)
		# Generate the mainpage html wrapper
		page_out = os.path.join(output_dir, filename)
		f = open(page_out, "wb")
		f.write(wrapper_template)
		f.close

	def create_page(title, body_filename_list, body_data, out_filename):
		"""
		Creates a new web page froma template.
		This function will first concatenate the templates from the list
		"body_filename_list".
		Then it will replace the template varaibles by the one specified
		in "body_data".
		An finally it will merge the resulting template with the main
		page template to fit in the webstire design and save the page
		with the name "out_filename".
		"""
		body_template = ""
		for body_filename in body_filename_list:
			# Load the body html template
			template_file_path = os.path.join(templatedir, body_filename)
			f = open(template_file_path, "r")
			part_body_template = f.read()
			f.close()
			# Fill the template
			for key, value in body_data.items():
				part_body_template = part_body_template.replace(key, value)
			# Concatenate the partial body template with the main one
			body_template += part_body_template
		# Merge with the mainpage template and save the page
		merge_with_template_and_save(title, body_template, body_data, out_filename)

	# Update the wrapper template
	def addslashes(s):
		d = {'"':'\\"', "'":"\\'", "\0":"\\\0", "\\":"\\\\", "\n":"\\n"}
		return ''.join(d.get(c, c) for c in s)

	wrapper_data = {
		"$DOXYGEN_PARTS$"   : "",
		"$DOXYGEN_BOARDS$"  : "",
		"$ASF_MCU_ARCH$"    : "",
		"$ASF_ARCH_TITLE$"  : "",
		"$ASF_CONTENTS$"    : "",
		"$ASF_VERSION$"     : db.get_framework_version_number(),
	}

	# Fill the arch to title translation table
	wrapper_data["$ASF_ARCH_TITLE$"] += "{"
	for arch in arch_to_dir_and_title.keys():
		if wrapper_data["$ASF_ARCH_TITLE$"][-1] != "{":
			wrapper_data["$ASF_ARCH_TITLE$"] += ","
		wrapper_data["$ASF_ARCH_TITLE$"] += "'" + arch + "':\"" + arch_to_dir_and_title[arch] + "\""
	wrapper_data["$ASF_ARCH_TITLE$"] += "}"

	# Update the MCU architecture
	wrapper_data["$ASF_MCU_ARCH$"] = print_json_mcu_group_arch()

	# Print the module list
	dev_map = db.lookup_by_id(GenericElement.mcu_map_id)
	wrapper_data["$ASF_CONTENTS$"] = "["
	for doc_module in doc_module_list:
		if wrapper_data["$ASF_CONTENTS$"][-1] != "[":
			wrapper_data["$ASF_CONTENTS$"] += ","
		wrapper_data["$ASF_CONTENTS$"] += "{"
		# ID
		wrapper_data["$ASF_CONTENTS$"] += "'id':\""
		wrapper_data["$ASF_CONTENTS$"] += addslashes(doc_module["object"].id)
		wrapper_data["$ASF_CONTENTS$"] += "\","
		# Type
		wrapper_data["$ASF_CONTENTS$"] += "'type':\""
		if doc_module["type"] in ["application", "example"]:
			wrapper_data["$ASF_CONTENTS$"] += "application"
		else:
			wrapper_data["$ASF_CONTENTS$"] += addslashes(doc_module["type"])
		wrapper_data["$ASF_CONTENTS$"] += "\","
		# ASF type (services, drivers, components, thirdparty, ...)
		wrapper_data["$ASF_CONTENTS$"] += "'asf_type':\""
		path_split = doc_module["object"].fromfile.replace("\\", "/").split('/')
		if path_split[1] == "thirdparty":
			wrapper_data["$ASF_CONTENTS$"] += "thirdparty"
		else:
			wrapper_data["$ASF_CONTENTS$"] += path_split[2]
		wrapper_data["$ASF_CONTENTS$"] += "\","
		# Name
		wrapper_data["$ASF_CONTENTS$"] += "'name':\"" + addslashes(doc_module["caption"]) + "\","
		# Description
		wrapper_data["$ASF_CONTENTS$"] += "'description':\"" + addslashes(doc_module["description"]) + "\","
		# Documentation URI
		wrapper_data["$ASF_CONTENTS$"] += "'uri':{"
		for arch in doc_module["uri"].keys():
			if wrapper_data["$ASF_CONTENTS$"][-1] != "{":
				wrapper_data["$ASF_CONTENTS$"] += ","
			wrapper_data["$ASF_CONTENTS$"] += "\"" + addslashes(arch) + "\":\"" + addslashes(doc_module["uri"][arch]) + "\""
		wrapper_data["$ASF_CONTENTS$"] += "},"
		# MCUs
		wrapper_data["$ASF_CONTENTS$"] += "'mcus':["
		for mcu in doc_module["mcu"]:
			if wrapper_data["$ASF_CONTENTS$"][-1] != "[":
				wrapper_data["$ASF_CONTENTS$"] += ","
			wrapper_data["$ASF_CONTENTS$"] += "\"" + addslashes(mcu) + "\""
		wrapper_data["$ASF_CONTENTS$"] += "],"
		# Boards
		wrapper_data["$ASF_CONTENTS$"] += "'boards':["
		for board in doc_module["board"]:
			if wrapper_data["$ASF_CONTENTS$"][-1] != "[":
				wrapper_data["$ASF_CONTENTS$"] += ","
			wrapper_data["$ASF_CONTENTS$"] += "\"" + addslashes(board) + "\""
		wrapper_data["$ASF_CONTENTS$"] += "],"
		# Categories
		wrapper_data["$ASF_CONTENTS$"] += "'category':["
		for technology in doc_module["technology"]:
			if wrapper_data["$ASF_CONTENTS$"][-1] != "[":
				wrapper_data["$ASF_CONTENTS$"] += ","
			wrapper_data["$ASF_CONTENTS$"] += "\"" + addslashes(technology) + "\""
		wrapper_data["$ASF_CONTENTS$"] += "],"
		# Application Notes
		wrapper_data["$ASF_CONTENTS$"] += "'appnote':["
		for appnote in doc_module["appnote"]:
			if wrapper_data["$ASF_CONTENTS$"][-1] != "[":
				wrapper_data["$ASF_CONTENTS$"] += ","
			wrapper_data["$ASF_CONTENTS$"] += "{'name':\"" + addslashes(appnote) + "\",'link':null}"
		wrapper_data["$ASF_CONTENTS$"] += "],"
		# Quickstart guides
		wrapper_data["$ASF_CONTENTS$"] += "'quickstart':{"
		for arch in doc_module["quickstart"].keys():
			if wrapper_data["$ASF_CONTENTS$"][-1] != "{":
				wrapper_data["$ASF_CONTENTS$"] += ","
			temp_data = "\"" + addslashes(arch) + "\":["
			is_quickstart = False
			for quickstart in doc_module["quickstart"][arch]:
				if temp_data[-1] != "[":
					temp_data += ","
				temp_data += "\"" + addslashes(quickstart) + "\""
				is_quickstart = True
			temp_data += "]"
			if is_quickstart:
				wrapper_data["$ASF_CONTENTS$"] += temp_data
		wrapper_data["$ASF_CONTENTS$"] += "}}\n"
	wrapper_data["$ASF_CONTENTS$"] += "]"

	# Update the supported devices section
	supported_devices = []
	for arch in arch_list:
		(doxy_title) = arch_to_dir_and_title[arch]
		supported_devices.append({
			"title" : doxy_title,
			"device"   : arch,
		})
	supported_devices.sort(key=lambda x: x["title"].lower())
	for item in supported_devices:
		wrapper_data["$DOXYGEN_PARTS$"] += "supported_devices_add(\"" + addslashes(item["title"]) + "\", \"" + addslashes(item["device"]) + "\");" + "\n";

	# Update the supported boards section
	supported_boards = []
	for doc_module in doc_module_list:
		for board in doc_module["board"]:
			if not board in supported_boards:
				supported_boards.append(board)
	supported_boards.sort()
	for item in supported_boards:
		wrapper_data["$DOXYGEN_BOARDS$"] += "supported_boards_add(\"" + addslashes(item) + "\");" + "\n";

	# Copy the mainpage html ressource files
	wrapper_rsrcs_src = os.path.join(templatedir, "doxygen_mainpage")
	wrapper_rsrcs_dst = os.path.join(output_dir, "doxygen_mainpage")
	shutil.rmtree(wrapper_rsrcs_dst, True)
	shutil.copytree(wrapper_rsrcs_src, wrapper_rsrcs_dst)

	# Generate the javascript data page
	# Load the template
	template_file_path = os.path.join(templatedir, "asf_content_template.js")
	f = open(template_file_path, "r")
	js_data_template = f.read()
	f.close()
	# Fill the template
	for key, value in wrapper_data.items():
		js_data_template = js_data_template.replace(key, value)
	# Generate the mainpage html wrapper
	page_out = os.path.join(output_dir, "doxygen_mainpage/asf_content.js")
	f = open(page_out, "wb")
	f.write(js_data_template)
	f.close

	# Create the different pages
	create_page("Application Programming Interface", ["documentation_body_api.html", "documentation_body_dynamic_table.html"], wrapper_data, "api.html")
	create_page("Applications", ["documentation_body_applications.html", "documentation_body_dynamic_table.html"], wrapper_data, "applications.html")
	create_page("Unit Tests", ["documentation_body_unit_tests.html", "documentation_body_dynamic_table.html"], wrapper_data, "unit_tests.html")
	create_page("Embedded Software Solution for Atmel Flash MCUs", ["documentation_body_mainpage.html"], wrapper_data, "index.html")
	create_page("ASF Architecture", ["documentation_body_architecture.html"], wrapper_data, "architecture.html")
	create_page("Download", ["documentation_body_download.html"], wrapper_data, "download.html")
	create_page("Get Started", ["documentation_body_get_started.html", "documentation_body_dynamic_table.html"], wrapper_data, "get_started.html")
	create_page("Bug Tracker", ["documentation_body_bug_tracker.html"], wrapper_data, "bug_tracker.html")
	create_page("Search Results", ["documentation_body_dynamic_table.html"], wrapper_data, "search.html")
	create_page("ASF-Sensors", ["documentation_body_asf_sensors.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_sensors.html")
	create_page("ASF-USB", ["documentation_body_asf_usb.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_usb.html")
	create_page("ASF-FS", ["documentation_body_asf_fs.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_fs.html")
	create_page("ASF-CAN/LIN", ["documentation_body_asf_can_lin.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_can_lin.html")
	create_page("ASF-GFX", ["documentation_body_asf_gfx.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_gfx.html")
	create_page("ASF-DSP", ["documentation_body_asf_dsp.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_dsp.html")
	create_page("Cortex Microcontroller Software Interface Standard", ["documentation_body_cmsis.html"], wrapper_data, "cmsis.html")
	create_page("3rd Party", ["documentation_body_3rd_party.html", "documentation_body_dynamic_table.html"], wrapper_data, "3rd_party.html")
	create_page("ASF-Drivers", ["documentation_body_asf_drivers.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_drivers.html")
	create_page("ASF-Services", ["documentation_body_asf_services.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_services.html")
	create_page("ASF-Components", ["documentation_body_asf_components.html", "documentation_body_dynamic_table.html"], wrapper_data, "asf_components.html")
	create_page("ASF-Boards", ["documentation_body_asf_boards.html"], wrapper_data, "asf_boards.html")

	print "Generating doxygen html wrapper"
