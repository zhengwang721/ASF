
from asf.toolchain.generic import GenericProject
from asf.toolchain.avrgcc import AVR32GCCProject
from asf.database import *

class ARMGCCProject(AVR32GCCProject):
	toolchain = "armgcc"
	linker_id = "sam.utils.linker_scripts"
	macro_id = "sam.utils.macro_files"
	arch_prefix = "cortex"
	config_stack = "armgcc.stack_size"
	config_heap = "armgcc.heap_size"
	config_defaultlibs = "armgcc.use_defaultlibs"
	config_stdlib = "armgcc.use_stdlib"
	config_startfiles = "armgcc.use_startfiles"
	config_startuplabel = "armgcc.startup_label"
	create_aux_default = GenericProject.config_enabled
	optlevel_default = 'low'
	# Name of the arch-specific makefile to look for
	makefile_name = 'Makefile.sam.in'

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
		startfiles = config.get(self.config_startfiles, self.config_enabled)

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
		return "" + ""

	def _add_arch_specifics(self, ASF, makefile_dir, config):
		# TODO: for now explicit set project type to elf, but this
		# should be either 'a' or 'elf' depending on if it is a library
		# or a standalone ELF file.
		ASF["$ASF_PROJECT_TYPE$"] = 'elf'

		# Leave PART parameter blank if compiling for architecture.
		# (Useful for currently unsupported devices.)
		target_type = config.get('target_type', 'mcu')
		if target_type == 'mcu':
			# Strip "at" from MCU name for GCC makefile
			ASF["$ASF_MCU_STRIP$"] = ASF["$ASF_MCU$"].lstrip("at")
		elif target_type == 'arch':
			ASF["$ASF_MCU_STRIP$"] = ''
		else:
			raise ConfigError("Project `%s' has unknown compile target type `%s' for toolchain `%s'" % (self.project.id, target_type, self.toolchain))

		linker_script = self.convert_path_for_root(self._get_linker_script(self.linker_id))
		self.project.filelist.add(linker_script)
		ASF["$ASF_LINKER_SCRIPT_FLASH$"] = linker_script
		linker_script = self.convert_path_for_root(self._get_aux_linker_script(self.linker_id))
		self.project.filelist.add(linker_script)
		ASF["$ASF_LINKER_SCRIPT_SRAM$"] = linker_script

		debug_script = self.convert_path_for_root(self._get_macro_file(self.macro_id))
		self.project.filelist.add(debug_script)
		ASF["$ASF_DEBUG_SCRIPT_FLASH$"] = debug_script
		debug_script = self.convert_path_for_root(self._get_aux_macro_file(self.macro_id))
		self.project.filelist.add(debug_script)
		ASF["$ASF_DEBUG_SCRIPT_SRAM$"] = debug_script

		if self.create_aux == GenericProject.config_enabled:
			ASF["$ASF_BUILD_OPTION$"] = "all"
		else:
			ASF["$ASF_BUILD_OPTION$"] = "flash"

		# Get MCU architecture, as it is required by ARMGCC
		ASF["$ASF_ARCH$"] = self.project.mcu.search_mcu_selection_list(self.arch_prefix)

		if ASF["$ASF_ARCH$"] is None:
			raise ConfigError("Could not determine architecture of device `%s' from map `%s'" % (ASF["$ASF_MCU$"], self.map_id))

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

		ASF['$ASF_LDFLAGS$'] += asf_ldflags.strip()

		# self.project.filelist.add("sam/utils/make/Makefile.in")
