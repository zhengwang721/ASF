
from asf.toolchain.generic import GenericProject
from asf.database import *

class AVRGCCProject(GenericProject):
	toolchain = "avrgcc"
	indent = 10
	just = 50
	create_aux_default = GenericProject.config_disabled

	optlevel_to_setting = {
			'none'   : '-O0',
			'low'    : '-O1',
			'medium' : '-O2',
			'high'   : '-O3',
			'size'   : '-Os',
	}
	optlevel_default = 'size'

	def __init__(self, project, db, runtime):
		super(AVRGCCProject, self).__init__(project, db, runtime)
		self.create_aux = self._get_total_config().get('create_aux', self.create_aux_default)

	def convert_path_for_root(self, path):
		return path.replace(os.sep, "/")

	def convert_path_for_makefile(self, path):
		return self.convert_path_for_root(path)

	def _add_arch_specifics(self, ASF, projectfile_dir, config):
		# TODO: compute __*_start__ based on device information
		ASF['$ASF_LDFLAGS$'] += ''
		# TODO: EEPROMFLAGS are intentionally left empty for now.
		ASF['$ASF_EEPROMFLAGS$'] = ''
		# TODO: FLASHFLAGS are intentionally left empty for now.
		ASF['$ASF_FLASHFLAGS$'] = ''

	def _add_list_entry(self, entry):
		return ' \\\n'.ljust(self.indent) + entry.ljust(self.just)

	def _get_projectfile_path(self):
		return os.path.join(self.project.basedir, 'gcc')

	def _get_makefile_path(self):
		return os.path.join(self._get_projectfile_path(), "Makefile")

	def _get_configmk_path(self):
		return os.path.join(self._get_projectfile_path(), "config.mk")

	def _get_asf_header_path(self):
		return os.path.join(self._get_projectfile_path(), "asf.h")

	def get_generated_files_list(self):
		return [self._get_makefile_path(), self._get_configmk_path(), self._get_asf_header_path()]

	def get_generated_dirs_list(self):
		return [self._get_projectfile_path()]

	def write(self):
		# Generate variables to use in makefile
		makefile_dir = self._get_projectfile_path()

		asf_project = self.get_workspace_name()
		asf_mcu   = self.project.mcu.name
		asf_root  = os.path.relpath(os.curdir, makefile_dir)
		ASF = {}

		self._check_for_config_headers()

		# Get compiler configuration from project and/or runtime
		compiler_config = self._get_compiler_config()
		toolchain_config = self._get_toolchain_config()

		total_config = compiler_config
		total_config.update(toolchain_config)

		# Handle optimization configuration
		optlevel = total_config.get('optimization', self.optlevel_default)
		try:
			asf_optlevel = self.optlevel_to_setting[optlevel]
		except KeyError:
			raise ConfigError("Unknown optimization level `%s' for project id `%s' with toolchain `%s'" % (level, self.project.id, self.toolchain))

		# Handle C, assembly and include paths
		type_mapping = {
			'$ASF_CSRC$'     : BuildC,
			'$ASF_ASSRC$'    : BuildAssembly,
			'$ASF_INC_PATH$' : BuildInclude,
			'$ASF_HSRC$'     : BuildHeader,
		}

		for declaration, filetype in type_mapping.items():
			str = ""
			for f in self.project.get_build(filetype, self.toolchain, sort=True):
				if filetype is not BuildInclude:
					self.project.filelist.add(f.replace(os.sep, "/"))
				f = self.convert_path_for_makefile(f)
				str += self._add_list_entry(f)
			ASF[declaration] = str.strip()

		# Add include path for asf.h
		asf_h_dir = os.path.dirname(self._get_asf_header_path())
		asf_h_dir = self.convert_path_for_makefile(asf_h_dir)
		ASF['$ASF_INC_PATH$'] += self._add_list_entry(asf_h_dir)

		# Get linker flags
		asf_ldflags = ''
		flags = self.project.get_build(BuildLinkerFlags, self.toolchain, sort=True)
		for flag in flags:
			asf_ldflags += self._add_list_entry('-Wl,' + flag.strip())

		ASF['$ASF_LDFLAGS$'] = asf_ldflags

		# Get assembler flags
		asf_asmflags = ''
		flags = self.project.get_build(BuildAssemblerFlags, self.toolchain, sort=True)
		for flag in flags:
			asf_asmflags += self._add_list_entry(flag.strip())

		ASF['$ASF_ASFLAGS$'] = asf_asmflags

		# Get C compiler flags
		asf_ccflags = ''
		flags = self.project.get_build(BuildCCompilerFlags, self.toolchain, sort=True)
		for flag in flags:
			asf_ccflags += self._add_list_entry(flag.strip())

		ASF['$ASF_CFLAGS$'] = asf_ccflags

		# Handle defines
		defines = self.project.get_build(BuildDefine, self.toolchain, sort=True)
		define_text = ""
		if not defines is None:
			for definition, value in defines:
				if value:
					define_text += self._add_list_entry("-D %s=%s " % (definition, value))
				else:
					define_text += self._add_list_entry("-D %s " % (definition))

		# Get preprocessor flags
		asf_ppflags = define_text.strip()
		flags = self.project.get_build(BuildPreprocessorFlags, self.toolchain, sort=True)
		for flag in flags:
			asf_ppflags += self._add_list_entry(flag.strip())

		ASF['$ASF_CPPFLAGS$'] = asf_ppflags

		# Get all library objects
		libs = self.project.get_build(BuildUserLibrary, self.toolchain, sort=True)

		asf_libs = ""
		asf_libpaths = ""

		# Process the library paths for GCC
		for lib in libs:
			(libpath, libfile) = os.path.split(self.convert_path_for_makefile(lib))
			# Remove "lib" prefix and file extension of the library file
			asf_libs += self._add_list_entry(libfile[len("lib"):libfile.rfind(".")])
			asf_libpaths += self._add_list_entry(libpath).lstrip(".")
			self.project.filelist.add(lib)

		# Get all compiler library objects
		libs = self.project.get_build(BuildCompilerLibrary, self.toolchain, sort=True)

		for lib in libs:
			asf_libs += self._add_list_entry(lib[len("lib"):])

		# Add files for distribution to filelist
		for f in self.project.get_build(BuildDistributeFile, self.toolchain) + \
				self.project.get_build(BuildDistributeUserFile, self.toolchain):
			self.project.filelist.add(f)
		for d in self.project.get_build(BuildDistributeDirectory, self.toolchain):
			self._add_dir_to_filelist(d)

		ASF["$ASF_OPTLEVEL$"] = asf_optlevel
		ASF["$ASF_PROJECT$"] = asf_project
		ASF["$ASF_MCU$"] = asf_mcu
		ASF["$ASF_ROOT$"] = self.convert_path_for_root(asf_root)
		ASF["$ASF_LIBS$"] = asf_libs
		ASF["$ASF_LIB_PATH$"] = asf_libpaths

		# TODO: for now explicit set project type to elf, but this
		# should be either 'a' or 'elf' depending on if it is a library
		# or a standalone ELF file.
		ASF["$ASF_PROJECT_TYPE$"] = 'elf'

		# TODO: DBGFLAGS are intentionally left empty for now.
		ASF['$ASF_DBGFLAGS$'] = ''

		# TODO: ARFLAGS are intentionally left empty for now.
		ASF['$ASF_ARFLAGS$'] = ''

		self._add_arch_specifics(ASF, makefile_dir, total_config)

		if not os.path.exists(makefile_dir):
			os.makedirs(makefile_dir)

		# Create Makefile
		makefile = self._read_file(os.path.join(self.templatedir, "%s_Makefile" % self.toolchain))
		makefile = self._replace_strings(makefile, ASF)
		makefile_name = self._get_makefile_path()
		self._write_file(makefile_name, makefile)
		self.project.filelist.add(makefile_name)

		# Create config.mk
		config_mk = self._read_file(os.path.join(self.templatedir, "%s_config.mk" % self.toolchain))
		config_mk = self._replace_strings(config_mk, ASF)
		config_mk_name = self._get_configmk_path()
		self._write_file(config_mk_name, config_mk)
		self.project.filelist.add(config_mk_name)

		# Create asf.h
		asf_h = self._generate_asf_header_content()
		asf_h_path = self._get_asf_header_path()
		self._write_file(asf_h_path, asf_h)
		self.project.filelist.add(asf_h_path)

		self.log.info("GCC makefile written:  " + config_mk_name)

class AVR32GCCProject(AVRGCCProject):
	toolchain = "avr32gcc"
	arch_directory = "avr32"
	linker_id = "avr32.utils.linker_scripts"
	arch_prefix = "ucr"
	config_stack = "avr32gcc.stack_size"
	config_heap = "avr32gcc.heap_size"
	config_programming_clock = "avr32gcc.programming_clock"
	config_programming_clock_default = "int"
	config_defaultlibs = "avr32gcc.use_defaultlibs"
	config_stdlib = "avr32gcc.use_stdlib"
	config_startfiles = "avr32gcc.use_startfiles"
	config_startuplabel = "avr32gcc.startup_label"
	config_trampoline = "config.avr32.utils.trampoline"

	def convert_path_for_makefile(self, path):
		return path.replace(os.sep, "/")

	def _generate_startlibs_config(self, config):
		""" Generate liker startup library configuration.

			Returns a string which can contain the following strings:
			-nostartfiles, -nostdlib, -nodefaultlibs
		"""
		# Load value from configuration. If not found, set default value enabled
		default_libs = config.get(self.config_defaultlibs, self.config_enabled)
		# Load value from configuration. If not found, set default value enabled
		stdlib = config.get(self.config_stdlib, self.config_enabled)
		# Load value from configuration. If not found, set default value disabled
		startfiles = config.get(self.config_startfiles, self.config_disabled)

		# -nostdlib includes -nodefaultlibs and -nostartfiles so there
		# is no need to set these options if -nostdlib is set.
		libconfig = ""
		if stdlib == self.config_disabled:
			libconfig += " -nostdlib"
		else:
			if default_libs == self.config_disabled:
				libconfig += " -nodefaultlibs"
			if startfiles == self.config_disabled:
				libconfig += " -nostartfiles"

		# Remove any leading space and return
		return libconfig.strip()

	def _generate_startup_label(self, config):
		""" Generate linker configuration for startup label.
			Returns a string with linker configuration
		"""

		# Linker prefix for setting startup label
		label_prefix = "-Wl,-e,"
		# Load startup label from config. None if no configuration exists
		startup_label = config.get(self.config_startuplabel, None)
		# Load trampoline configuration from project/runtime settings. Default value is enabled
		trampoline = self.get_config(self.config_trampoline)
		if trampoline is None:
			trampoline = self.config_enabled

		# If no startup label has been set, check if trampoline has
		# been disabled.
		if startup_label is None:
			if trampoline == self.config_enabled:
				# Trampoline is enabled:
				startup_label = "_trampoline"
			else:
				# Tramponline has been disabled, no startup label
				label_prefix = ""
				startup_label = ""

		return label_prefix + startup_label

	def _add_arch_specifics(self, ASF, makefile_dir, config):
		# TODO: for now explicit set project type to elf, but this
		# should be either 'a' or 'elf' depending on if it is a library
		# or a standalone ELF file.
		ASF["$ASF_PROJECT_TYPE$"] = 'elf'

		# Leave PART parameter blank if compiling for architecture.
		# (Useful for currently unsupported devices.)
		target_type = config.get('target_type', 'mcu')
		if target_type == 'mcu':
			# Strip "at32" from MCU name for GCC makefile
			ASF["$ASF_MCU_STRIP$"] = ASF["$ASF_MCU$"].lstrip("at32")
		elif target_type == 'arch':
			ASF["$ASF_MCU_STRIP$"] = ''
		else:
			raise ConfigError("Project `%s' has unknown compile target type `%s' for toolchain `%s'" % (self.project.id, target_type, self.toolchain))

		linker_script = self.convert_path_for_root(self._get_linker_script(self.linker_id))
		self.project.filelist.add(linker_script)
		#linker_script = self.convert_path_for_makefile(linker_script)
		ASF["$ASF_LINKER_SCRIPT$"] = linker_script

		# Get MCU architecture, as it is required by AVR32GCC
		ASF["$ASF_ARCH$"] = self.project.mcu.search_mcu_selection_list(self.arch_prefix)

		if ASF["$ASF_ARCH$"] is None:
			raise ConfigError("Could not determine architecture of device `%s' from map `%s'" % (ASF["$ASF_MCU$"], self.map_id))

		# Handle architecture top level arch directory name
		ASF["$ASF_ARCH_DIRECTORY$"] = self.arch_directory

		# Handle linker configuration
		startlibs_flags = self._generate_startlibs_config(config)
		startuplabel_flags = self._generate_startup_label(config)
		asf_ldflags = self._add_list_entry(startlibs_flags + " " + startuplabel_flags)

		# Handle stack size configuration
		if self.config_stack in config:
			asf_ldflags += self._add_list_entry('-Wl,--defsym,__stack_size__=%s' % config[self.config_stack])

		# Handle heap size configuration
		if self.config_heap in config:
			asf_ldflags += self._add_list_entry('-Wl--defsym,__heap_size__=%s' % config[self.config_heap])

		# Get configured programming clock, set to config_programming_clock_default if not found
		ASF["$ASF_PROG_CLOCK$"] = config.get(self.config_programming_clock, self.config_programming_clock_default)

		ASF['$ASF_LDFLAGS$'] += asf_ldflags.strip()

		self.project.filelist.add("%s/utils/make/Makefile.in" % self.arch_directory)
		self.project.filelist.add("%s/utils/make/Makefile.avr32program.in" % self.arch_directory)
		self.project.filelist.add("%s/utils/make/Makefile.batchisp.in" % self.arch_directory)
