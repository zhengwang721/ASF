from asf.build import *
from asf.exception import *
from asf.database import *

class GenericElement(object):
	toolchain = "*"
	mcu_map_id = "map.atmel"
	documentation_server_hostname = "http://asf.atmel.com"

	config_enabled = "yes"
	config_disabled = "no"

	def __init__(self, project, db, runtime):
		self.project = project
		self.db = db
		self.runtime = runtime
		self.log = runtime.log
		self.templatedir = runtime.template_dir
		self.configuration = PrioritizedConfigurationHandler(runtime.configuration, project.get_configuration())

	def _read_file(self, filename):
		"""
		Opens the specified file and returns its contents, if it
		could be opened. Otherwise, an error message is printed.
		"""
		try:
			f = open(filename, "rb")
		except IOError:
			raise IOError("Project generator for toolchain `%s' could not read file `%s'" % (self.toolchain, filename))

		file = f.read()
		f.close()
		return file

	def _write_file(self, filename, content):
		"""
		Write the content to the file directly if it does not exist. If
		the file exists, open it for reading and compare it with the
		new content.

		The file is only updated if the new content differs from the
		original. This is done to not touch files that does not need an
		update, since the build process will lookup the various
		modified timestamps and trigger unnecessary builds.

		If unable to open the file an error message is printed.
		"""
		try:
			if not os.path.exists(filename):
				f = open(filename, "wb")
				f.write(content)
			else:
				f = open(filename, "rb")
				current_content = f.read()
				if not current_content == content:
					f.close()
					f = open(filename, "wb")
					f.write(content)
			f.close()
		except IOError:
			raise IOError("Project generator for toolchain `%s' could not write file `%s'" % (self.toolchain, filename))

	def _replace_strings(self, content, dictionary):
		"""
		Replaces all occurences of the dictionary keys (strings)
		in content with the corresponding values from dictionary.
		"""
		for key, value in dictionary.items():
			content = content.replace(key, value)

		return content

	def _get_build_from_project_or_selector(self, build_type, selector_id):
		"""
		Gets a list of build items of the specified type from the project,
		alternatively from a specifiable selector that can reside in either
		the project's home database or in ASF.

		Assuming the project does not contain any build items of the
		specified type and the ID of a selector has been specified, the
		function will first try to look up that selector in the project's
		home database.
		If this fails, the function will try to find ASF via the dependency
		list of the project's home extension, then look up the selector in
		the ASF database.
		If a selector is found in either of the databases, its build items
		of the specified type are returned.


		Returns an empty list if project does not contain specified build
		type and no selector has been specified.

		Raises DbError if project does not contain the specified build type,
		a selector was specified, but could not be found in the project's
		home database, and a dependency on ASF is missing or ASF cannot be
		loaded.

		Raises NotFoundError if project does not contain the specified build
		type, and a selector was specified, but could not be found in the
		project's home database nor in ASF.

		Raises NoSelectionError if the found selector cannot make a selection
		for the current MCU and configuration.
		"""
		asf_uuid = "Atmel.ASF"
		# Check for custom build items
		build_items = self.project.get_build(build_type, self.toolchain)

		# If no build item was found and a selector is defined for toolchain, use it
		if not build_items:
			if selector_id:
				# Try to find selector in the project's home database, or in ASF
				try:
					build_selector = self.db.lookup_by_id(selector_id)
				except NotFoundError:
					# Re-raise exception if this actually was the ASF database
					if self.db.extension.uuid == asf_uuid:
						raise

					# Otherwise, try looking up in ASF database via extension dependencies
					req_eid, req_ver = self.db.extension.get_dependency_by_uuid(asf_uuid)
					if req_eid is not None:
						# This raises NotFoundError if selector is not found
						build_selector = self.db.lookup_external_by_id(req_eid, selector_id)
					else:
						raise DbError("Could not find selector `%s' in database. Either add build type/subtype `%s/%s' to the project or a dependency on ASF to the extension." % (selector_id, build_type.type, build_type.subtype))

				build_selector.resolve_all_selections(self.configuration, self.project.mcu)
				build_items = build_selector.get_build(build_type, self.toolchain)

		return build_items

	def _get_linker_script(self, linker_id=None):
		"""
		Function which returns the linker script for the project. If no
		custom script is found, the function will return None unless a
		linker script selector was specified. In this case, the
		selector is used to get the linker script.
		"""
		linker_script = self._get_build_from_project_or_selector(BuildLinkerScript, linker_id)

		if len(linker_script) != 1:
			raise ConfigError("Found %s linker scripts for device `%s' and toolchain `%s', expected 1" % (len(linker_script), self.project.mcu.name, self.toolchain))

		return linker_script[0]

	def _get_aux_linker_script(self, linker_id=None):
		"""
		Function which returns the auxiliary linker script for the project. If no
		custom script is found, the function will return None unless a
		linker script selector was specified. In this case, the
		selector is used to get the auxiliary linker script.
		"""
		aux_linker_script = self._get_build_from_project_or_selector(BuildAuxLinkerScript, linker_id)

		if len(aux_linker_script) != 1:
			raise ConfigError("Found %s aux linker scripts for device `%s' and toolchain `%s', expected 1" % (len(aux_linker_script), self.project.mcu.name, self.toolchain))

		return aux_linker_script[0]

	def _get_macro_file(self, macro_id=None):
		"""
		Function which returns the macro file for the project. If no
		custom script is found, the function will return None unless a
		macro file selector was specified. In this case, the
		selector is used to get the macro file.
		"""
		macro_file = self._get_build_from_project_or_selector(BuildMacroFile, macro_id)

		if len(macro_file) != 1:
			raise ConfigError("Found %s macro file for device `%s' and toolchain `%s', expected 1" % (len(macro_file), self.project.mcu.name, self.toolchain))

		return macro_file[0]

	def _get_aux_macro_file(self, macro_id=None):
		"""
		Function which returns the auxiliary macro file for the project. If no
		custom script is found, the function will return None unless a
		macro file selector was specified. In this case, the
		selector is used to get the auxiliary macro file.
		"""
		aux_macro_file = self._get_build_from_project_or_selector(BuildAuxMacroFile, macro_id)

		if len(aux_macro_file) != 1:
			raise ConfigError("Found %s macro file  for device `%s' and toolchain `%s', expected 1" % (len(aux_macro_file), self.project.mcu.name, self.toolchain))

		return aux_macro_file[0]

	def get_config(self, name):
		"""
		Returns the specified config from either the runtime or project
		settings. Project settings are overridden by runtime settings.
		"""
		config = self.db.runtime.get_config(name)
		if not config:
			config = self.project.get_config(name)
		return config

	def get_config_group(self, group):
		"""
		Returns the specified config group from the runtime and/or
		project settings. Project settings are overridden by runtime
		settings.
		"""
		config = self.project.get_config_group(group)
		config.update(self.db.runtime.get_config_group(group))
		return config

	def _get_compiler_config(self):
		"""
		Returns a dictionary with any generic compiler configuration
		options that have been set.

		The following compiler configuration:
		<config name="config.compiler.bar" value="bar" />
		<config name="config.compiler.foo.bar" value="foo" />

		Will result in a dictionary on the form:
		config = { "bar" : "bar", "foo.bar" : "foo" }
		"""
		return self.get_config_group("config.compiler")

	def _get_toolchain_config(self):
		"""
		Returns a dictionary with any toolchain-specific compiler
		configuration options that have been set.

		The following compiler configuration:
		<config name="config.compiler.a" value="foobar" />
		<config name="config.compiler.b.bar" value="bar" />
		<config name="config.compiler.b.foo.bar" value="foo" />
		<config name="config.compiler.c.foo.bar" value="bar" />

		Will result in a dictionary on the following form, for
		toolchain b:
		config = { "bar" : "bar", "foo.bar" : "foo" }
		"""
		return self.get_config_group("config.compiler." + self.toolchain)

	def _get_total_config(self):
		# Get total configs
		generic_config = self._get_compiler_config()
		toolchain_config = self._get_toolchain_config()
		total_config = generic_config
		total_config.update(toolchain_config)
		return total_config

	def get_toolchain_configuration(self):
		"""
		Returns a tuple with two dictionaries, respectively mapping toolchain config
		names to single values and to lists of values. The toolchain configurations must
		be set with the toolchain-config or toolchain-config-list elements in the board
		or the project.
		
		The project will override any same-named toolchain-config that is set in the
		board, while for toolchain-config-list the values are simply added appended to
		the list.
		"""
		# Fetch toolchain configs and config lists from board
		if self.project._board:
			(configs, list_configs) = self.project._board._get_toolchain_configuration_from_element(self.toolchain)
		else:
			configs = {}
			list_configs = {}

		# Update configs with those from project
		(more_configs, more_list_configs) = self.project._get_toolchain_configuration_from_element(self.toolchain)
		configs.update(more_configs)

		# Add list configs from project to the board's list configs
		for config_name, config_values in more_list_configs.items():
			list_configs[config_name] = list_configs.get(config_name, []) + config_values

		return (configs, list_configs)

	def _resolve_child_modules(self, root_module, module_type=None):
		"""
		Returns a tuple with two lists of modules which are directly
		required by the specified root module.

		The two lists contain modules of an optional, specified type,
		and modules of type meta. If no type is specified, the first
		list will be empty. If the type is specified as *, all module
		types except meta will be added to the first list.

		The function will recursively resolve module selectors.
		"""
		type_modules = []
		meta_modules = []

		req_modules = root_module.get_prerequisites(recursive=False)

		for module in req_modules:
			if module.type == 'meta':
				meta_modules.append(module)

			elif (module.type == None) and isinstance(module, ModuleSelector):
				new_req_module = module.get_prerequisites(recursive=False)
				req_modules.extend(new_req_module)

			elif (module_type == '*') or (module.type == module_type):
				type_modules.append(module)

		return (type_modules, meta_modules)

	def _resolve_application_modules(self):
		"""
		Return a list with the application modules of the project, and
		any meta modules which they require.
		"""
		app_modules  = []
		proj_modules = self.project.get_prerequisites(recursive=True)

		# Get all application modules in the entire project
		for module in proj_modules:
			if module.type == 'application':
				app_modules.append(module)

		# Add meta modules which the application modules may require
		meta_modules = []
		for module in app_modules:
			new_meta_modules = self._resolve_sub_meta_modules(module)
			meta_modules.extend(new_meta_modules)

		app_modules.extend(meta_modules)

		return app_modules

	def _resolve_board_modules(self):
		"""
		Return a list with the board modules of the project, and
		any meta modules which they require.
		"""
		board_modules  = []
		proj_modules = self.project.get_prerequisites(recursive=True)

		# Get all board modules in the entire project
		for module in proj_modules:
			if module.type == 'board':
				board_modules.append(module)

		# Add meta modules which the board modules may require
		meta_modules = []
		for module in board_modules:
			new_meta_modules = self._resolve_sub_meta_modules(module)
			meta_modules.extend(new_meta_modules)

		board_modules.extend(meta_modules)

		return board_modules

	def _resolve_sub_meta_modules(self, root_module):
		"""
		Return a list of all meta modules which are required by the
		specified module, resolving recursively for sub meta modules
		and taking module selectors into account.
		"""
		(dummy, meta_modules) = self._resolve_child_modules(root_module)

		for module in meta_modules:
			sub_meta_modules = self._resolve_sub_meta_modules(module)
			meta_modules.extend(sub_meta_modules)

		return meta_modules

	def _get_application_modules(self):
		"""
		Return a list with the child modules of the project which are
		assumed to constitute the application of the project.

		If no application modules are found, a NotFoundError exception
		is raised.
		"""
		try:
			modules = self.application_modules
		except AttributeError:
			modules = self._resolve_application_modules()
			self.application_modules = modules

		return modules

	def _get_board_modules(self):
		"""
		Return a list with the child modules of the project which are
		assumed to define the board of the project.
		"""
		try:
			modules = self.board_modules
		except AttributeError:
			modules = self._resolve_board_modules()
			self.board_modules = modules

		return modules

	def _get_build_from_module_list(self, build_type, module_list, recursive=False):
		"""
		Returns a list with build items of the specified type from the
		modules in the supplied list.

		The build items are by default fetched non-recursively, i.e.,
		only the listed module's build items are returned.
		"""
		items = []

		for module in module_list:
			items.extend(module.get_build(build_type, self.toolchain, recursive))

		# Remove duplicates
		items = list(set(items))

		return items

	def _get_api_header_files(self, module, recursive=False):
		"""
		Find and return all api header files with correct paths.
		"""
		api_files = []

		if not module.fromfile:
			module_path = ''
		else:
			module_path = os.path.dirname(module.fromfile)
		api_headers = module.get_build(BuildApiHeader, self.toolchain, recursive)
		include_paths = module.get_build(BuildInclude, self.toolchain, recursive)

		# Use the shortest relative path between include path and API header for includes
		for header_path in api_headers:
			# Find shortest include path
			shortest_path = None
			for inc_path in include_paths:
				new_path = os.path.relpath(header_path, inc_path)
				# Check if this path is a better choice than the current one
				if not shortest_path or new_path.count(os.sep) < shortest_path.count(os.sep):
					# Win.
					shortest_path = new_path
			# If no path was found, use the exact path specified in asf.xml and warn
			if not shortest_path:
				shortest_path = os.path.relpath(header_path, module_path)
				self.runtime.log.warning("Include path for API header file `%s' in module `%s' was not found" % (shortest_path, module.id))
			api_files.append(shortest_path)
		return api_files

	def _get_api_modules_files(self, modules=None):
		dict = {}
		for module in modules or self.project.get_prerequisites(recursive=True):
			files = self._get_api_header_files(module, recursive=False)
			if files:
				dict[module] = files
		return dict

	def _generate_asf_header_content(self, modules=None, template_file=None):
		"""
		Generates the content for the asf.h file -- the API include file
		which should be used by applications.

		A list of modules for which to add #includes can be supplied. If
		it is not supplied, includes will be added for all modules in
		the project.

		It is possible to specify the template file to use. It must
		contain the string $ASF_HEADER_FILES$ where the includes are to
		be added.

		The includes are divided into blocks, corresponding to the
		modules from which they originate. The blocks are ordered
		alphabetically with regards to the caption of the originating
		module. The individual includes per block are also ordered
		alphabetically.

		The order can be reversed by setting the configuration named
		'config.generator.reverse_asf_header' to 'yes'.
		"""
		config_name = 'config.generator.reverse_asf_header'
		api_includes = ''
		caption_to_files = {}

		reverse_sort = self.configuration.get_config(config_name) == self.config_enabled

		for module, files in self._get_api_modules_files(modules).items():
			caption_to_files[module.caption] = files

		for caption, files in sorted(caption_to_files.items(), reverse=reverse_sort):
			api_includes += '\r\n// From module: %s\r\n' % caption
			for file in sorted(files, reverse=reverse_sort):
				api_includes += '#include <%s>\r\n' % file.replace(os.sep, '/')

		if template_file is None:
			asf_h = self._read_file(os.path.join(self.templatedir, 'asf.h'))
		else:
			asf_h = self._read_file(template_file)

		return asf_h.replace('$ASF_HEADER_FILES$', api_includes)

	def _check_for_config_headers(self):
		"""
		Checks whether any config headers that are needed by modules in
		the project can be found in its header file list.
		"""
		indent = 4
		config_headers = self.project.get_build(BuildModuleConfigRequiredHeaderFile, self.toolchain)
		header_files = self.project.get_build(BuildHeader, self.toolchain)

		missing_configs = ""
		found = bool

		for config in config_headers:
			found = False

			for header in header_files:
				if header[-len(config):] == config:
					found = True
					break

			if not found:
				missing_configs += '\n'.ljust(indent) + config

		if missing_configs != "":
			raise ConfigError("Project `%s' is missing config headers: " % (self.project.id) + missing_configs)

	def _check_for_device_support(self):
		"""
		Checks if device-support is correct for all requirements/prerquisites of
		the project.

		Returns False if an error was discovered, True if no problem was found.
		"""
		mcu_search_map = self.project.mcu.get_group_map()

		no_support_list = self.project.resolve_device_support(mcu_search_map)
		if len(no_support_list) > 0:
			self.log.warning("Project %s uses MCU %s, which is not supported by required module(s):" % (self.project.id, self.project.mcu.name))
			for item in no_support_list:
				dev_supp = item.get_device_support()
				if len(dev_supp) > 0:
					dev_supp = ' '.join(dev_supp)
				else:
					dev_supp = "<device support not specified>"

				self.log.warning("  %s [%s]" % (item.id, dev_supp))
			return False
		else:
			return True

	def _add_dir_to_filelist(self, dir):
		"""
		Scan thru given directory and add all files.
		Will ignore .svn directories
		"""
		for dirpath, dirnames, filenames in os.walk(dir, topdown = True):
			if ".svn" in dirnames:
				dirnames.remove(".svn")
			for f in filenames:
				self.project.filelist.add(os.path.join(dirpath, f))

	def get_generated_files_list(self):
		"""
		Return a list with the files generated by the generator when running the "write" function.
		"""
		raise Exception("Abstract method 'get_generated_files_list' called on " + self.__class__.__name__)

	def get_generated_dirs_list(self):
		"""
		Return a list with the directories generated by the generator when running the "write" function.
		"""
		raise Exception("Abstract method 'get_generated_dirs_list' called on " + self.__class__.__name__)

	def write(self):
		raise Exception("Abstract class method called")

class GenericProject(GenericElement):

	def __init__(self, project, db, runtime):
		GenericElement.__init__(self, project, db, runtime)

		self.project.resolve_all_selections(self.configuration, project.mcu)
		self._check_for_device_support()

	def _resolve_workspace_name(self):
		"""
		This function resolves the workspace name for the project by first checking if
		one was specified in the project's XML, and if not, constructing it from the
		ID of the main application module.

		The construction algorithm tries to determine if the project is an example or
		an application by checking if the second, dot-separated element in the ID of
		the main application module contains 'application'.
		If so, then the workspace name is constructed from all but the first two
		dot-separated elements of the ID.
		If not, then the workspace name is constructed from the last two dot-separated
		elements of the ID.

		Examples:
		xxx.applications.audio.player            -> AUDIO_PLAYER
		xxx.applications.led_chaser              -> LED_CHASER
		xxx.drivers.dac.example1                 -> DAC_EXAMPLE1
		xxx.components.memory.dataflash.example1 -> DATAFLASH_EXAMPLE1
		"""
		name = self.project.workspace_name

		if name == None:
			# Find the first application module. Use project if no application module found.
			modules = self._get_application_modules()
			if not modules:
				module = self.project
			else:
				module = modules[0]
			# Get the ID, remove first hash and all trailing characters
			module_id = module.id.partition('#')[0]

			# Split the ID for construction of project name
			split_id = module_id.split('.')

			# Get the parts of the project name by guessing the type
			# from module ID (..X.exampleN or X.applications.Y..)
			if len(split_id) >= 3 and 'application' in split_id[1]:
				# Skip the two first elements
				name = '_'.join(split_id[2:])

			elif len(split_id) >= 4:
				# Grab the two last elements
				name = '_'.join(split_id[-2:])

			else:
				# Application module ID is malformed
				name = '_'.join(split_id)
				self.log.warning("Unable to resolve workspace name for project `%s' from application module ID `%s'. Set the workspace-name attribute or modify the module ID to avoid this warning." % (self.project.id, module_id))

		return name

	def get_workspace_name(self):
		"""
		Return the project name to use in the generated workspace.
		"""
		try:
			name = self._workspace_name

		except AttributeError:
			name = self._resolve_workspace_name()
			self._workspace_name = name

		return name
