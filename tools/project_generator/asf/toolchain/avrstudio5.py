from asf.database import *
from asf.toolchain.generic import GenericProject
from cStringIO import StringIO
from zipfile import ZipFile, ZIP_DEFLATED
import asf.helper
import copy
import os
import re
import uuid
import shutil
import asf.etreeabstraction as ET
from asf.helper import using_ironpython

vstemplate_namespace = "http://schemas.microsoft.com/developer/vstemplate/2005"
avrgccproj_namespace = "http://schemas.microsoft.com/developer/msbuild/2003"

documentation_server_basedir  = "/docs/"
documentation_project_suffix  = "/doxygen/html/"


# Must define namespace modifiers according to the Python implementation in use
if using_ironpython():
	def vs_ns(name):
		return '[namespace-uri()=\'%s\' and local-name()=\'%s\']' % (vstemplate_namespace, name)
	def agp_ns(name):
		return '[namespace-uri()=\'%s\' and local-name()=\'%s\']' % (avrgccproj_namespace, name)
else:
	def vs_ns(name):
		return '{%s}%s' % (vstemplate_namespace, name)
	def agp_ns(name):
		return '{%s}%s' % (avrgccproj_namespace, name)


class AVRStudio5Project(GenericProject):
	generator_tag = 'as5_8'
	framework_content_id = 'Atmel.ASF'
	toolchain = 'avrgcc'
	toolchain_tag = 'AvrGcc'
	toolchain_name = 'com.Atmel.AVRGCC8'
	toolchain_language = 'C'
	template_prefix = 'avrstudio5'
	vsproject_to_import = r'$(AVRSTUDIO_EXE_PATH)\\Vs\\AvrGCC.targets'

	# Optimization level settings
	optlevel_to_text = {
			'none'   : 'None (-O0)',
			'low'    : 'Optimize (-O1)',
			'medium' : 'Optimize more (-O2)',
			'high'   : 'Optimize most (-O3)',
			'size'   : 'Optimize for size (-Os)',
	}
	optlevel_default = 'size'
	optlevel_default_for_debug = 'low'

	# Standard library and startup file config names
	config_use_stdlib = 'use_stdlib'
	config_use_deflibs = 'use_defaultlibs'
	config_use_startfiles = 'use_startfiles'

	# Location, under the template directory, of image files
	image_dir = 'avrstudio5_images'

	# Default icon and image file names
	icon_name = '__TemplateIcon.png'
	image_name = '__PreviewImage.jpg'

	# Default directories to place files in
	prj_root_dir = ''
	src_root_dir = 'src'
	app_dir = src_root_dir
	asf_dir = os.path.join(src_root_dir, 'asf')
	conf_dir = os.path.join(src_root_dir, 'config')

	# List of project types to add vstemplate etc. for
	project_types_with_vstemplate = ['template']

	@staticmethod
	def file_is_config_header(filename):
		# Return true if filename ends with conf_*.h
		if re.search("conf_[a-zA-Z0-9\-_]*\.h$", filename):
			return True
		else:
			return False


	@staticmethod
	def get_regexp_to_value_match(regexp_to_value, string):
		for regexp, value in regexp_to_value.items():
			if re.match(regexp, string):
				return value

		return None


	@staticmethod
	def get_regexp_to_value_match_in_list(regexp_to_value, string_list):
		for string in string_list:
			value = AVRStudio5Project.get_regexp_to_value_match(regexp_to_value, string)
			if value:
				return value

		return None

	@staticmethod
	def sort_flags(flag_string):
		"""
		Sort space-separated flags in supplied string, removing
		duplicates and extra whitespace at the same time.
		"""
		flag_set = set(flag_string.split(' '))
		flag_set -= set([''])

		flag_list = list(flag_set)
		flag_list.sort()

		return ' '.join(flag_list)

	@property
	def make_vstemplate(self):
		try:
			do_make = self._make_vstemplate
		except AttributeError:
			do_make = self.project.type in self.project_types_with_vstemplate
			self._make_vstemplate = do_make

		return do_make

	def _get_build_file_dict_from_modules(self, modules):
		"""
		Returns a dictionary containing all the build files for the
		modules which are passed to the function. The dictionary keys
		are the attribute property of the relevant Build item classes:
			- BuildC
			- BuildAssembly
			- BuildHeader
			- BuildUserLibrary
		The keys map to new dictionaries, in which the key is the source
		filename and the corresponding value is None, but is meant to
		contain the target filename in the project output.
		"""
		files = {}

		file_build_type_classes = [
			BuildC,
			BuildAssembly,
			BuildHeader,
			BuildUserLibrary,
			BuildDistributeLicense,
			BuildDistributeFile,
			BuildDistributeUserFile,
		]

		for type_class in file_build_type_classes:
			file_list = []

			for module in modules:
				module_files = module.get_build(type_class, self.toolchain, recursive=False)
				file_list.extend(module_files)

			files[type_class] = dict([(f, None) for f in file_list])

		# Add the files contained in the distribute-dirs as files of
		# type distribute-file. Note that the individual files
		# specified with build type distribute-file were added above.
		distribute_file_list = []

		for module in modules:
			module_distribute_dirs = module.get_build(BuildDistributeDirectory, self.toolchain, recursive=False)

			for directory in module_distribute_dirs:
				for root_dir, sub_dirs, dir_files in os.walk(directory):
					if ".svn" in sub_dirs:
						sub_dirs.remove(".svn")

					distribute_file_list.extend([os.path.join(root_dir, f) for f in dir_files])

		files[BuildDistributeFile].update(dict([(f, None) for f in distribute_file_list]))

		return files


	def _add_files_to_avrgccproj_itemgroup(self, itemgroup_e, file_list, maintype, subtype=None):
		"""
		Add all items in the file list to the supplied ItemGroup
		element, with the specified main and sub types. If the
		sub type is not specified, it is not set.
		The main_type is essentially the top tag to use.

		The following list:
			['foo\\bar.c', 'foobar\\foo\\bar.c']
		Is added as (with maintype = 'Compile', subtype = 'compile'):
			<Compile Include="foo\bar.c">
				<SubType>compile</SubType>
			</Compile>
			<Compile Include="foobar\foo\bar.c">
				<SubType>compile</SubType>
			</Compile>
		"""
		file_list.sort()
		for f in file_list:
			content_e = ET.SubElement(itemgroup_e, agp_ns(maintype))
			content_e.attrib['Include'] = f.replace(os.sep, '\\')

			if subtype:
				subtype_e = ET.SubElement(content_e, agp_ns('SubType'))
				subtype_e.text = subtype


	def _add_directories_to_avrgccproj_itemgroup(self, itemgroup_e, dir_list):
		"""
		Add all items in the directory list to the supplied ItemGroup
		element.

		The following list:
			['foo\\bar', 'foobar\\foo\\bar']
		Is added as:
			<Folder Include="foo\bar\"/>
			<Folder Include="foobar\foo\bar\"/>
		"""
		# Ensure backslash is used as path separator
		dir_list = [d.replace(os.sep, '\\') for d in dir_list]
		dir_list.sort()
		for d in dir_list:
			# Ensure that directory ends with a backslash
			if d[-1] != '\\':
				d = d + '\\'
			content_e = ET.SubElement(itemgroup_e, agp_ns('Folder'))
			content_e.attrib['Include'] = d


	def _add_files_to_vstemplate_project(self, project_e, file_list, replace_parameters=False):
		"""
		Adds the items in a file list to the supplied root XML element,
		as XML elements with the structure needed by VS.

		For example, this list:
			['root', 'dir1\dir2\sub']
		Is added as (assuming default value for parameter replacement):
			<ProjectItem ReplaceParameters="false" TargetFileName="root">root</ProjectItem>
			<Folder Name="dir1" TargetFolderName="dir1">
				<Folder Name="dir2" TargetFolderName="dir2">
					<ProjectItem ReplaceParameters="false" TargetFileName="sub">sub</ProjectItem>
				</Folder>
			</Folder>
		"""
		if replace_parameters:
			repl_param_value = 'true'
		else:
			repl_param_value = 'false'

		# Add the individual files in their respective <Folder>
		for f in file_list:
			(f_dir, f_name) = os.path.split(f)

			# Make a list of the directory structure
			f_dir_list = []
			while f_dir:
				(f_dir, temp) = os.path.split(f_dir)
				f_dir_list.append(temp)

			# Find/create the <Folder> elements under which the file belongs, if any
			current_e = project_e
			while f_dir_list:
				current_dir = f_dir_list.pop()
				next_e = current_e.find("./%s[@%s='%s']" % (vs_ns('Folder'), 'Name', current_dir))
				if next_e is None:
					current_e = ET.SubElement(current_e, vs_ns('Folder'))
					current_e.attrib['Name'] = current_dir
					current_e.attrib['TargetFolderName'] = current_dir
				else:
					current_e = next_e

			# Add the file as a <ProjectItem> to the current <Folder>, if any
			file_e = ET.SubElement(current_e, vs_ns('ProjectItem'))
			file_e.text = f_name
			file_e.attrib['TargetFileName'] = f_name
			file_e.attrib['ReplaceParameters'] = repl_param_value


	def _add_files_to_framework_config_files(self, files_e, file_list, framework_name, framework_version, changed=False):
		"""
		Adds the items in a file list to the supplied root XML element,
		as XML elements with the specified framework name and version,
		as well as change status.
		The items in the file list must be 2-value tuples, where the
		first value is corresponds to the file source, and the second to
		its path in the project.

		The following list:
			[("foo\\bar\\foobar.c", "bar\\foo\\barfoo.c"),
			 ("foo\\bar.c", "bar\\foo.c")]
		Is added as (assuming framework name is X and version is Y, and changed is False or unspecified):
			<file framework="X" version="Y" path="bar\foo\barfoo.c" source="foo\bar\foobar.c" changed="False" />
			<file framework="X" version="Y" path="bar\foo.c" source="foo\bar.c" changed="False" />
		"""
		def fix_path_sep(path):
			return path.replace(os.sep, '/')

		file_list.sort()
		for (f_source, f_target) in file_list:
			e = ET.SubElement(files_e, "file")
			e.set("framework", framework_name)
			e.set("version", framework_version)
			e.set("path", fix_path_sep(f_target))
			e.set("source", fix_path_sep(f_source))
			e.set("changed", str(changed))
			e.set("content-id", self.framework_content_id)


	def _get_selected_device(self):
		"""
		Return a properly formatted string with the project MCU. This
		must correspond to a part name in the AVR Studio 5 part
		description XML files.

		If the project MCU name starts with "unspecified", an empty
		string is returned.
		"""
		mcu = self.project.mcu.name

		if re.match('unspecified.*', mcu):
			return ''
		else:
			# Format the MCU name:
			# - (x)mega should have capital 'AT' and family designator (letters + digits after '(x)mega')
			x_mega_match = re.match('(at)(x?mega)([a-z0-9]*)', mcu)
			if x_mega_match:
				return x_mega_match.group(1).upper() + x_mega_match.group(2) + x_mega_match.group(3).upper()

		return mcu


	def _get_selected_device_series(self):
		"""
		Return the series that the selected device belongs to.
		This corresponds to the doc-arch under which it is grouped.
		"""
		return self.project.mcu.get_doc_arch_group()



	def _get_main_application_module(self, modules):
		application_modules = [m for m in modules if m.type == 'application']
		return application_modules[0]


	def _get_formatted_description(self, module):
		description_lines = module.get_description_summary_text_list()

		description = ''

		if description_lines:
			for line in description_lines:
				description += line + ' '
			description = description[:-1]
		else:
			self.log.warning('Module %s does not have a description, using empty string in AS5 export' % module.id)

		return description


	def _generate_vstemplate_tree(self):
		"""
		Generate the project .vstemplate element tree.
		"""

		# Parse the template XML
		xml_tree = ET.parse(os.path.join(self.templatedir, self.template_prefix + '.vstemplate'))
		root_element = xml_tree.getroot()

		# -- Update content of TemplateData --

		templatedata_e = root_element.find('./' + vs_ns('TemplateData'))

		# Update tags with text content
		templatedata_elements_to_update_with_text = {
			'Name'         : self.project_caption,
			'Description'  : self.project_description,
			'DefaultName'  : self.project_name,
		}

		# Icon and preview image should only be added for, e.g., template projects
		if self._get_vstemplate_icon_path():
			templatedata_elements_to_update_with_text['Icon'] = self.icon_name

		if self._get_vstemplate_image_path():
			templatedata_elements_to_update_with_text['PreviewImage'] = self.image_name

		for (tag, text) in templatedata_elements_to_update_with_text.items():
			e = templatedata_e.find('./' + vs_ns(tag))
			if e is None:
				self.log.error("Could not find `%s' in TemplateData element in %s.vstemplate" % (tag, self.template_prefix))
			e.text = text

		# -- Update content of TemplateContent --

		# Update <Project>
		project_e = root_element.find('./%s/%s' % (vs_ns('TemplateContent'), vs_ns('Project')))
		avrgccproj_file = self.project_name + '.cproj'

		# Set the project configuration file (.cproj)
		project_e.attrib['File'] = avrgccproj_file
		project_e.attrib['TargetFileName'] = avrgccproj_file
		project_e.attrib['ReplaceParameters'] = 'true'

		# Add asf.h + the project file list to <Project>
		file_target_list = self.project_compiler_files_dict.values() + self.project_distribute_files_dict.values() + [os.path.join(self.app_dir, 'asf.h')]
		self._add_files_to_vstemplate_project(project_e, file_target_list)

		# -- Generate custom parameters
		# Device series can be added in any case
		customparam_to_value = {
			'$deviceseries$'   : self._get_selected_device_series(),
			'$fullyqualifiedtoolchainname$' : self.toolchain_name,
			'$Language$' : self.toolchain_language,
		}

		device = self._get_selected_device()

		# If an unspecified device is used, make the device selection wizard pop up
		if not device:
			wizard_e = ET.SubElement(root_element, 'WizardExtension')
			assembly_e = ET.SubElement(wizard_e, 'Assembly')
			assembly_e.text = 'AvrGCC, Version=5.1.0.0, Culture=neutral, PublicKeyToken=d264112969646cc9'
			classname_e = ET.SubElement(wizard_e, 'FullClassName')
			classname_e.text = 'Atmel.VsIde.AvrStudio.Project.Gcc.Wizard.GccProjectWizard'
		# Otherwise, add the custom parameter for the selected device
		else:
			customparam_to_value['$selecteddevice$'] = device

		customparams_e = root_element.find('./%s/%s' % (vs_ns('TemplateContent'), vs_ns('CustomParameters')))
		for (name, value) in customparam_to_value.items():
			e = ET.SubElement(customparams_e, vs_ns('CustomParameter'))
			e.set('Name', name)
			e.set('Value', str(value))

		# Fix indentation before returning
		asf.helper.indent_xml(root_element)

		return xml_tree


	def _get_avrgccproj_template(self):
		return '%s_%s.cproj' % (self.template_prefix, self.toolchain)


	def _get_avrgccproj_default_toolchain_settings(self, configuration):
		"""
		Returns the default toolchain settings for the specified project configuration, i.e.,
		Release or Debug.
		"""
		common_defaults = {
				'assembler.general.AssemblerFlags' :
					'-mrelax',
				'compiler.general.ChangeDefaultBitFieldUnsigned' :
					'False',
				'compiler.general.ChangeDefaultCharTypeUnsigned' :
					'False',
				'compiler.optimization.OtherFlags' :
					'-fdata-sections',
				'compiler.optimization.PrepareFunctionsForGarbageCollection' :
					'True',
				'compiler.warnings.AllWarnings' :
					'True',
				'linker.optimization.GarbageCollectUnusedSections' :
					'True',
				'linker.optimization.RelaxBranches' :
					'True',
		}

		release_defaults = {
			'compiler.optimization.DebugLevel' :
				'None',
		}

		debug_defaults = {
			'compiler.optimization.DebugLevel' :
				'Maximum (-g3)',
		}

		# Build the default dictionary according to the specific project config
		defaults = common_defaults
		if configuration.lower() == 'release':
			defaults.update(release_defaults)
		elif configuration.lower() == 'debug':
			defaults.update(debug_defaults)
		else:
			raise Exception("Unknown project configuration `%s'" % configuration)

		return defaults


	def _get_avrgccproj_toolchain_settings(self, configuration):
		"""
		Returns a dictionary with toolchain settings for the specified project configuration.

		The dictionary keys correspond to the XML tags in the avrgcc project file for AVR Studio 5.
		The dictionary values are either strings, for text-elements, or lists of strings, for
		settings which should be output as XML to the avrgcc project file accordingly:
			<ListValues>
				<Value>...</Value>
				...
			</ListValues>
		"""
		settings = self._get_avrgccproj_default_toolchain_settings(configuration)

		# Get the total configuration
		total_config = self.get_total_config()
		if configuration.lower() == 'debug':
			total_config['optimization'] = self.optlevel_default_for_debug

		# Get {tag : content} dictionaries for toolchain configuration
		# of both the release and the debug target
		text_settings = self._get_avrgccproj_toolchain_text_content(total_config)
		listvalues_settings = self._get_avrgccproj_toolchain_listvalues_content(total_config)

		settings.update(text_settings)
		settings.update(listvalues_settings)

	# --- Start temporary workaround for AVR Studio bug 14500. TODO: Remove
		asm_flags = ""
		for (symbol, value) in self.project_build_items[BuildDefine]:
			if (value == ""):
				asm_flags += " -D%s" % (symbol)
			else:
				asm_flags += " -D%s=%s" % (symbol, value)

		for key in self._get_toolchain_assembler_flag_key():
			old_asm_flags = settings.get(key, "")
			settings[key] = self.sort_flags(old_asm_flags + asm_flags)
	# --- End temporary workaround

		return self._prepend_toolchain_to_dict_keys(settings)


	# --- Start temporary workaround for AVR Studio bug 14500. TODO: Remove
	def _get_toolchain_assembler_flag_key(self):
		return ["assembler.general.AssemblerFlags"]
	# -- End temporary workaround

	def _generate_avrgccproj_tree(self):
		"""
		Generate the project .cproj element tree.
		"""
		# Parse the project XML
		template_file = self._get_avrgccproj_template()
		xml_tree = ET.parse(os.path.join(self.templatedir, template_file))
		root_element = xml_tree.getroot()

		# -- Process the root <Project> element --

		# Add a VS project to import -- this is architecture dependent
		import_e = ET.SubElement(root_element, agp_ns('Import'))
		import_e.attrib['Project'] = self.vsproject_to_import

		# -- Process the three <PropertyGroup> elements --
		# -- First <PropertyGroup>
		propertygroups_e = root_element.findall('./%s' % agp_ns('PropertyGroup'))
		if len(propertygroups_e) != 3:
			self.log.critical('Found %s PropertyGroup elements in %s, expected 3' % (len(propertygroups_e), template_file))

		# Insert project GUID, set device series for selection wizard
		tag_to_text = {
			'AsfVersion'      : self.db.get_framework_version_number(),
			'ToolchainName'   : self.toolchain_name,
			'Language'        : self.toolchain_language,
			'ProjectGuid'     : str(uuid.uuid1()),
			'avrdeviceseries' : self._get_selected_device_series(),
		}


		# If a device is specified, set the device name
		device = self._get_selected_device()
		if device:
			tag_to_text['avrdevice'] = device


		for tag, text in tag_to_text.items():
			tag_e = propertygroups_e[0].find('./%s' % agp_ns(tag))
			tag_e.text = text

		# Update the framework configuration
		asf_data_e = self._generate_framework_config_tree()
		tag_e = propertygroups_e[0].find('./%s' % agp_ns('AsfFrameworkConfig'))
		tag_e.clear()
		tag_e.append(asf_data_e.getroot())

		# -- Remaining <PropertyGroup>

		# Make <ListValues> with include paths

		# Ensure that slash is used as path separator
		includepaths = [path.replace(os.sep, '/') for path in self.project_build_items[BuildInclude]]

		includepath_listvalues_e = self._generate_avrgccproj_listvalues(includepaths, '../')

		# Make <ListValues> with defines -- convert tuple of strings to single string with format 'symbol=value'
		defines = []
		for (symbol, value) in self.project_build_items[BuildDefine]:
			if(value==""):
				defines.append(symbol)
			else:
				defines.append('%s=%s' % (symbol, value))
		define_listvalues_e = self._generate_avrgccproj_listvalues(defines)

		# Pair the <ListValues> with tags to insert them into
		listvalues_element_and_taglist_pairs = [
			(includepath_listvalues_e, self._get_avrgccproj_includepath_tags()),
			(define_listvalues_e,      self._get_avrgccproj_define_tags()),
		]

		for propertygroup_e in propertygroups_e[1:]:
			for (listvalues_e, taglist) in listvalues_element_and_taglist_pairs:
				for tag in taglist:
					tag = '%s.%s' % (self.toolchain, tag)
					e = propertygroup_e.find('.//%s' % agp_ns(tag))
					e.append(copy.deepcopy(listvalues_e))


		# -- Process the two <ItemGroup> elements --
		itemgroups_e = root_element.findall('./%s' % agp_ns('ItemGroup'))
		if len(itemgroups_e) != 2:
			self.log.critical('Found %s ItemGroup elements in %s, expected 2' % (len(itemgroups_e), template_file))

		# Add _all_ the files to first element, create a set with their directories at the same time
		dir_set = set()

		file_target_list = self.project_compiler_files_dict.values()
		self._add_files_to_avrgccproj_itemgroup(itemgroups_e[0], file_target_list, 'Compile', 'compile')

		dir_set |= set(os.path.dirname(file_target) for file_target in file_target_list)

		# TODO Find the correct tag and type to use.
		file_target_list = self.project_distribute_files_dict.values() + [os.path.join(self.app_dir, 'asf.h')]
		self._add_files_to_avrgccproj_itemgroup(itemgroups_e[0], file_target_list, 'None')

		dir_set |= set(os.path.dirname(file_target) for file_target in file_target_list)

		# Add all parent directories to the set: "a\b\c" -> "a", "a\b", "a\b\c"
		parent_dir_set = set()
		for d in dir_set:
			d_strip = os.path.dirname(d)
			while d_strip:
				parent_dir_set |= set([d_strip])
				d_strip = os.path.dirname(d_strip)

		dir_set |= parent_dir_set
		dir_set -= set([self.prj_root_dir])

		# Add the directories to the second item group
		self._add_directories_to_avrgccproj_itemgroup(itemgroups_e[1], list(dir_set))

		# Update toolchain settings for the release and debug configurations
		for propertygroup_e in propertygroups_e[1:]:
			condition = propertygroup_e.get('Condition')
			config_match = re.findall("'\$\(Configuration\)' == '(.*)'", condition)

			try:
				configuration = config_match[0]
			except IndexError:
				self.log.error("Could not determine project configuration from PropertyGroup's Condition: `%s'" % condition)
				raise

			# Fetch the element to contain our toolchain's settings
			xpath = './' + agp_ns("ToolchainSettings/" + agp_ns(self.toolchain_tag))

			matches = propertygroup_e.findall(xpath)
			nr_of_matches = len(matches)

			if nr_of_matches != 1:
				error_log = "Found %s elements to contain toolchain settings (`%s') for configuration `%s', expected one" % (nr_of_matches, self.toolchain_tag, configuration)
				self.log.error(error_log)
				raise Exception(error_log)

			toolchain_tag_e = matches[0]

			tag_to_content = self._get_avrgccproj_toolchain_settings(configuration)
			self._update_avrgccproj_tag_content(toolchain_tag_e, tag_to_content)

		# Fix indentation before returning
		asf.helper.indent_xml(root_element)

		return xml_tree

	def _update_avrgccproj_tag_content(self, root_element, tag_to_content):
		"""
		Update the sub elements of the specified root with text and/or
		lists of values. tag_to_content is a dictionary which maps from
		an element tag to the new content.
		"""
		template_file = self._get_avrgccproj_template()

		# Update elements with text content
		for tag, content in tag_to_content.items():
			xpath = './/%s' % agp_ns(tag)
			matches = root_element.findall(xpath)
			nr_of_matches = len(matches)

			if nr_of_matches > 1:
				self.log.error("XPath `%s' gave %s matches in avrgccproj propertygroup, expected at most one" % (xpath, nr_of_matches))

			try:
				tag_e = matches[0]
			except IndexError:
				tag_e = ET.SubElement(root_element, tag)
			else:
				tag_e.clear()

			c_type = type(content)

			if c_type == list:
				listvalues_e = self._generate_avrgccproj_listvalues(content)
				tag_e.append(listvalues_e)

			elif c_type == str:
				tag_e.text = content

			else:
				c_type_str = str(c_type)[len('<type ') : -len('>')]
				self.log.error('Unknown content type %s for avrgccproj propertygroup' % c_type_str)


	def _get_avrgccproj_optimization_level(self, config):
		"""
		Get a dictionary which maps from the optimization level tag to
		the text content corresponding to the level set for the project.
		"""
		tag_to_text = {}

		opt_level = config.get('optimization', self.optlevel_default)
		text = self.optlevel_to_text[opt_level]
		tag_to_text['compiler.optimization.level'] = text

		return tag_to_text


	def _get_avrgccproj_stdlib_settings(self, config):
		"""
		Get a dictionary which maps from the standard library and
		start-up file configuration tags to the text content
		corresponding to the project configuration.
		"""
		# Not currently needed for 8-bit AVR
		return {}


	def _get_avrgccproj_compiler_flags(self, config):
		"""
		Get a dictionary which maps from the other compiler flags tag to
		the text content which is given by the default flags and any
		flags that are set in the project.
		"""
		tag = 'compiler.miscellaneous.OtherFlags'
		flags = '-std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax'

		for flag in self.project.get_build(BuildCCompilerFlags, self.toolchain):
			flags += ' ' + flag.strip()

		return {tag : self.sort_flags(flags)}


	def _get_avrgccproj_linker_flags(self, config):
		"""
		Get a dictionary which maps from the linker flags tag to the
		text content which is given by the project configuration.
		This includes the linker script, if any was found/specified.
		"""
		tag_to_text = {}
		text = ''

		# Add the linker script, if any was specified or found
		linker_script = self.project_build_items.get(BuildLinkerScript, None)
		if linker_script:
			target_file = linker_script.values()
			text += ' -T%s' % os.path.join('..', target_file[0]).replace(os.sep, '/')

		# Add any linker flags that have been set in the project
		linker_flags = self.project_build_items[BuildLinkerFlags]
		for flag in linker_flags:
			text += ' -Wl,' + flag.strip()

		# Add any architecture-specific linker flags
		text += self._get_avrgccproj_arch_specific_linker_flags(config)

		tag_to_text['linker.miscellaneous.LinkerFlags'] = self.sort_flags(text)

		return tag_to_text


	def _prepend_toolchain_to_dict_keys(self, in_dict):
		"""
		Prepend the toolchain to the keys in the supplied dictionary.

		The following dictionary:
			{ 'foo' : 'bar' }
		Is returned as (assuming avrgcc toolchain):
			{ 'avrgcc.foo' : 'bar' }
		"""
		out_dict = {}

		for (key, item) in in_dict.items():
			out_dict['%s.%s' % (self.toolchain, key)] = item

		return out_dict


	def _get_avrgccproj_arch_specific_linker_flags(self, config):
		"""
		Return a string with architecture-specific linker flags.
		"""
		return ' -Wl,--relax'


	def _get_avrgccproj_toolchain_text_content(self, config):
		"""
		Get a dictionary which maps from toolchain configuration tags to
		text content.

		The functions which are called to build this dictionary must not
		prepend the toolchain to their tag -- this is done before this
		function returns.
		"""
		tag_to_text = {}

		tag_to_text.update(self._get_avrgccproj_optimization_level(config))
		tag_to_text.update(self._get_avrgccproj_compiler_flags(config))
		tag_to_text.update(self._get_avrgccproj_linker_flags(config))
		tag_to_text.update(self._get_avrgccproj_stdlib_settings(config))

		return tag_to_text


	def _get_avrgccproj_libraries(self, config):
		"""
		Get a dictionary which maps from the tags for library names and
		paths to ListValue elements, containing these items.
		"""
		tag_to_listvalues = {}

		libs = []
		libpaths = []

		for lib_source, lib_target in self.project_build_items[BuildUserLibrary].items():
			(libpath, libfile) = os.path.split(lib_target)
			# Library name must be on the form 'libNN.a'
			libs.append(libfile[len('lib'):libfile.rfind('.')])
			libpath = os.path.join('..', libpath)
			libpaths.append(libpath.replace(os.sep, '/'))

		for lib in self.project_build_items[BuildCompilerLibrary]:
			libs.append(lib[len('lib'):])

		tag_to_listvalues['linker.libraries.Libraries'] = libs
		tag_to_listvalues['linker.libraries.LibrarySearchPaths'] = libpaths

		return tag_to_listvalues


	def _get_avrgccproj_toolchain_listvalues_content(self, config):
		"""
		Get a dictionary which maps from toolchain configuration tags to
		ListValues elements.

		The functions which are called to build this dictionary must not
		prepend the toolchain to their tag -- this is done before this
		function returns.
		"""
		tag_to_listvalues = {}

		tag_to_listvalues.update(self._get_avrgccproj_libraries(config))

		return tag_to_listvalues


	def _get_avrgccproj_includepath_tags(self):
		"""
		Get a list with tags to insert the project include paths into.
		"""
		tags = [
			'compiler.directories.IncludePaths',
			'assembler.general.IncludePaths',
		]

		return tags


	def _get_avrgccproj_define_tags(self):
		"""
		Get a list with tags to insert the project defines into.
		"""
		tags = [
			'compiler.symbols.DefSymbols'
		]

		return tags


	def _add_items_to_avrgccproj_listvalues(self, listvalues_element, item_list, prefix=''):
		"""
		Add items from a list as Value elements in the supplied element,
		which should be a ListValues element. A prefix to prepend to
		each item can be specified.

		The following list:
			['foo', 'bar']
		Is added as (assuming no prefix is specified):
			<Value>foo</Value>
			<Value>bar</Value>
		"""
		item_list.sort()
		for i in item_list:
			e = ET.SubElement(listvalues_element, agp_ns('Value'))
			e.text = prefix + i

		return listvalues_element


	def _generate_avrgccproj_listvalues(self, item_list, prefix=''):
		"""
		Create a ListValues element and fill it with Value elements from
		the supplied list of items. A prefix to prepend to each of the
		items can be specified.

		The following list:
			['foo', 'bar', 'foo\\bar']
		Results in the following element structure (assuming no prefix):
			<ListValues>
				<Value>foo</Value>
				<Value>bar</Value>
				<Value>foo\\bar</Value>
			</ListValues>
		"""
		e = ET.Element(agp_ns('ListValues'))
		self._add_items_to_avrgccproj_listvalues(e, item_list, prefix)
		return e


	def _get_documentation_help_url(self, project):
		"""
		Get the documentation URL given a project object.
		"""

		return project.get_help_url(project.id)


	def _generate_framework_config_tree(self):
		"""
		Generate framework configuration element tree
		"""
		# Parse the framework config template XML
		framework_config_template = """<?xml version="1.0" encoding="utf-8"?>
		<framework-data>
			<options/>
			<configurations/>
			<files/>
		</framework-data>
		"""
		xml_tree = ET.fromstring(framework_config_template).getroottree()

		# Get the elements to update
		root_element = xml_tree.getroot()
		options_e = root_element.find('options')
		configs_e = root_element.find('configurations')
		files_e = root_element.find('files')

		# Add board and project identifiers to framework-data
		default_attribs = {
			"value"      : "Add",
			"config"     : "",
			"content-id" : self.framework_content_id,
		}

		board_id_e = ET.SubElement(root_element, "board")
		board_id_e.set("id", self.project.board)
		board_id_e.attrib.update(default_attribs)

		project_id_e = ET.SubElement(root_element, "project")
		project_id_e.set("id", self.project.id)
		project_id_e.attrib.update(default_attribs)

		arch = self._get_selected_device_series()

		remote_id_to_selectbyconfig_id = {}
		remote_id_to_last_prereq_id = {}
		remote_id_to_value = {}

		# Generate the list of selected ASF prerequisites, i.e., options
		if not arch:
			self.log.warning("Project %s uses device %s which is not part of a supported architecture" % (self.project.id, self.project.mcu.name))
		else:
			# Get a list of project, board and application prerequisites
			prerequisites = []
			for module in [self.project] + self.application_modules:
					prerequisites.extend(module.get_prerequisites(recursive=False))

			project_board = self.db.lookup_by_id(self.project.board)
			if not project_board:
				self.log.warning("Project %s does not require a board definition" % self.project.id)
			# Add prerequisites from the board definition.
			else:
				board_file = project_board.fromfile
				board_prereqs = project_board.get_prerequisites(recursive=False)

				# Resolve dependencies recursively -- add only
				# those which are required from the board's
				# asf.xml, but are defined elsewhere.
				for prereq in board_prereqs:
					if prereq.fromfile != board_file:
						prerequisites.append(prereq)
					else:
						new_board_prereqs = prereq.get_prerequisites(recursive=False)
						board_prereqs.extend(new_board_prereqs)

			# Remove duplicates
			prerequisites = list(set(prerequisites))

			for prereq in prerequisites:
				# Skip meta modules and board definition
				if prereq.type == 'meta' or prereq.tag == 'board':
					continue

				remote_id = prereq.id

				# The value for the option depends on whether it is a check box or combo list,
				# i.e., a Module or SelectByConfig. For the latter class, allow the previously
				# set value to be overridden as it was set based on a selected Module.
				# TODO This is not nested-SelectByConfig-proof...
				prereq_is_selectbyconfig = isinstance(prereq, SelectByConfig)
				if prereq_is_selectbyconfig:
					value = prereq._get_search_list(self.configuration, self.project.mcu)[0]
				else:
					value = ''

				# If the option value was _not_ already set due to a SelectByConfig prerequisite,
				# it should be safe to (re)set the value in any case.
				# If the value _was_ already set based on a SelectByConfig, log an error if the
				# new value is also from a SelectByConfig. Otherwise, discard the new value.
				if remote_id not in remote_id_to_selectbyconfig_id:
					if remote_id in remote_id_to_value:
						# Only update and output to debug log if the value is actually changed
						old_value = remote_id_to_value[remote_id]
						if value != old_value:
							self.log.debug("Value for option with remote ID %s changed from `%s' to `%s' by prerequisite with ID %s -- last set by prerequisite with ID %s" % (remote_id, old_value, value, prereq.id, remote_id_to_last_prereq_id[remote_id]))

							remote_id_to_last_prereq_id[remote_id] = prereq.id
							remote_id_to_value[remote_id] = value
					else:
						remote_id_to_last_prereq_id[remote_id] = prereq.id
						remote_id_to_value[remote_id] = value

					if prereq_is_selectbyconfig:
						remote_id_to_selectbyconfig_id[remote_id] = prereq.id
				else:
					if prereq_is_selectbyconfig:
						# Log an error if multiple SelectByConfig try to set a value for
						# the same option. This would probably be due to nested
						# select-by-config elements...
						self.log.error("Value for option with remote ID %s first set by SelectByConfig %s to `%s', then by %s to `%s' -- using first value in framework config" % (remote_id, remote_id_to_selectbyconfig_id[remote_id], remote_id_to_value[remote_id], prereq.id, value))

			# Generate and add the option elements
			for (remote_id, value) in sorted(remote_id_to_value.items()):
				e = ET.SubElement(options_e, "option")
				e.set("id", remote_id)
				e.set("value", "Add")
				e.set("config", value)
				e.set("content-id", self.framework_content_id)

		# Add URL to Doxygen generated documentation for application
		documentation_help_url = self._get_documentation_help_url(self.project)
		documentation_e = ET.SubElement(root_element, "documentation")
		documentation_e.set("help", documentation_help_url)

		framework_version = self.db.get_framework_version_number()

		# Generate the list of added files
		files_to_add = []
		for file_source, file_target in self.project_compiler_files_dict.items() + self.project_distribute_files_dict.items():
			files_to_add.append((file_source, file_target))

		self._add_files_to_framework_config_files(files_e, files_to_add, "", framework_version)

		# Generate list of configurations
		for key, value in self.project._project_board_configuration.config.items():
			e = ET.SubElement(configs_e, "configuration")
			e.set("key", key)
			e.set("value", value)
			e.set("default", value)
			e.set("content-id", self.framework_content_id)

		# Fix indentation before returning
		asf.helper.indent_xml(root_element)

		return xml_tree


	def _get_device_description(self):
		# Try using the device name
		description = self._get_selected_device()

		# If no device name, use the doc-arch
		if not description:
			description = self.project.mcu.get_doc_arch_name()

		return description


	def _get_vstemplate_image_path(self):
		"""
		Get filepath of preview image for the VS template
		"""
		try:
			image_path = self.vstemplate_image_path

		except AttributeError:
			board_regexp_to_image = {
				'.*_qt600'            : 'QT600.jpg',
				'evk1100'             : 'EVK1100.png',
				'evk1101'             : 'EVK1101.png',
				'evk1104.?'           : 'EVK1104.jpg',
				'evk1105'             : 'EVK1105.jpg',
				'mega-1284p_xplained' : 'MEGA-1284P-Xplained.jpg',
				'rz600.?'             : 'RZ600.jpg',
				'sam3n_ek'            : 'SAM3N-EK.jpg',
				'sam3s_ek'            : 'SAM3S-EK.jpg',
				'sam3s_ek2'           : 'SAM3S-EK2.png',
				'sam3u_ek'            : 'SAM3U-EK.jpg',
				'sam3x_ek'            : 'SAM3X-EK.jpg',
				'sam4s_ek'            : 'SAM4S-EK.jpg',
				'sam4l_ek'            : 'SAM4L-EK.jpg',
				'stk500.*'            : 'STK500.jpg',
				'stk600.*'            : 'STK600.jpg',
				'uc3_a3_xplained'     : 'UC3-A3-Xplained.jpg',
				'uc3_l0_xplained'     : 'UC3-L0-Xplained.jpg',
				'uc3c_ek'             : 'AT32UC3C-EK.jpg',
				'uc3l_ek'             : 'AT32UC3L-EK.jpg',
				'xmega_a1_xplained'   : 'XMEGA-A1-Xplained.jpg',
				'xmega_a3bu_xplained' : 'XMEGA-A3BU-Xplained.jpg',
				'xmega_b1_xplained'   : 'XMEGA-B1-Xplained.jpg',
				'xmega_c3_xplained'   : 'XMEGA-C3-Xplained.jpg',
				'xplain'              : 'Xplain.jpg',
			}

			# Strip the common board. prefix from board ID to get its name
			board_name = self.project.board[len('board.'):]
			# Start with empty image_path - returned if no image found
			image_path = None

			# Look for a regexp match
			for regexp, image in board_regexp_to_image.items():
				if re.match(regexp, board_name):
					image_name = image
					image_dir = os.path.join(self.templatedir, self.image_dir, 'kits')
					image_path = os.path.join(image_dir, image_name)
					break

			self.vstemplate_image_path = image_path

		return image_path


	def _get_product_line(self):
		"""
		Returns the product line, useful for picking the icon to use
		in the VStemplate.
		"""
		avr_product_lines = [
			'mega',
			'tiny',
			'uc3',
			'xmega',
		]
		group_map = self.project.mcu.get_group_map()

		for line in avr_product_lines:
			if line in group_map:
				return line

		return None


	def _get_vstemplate_icon_path(self):
		"""
		Get filepath of icon for the VS template
		"""
		try:
			icon_path = self.vstemplate_icon_path

		except AttributeError:
			product_line = self._get_product_line()

			if not product_line:
				icon_path = None
			else:
				icon_name = product_line + '.png'
				icon_dir = os.path.join(self.templatedir, self.image_dir, 'icons')
				icon_path = os.path.join(icon_dir, icon_name)

			self.vstemplate_icon_path = icon_path

		return icon_path


	def _get_project_output_dir(self):
		"""
		Get the directory into which the generated files are written.
		"""
		return os.path.join(self.project.basedir, self.generator_tag)


	def _get_asf_header_name(self):
		"""
		Get the full filename of the autogenerated ASF header file.
		"""
		return self._get_exported_project_file_name() + '.h'


	def _get_vstemplate_name(self):
		"""
		Get the filename of the VS template.
		"""
		return self._get_exported_project_file_name() + '.vstemplate'


	def _get_avrgccproj_name(self):
		"""
		Get the filename of the AVRGCC project file.
		"""
		return self._get_exported_project_file_name() + '.cproj'


	def _get_exported_project_file_name(self):
		return os.path.join(self._get_project_output_dir(), self.project.id.replace('.','_'))


	def _get_zip_archive_name(self):
		"""
		Get the filename of the ZIP archive to contain the template.
		"""
		return self._get_exported_project_file_name() + '.zip'


	def get_generated_files_list(self):
		"""
		Get a list of all the generated files -- this is used for
		cleaning the ASF tree.
		"""
		files = []

		if not self.runtime.no_zip:
			files.append(self._get_zip_archive_name())

		if self.runtime.debug:
			debug_files = [
				self._get_avrgccproj_name(),
				self._get_asf_header_name(),
			]

			if self.make_vstemplate:
				debug_files.append(self._get_vstemplate_name())

			files.extend(debug_files)

		return files


	def get_generated_dirs_list(self):
		return [self._get_project_output_dir()]

	def get_module_licenses(self, main_app):
		module_licenses = {}
		prereqs = main_app.get_prerequisites(recursive=True)
		for preq in prereqs:
			licenses = preq.get_build(BuildDistributeLicense, self.toolchain, recursive=False)
			if licenses:
				module_licenses[preq] = licenses
		return module_licenses

	def _set_file_targets(self, file_dict, config_dir, config_headers, source_dir, root_to_source_dir=False):
		"""
		Set the target for all the files in the specified file dict.
		This means setting a value (target file) for each key (source
		file) in the dictionary.

		All files with a basename that is in config_headers are moved
		to the root of config_dir. Files that appear to be a config
		header (name is conf_*.h), but are not in the config_headers
		list, are also moved -- with a logged warning message.
		All other files are moved to source_dir. If root_to_source_dir
		is true, the directory structure is made flat for these files
		as well, i.e., they are moved to the root of source_dir.
		"""
		for file_source in file_dict:
			file_basename = os.path.basename(file_source)

			if root_to_source_dir:
				file_basename_to_move = file_basename
			else:
				file_basename_to_move = file_source

			if file_basename in config_headers:
				file_target = os.path.join(config_dir, file_basename)

			elif self.file_is_config_header(file_basename):
				self.log.warning("Project %s has file %s which is not a required config header, but will be moved to %s in AS5 export" % (self.project.id, file_source, config_dir))
				file_target = os.path.join(config_dir, file_basename)

			else:
				file_target = os.path.join(source_dir, file_basename_to_move)

			file_dict[file_source] = file_target


	def _get_project_data(self):
		"""
		Get project data required for several stages of the export:
		"""
		# Get lists of the modules that constitute the application itself, and the rest
		application_modules = self._get_application_modules()
		other_project_modules = [self.project] + list(set(self.project.get_prerequisites(recursive=True)) - set(application_modules))

		# Two types of files to note:
		# 1) types that should be compiled, not just distributed
		# 2) types that should be moved to source root (from application)
		compile_types = [BuildC, BuildAssembly]
		src_root_types = compile_types + [BuildHeader]
		prj_root_types = [BuildDistributeUserFile]


		# -- Add build file lists and mappings --
		# Get all the build files for the project
		application_root_files = {}
		project_root_files = {}
		application_files = self._get_build_file_dict_from_modules(application_modules)
		other_project_files = self._get_build_file_dict_from_modules(other_project_modules)

		# Sift out the application files to move to source or project root
		for f_type, f_dict in application_files.items():
			if f_type in src_root_types:
				application_root_files[f_type] = f_dict
				del application_files[f_type]
			elif f_type in prj_root_types:
				project_root_files[f_type]= f_dict
				del application_files[f_type]

		# Sift out the project files to move to project root
		for f_type, f_dict in other_project_files.items():
			if f_type in prj_root_types:
				project_root_files[f_type].update(f_dict)
				del other_project_files[f_type]

		# Look for linker script unless project has unspecified device
		if not re.match('unspecified.*', self.project.mcu.name):
			# Add linker script, if any is found -- this is architecture-specific since it might not be required by the toolchain
			linker_script = self._get_linker_script()
			if linker_script:
				other_project_files[BuildLinkerScript] = {linker_script : None}


		# Move the files:
		# - distribute files should be placed in asf_dir
		# - config headers should be placed in root of conf_dir
		# - application files should be placed in root of app_dir
		# - remaining files should be placed in asf_dir
		required_config_headers = self.project.get_build(BuildModuleConfigRequiredHeaderFile, self.toolchain)
		common_config_headers = [
			'conf_board.h',
			'conf_clock.h',
		]
		config_headers_to_move = common_config_headers + required_config_headers

		for file_dict in application_root_files.values():
			self._set_file_targets(file_dict, self.conf_dir, config_headers_to_move, self.app_dir, root_to_source_dir=True)

		for file_dict in project_root_files.values():
			self._set_file_targets(file_dict, '', [], self.prj_root_dir, root_to_source_dir=True)

		for file_type, file_dict in application_files.items() + other_project_files.items():
			if file_type == BuildDistributeFile:
				self._set_file_targets(file_dict, '', [], self.asf_dir)
			else:
				self._set_file_targets(file_dict, self.conf_dir, config_headers_to_move, self.asf_dir)


		# Classify files according to Compile/None types
		project_build_items = {}
		project_compiler_files_dict = {}
		project_distribute_files_dict = {}

		for f_type, f_dict in application_files.items() + application_root_files.items() + project_root_files.items() + other_project_files.items():
			if f_type in compile_types:
				project_compiler_files_dict.update(f_dict)
			else:
				project_distribute_files_dict.update(f_dict)

			# Must add all items to master dictionary of build items
			items = project_build_items.get(f_type, {})
			items.update(f_dict)
			project_build_items[f_type] = items


		# Check for duplicate file targets, i.e., whether any files are
		# overwritten. Output a list of the affected files, if any.
		file_target_list = project_compiler_files_dict.values() + project_distribute_files_dict.values()
		file_target_set = set(file_target_list)

		if len(file_target_set) < len(file_target_list):
			self.log.error("Duplicate target files detected in AS5 export for project %s:" % self.project.id)

			for file_target in file_target_set:
				if file_target_list.count(file_target) > 1:
					self.log.error("\t" + file_target)

		# -- Add include paths --

		# Get the include paths of the entire project
		include_paths = self.project.get_build(BuildInclude, self.toolchain, recursive=True)

		# Move them to asf_dir, add the application and config dirs
		include_paths = [os.path.join(self.asf_dir, path) for path in include_paths]
		include_paths.extend([self.app_dir, self.conf_dir])

		project_build_items[BuildInclude] = include_paths


		# -- Add defines --
		project_build_items[BuildDefine]= self.project.get_build(BuildDefine, self.toolchain)


		# -- Add compiler libraries --
		project_build_items[BuildCompilerLibrary]= self.project.get_build(BuildCompilerLibrary, self.toolchain)


		# -- Add linker flags --
		project_build_items[BuildLinkerFlags] = self.project.get_build(BuildLinkerFlags, self.toolchain)


		# Store the build item and file dictionaries in the object
		self.project_build_items = project_build_items
		self.project_compiler_files_dict = project_compiler_files_dict
		self.project_distribute_files_dict = project_distribute_files_dict

		# Get the example application's caption and description
		# and the name of the project as it will show up in the
		# AVR Studio 5 solution explorer.
		kit_name = self._get_kit_name()
		if not kit_name:
			kit_name = 'Unknown Board'

		device_description = self._get_device_description()
		if not device_description:
			device_description = 'Unknown Device'

		main_app = self._get_main_application_module(application_modules)

		# Build a list of items for the project's caption based on the project type (application or template)
		caption_items = []
		if self.make_vstemplate:
			caption_items.extend((kit_name, device_description))
		else:
			# Special case for unknown boards in examples - show device instead
			if (kit_name == "Unknown Board"):
				caption_items.extend((main_app.caption, device_description))
			else:
				caption_items.extend((main_app.caption, kit_name))

			# Special case for boards which can support several target devices
			if kit_name == "STK600" or kit_name == "STK500" or kit_name == "Simulator":
				caption_items.append(device_description)

		self.project_caption = " - ".join(caption_items)
		self.project_description = self._get_formatted_description(main_app)
		# Add detailed project caption to description in case caption is truncated.
		self.project_description += " [%s - %s - %s]" % (main_app.caption, kit_name, device_description)

		# Check if we need to truncate template-caption to make AVR Studio happy.
		if self.make_vstemplate:
			char_limit = 60
			if len(self.project_caption) > char_limit:
				self.log.warning("AVR Studio 5 template name/caption too long. String '%s' has length %d. Truncating it to %d chars." %
					(self.project_caption, len(self.project_caption), char_limit))

				self.project_caption = asf.helper.trunc_string(self.project_caption, char_limit)

		self.project_name = self.get_workspace_name().upper()
		self.project_licenses = self.get_module_licenses(main_app)


	@property
	def project_id(self):
		return self.project.id

	@property
	def project_type(self):
		return self.project.type

	@property
	def project_board_vendor(self):
		return self.project._board.vendor

	@property
	def project_board_addons(self):
		return self.project.get_board_addons()

	@property
	def project_device(self):
		return self._get_selected_device()

	@property
	def project_device_series(self):
		return self._get_selected_device_series()

	@property
	def project_help_url(self):
		return self._get_documentation_help_url(self.project)

	@property
	def project_keywords(self):
		return self._get_project_keywords()

	@property
	def project_toolchain_settings(self):
		project_configs = ['Release', 'Debug']
		settings = {}

		for config in project_configs:
			settings[config] = self._get_avrgccproj_toolchain_settings(config)

		return settings

	def get_framework_config(self):
		return ET.tostring(self._generate_framework_config_tree())


	def _get_project_keywords(self):
		"""
		Return a dictionary of all the keywords in the project, i.e.,
		the project category, kit and any keywords from the main
		application module.
		"""
		def capitalize_first_letter(string):
			return string[:1].upper() + string[1:]


		# Start generating the tree with keywords for the project
		# Grab the keywords from the application modules -- we don't
		# want to get the keywords for the entire project due to a lot
		# of common modules being used.

		# Should ideally only be _one_ application module!
		apps = self._get_application_modules()
		keyword_dict = {}

		for app in apps:
			app_keywords = app.get_keyword_dict(recursive=False)

			for key, words in app_keywords.items():
				capital_key = capitalize_first_letter(key)

				old_words = keyword_dict.get(capital_key, [])
				new_words = list(set(old_words) | set(words))
				keyword_dict[capital_key] = new_words

		# If no kit name was found, don't trigger output of the category
		kit_name = self._get_kit_name()
		if kit_name:
			keyword_dict['Kit'] = [kit_name]

		addons = self.project.get_board_addons()
		if addons:
			keyword_dict['Addon'] = addons.keys()

		# Add keyword for the project category -- this should be the
		# second part of the project ID, f.ex. avr32.>>drivers<<...,
		# common.>>components<<... and xmega.>>applications<<...
		try:
			split_id = self.project.id.split('.')
			project_category = split_id[1]
		# Must handle malformed project IDs...
		except IndexError:
			project_category = 'unspecified'

		keyword_dict['Category'] = [capitalize_first_letter(project_category)]

		return keyword_dict


	def get_build_from_export(self, build_type):
		"""
		Return build items of the specified type which will be exported
		for this project.
		"""
		return self.project_build_items.get(build_type, [])


	def _get_license_to_ids(self):
		"""
		Return dictionary which maps a license file to the IDs of
		whichever modules have specified it as their license.
		"""
		license_to_ids = {}

		# Get the licenses from all modules
		for module in [self.project] + self.project.get_prerequisites(recursive=True):
			licenses = module.get_build(BuildDistributeLicense, self.toolchain)

			for license in licenses:
				ids = license_to_ids.get(license, [])
				ids.append(module.id)
				license_to_ids[license] = ids

		return license_to_ids


	def get_compile_files(self):
		"""
		Return dictionary of files for the compiler.
		"""
		return self.project_compiler_files_dict


	def get_total_config(self):
		"""
		Return dictionary of configuration options, prioritized so that
		toolchain specific configs override generic compiler configs:

		These configs:
			config.compiler.avrgcc.optimization = high
			config.compiler.optimization = none

		..will give:
			total_config['optimization'] = high
		"""
		#Read out the total project compiler configuration
		generic_config = self._get_compiler_config()
		toolchain_config = self._get_toolchain_config()
		total_config = generic_config

		# Note that toolchain-specific config overrides any generic config
		total_config.update(toolchain_config)

		return total_config


	def _get_linker_script(self):
		"""
		Get the filename of the linker script, if any is specified/available.
		"""
		mcu_family_to_selector_id = {
			'xmegaa' : 'xmega.utils.linker_scripts',
			'xmegaau' : 'xmega.utils.linker_scripts',
			'xmegab' : 'xmega.utils.linker_scripts',
			'xmegad' : 'xmega.utils.linker_scripts',
		}

		mcu_family = self.project.mcu.get_group_map()[-2]

		try:
			selector_id = mcu_family_to_selector_id[mcu_family]
			filepath = GenericProject._get_linker_script(self, selector_id)
		except:
			filepath = None

		return filepath


	def _get_kit_name(self):
		try:
			project_board = self.db.lookup_by_id(self.project.board)
			return project_board.caption
		except:
			return None


	def write(self):
		"""
		Generate and write all the files for the project.
		This results in four files to be output:
		- a Visual Studio template
		- an AVR GCC project file
		- a framework configuration file
		- a ZIP archive containing source code, the three files
		  above and two images
		"""
		self.log.info("Starting export of AVR Studio 5 project")

		self._check_for_config_headers()

		# -- Generate and write the needed XML files --
		self._get_project_data()

		output_dir = self._get_project_output_dir()
		asf_root_dir = os.path.relpath(os.curdir, output_dir)

		# Create the output directory, if it does not already exist
		if not os.path.exists(output_dir):
			os.makedirs(output_dir)

		# Generate content of mandatory files
		asf_header_content = StringIO(self._generate_asf_header_content())
		cproj_content = StringIO()
		cproj_tree = self._generate_avrgccproj_tree()
		cproj_tree.write(cproj_content)

		# List generated files and content to output
		extra_files = []
		extra_content = [(cproj_content, self.project_name + '.cproj'),
				(asf_header_content, os.path.join(self.app_dir, 'asf.h'))]

		# Generate .vstemplate and find images if needed
		if self.make_vstemplate:
			vstemplate_content = StringIO()
			vstemplate_tree = self._generate_vstemplate_tree()
			vstemplate_tree.write(vstemplate_content)

			extra_content.append((vstemplate_content, self.project_name + '.vstemplate'))

			vstemplate_icon = self._get_vstemplate_icon_path()
			if vstemplate_icon:
				extra_files.append((vstemplate_icon, self.icon_name))

			vstemplate_image = self._get_vstemplate_image_path()
			if vstemplate_image:
				extra_files.append((vstemplate_image, self.image_name))

		# Set up functions to create ZIP archive or copy/write files
		# in the output directory, depending on the setting
		if not self.runtime.no_zip:
			zip_archive_name = self._get_zip_archive_name()
			zip_archive = ZipFile(zip_archive_name, 'w', ZIP_DEFLATED)

			copy_project_file = zip_archive.write

			# Define function to work around inconsistent API
			def create_project_file(stringio_obj, file_target):
				zip_archive.writestr(file_target, stringio_obj.getvalue())
		else:
			def copy_project_file(file_source, file_target):
				# Copy to project directory
				file_target = os.path.join(self._get_project_output_dir(), file_target)

				# Make sure directories are created
				if not os.path.exists(os.path.dirname(file_target)):
					os.makedirs(os.path.dirname(file_target))

				# Copy file
				shutil.copy(file_source, file_target)

			def create_project_file(stringio_obj, file_target):
				# Write to project directory
				file_target = os.path.join(self._get_project_output_dir(), file_target)

				# Make sure directories are created
				if not os.path.exists(os.path.dirname(file_target)):
					os.makedirs(os.path.dirname(file_target))

				# Write content to file
				self._write_file(file_target, stringio_obj.getvalue())

		# Copy all the project's files
		for file_source, file_target in self.project_compiler_files_dict.items() + self.project_distribute_files_dict.items() + extra_files:
			copy_project_file(file_source, file_target)

		# Output the generated files
		for file_content, file_target in extra_content:
			create_project_file(file_content, file_target)

		if not self.runtime.no_zip:
			# Close ZIP archive
			zip_archive.close()
			self.project.filelist.add(zip_archive_name)

		# Output generated files if debugging
		if self.runtime.debug:
			asf_header_name = self._get_asf_header_name()
			cproj_name = self._get_avrgccproj_name()
			vstemplate_name = self._get_vstemplate_name()

			self._write_file(asf_header_name, asf_header_content.getvalue())
			cproj_tree.write(cproj_name)

			if self.make_vstemplate:
				vstemplate_tree.write(vstemplate_name)

		self.log.info("Finished exporting AVR Studio 5 project")


class AVRStudio5Project32(AVRStudio5Project):
	generator_tag = 'as5_32'
	toolchain = 'avr32gcc'
	toolchain_tag = 'Avr32Gcc'
	toolchain_name = 'com.Atmel.AVRGCC32'
	vsproject_to_import = r'$(AVRSTUDIO_EXE_PATH)\\Vs\\AvrGCC32.targets'

	config_startuplabel = 'startup_label'
	config_stack = 'stack_size'
	config_heap = 'heap_size'
	# This config is not in total_config -- get it from the project
	config_trampoline = 'config.avr32.utils.trampoline'

	# --- Start temporary workaround for AVR Studio bug 14500. TODO: Remove
	def _get_toolchain_assembler_flag_key(self):
		return ["preprocessingassembler.general.AssemblerFlags"]
	# -- End temporary workaround

	def _get_avrgccproj_compiler_flags(self, config):
		"""
		Get a dictionary which maps from the other compiler flags tag to
		the text content which is given by the default flags and any
		flags that are set in the project.
		"""
		tag = 'compiler.miscellaneous.OtherFlags'
		flags = '-std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -mno-cond-exec-before-reload'

		for flag in self.project.get_build(BuildCCompilerFlags, self.toolchain):
			flags += ' ' + flag.strip()

		return {tag : self.sort_flags(flags)}


	def _get_linker_script(self):
		selector_id = 'avr32.utils.linker_scripts'
		return GenericProject._get_linker_script(self, selector_id)


	def _get_selected_device(self):
		mcu = self.project.mcu.name

		if re.match('unspecified.*', mcu):
			return ''
		else:
			# Format the MCU name, should be all capital letters
			return mcu.upper()

	def _get_avrgccproj_includepath_tags(self):
		tags = [
			'compiler.directories.IncludePaths',
			'assembler.general.IncludePaths',
			'preprocessingassembler.general.IncludePaths',
		]

		return tags


	def _get_avrgccproj_define_tags(self):
		tags = [
			'compiler.symbols.DefSymbols'
		]

		return tags


	def _get_avrgccproj_default_toolchain_settings(self, configuration):
		common_defaults = {
			'assembler.general.AssemblerFlags' :
				'-mrelax',
			'compiler.optimization.DebugLevel' :
				'None',
			'compiler.optimization.OtherFlags' :
				'-fdata-sections',
			'compiler.optimization.PrepareFunctionsForGarbageCollection' :
				'True',
			'compiler.warnings.AllWarnings' :
				'True',
			'linker.optimization.GarbageCollectUnusedSections' :
				'True',
			'preprocessingassembler.general.AssemblerFlags' :
				'-mrelax',
		}

		debug_defaults = {
			'compiler.optimization.DebugLevel' :
				'Maximum (-g3)',
		}

		release_defaults = {
			'compiler.optimization.DebugLevel' :
				'None',
		}

		# Build the default dictionary according to the specific project config
		defaults = common_defaults
		if configuration.lower() == 'release':
			defaults.update(release_defaults)
		elif configuration.lower() == 'debug':
			defaults.update(debug_defaults)
		else:
			raise Exception("Unknown project configuration `%s'" % configuration)

		return defaults


	def _get_avrgccproj_stdlib_settings(self, config):
		tag_to_text = {}
		tag_prefix = 'linker.general.'

		# Read out configuration for standard library and startup file usage
		# Default settings: use std. libraries only -- not the startup files
		use_stdlib = config.get(self.config_use_stdlib, self.config_enabled)
		use_deflibs = config.get(self.config_use_deflibs, self.config_enabled)
		use_startfiles = config.get(self.config_use_startfiles, self.config_disabled)

		# The use_stdlib config overrides the other two config options
		if use_stdlib == self.config_disabled:
			tag_to_text[tag_prefix + 'NoStartupOrDefaultLibs'] = 'True'
		else:
			tag_to_text[tag_prefix + 'NoStartupOrDefaultLibs'] = 'False'

			if use_deflibs == self.config_disabled:
				tag_to_text[tag_prefix + 'DoNotUseDefaultLibraries'] = 'True'
			if use_startfiles == self.config_disabled:
				tag_to_text[tag_prefix + 'DoNotUseStandardStartFiles'] = 'True'

		return tag_to_text


	def _get_avrgccproj_arch_specific_linker_flags(self, config):
		text = ' -Wl,--relax'

		# Get configuration affecting the startup-label
		startup_label = config.get(self.config_startuplabel, None)
		use_trampoline = self.get_config(self.config_trampoline)
		if use_trampoline is None:
			use_trampoline = self.config_enabled

		# Now get the label to set based on the configuration
		label_to_set = None
		if startup_label is None:
			if use_trampoline == self.config_enabled:
				label_to_set = '_trampoline'
		else:
			label_to_set = startup_label

		if label_to_set is not None:
			text += ' -Wl,-e,' + label_to_set

		# Handle stack and heap size configuration
		stack_size = config.get(self.config_stack, None)
		if stack_size is not None:
			text += ' -Wl,--defsym,__stack_size__=%s' % stack_size

		heap_size = config.get(self.config_heap, None)
		if heap_size is not None:
			text += ' -Wl,--defsym,__heap_size__=%s' % heap_size

		return text


class AVRStudio5ProjectARM(AVRStudio5Project):
	generator_tag = 'as5_arm'
	toolchain = 'armgcc'
	toolchain_tag = 'ArmGcc'
	toolchain_name = 'com.Atmel.ARMGCC'
	vsproject_to_import = r'$(AVRSTUDIO_EXE_PATH)\\Vs\\Compiler.targets'

	config_startuplabel = 'startup_label'
	config_stack = 'stack_size'
	config_heap = 'heap_size'

	# --- Start temporary workaround for AVR Studio bug 14500. TODO: Remove
	def _get_toolchain_assembler_flag_key(self):
		return ["preprocessingassembler.general.AssemblerFlags"]
	# -- End temporary workaround

	def _get_linker_script(self):
		selector_id = 'sam.utils.linker_scripts'
		return GenericProject._get_linker_script(self, selector_id)


	def _get_product_line(self):
		return 'sam'


	def _get_selected_device(self):
		mcu = self.project.mcu.name

		if re.match('unspecified.*', mcu):
			return ''
		else:
			# Format the MCU name, should be all capital letters
			return mcu.upper()


	def _get_avrgccproj_includepath_tags(self):
		tags = [
			'compiler.directories.IncludePaths',
			'assembler.general.IncludePaths',
			'preprocessingassembler.general.IncludePaths',
		]

		return tags

	def _get_avrgccproj_compiler_flags(self, config):
		"""
		Get a dictionary which maps from the other compiler flags tag to
		the text content which is given by the default flags and any
		flags that are set in the project.
		"""
		tag = 'compiler.miscellaneous.OtherFlags'
		flags = '-pipe -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -std=gnu99 -ffunction-sections -fdata-sections -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int -Wmain -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow -Wbad-function-cast -Wwrite-strings -Wsign-compare -Waggregate-return  -Wmissing-declarations -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long -Wunreachable-code -Wcast-align --param max-inline-insns-single=500 -Dprintf=iprintf'

		for flag in self.project.get_build(BuildCCompilerFlags, self.toolchain):
			flags += ' ' + flag.strip()

		return {tag : flags}

	def _get_avrgccproj_define_tags(self):
		tags = [
			'compiler.symbols.DefSymbols'
		]

		return tags


	def _get_avrgccproj_default_toolchain_settings(self, configuration):
		common_defaults = {
			'compiler.optimization.DebugLevel' :
				'None',
			'compiler.optimization.OtherFlags' :
				'-fdata-sections',
			'compiler.optimization.PrepareFunctionsForGarbageCollection' :
				'True',
			'compiler.warnings.AllWarnings' :
				'True',
			'linker.optimization.GarbageCollectUnusedSections' :
				'True',
			'preprocessingassembler.general.AssemblerFlags' :
				'',
		}

		debug_defaults = {
			'compiler.optimization.DebugLevel' :
				'Maximum (-g3)',
		}

		release_defaults = {
			'compiler.optimization.DebugLevel' :
				'None',
		}

		# Build the default dictionary according to the specific project config
		defaults = common_defaults
		if configuration.lower() == 'release':
			defaults.update(release_defaults)
		elif configuration.lower() == 'debug':
			defaults.update(debug_defaults)
		else:
			raise Exception("Unknown project configuration `%s'" % configuration)

		return defaults

	def _get_avrgccproj_arch_specific_linker_flags(self, config):
		text = ' -Wl,--entry=Reset_Handler -Wl,--cref -mthumb '

		# Get configuration affecting the startup-label
		startup_label = config.get(self.config_startuplabel, None)

		# Now get the label to set based on the configuration
		label_to_set = None
		if startup_label:
			label_to_set = startup_label

		if label_to_set is not None:
			text += ' -Wl,-e,' + label_to_set

		# Handle stack and heap size configuration
		stack_size = config.get(self.config_stack, None)
		if stack_size is not None:
			text += ' -Wl,--defsym,__stack_size__=%s' % stack_size

		heap_size = config.get(self.config_heap, None)
		if heap_size is not None:
			text += ' -Wl,--defsym,__heap_size__=%s' % heap_size

		return text


class AVRStudio5ProjectTemplate(AVRStudio5Project):
	generator_tag = 'as5_8_template'

class AVRStudio5Project32Template(AVRStudio5Project32):
	generator_tag = 'as5_32_template'

class AVRStudio5ProjectARMTemplate(AVRStudio5ProjectARM):
	generator_tag = 'as5_arm_template'
