
from asf.toolchain.iarew import *
from asf.database import *

class IAREWARMProject(IAREWProject):
	toolchain = "iarewarm"

	"""
	Parent class for all IAR EW generators for ARM

	Contains functions that are common to all versions of the toolchain/IDE.
	"""
	toolchain = "iarewarm"


	@staticmethod
	def get_generator(project, db, runtime):
		return IAREWProject.get_generator(project, db, runtime, IAREWARMProject)


	def __init__(self, project, db, runtime):
		if type(self) == IAREWARMProject:
			raise Exception("Abstract class %s instantiated" % self.__class__.__name__)
		else:
			super(IAREWARMProject, self).__init__(project, db, runtime)


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
		heap_size = config.get('heap_size', None)
		params['heap_size'] = heap_size

		sys_stack_size = config.get('sys_stack_size', None)
		params['sys_stack_size'] = sys_stack_size

		app_stack_size = config.get('app_stack_size', None)
		params['app_stack_size'] = app_stack_size

		return params

class IAREWARMProject_v600(IAREWARMProject):
	toolchain_version = "600"
	supported_mcus = [
		# Individual templates
		'atsam3n4c',
		'atsam3s4c',
		'atsam3sd8c',
		'atsam3u4e',
		'atsam3x8e',
		'atsam3x8h',
		'atsam4s16c',
		'atsam4lc4c',
		'atsam4sd32c',
	]
	linker_id = "sam.utils.linker_scripts"
	compiler_config_group = "config.compiler.sam"
	create_aux_default = GenericProject.config_enabled
	macro_id = "sam.utils.macro_files"

	# IAR EWARM optimization settings
	optlevel_paramlist = ['level', 'strategy']
	optlevel_to_arglist = {
			'none'   : ['none',   'speed'],
			'low'    : ['low',    'speed'],
			'medium' : ['medium', 'speed'],
			'high'   : ['high',   'speed'],
			'size'   : ['high',   'size'],
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
		# Only insert Stack size option if user wants to overwrite default value
		option_set.add_option(
			DualInsertionOption(
				'IlinkExtraOptions', '--config_def __ICFEDIT_size_cstack__=%s', '--config_def __ICFEDIT_size_heap__=%s', 'sys_stack_size','heap_size'
			)
		)
		option_set.add_option(
			SimpleLookupOption(
				'RTConfigPath2',
				{
					'normal' : '$TOOLKIT_DIR$\INC\c\DLib_Config_Normal.h',
					'full' : '$TOOLKIT_DIR$\INC\c\DLib_Config_Full.h',
				},
				'dlib_config_path'
			)
		)
		option_set.add_option(
                        SimpleLookupOption(
                                'GRuntimeLibSelect',
                                {
                                        'normal' : '1',
                                        'full' 	 : '2',
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
                                        'full' 	 : '2',
                                },
								'combo_select_slave',
								'0'
						)
		)

		return option_set

	def _update_arch_specific_project_xml(self, mcu):
		# Fill project options.
		self._insert_state_elements(self.project_root_element, "CCDefines", self.define_list)
		self._insert_state_elements(self.project_root_element, "ADefines", self.define_list)
		self._insert_state_elements(self.project_root_element, "CCIncludePath2", self.include_path_list)
		self._insert_state_elements(self.project_root_element, "AUserIncludes", self.include_path_list)
		self._insert_single_state_element(self.project_root_element, "IlinkIcfFile", self.linker_script)

		# Fill workspace options.
		self._insert_project_element(self.workspace_root_element, "$WS_DIR$\\"+self._get_project_name()+self._get_project_name_suffix()+".ewp")

		# Fill debugger options.
		if self.debugger_use_flash_loader == self.config_enabled:
			self._insert_single_state_element(self.debuggerfile_root_element, "UseFlashLoader", "1")
		else:
			self._insert_single_state_element(self.debuggerfile_root_element, "UseFlashLoader", "0")

		# Get MacroFile
		file_macro_file = self._get_macro_file(self.macro_id)
		self.project.filelist.add(file_macro_file)
		projectfile_dir = self._get_project_file_dir()
		file_macro_file = os.path.relpath(file_macro_file, projectfile_dir)
		self._insert_single_state_element(self.debuggerfile_root_element, "MacFile", "$PROJ_DIR$\\"+file_macro_file)

		# Set Output file name
		self._insert_single_state_element(self.project_root_element, "IlinkOutputFile", self._get_project_name()+self._get_project_name_suffix()+'.out')

		# Set .bin Output file name
		self._insert_single_state_element(self.project_root_element, "OOCOutputFile", self._get_project_name()+self._get_project_name_suffix()+'.bin')

	def _update_arch_specific_aux_project_xml(self, mcu):
		# Fill auxiliary project options.
		self._insert_single_state_element(self.project_root_element, "IlinkIcfFile", self.aux_linker_script)

		# Fill auxiliary workspace project options.
		self._insert_project_element(self.workspace_root_element, "$WS_DIR$\\"+self._get_project_name()+self._get_aux_project_name_suffix()+".ewp")

		# Fill auxiliary debugger options
		self._insert_single_state_element(self.debuggerfile_root_element, "UseFlashLoader", "0")

		# Get Auxiliary MacroFile
		file_aux_macro_file = self._get_aux_macro_file(self.macro_id)
		self.project.filelist.add(file_aux_macro_file)
		projectfile_dir = self._get_project_file_dir()
		file_aux_macro_file = os.path.relpath(file_aux_macro_file, projectfile_dir)
		self._insert_single_state_element(self.debuggerfile_root_element, "MacFile", "$PROJ_DIR$\\"+file_aux_macro_file)

		# Set Output file name
		self._insert_single_state_element(self.project_root_element, "IlinkOutputFile", self._get_project_name()+self._get_aux_project_name_suffix()+'.out')

		# Set .bin Output file name
		self._insert_single_state_element(self.project_root_element, "OOCOutputFile", self._get_project_name()+self._get_aux_project_name_suffix()+'.bin')

	def _get_project_name_suffix(self):
		if self.debugger_use_flash_loader == self.config_enabled:
			return '_flash'
		else:
			return '_sram'

	def _get_aux_project_name_suffix(self):
		return '_sram'
