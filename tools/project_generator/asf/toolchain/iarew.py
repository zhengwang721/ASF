
from asf.toolchain.generic import GenericProject
from asf.database import *
import asf.etreeabstraction as ET
import asf.helper
import StringIO

class OptionItem(object):
	def __init__(self):
		pass

	def get_option(self, params):
		raise Exception("Abstract class method called")


class SimpleLookupOption(OptionItem):
	def __init__(self, name, states, param, ver=None):
		self.name = name
		self.states = states
		self.param = param
		self.ver = ver

	def get_option(self, params):
		arg = params.get(self.param, None)
		if not arg:
			raise ConfigError("No parameter `%s' for IAREW option with name `%s'" % (self.param, self.name))
		elif arg not in self.states:
			raise ConfigError("Parameter `%s' has invalid value `%s' for IAREW option with name `%s'" % (self.param, arg, self.name))

		option = {
				'name'    : self.name,
				'state'   : self.states[arg],
				'version' : self.ver,
			}

		return option

class SimplePassthroughOption(OptionItem):
	def __init__(self, name, param, ver=None):
		self.name = name
		self.param = param
		self.ver = ver

	def get_option(self, params):
		arg = params.get(self.param, None)

		option = {
				'name'    : self.name,
				'state'   : arg,
				'version' : self.ver,
			}

		return option

class DualInsertionOption(OptionItem):

	def __init__(self, name, insert_string0, insert_string1, param0, param1, ver=None):
		self.name = name
		self.insert_string0 = insert_string0
		self.insert_string1 = insert_string1
		self.param0 = param0
		self.param1 = param1
		self.ver = ver

	def get_option(self, params):
		state = ''
		arg0 = params.get(self.param0, None)
		arg1 = params.get(self.param1, None)
		if (arg0 is not None):
			state += self.insert_string0 % arg0
			if (arg1 is not None):
				state += ','
		if (arg1 is not None):
			state += self.insert_string1 % arg1
		option = {
				'name'    : self.name,
				'state'   : state,
				'version' : self.ver,
				}
		return option

class ComplexFunctionOption(OptionItem):
	"""
	Option type which allows for a function with an arbitrary number of
	parameters to be called, with its arguments generated from the
	parameters dictionary that is passed to it.

	This option type must be initialized with a function and a list of
	the parameters to get. Note that the order of the parameter list must
	match the actual parameter order of the function.

	Example:
		def foo(use_foo, use_bar):
			...

		foo_option = ComplexFunctionOption(foo, ['use_foo_config', 'use_bar_config'])

		parameters = {
			'use_foo_config' : 'no',
			'use_bar_config' : 'yes',
		}

		foo_option.get_option(parameters)

	Will cause a call to:
		foo('no', 'yes')
	"""
	def __init__(self, function, param_list):
		if not isinstance(param_list, list):
			raise Exception("List of parameters must be specified for IAREW option with function %s" % function.__name__)

		self.function = function
		self.param_list = param_list

	def get_option(self, params):
		arg_list = []

		for param in self.param_list:
			arg_list.append(params.get(param))

		option = self.function(*arg_list)

		return option

class OptionSet(object):
	def __init__(self):
		self.the_set = []

	def add_option(self, option_item):
		if not isinstance(option_item, OptionItem):
			raise TypeError("`%s' must be a subclass of %s" % ('option_item', 'OptionItem'))
		else:
			self.the_set.append(option_item)

	def get_options(self, parameters):
		option_dict = []

		for option in self.the_set:
			new_option = option.get_option(parameters)
			if new_option:
				option_dict.append(new_option)

		return option_dict


class IAREWProject(GenericProject):
	"""
	Abstract class with common functions for the generators for the various
	versions of IAR Embedded Workbench, and a method for finding the correct
	generator class for a given project.

	A parent class for each device family should be created and based on
	this class, and actual generator classes be based on those parent
	classes.

	For example, IAREW8Project is the parent class for 8-bit AVR generators
	and IAREW32Project for 32-bit AVR generators.

	The generators must have an attribute named "supported_mcus", which is a
	list of supported MCUs and MCU groups, and one name "toolchain_version",
	which is a "filename-safe" string with the toolchain version.
	"""
	optlevel_default = 'size'
	create_aux_default = GenericProject.config_disabled
	debugger_use_flash_loader_default = GenericProject.config_enabled
	unhandled_build_types = [BuildLinkerFlags, BuildBuildConfig]

	def __init__(self, project, db, runtime):
		if type(self) == IAREWProject:
			raise Exception("Abstract class %s instantiated" % self.__class__.__name__)
		else:
			super(IAREWProject, self).__init__(project, db, runtime)

			self.linker_script = ""
			self.buildc_dict = {}
			self.builda_dict = {}
			self.buildh_list = []
			self.include_path_list = []
			self.buildlib_dict = {}
			self.define_list = []
			self.project_group_dict = {}
		
		self.create_aux = self._get_total_config().get('create_aux', self.create_aux_default)
		self.debugger_use_flash_loader = self._get_total_config().get('debugger_use_flash_loader', self.debugger_use_flash_loader_default)

		self.create_aux = self._get_total_config().get('create_aux', self.create_aux_default)
		self.debugger_use_flash_loader = self._get_total_config().get('debugger_use_flash_loader', self.debugger_use_flash_loader_default)


	@staticmethod
	def get_generator(project, db, runtime, baseclass):
		# Find the project MCU search list
		mcu = project.mcu
		search_list = mcu.get_group_map()

		# Search recursively in subclasses for generator with support
		# for current MCU, and return an instance if it was found
		for childclass in baseclass.__subclasses__():
			# Check mcu support of child class if it is defined
			try:
				supported_mcus = getattr(childclass, 'supported_mcus')
			except AttributeError:
				pass
			else:
				for term in search_list:
					if term in supported_mcus:
						return childclass(project, db, runtime)

			# Check subclasses
			try:
				return IAREWProject.get_generator(project, db, runtime, childclass)
			# Catch exceptions so we can search in the rest of the child classes
			except GeneratorError:
				pass

		# No class found
		raise GeneratorError("Could not find IAR project generator with support for device `%s'" % mcu.name)


	def _get_optimization_params(self, compiler_config):
		optlevel = compiler_config.get('optimization', self.optlevel_default)
		if optlevel in self.optlevel_to_arglist:
			arglist = self.optlevel_to_arglist[optlevel]
		else:
			raise ConfigError("Project `%s' has unknown optimization level `%s' for toolchain `%s'" % (self.project.id, optlevel, self.toolchain))
		params = dict(zip(self.optlevel_paramlist, arglist))

		# Handle multi-file compilation settings
		mfc_params = [compiler_config.get(param) for param in self.mfc_paramlist]
		mfc_settings = dict(zip(self.mfc_paramlist, mfc_params))
		# Add the root element of the current project to params for the multi-file compilation option
		mfc_settings['root_element'] = self.project_root_element

		params.update(mfc_settings)

		return params


	# Multi-file compilation settings
	mfc_paramlist = ['root_element', 'enable_mfc', 'mfc_both_configs', 'discard_unused_publics']


	def _set_multi_file_compilation(self, root_element, enable_mfc, mfc_both_configs, discard_unused_publics):
		"""
		Configure multi-file compilation (MFC) of the project, based on
		the supplied arguments.

		By default, MFC is disabled, and is only enabled for the Release
		configuration unless it is specified that it should be enabled
		for both project configurations.
		With MFC enabled, it is also possible to discard unused public
		functions and variables, though this is disabled by default.
		"""
		if enable_mfc == self.config_enabled:
			if discard_unused_publics == self.config_enabled:
				tag = 'mfc_discard'
			else:
				tag = 'mfc'

			mfc_e = ET.SubElement(self.project_root_element, tag)

			config_list = ['Release']
			if mfc_both_configs == self.config_enabled:
				config_list.append('Debug')

			for config in config_list:
				conf_e = ET.SubElement(mfc_e, 'configuration')
				conf_e.text = config


	def _get_common_option_set(self):
		option_set = OptionSet()

		option_set.add_option(
			ComplexFunctionOption(
				self._set_multi_file_compilation,
				self.mfc_paramlist
			)
		)

		return option_set


	def _get_compiler_settings(self):
		params = {}

		option_set = self._get_option_set()

		# Get generic, toolchain and "total" configuration
		# Toolchain-specific configs override generic configs
		generic_config = self._get_compiler_config()
		toolchain_config = self._get_toolchain_config()
		total_config = generic_config
		total_config.update(toolchain_config)
		params = self._get_arch_specific_params(total_config)
		params.update(self._get_optimization_params(total_config))
		options = option_set.get_options(params)
		return options

	def _find_template_file(self, extension, toolchain=None, toolchain_version=None):
		toolchain = toolchain or self.toolchain
		toolchain_version = toolchain_version or self.toolchain_version

		# Try mcu-specific template filename first, then the generic
		basename = "%s_v%s_%s.%s" % (toolchain, toolchain_version, "%s", extension)
		filename_list = [
			basename % self.project.mcu.name,
			basename % "generic",
		]

		for filename in filename_list:
			path = os.path.join(self.templatedir, filename)
			if os.path.exists(path):
				return path

		raise Exception("Generator `%s' is missing a template file, tried: %s" % (self.__class__.__name__, ", ".join(filename_list)))

	def _get_project_template(self):
		return self._find_template_file('ewp')

	def _get_workspace_template(self):
		path = os.path.join(self.templatedir, 'iarew.eww')
		return path

	def _get_debuggerfile_template(self):
		return self._find_template_file('ewd')

	def _get_project_file_dir(self):
		return os.path.join(self.project.basedir, "iar")

	def _get_project_name(self):
		return self.get_workspace_name()

	def _get_project_name_suffix(self):
		return ''

	def _get_aux_project_name_suffix(self):
		return ''

	def _get_workspacefile_path(self):
		return os.path.join(self._get_project_file_dir(), self._get_project_name() + ".eww")

	def _get_projectfile_path(self):
		return os.path.join(self._get_project_file_dir(), self._get_project_name()  + self._get_project_name_suffix() + ".ewp")

	def _get_aux_projectfile_path(self):
		return os.path.join(self._get_project_file_dir(), self._get_project_name() + self._get_aux_project_name_suffix()  + ".ewp")

	def _get_debuggerfile_path(self):
		return os.path.join(self._get_project_file_dir(), self._get_project_name() + self._get_project_name_suffix() + ".ewd")

	def _get_aux_debuggerfile_path(self):
		return os.path.join(self._get_project_file_dir(), self._get_project_name() + self._get_aux_project_name_suffix()  + ".ewd")

	def _get_asf_header_path(self):
		return os.path.join(self._get_project_file_dir(), "asf.h")

	def get_generated_files_list(self):
		return [self._get_workspacefile_path(), self._get_projectfile_path(), self._get_debuggerfile_path(), self._get_asf_header_path()]

	def get_generated_dirs_list(self):
		return [self._get_project_file_dir()]


	def _add_proj_dir(self, path):
		return '$PROJ_DIR$' + os.sep + path

	# Function _insert_project_element for dummies:
	# XML-tree before function call:
	# <workspace>
	#   <batchBuild/>
	# </workspace>
	#  _insert_project_element ('MyProject')
	# This is the result
	# <workspace>
	#   <project>
	#     <path>MyProject</path>
	#   </project>
	#   <batchBuild/>
	# </workspace>
	def _insert_project_element(self, root_element, project_path):
		"""
		Function _insert_project_element for dummies:
		XML-tree before function call:
		<workspace>
		  <batchBuild/>
		</workspace>
		 _insert_project_element ('MyProject')
		This is the result
		<workspace>
		  <project>
			<path>MyProject</path>
		  </project>
		  <batchBuild/>
		</workspace>
		"""
		# add new element
		project_element = ET.SubElement(root_element, 'project')
		path_element = ET.SubElement(project_element, 'path')
		path_element.text = project_path
	# Function _insert_new_elements for dummies:
	#
	# XML-tree before function call:
	# <xml>
	#   <level1>
	#     <level2>
	#       <my_tag>Add elements here</my_tag>
	#     <level2>
	#   <level1>
	# </xml>
	#
	# _insert_new_elements("level1/level2/my_tag", "Add elements here", ["inserted_level1", "inserted_level2"],
	#	["This is a inserted element", "This is another inserted element"])
	#
	# This is the result.
	#
	# <xml>
	#   <level1>
	#     <level2>
	#       <my_tag>Add elements here</my_tag>
	#       <inserted_level1>
	#         <inserted_level2>This is a inserted element</inserted_level2>
	#       </inserted_level1>
	#       <inserted_level1>
	#         <inserted_level2>This is another inserted element</inserted_level2>
	#       </inserted_level1>
	#     <level2>
	#   <level1>
	# </xml>
	#
	# If you set remove_old to True, all old elements at "level1/level2/my_tag" with tags equal to "inserted_level1"
	# will be removed before adding new.
	#
	# If you have multiple elements at "level1/level2/my_tag" with text maching "Add elements here", elements will be
	# added at all instances, unless insert_once is set to True, then it's only added at first instance found.
	#
	def _insert_new_elements(self, root_element, element_location, element_text, new_element_tags, new_element_text_values, remove_old=True, insert_once=False):
		# find all sub-elements at element_location
		for name_element in root_element.findall(element_location):
			# find all elements with given element_text
			if name_element.text.strip() == element_text:
				# Remember parent element
				parent_element = name_element.find("..")
				previous_element = parent_element
				# Remove existing sub-element if wanted
				if remove_old == True:
					existing_element = parent_element.find(new_element_tags[0])
					if existing_element != None:
						parent_element.remove(existing_element)
				# Insert all new text elements
				for new_text in new_element_text_values:
					# Insert all new sub-levels
					for index, new_tag in enumerate(new_element_tags):
						# add new element
						element = ET.SubElement(previous_element, new_tag)
						if index == (len(new_element_tags)-1):
							# At deepest level, insert text
							element.text = new_text
						# insert next sub-element below the one we just added
						previous_element = element
					# Done iterating all levels, start on next tag, insert first below parent
					previous_element = parent_element
				if insert_once == True:
					# We only want to insert our new elements once, so say bye-bye
					return

	def _insert_option_elements(self, root_element, name_value, element_tag, element_text_values, remove_old=True):
		self._insert_new_elements(root_element, "configuration/settings/data/option/name", name_value, [element_tag], element_text_values, remove_old)

	def _insert_state_elements(self, root_element, name_value, element_text_values, remove_old=True):
		self._insert_option_elements(root_element, name_value, "state", element_text_values, remove_old)

	def _insert_single_state_element(self, root_element, name_value, value):
		self._insert_state_elements(root_element, name_value, [value])

	def _insert_single_version_element(self, root_element, name_value, value):
		self._insert_option_elements(root_element, name_value, "version", [value])

	def _insert_file_elements(self, root_element, file_dict):
		"""
		Add files in the supplied dictionary to the project, with
		grouping that corresponds to the files' paths.

		The keys and values of the dictionary are expected to be the
		file name in the IAR project and the file name in ASF,
		respectively.

		For example, this dictionary:
			a = {
				'$PROJ_DIR$\\..\\bar.c' : 'avr32\\foo\\bar.c',
				'$PROJ_DIR$\\..\\foo.c' : 'avr32\\foo\\foo.c',
				}
		...will be added to the project explorer, grouped as:
			+ avr32
			  + foo
			    | bar.c
			    | foo.c
		"""
		for (proj_file, asf_file) in file_dict.items():
			asf_dir = os.path.dirname(asf_file)
			asf_dir_list = []

			# Peel away the directories, looking for the lowest
			# level group which has been added to the project and
			# making a list of the missing groups to add as we go.
			while asf_dir and (asf_dir not in self.project_group_dict):
				(asf_dir, missing_dir) = os.path.split(asf_dir)
				asf_dir_list.append(missing_dir)

			# Get the root element to start adding the new groups to
			# -- this is either a project group or the root element,
			# depending on whether any of the groups have been added
			# and if any groups are needed at all.
			if asf_dir:
				container_e = self.project_group_dict[asf_dir]
			else:
				container_e = root_element

			# Generate the new sub group elements, adding them as
			# sub-elements to existing groups, and also adding them
			# to the object's group dictionary at the same time.
			while asf_dir_list:
				new_group_name = asf_dir_list.pop()
				asf_dir = os.path.join(asf_dir, new_group_name)
				container_e = ET.SubElement(container_e, "group")
				name_e = ET.SubElement(container_e, "name")
				name_e.text = new_group_name
				self.project_group_dict[asf_dir] = container_e

			# Add the file to the last container element.
			file_e = ET.SubElement(container_e, "file")
			name_e = ET.SubElement(file_e, "name")
			name_e.text = proj_file

	def _insert_single_file_element(self, root_element, file_dict):
		self._insert_file_elements(root_element, [file_dict])

	def _update_project_xml(self, mcu):
		self._update_arch_specific_project_xml(mcu)

		self._insert_file_elements(self.project_root_element, self.buildlib_dict)
		self._insert_file_elements(self.project_root_element, self.buildc_dict)
		self._insert_file_elements(self.project_root_element, self.builda_dict)

		# Add the compiler settings
		custom_params = self._get_compiler_settings()
		for param in custom_params:
			# Only update a setting if a state is set
			if param['state'] != None:
				# Set <version> (if any)
				if param['version'] != None:
					self._insert_single_version_element(self.project_root_element, param['name'], param['version'])
				# Set <state>
				self._insert_single_state_element(self.project_root_element, param['name'], param['state'])

	def _update_aux_project_xml(self, mcu):
		self._update_arch_specific_aux_project_xml(mcu)

	def _aux_project_is_enabled(self):
		"""
		Check if auxillary project is enabled.
		Returns by default False as auxillary projects are not used for standard project generation.
		"""
		return self.create_aux == self.config_enabled

	def _parse_iar_file(self,file):
			# Parse Iar file and return root element
			tree = ET.parse(file)
			return tree

	def _write_iar_file(self, iar_file_name, iar_file_tree, iar_file_root, log_string):
			# Write into IAR file
			iar_file_content = StringIO.StringIO()
			self.project.filelist.add(iar_file_name)
			asf.helper.indent_xml(iar_file_root)
			iar_file_tree.write_c14n(iar_file_content)
			self._write_file(iar_file_name, iar_file_content.getvalue())
			self.log.info(log_string + iar_file_name)
			iar_file_content.close()

	def write(self):
		# Generate variables to use in project file
		projectfile_dir = self._get_project_file_dir()

		asf_mcu   = self.project.mcu.name
		asf_root  = os.path.relpath(os.curdir, projectfile_dir)

		self._check_for_config_headers()

		# Get linker script
		try:
			(file, file_origin) = self._get_linker_script(self.linker_id)
			self.project.filelist.add(file)
			file = os.path.relpath(file, projectfile_dir)
			self.linker_script = self._add_proj_dir(file)
		except:
			pass

		# Check for flags for linker, compiler, assembler or preprocessor
		for module in [self.project] + self.project.get_prerequisites(recursive=True):
			for build_type in self.unhandled_build_types:
				unhandled_items = module.get_build(build_type, self.toolchain, recursive=False)
				if len(unhandled_items) > 0:
					raise ConfigError("Module `%s' has build item of type `%s' and subtype `%s' which is not supported for toolchain `%s'" % (module.id, build_type.type, build_type.subtype, self.toolchain))

		# Create list of C files
		self.buildc_dict = {}
		for asf_file in self.project.get_build(BuildC, self.toolchain):
			self.project.filelist.add(asf_file)
			project_file = os.path.relpath(asf_file, projectfile_dir)
			self.buildc_dict[self._add_proj_dir(project_file)] = asf_file

		# Create list of assembly files
		self.builda_dict = {}
		for asf_file in self.project.get_build(BuildAssembly, self.toolchain):
			self.project.filelist.add(asf_file)
			project_file = os.path.relpath(asf_file, projectfile_dir)
			self.builda_dict[self._add_proj_dir(project_file)] = asf_file

		# Create list of header files
		self.buildh_list = []
		for file in self.project.get_build(BuildHeader, self.toolchain) \
				+ [self._get_asf_header_path()]:
			self.project.filelist.add(file)
			file = os.path.relpath(file, projectfile_dir)
			self.buildh_list.append(self._add_proj_dir(file))

		# Create list of include paths
		self.include_path_list = []
		for file in self.project.get_build(BuildInclude, self.toolchain) \
				+ [os.path.dirname(self._get_asf_header_path())]:
			file = os.path.relpath(file, projectfile_dir)
			self.include_path_list.append(self._add_proj_dir(file))

		# Create list of libs
		self.buildlib_dict = {}
		libs = self.project.get_build(BuildUserLibrary, self.toolchain)
		libs.sort()
		for asf_file in libs:
			self.project.filelist.add(asf_file)
			project_file = os.path.relpath(asf_file, projectfile_dir)
			self.buildlib_dict[self._add_proj_dir(project_file)] = asf_file

		# Create list of definitions
		defines = self.project.get_build(BuildDefine, self.toolchain)
		self.define_list = []
		if not defines is None:
			for definition, value in defines:
				if value:
					self.define_list.append("%s=%s" % (definition, value))
				else:
					self.define_list.append(definition)

		if not os.path.exists(projectfile_dir):
			os.makedirs(projectfile_dir)

		# Add files for distribution to filelist
		for f in self.project.get_build(BuildDistributeFile, self.toolchain) + \
				self.project.get_build(BuildDistributeUserFile, self.toolchain):
			self.project.filelist.add(f)
		for d in self.project.get_build(BuildDistributeDirectory, self.toolchain):
			self._add_dir_to_filelist(d)

		# Create asf.h
		asf_h_path = self._get_asf_header_path()
		asf_h = self._generate_asf_header_content()
		self._write_file(asf_h_path, asf_h)

		# Parse and update project template
		project_tree = self._parse_iar_file(self._get_project_template())
		self.project_root_element = project_tree.getroot()

		# Parse and update debugger file template
		debuggerfile_tree = self._parse_iar_file(self._get_debuggerfile_template())
		self.debuggerfile_root_element = debuggerfile_tree.getroot()

		# Parse and update workspace template
		workspace_tree = self._parse_iar_file(self._get_workspace_template())
		self.workspace_root_element = workspace_tree.getroot()

		# Get auxiliary linker script if the option is enabled
		if self._aux_project_is_enabled():
			# Get auxiliary linker script
			try:
				(file, file_origin) = self._get_aux_linker_script(self.linker_id)
				self.project.filelist.add(file)
				file = os.path.relpath(file, projectfile_dir)
				self.aux_linker_script = self._add_proj_dir(file)
			except:
				pass

		# Update XML contents: project file, debugger file and workspace file.
		self._update_project_xml(asf_mcu)

		# Create project file
		projectfile_dir = os.path.join(self.project.basedir, "iar")
		self._write_iar_file(self._get_projectfile_path(), project_tree, self.project_root_element, "IAR project written: ")
		# Create debugger file file
		self._write_iar_file(self._get_debuggerfile_path(), debuggerfile_tree, self.debuggerfile_root_element, "IAR debugger file written: ")

		if self._aux_project_is_enabled():
			# Update AUX XML contents: auxiliary project file, debugger file and workspace file if the option is enabled.
			self._update_aux_project_xml(asf_mcu)
			self._write_iar_file(self._get_aux_debuggerfile_path(), debuggerfile_tree, self.debuggerfile_root_element, "IAR AUX debugger file written: ")
			self._write_iar_file(self._get_aux_projectfile_path(), project_tree, self.project_root_element, "IAR AUX project written: ")

		# Create workspace file
		self._write_iar_file(self._get_workspacefile_path(), workspace_tree, self.workspace_root_element, "IAR workspace file written: ")

class IAREW8Project(IAREWProject):
	"""
	Parent class for all IAR EW generators for 8-bit AVR.

	Contains functions that are common to all versions of the toolchain/IDE.
	"""
	toolchain = "iarew"


	@staticmethod
	def get_generator(project, db, runtime):
		return IAREWProject.get_generator(project, db, runtime, IAREW8Project)


	def __init__(self, project, db, runtime):
		if type(self) == IAREW8Project:
			raise Exception("Abstract class %s instantiated" % self.__class__.__name__)
		else:
			super(IAREW8Project, self).__init__(project, db, runtime)


	def _get_arch_specific_params(self, config):
		params = {}

		# DLIB configuration
		dlib_conf = config.get('dlib', 'normal')
		params['dlib_config_path'] = dlib_conf
		params['dlib_path'] = dlib_conf
		params['dlib_desc'] = dlib_conf
		params['combo_select'] = dlib_conf
		params['combo_select_slave'] = dlib_conf

		# Setup stack and heap size
		near_heap_size = config.get('near_heap_size', None)
		params['near_heap_size'] = near_heap_size

		far_heap_size = config.get('far_heap_size', None)
		params['far_heap_size'] = far_heap_size

		huge_heap_size = config.get('huge_heap_size', None)
		params['huge_heap_size'] = huge_heap_size

		data_stack_size = config.get('data_stack_size', None)
		params['data_stack_size'] = data_stack_size

		return_stack_size = config.get('return_stack_size', None)
		params['return_stack_size'] = return_stack_size

		# New options in v610
		params['unsigned_char'] = config.get('unsigned_char', 'yes')
		params['fp_semantics'] = config.get('fp_semantics', 'strict')

		# System configuration
		# Enable Configure system using dialogs (not in .XCL file)
		params['enable_config'] = config.get('enable_config', 'yes')

		return params


class IAREW32Project(IAREWProject):
	"""
	Parent class for all IAR EW generators for 32-bit AVR.

	Contains functions that are common to all versions of the toolchain/IDE.
	"""
	toolchain = "iarew32"


	@staticmethod
	def get_generator(project, db, runtime):
		return IAREWProject.get_generator(project, db, runtime, IAREW32Project)


	def __init__(self, project, db, runtime):
		if type(self) == IAREW32Project:
			raise Exception("Abstract class %s instantiated" % self.__class__.__name__)
		else:
			super(IAREW32Project, self).__init__(project, db, runtime)


	def _get_arch_specific_params(self, config):
		params = {}

		# If INTC driver is used, IAR should by default ignore unhandled
		# interrupts and exceptions
		conf_keys = [
			'catch_unimplemented_interrupts',
			'catch_unhandled_cpu_exceptions',
		]

		module_ids = self.project.get_prerequisite_ids()

		if 'avr32.drivers.intc' in module_ids:
			default_setting = 'no'
		else:
			default_setting = 'yes'

		for key in conf_keys:
			if key not in config:
				params[key] = default_setting
			else:
				params[key] = config[key]

		# DLIB configuration 
		dlib_conf = config.get('dlib', 'normal')
		params['dlib_config_path'] = dlib_conf
		params['dlib_path'] = dlib_conf
		params['dlib_desc'] = dlib_conf
		params['combo_select'] = dlib_conf
		params['combo_select_slave'] = dlib_conf

		# Setup stack and heap size
		heap_size = config.get('heap_size', None)
		params['heap_size'] = heap_size

		sys_stack_size = config.get('sys_stack_size', None)
		params['sys_stack_size'] = sys_stack_size

		app_stack_size = config.get('app_stack_size', None)
		params['app_stack_size'] = app_stack_size

		# Handle trampoline / program entry configuration
		# If a startup-label is set, it will override IAR's default
		# program entry point. If a startup-label is not set, and the
		# project is configured to use the trampoline, the startup-label
		# is set to __trampoline -- otherwise it is set to the the
		# default for IAR: __program_start.
		use_trampoline = self.configuration.get_config('config.avr32.utils.trampoline')
		if not use_trampoline:
			use_trampoline = self.db.lookup_by_id('config.avr32.utils.trampoline').default

		startup_label = config.get('startup_label', None)

		if startup_label is None:
			if use_trampoline == 'yes':
				startup_label = '__trampoline'
			else:
				startup_label = '__program_start'

		params['override_program_entry'] = use_trampoline
		params['program_entry_label'] = startup_label

		return params

# --- Actual generator implementations below here ---

class IAREW8Project_v550(IAREW8Project):
	toolchain_version = "550"
	supported_mcus = [
		# Mega devices -- these use a generic template
		'atmega128',
		'atmega128a',
		'atmega128rfa1',
		'atmega1284',
		'atmega1284p',
		'atmega16',
		'atmega16a',
		'atmega168',
		'atmega168a',
		'atmega168p',
		'atmega168pa',
		'atmega169',
		'atmega169p',
		'atmega169pa',
		'atmega2560',
		'atmega32',
		'atmega32a',
		'atmega324a',
		'atmega324p',
		'atmega324p',
		'atmega329',
		'atmega329p',
		'atmega329pa',
		'atmega48',
		'atmega48a',
		'atmega48p',
		'atmega48pa',
		'atmega64',
		'atmega64a',
		'atmega640',
		'atmega644',
		'atmega644a',
		'atmega644p',
		'atmega644pa',
		'atmega88',
		'atmega88a',
		'atmega88p',
		'atmega88pa',
		# XMEGA devices -- individual templates
		'atxmega32a4',
		'atxmega128a1',
		'atxmega256a3',
		'atxmega256a3b',
	]

	linker_id = "xmega.utils.linker_scripts"

	# IAR EW optimization settings
	optlevel_paramlist = ['level', 'strategy', 'crosscall_state', 'crosscall_limit']
	optlevel_to_arglist = {
			'none'   : ['none',   'speed', 'off', 'unlimited'],
			'low'    : ['low',    'speed', 'off', 'unlimited'],
			'medium' : ['medium', 'speed', 'off', 'unlimited'],
			'high'   : ['high',   'speed', 'off', 'unlimited'],
			'size'   : ['high',   'size',  'off', 'unlimited'],
	}

	# IAR device name to id number mapping. For version 32 of this field in
	# IAR project file. These values seems to be based on the position they
	# got in the IAR processor drop-down box, so there will be big changes
	# when new devices are introduced. But by using the version element
	# this should be safe for other newer versions of IAR.
	device_iar_id_v32_map = {
		'atmega128'     : 37,
		'atmega128a'    : 38,
		'atmega128rfa1' : 39,
		'atmega1284'    : 42,
		'atmega1284p'   : 43,
		'atmega16'      : 44,
		'atmega16a'     : 42,
		'atmega168'     : 61,
		'atmega168a'    : 62,
		'atmega168p'    : 63,
		'atmega168pa'   : 64,
		'atmega169'     : 65,
		'atmega169p'    : 66,
		'atmega169pa'   : 67,
		'atmega2560'    : 68,
		'atmega32'      : 71,
		'atmega32a'     : 72,
		'atmega324a'    : 80,
		'atmega324p'    : 81,
		'atmega324p'    : 82,
		'atmega329'     : 91,
		'atmega329p'    : 92,
		'atmega329pa'   : 93,
		'atmega48'      : 99,
		'atmega48a'     : 100,
		'atmega48p'     : 101,
		'atmega48pa'    : 102,
		'atmega64'      : 103,
		'atmega64a'     : 104,
		'atmega640'     : 108,
		'atmega644'     : 109,
		'atmega644a'    : 110,
		'atmega644p'    : 111,
		'atmega644pa'   : 112,
		'atmega88'      : 125,
		'atmega88a'     : 126,
		'atmega88p'     : 127,
		'atmega88pa'    : 128,
	}

	def _get_option_set(self):
		option_set = self._get_common_option_set()

		option_set.add_option(
			SimpleLookupOption(
				'CCOptimization',
				{
					'none'   : '1',
					'low'    : '2',
					'medium' : '3',
					'high'   : '4',
				},
				'level',
				'1'
			 )
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptimizationSlave',
				{
					'none'   : '1',
					'low'    : '2',
					'medium' : '3',
					'high'   : '4',
				},
				'level',
				'1'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCAllowList',
				{
					'none'   : '000000',
					'low'    : '000000',
					'medium' : '101010',
					'high'   : '111111',
				},
				'level',
				'3'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'RTDescription',
				{
					'normal' : 'Use the normal configuration of the C/EC++ runtime library. No locale interface, C locale, no file descriptor support, no multibytes in printf and scanf, and no hex floats in strtod.',
					'full' : 'Use the full configuration of the C/EC++ runtime library. Full locale interface, C locale, file descriptor support, multibytes in printf and scanf, and hex floats in strtod.',
				},
				'dlib_desc'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'RTConfigPath',
				{
					'normal' : '$TOOLKIT_DIR$\LIB\DLIB\dlAVR-6s-xmega-n.h',
					'full' : '$TOOLKIT_DIR$\LIB\DLIB\dlAVR-6s-xmega-f.h',
				},
				'dlib_config_path'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'RTLibraryPath',
				{
					'normal' : '$TOOLKIT_DIR$\LIB\DLIB\dlAVR-6s-xmega-n.r90',
					'full'  : '$TOOLKIT_DIR$\LIB\DLIB\dlAVR-6s-xmega-f.r90',
				},
				'dlib_path'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GRuntimeLibSelect',
				{
					'normal' : '1',
					'full'   : '2',
				},
				'combo_select',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GRuntimeLibSelectSlave',
				{
					'normal' : '1',
					'full'   : '2',
				},
				'combo_select_slave',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptSizeSpeed',
				{
					'size'  : '0',
					'speed' : '1',
				},
				'strategy'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptSizeSpeedSlave',
				{
					'size'  : '0',
					'speed' : '1',
				},
				'strategy'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptForceCrossCall',
				{
					'off' : '0',
					'on'  : '1',
				},
				'crosscall_state'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCCrossCallPassesList',
				{
					'unlimited' : '5',
					'5'         : '4',
					'4'         : '3',
					'3'         : '2',
					'2'         : '1',
					'1'         : '0',
				},
				'crosscall_limit',
				'8'
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'SCCStackSize',
				'data_stack_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'SCRStackSize',
				'return_stack_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'LCNearHeapSize',
				'near_heap_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'LCFarHeapSize',
				'far_heap_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'LCHugeHeapSize',
				'huge_heap_size',
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GGEnableConfig',
				{
					'yes' : '1',
					'no'  : '0',
				},
				'enable_config'
			)
		)

		return option_set

	def _update_arch_specific_aux_project_xml(self, mcu):
		pass

	def _update_arch_specific_project_xml(self, mcu):

		# Fill workspace options.
		self._insert_project_element(self.workspace_root_element, "$WS_DIR$\\"+self._get_project_name()+self._get_project_name_suffix()+".ewp")

		# If device found in map set it accordingly
		if self.device_iar_id_v32_map.has_key(mcu):
			self._insert_single_version_element(self.project_root_element, "New Variant Processor", "32")
			self._insert_single_state_element(self.project_root_element, "New Variant Processor", str(self.device_iar_id_v32_map[mcu]))

		self._insert_state_elements(self.project_root_element, "CCDefines", self.define_list, False)
		self._insert_state_elements(self.project_root_element, "ADefines", self.define_list)

		self._insert_state_elements(self.project_root_element, "newCCIncludePaths", self.include_path_list, False) # BEWARE! don't delete old ones!
		self._insert_state_elements(self.project_root_element, "ANewIncludes", self.include_path_list, False) # BEWARE! don't delete old ones!

		if self.linker_script == "":
			# No (custom) linker script
			self._insert_single_state_element(self.project_root_element, "XclOverride", "0")
		else:
			self._insert_single_state_element(self.project_root_element, "XclFile", self.linker_script)
			self._insert_single_state_element(self.project_root_element, "XclOverride", "1")

class IAREW8Project_v610(IAREW8Project_v550):
	toolchain_version = "610"
	supported_mcus = [
		'atxmega128a3u',
		'atxmega16a4u',
		'atxmega192a3u',
		'atxmega256a3bu',
		'atxmega256a3u',
		'atxmega32a4u',
		'atxmega64a3u',
		'atxmega128a1u',
		'atxmega64a1u',
		'atxmega128a4u',
		'atxmega64a4u',
		'atxmega128b1',
		'atxmega64b1',
		'atxmega128b3',
		'atxmega64b3',
		'atxmega64a3',
		'atxmega384c3',
		'atxmega256c3',
		'atxmega128c3',
		'atxmega64c3',
		'atxmega32c4',
		'atxmega16c4',
		'atxmega384d3',
		'atxmega256d3',
		'atxmega192d3',
		'atxmega128d3',
		'atxmega64d3',
		'atxmega128d4',
		'atxmega64d4',
		'atxmega32d4',
		'atxmega16d4',
		'atxmega32e5',
		'atxmega16e5',
		'atxmega8e5',
	]

	# IAR EW optimization settings
	optlevel_paramlist = ['level', 'strategy', 'crosscall_state', 'crosscall_limit']
	optlevel_to_arglist = {
			'none'   : ['none',   'speed', 'off', 'unlimited'],
			'low'    : ['low',    'speed', 'off', 'unlimited'],
			'medium' : ['medium', 'speed', 'off', 'unlimited'],
			'high'   : ['high',   'speed', 'off', 'unlimited'],
			'size'   : ['high',   'size',  'off', 'unlimited'],
	}

	def _get_option_set(self):
		option_set = self._get_common_option_set()

		option_set.add_option(
			SimpleLookupOption(
				'CCAllowList',
				{
					'none'   : '000000',
					'low'    : '000000',
					'medium' : '101010',
					'high'   : '111111',
				},
				'level',
				'3'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCCharIs',
				{
					'no'  : '0',
					'yes' : '1',
				},
				'unsigned_char',
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'IccFloatSemantics',
				{
					'strict'  : '0',
					'relaxed' : '1',
				},
				'fp_semantics',
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCCrossCallPassesList',
				{
					'unlimited' : '5',
					'5'         : '4',
					'4'         : '3',
					'3'         : '2',
					'2'         : '1',
					'1'         : '0',
				},
				'crosscall_limit',
				'8'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptForceCrossCall',
				{
					'off' : '0',
					'on'  : '1',
				},
				'crosscall_state'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptLevel',
				{
					'none'   : '0',
					'low'    : '1',
					'medium' : '2',
					'high'   : '3',
				},
				'level'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptLevelSlave',
				{
					'none'   : '0',
					'low'    : '1',
					'medium' : '2',
					'high'   : '3',
				},
				'level'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptStrategy',
				{
					'balanced' : '0',
					'size'     : '1',
					'speed'    : '2',
				},
				'strategy',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GGEnableConfig',
				{
					'yes' : '1',
					'no'  : '0',
				},
				'enable_config'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GRuntimeLibSelect',
				{
					'normal' : '1',
					'full'   : '2',
				},
				'combo_select',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GRuntimeLibSelectSlave',
				{
					'normal' : '1',
					'full'   : '2',
				},
				'combo_select_slave',
				'0'
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'LCFarHeapSize',
				'far_heap_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'LCHugeHeapSize',
				'huge_heap_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'LCNearHeapSize',
				'near_heap_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'SCCStackSize',
				'data_stack_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'SCRStackSize',
				'return_stack_size',
			)
		)
		return option_set


	def _update_arch_specific_project_xml(self, mcu_name):
		def get_mcu_selection_string(mcu_name):
			variant = mcu_name.lstrip('atxmega')

			shortname = 'xm' + variant.lower()
			longname = 'ATxmega' + variant.upper()

			return "%s\t%s" % (shortname, longname)

		# Fill workspace options.
		self._insert_project_element(self.workspace_root_element, "$WS_DIR$\\"+self._get_project_name()+self._get_project_name_suffix()+".ewp")

		self._insert_single_state_element(self.project_root_element, 'GenDeviceSelectMenu', get_mcu_selection_string(mcu_name))

		self._insert_state_elements(self.project_root_element, "CCDefines", self.define_list, False)
		self._insert_state_elements(self.project_root_element, "ADefines", self.define_list)

		self._insert_state_elements(self.project_root_element, "newCCIncludePaths", self.include_path_list, False) # BEWARE! don't delete old ones!
		self._insert_state_elements(self.project_root_element, "ANewIncludes", self.include_path_list, False) # BEWARE! don't delete old ones!

		if self.linker_script == "":
			# No (custom) linker script
			self._insert_single_state_element(self.project_root_element,"XclOverride", "0")
		else:
			self._insert_single_state_element(self.project_root_element,"XclFile", self.linker_script)
			self._insert_single_state_element(self.project_root_element,"XclOverride", "1")


class IAREW32Project_v330(IAREW32Project):
	toolchain_version = "330"
	supported_mcus = [
		# Individual templates
		'at32uc3a0512',
		'at32uc3a3256',
		'at32uc3a3256s',
		'at32uc3b0128',
		'at32uc3b0256',
		'at32uc3b1256',
		'at32uc3c0256c',
		'at32uc3c0512c',
		'at32uc3c2256c',
		'at32uc3c2512c',
		'at32uc3l016',
		'at32uc3l064',
		'at32uc3l0128',
		'at32uc3l0256',
		'atuc128d3'
	]


	linker_id = "avr32.utils.linker_scripts"
	compiler_config_group = "config.compiler.iarew32"


	# IAR EW32 optimization settings
	optlevel_paramlist = ['level', 'strategy', 'fp_library']
	optlevel_to_arglist = {
			'none'   : ['none',   'speed', 'default'],
			'low'    : ['low',    'speed', 'default'],
			'medium' : ['medium', 'speed', 'default'],
			'high'   : ['high',   'speed', 'default'],
			'size'   : ['high',   'size',  'default'],
	}


	def _get_option_set(self):
		# Create a set with all options
		option_set = self._get_common_option_set()

		# Compiler optimization
		option_set.add_option(
			SimpleLookupOption(
				'CCOptLevel',
				{
					'none'   : '0',
					'low'    : '1',
					'medium' : '2',
					'high'   : '3',
				},
				'level'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptLevelSlave',
				{
					'none'   : '0',
					'low'    : '1',
					'medium' : '2',
					'high'   : '3',
				},
				'level'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCAllowList',
				{
					'none'   : '0000000',
					'low'    : '0000000',
					'medium' : '1001001',
					'high'   : '1111111',
				},
				'level',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCOptStrategy',
				{
					'balanced' : '0',
					'size'     : '1',
					'speed'    : '2',
				},
				'strategy',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCFPIMPLLIST',
				{
					'default' : '0',
					'size'    : '1',
					'speed'   : '2',
				},
				'fp_library',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'CCFPIMPL description',
				{
					'default' : 'Use fast implementation if optimizing for speed.',
					'size'    : 'Use the space efficient but slower implementation unless another module uses the fast implementation.',
					'speed'   : 'Always use the fast implementation.',
				},
				'fp_library'
			)
		)

		# Unhandled interrupts and exceptions
		option_set.add_option(
			SimpleLookupOption(
				'GUnhandledInterrupts',
				{
					'no'  : '0',
					'yes' : '1',
				},
				'catch_unimplemented_interrupts',
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GUnhandledExceptions',
				{
					'no'  : '0',
					'yes' : '1',
				},
				'catch_unhandled_cpu_exceptions',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'GSStackSize',
				'sys_stack_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'GCStackSize',
				'app_stack_size',
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'GHeapSize',
				'heap_size',
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'RTConfigPath',
				{
					'normal' : '$TOOLKIT_DIR$\lib\dlavr32bllasn.h',
					'full' : '$TOOLKIT_DIR$\lib\dlavr32bllasf.h',
				},
				'dlib_config_path'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'RTLibraryPath',
				{
					'normal' : '$TOOLKIT_DIR$\lib\dlavr32bllasn.r82',
					'full' : '$TOOLKIT_DIR$\lib\dlavr32bllasf.r82',
				},
				'dlib_path'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'RTDescription',
				{
					'normal' : 'Use the normal configuration of the C/EC++ runtime library. No locale interface, C locale, no file descriptor support, no multibytes in printf and scanf, and no hex floats in strtod.',
					'full' : 'Use the full configuration of the C/EC++ runtime library. Full locale interface, C locale, file descriptor support, multibytes in printf and scanf, and hex floats in strtod.',
				},
				'dlib_desc'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GRuntimeLibSelect',
				{
					'normal' : '1',
					'full'   : '2',
				},
				'combo_select',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'GRuntimeLibSelectSlave',
				{
					'normal' : '1',
					'full'   : '2',
				},
				'combo_select_slave',
				'0'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'xcOverrideProgramEntryLabel',
				{
					'no'  : '0',
					'yes' : '1',
				},
				'override_program_entry'
			)
		)
		option_set.add_option(
			SimplePassthroughOption(
				'xcProgramEntryLabel',
				'program_entry_label'
			)
		)

		return option_set


	def _update_arch_specific_project_xml(self, mcu):
		# Fill workspace options.
		self._insert_project_element(self.workspace_root_element, "$WS_DIR$\\"+self._get_project_name()+self._get_project_name_suffix()+".ewp")

		self._insert_state_elements(self.project_root_element, "CCDefines", self.define_list)
		self._insert_state_elements(self.project_root_element, "ADefines", self.define_list)

		self._insert_state_elements(self.project_root_element, "CCIncludePath2", self.include_path_list)
		self._insert_state_elements(self.project_root_element, "AIncludePaths", self.include_path_list)

		self._insert_single_state_element(self.project_root_element, "XclFile", self.linker_script)
