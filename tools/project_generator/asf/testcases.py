import StringIO
import sys
import unittest

from asf.configuration import ConfigurationHandler
from asf.database import *
from asf.extension import *
from asf.extensionmanager import *
from asf.helper import *
from asf.runtime import Runtime
from asf.toolchain.avrstudio5 import AVRStudio5Project, AVRStudio5Project32
from asf.toolchain.generic import GenericProject
from asf_avrstudio5_interface import PythonFacade


class UnitTestProject(GenericProject):
	def write(self):
		pass

class UnitTestRuntime(Runtime):
	pass

class UnitTestConfigItem(ConfigItem):
	pass

class UnitTestDummyClass(object):
	pass

def norm_path_join(*dirs):
	norm_dirs = []
	for dir in dirs:
		norm_dirs.append(os.path.normpath(dir))

	return os.path.join(*tuple(norm_dirs))

def get_elementtree_as_string(root_element):
	"""
	Convert root element of an elementtree into a canonical string for comparint two XML trees

	Returns XML tree as a string
	"""
	dummy_file = StringIO.StringIO()
	indent_xml(root_element)
	e_tree = ET.ElementTree(root_element)
	e_tree.write_c14n(dummy_file)

	return dummy_file.getvalue()

def separator_replace_list(list_to_fix):
	"""
	Fix path separators in list elements
	"""
	new_list = []
	for element in list_to_fix:
		new_list.append(element.replace("\\", '/'))
	return new_list

def separator_replace_dic(dic_to_fix):
	"""
	Fix path separators in dictionary (both keys and values)
	and returns a sorted list.
	"""
	new_dic = {}
	for key, value in dic_to_fix.items():
		new_dic[key.replace("\\",'/')] = value.replace("\\",'/');
	return sorted(new_dic.items())

def remove_newlines(input_string):
	"""
	Remove all newline characters (\r\n) from a given string
	"""
	return input_string.replace('\r','').replace('\n','');


class SetupTestCase(unittest.TestCase):
	xml_basedir = os.path.normcase("asf/test-input")
	xml_filename = "default.xml"
	# If desired, specify a XML file to load the device map from
	# devmap_xml_filename = xml_filename # Can use same xml as is test input
	xml_validation = os.path.join("schemas", "fdk_content.xsd")

	def setUp(self):
		script_path = sys.path[0]

		# Load XML validation scheme
		ConfigDB.load_schema(self.xml_validation)
		# Set filename for test content XML
		ConfigDB.xml_filename = self.xml_filename

		# Load device map if one was specified
		try:
			devmap_xml_filepath = os.path.join(self.xml_basedir, self.devmap_xml_filename)
		except AttributeError:
			pass
		else:
			ConfigDB.load_device_map(devmap_xml_filepath)

		self.runtime = Runtime("", ConfigurationHandler())
		level = self.runtime.get_log_level_from_text("critical")
		self.runtime.setup_log(level)
		self.runtime.log.info("test")

		self.db = ConfigDB(self.runtime, extension=None)

	def tearDown(self):
		self.runtime = None
		self.db = None

	def assertItemsEqual(self, a, b):
		"""
		Compatibility for Python 2.6. Remove when 2.6 support is not needed.
		"""
		a.sort()
		b.sort()
		self.assertEqual(a,b)

class BuildTypeTestCase(SetupTestCase):
	xml_filename = "buildtypes.xml"
	application_id = "test.app"
	toolchain = "*"

	def test_class_to_type(self):
		class TestBuildType1(BuildType):
			type = "test_type"

		class TestBuildType2(TestBuildType1):
			subtype = "test_subtype_1"

		class TestBuildType3(TestBuildType2):
			subtype = "unused_test_subtype_1"

		class TestBuildType4(TestBuildType2):
			subtype = "unused_test_subtype_2"

		class TestBuildType5(TestBuildType4):
			subtype = "test_subtype_2"

		expected_build_type_1 = TestBuildType1
		expected_build_type_2 = TestBuildType2
		expected_build_type_3 = TestBuildType5

		build_type_1 = BuildType.type_to_class('test_type', None)
		build_type_2 = BuildType.type_to_class('test_type', 'test_subtype_1')
		build_type_3 = BuildType.type_to_class('test_type', 'test_subtype_2')

		self.assertEqual(build_type_1, expected_build_type_1)
		self.assertEqual(build_type_2, expected_build_type_2)
		self.assertEqual(build_type_3, expected_build_type_3)

		self.assertRaises(DbError, BuildType.type_to_class, 'test_type', 'no_such_subtype')

	def test_build_sort(self):
		expected_header_files = [
				os.path.join(self.xml_basedir, "test-api.h"),
				os.path.join(self.xml_basedir, "sort-test.h"),
				os.path.join(self.xml_basedir, "test.h")
		]
		expected_header_files.sort()

		test_app = self.db.lookup_by_id(self.application_id)

		header_files = test_app.get_build(BuildHeader, self.toolchain, sort=True)

		self.assertEqual(header_files, expected_header_files)

	def test_source_types(self):
		expected_asm_sources      = [os.path.join(self.xml_basedir, "test.s")]
		expected_c_sources        = [os.path.join(self.xml_basedir, "test.c")]
		expected_defines          = [("TEST", "test")]
		expected_header_files     = [
				os.path.join(self.xml_basedir, "test-api.h"),
				os.path.join(self.xml_basedir, "sort-test.h"),
				os.path.join(self.xml_basedir, "test.h")
		]
		expected_api_header_files = [os.path.join(self.xml_basedir, "test-api.h")]
		expected_include_paths    = [os.path.join(self.xml_basedir, "test")]

		test_app = self.db.lookup_by_id(self.application_id)

		asm_sources      = test_app.get_build(BuildAssembly, self.toolchain)
		c_sources        = test_app.get_build(BuildC, self.toolchain)
		defines          = test_app.get_build(BuildDefine, self.toolchain)
		header_files     = test_app.get_build(BuildHeader, self.toolchain)
		api_header_files = test_app.get_build(BuildApiHeader, self.toolchain)
		include_paths    = test_app.get_build(BuildInclude, self.toolchain)

		self.assertEqual(asm_sources, expected_asm_sources)
		self.assertEqual(c_sources, expected_c_sources)
		self.assertEqual(defines, expected_defines)
		self.assertItemsEqual(header_files, expected_header_files)
		self.assertEqual(api_header_files, expected_api_header_files)
		self.assertEqual(include_paths, expected_include_paths)

	def test_distribute_types(self):
		expected_distribute_files = [os.path.join(self.xml_basedir, "test.file")]
		expected_distribute_dirs  = [os.path.join(self.xml_basedir, "test_dir")]

		test_app = self.db.lookup_by_id(self.application_id)

		distribute_files = test_app.get_build(BuildDistributeFile, self.toolchain)
		distribute_dirs  = test_app.get_build(BuildDistributeDirectory, self.toolchain)

		self.assertEqual(distribute_files, expected_distribute_files)
		self.assertEqual(distribute_dirs, expected_distribute_dirs)

	def test_module_config_types(self):
		expected_module_config_headers = ["conf_test.h"]
		expected_module_config_paths   = [os.path.join(self.xml_basedir, "conf_path")]

		test_app = self.db.lookup_by_id(self.application_id)

		module_config_headers = test_app.get_build(BuildModuleConfigRequiredHeaderFile, self.toolchain)
		module_config_paths   = test_app.get_build(BuildModuleConfigPath, self.toolchain)

		self.assertEqual(module_config_headers, expected_module_config_headers)
		self.assertEqual(module_config_paths, expected_module_config_paths)

	def test_build_config_types(self):
		expected_assembler_flags    = ['-assembler-flag']
		expected_c_compiler_flags   = ['-c-compiler-flag']
		expected_cpp_compiler_flags = ['-cpp-compiler-flag']
		expected_preprocessor_flags = ['-preprocessor-flag']

		test_app = self.db.lookup_by_id(self.application_id)

		assembler_flags = test_app.get_build(BuildAssemblerFlags, self.toolchain)
		c_compiler_flags = test_app.get_build(BuildCCompilerFlags, self.toolchain)
		cpp_compiler_flags = test_app.get_build(BuildCppCompilerFlags, self.toolchain)
		preprocessor_flags = test_app.get_build(BuildPreprocessorFlags, self.toolchain)

		self.assertEqual(assembler_flags, expected_assembler_flags)
		self.assertEqual(c_compiler_flags, expected_c_compiler_flags)
		self.assertEqual(cpp_compiler_flags, expected_cpp_compiler_flags)
		self.assertEqual(preprocessor_flags, expected_preprocessor_flags)

	def test_linker_config_types(self):
		expected_linker_scripts = [os.path.join(self.xml_basedir, "test.script")]
		expected_linker_flags   = ["test-flags"]

		test_app = self.db.lookup_by_id(self.application_id)

		linker_scripts = test_app.get_build(BuildLinkerScript, self.toolchain)
		linker_flags   = test_app.get_build(BuildLinkerFlags, self.toolchain)

		self.assertEqual(linker_scripts, expected_linker_scripts)
		self.assertEqual(linker_flags, expected_linker_flags)

	def test_library_types(self):
		expected_compiler_libraries = ["libtest"]
		expected_user_libraries     = [os.path.join(self.xml_basedir, "test_lib.a")]

		test_app = self.db.lookup_by_id(self.application_id)

		compiler_libraries = test_app.get_build(BuildCompilerLibrary, self.toolchain)
		user_libraries     = test_app.get_build(BuildUserLibrary, self.toolchain)

		self.assertEqual(compiler_libraries, expected_compiler_libraries)
		self.assertEqual(user_libraries, expected_user_libraries)


class InfoTestCase(SetupTestCase):
	xml_filename = "info.xml"
	devmap_xml_filename = xml_filename

	def test_gui_flags(self):
		project = self.db.lookup_by_id("project")
		basic_module = self.db.lookup_by_id("basic")

		expected_gui_flag = "hidden"
		expected_recursive_gui_flag = ["hidden", "visible"]

		# Test non-recursive info function
		result = project.get_gui_flag_list(recursive=False)
		self.assertEqual(result, [expected_gui_flag])

		# Test recursive info function
		result = project.get_gui_flag_list(recursive=True)
		self.assertEqual(set(result), set(expected_recursive_gui_flag))

		# Test module without gui flag set:
		result = basic_module.get_gui_flag_list()
		self.assertEqual(result, [])


class CircularTestCase(SetupTestCase):
	xml_filename = "circular.xml"

	def test_circular(self):
		circular_id = "test1"

		self.assertRaises(ConfigError, self.db.lookup_by_id, circular_id)


class ConfigDBTestCase(SetupTestCase):
	devmap_xml_filename = SetupTestCase.xml_filename

	expected_modules = 8
	expected_boards = 2
	expected_projects = 5

	def test_modules(self):

		count = 0
		for module in self.db.iter_modules():
			count += 1

		self.assertEqual(count, self.expected_modules)

	def test_boards(self):

		count = 0
		for module in self.db.iter_components(Board):
			count += 1

		self.assertEqual(count, self.expected_boards)

	def test_projects(self):

		count = 0
		for project in self.db.iter_projects():
			count += 1

		self.assertEqual(count, self.expected_projects)

	def test_lookup(self):
		discoverable_lookupid = "basic"
		non_discoverable_lookupid = "myproj"

		expected_module = "basic"
		expected_module_type = Module
		not_expected_module_type = Project

		non_existing_module = "om nom nom"

		# Lookup one specific driver
		result = self.db.lookup_by_id(discoverable_lookupid)
		self.assertEquals(expected_module, result.id)
		assert isinstance(result, expected_module_type)

		# Set the expected_module element as root node
		root_element = result.element

		# Test relative lookup from the root node, it should find the module
		result = self.db.relative_lookup_by_id(root_element, discoverable_lookupid)
		self.assertEquals(expected_module, result.id)

		# Test relative lookup under a root node, it should not find the module
		self.assertRaises(NotFoundError, self.db.relative_lookup_by_id, root_element, non_discoverable_lookupid)

		# Check that we cannot load the driver with a different class
		self.assertRaises(AssertionError, self.db.lookup_by_id, discoverable_lookupid, not_expected_module_type)

		# Check that we get an error if we try to load a non existing module
		self.assertRaises(NotFoundError, self.db.lookup_by_id, non_existing_module)

	def test_find_components(self):
		expected_unit_test_id = ['statusproj1']
		expected_normal_id    = ['statusproj2']
		expected_typed_ids    = expected_unit_test_id + expected_normal_id
		expected_typeless_ids = ['gen_name_proj', 'my_name_proj', 'myproj'] + expected_typed_ids

		unit_test_e = self.db.find_components(Project, {'type' : 'unit-test', 'caption' : 'status project 1'})
		normal_e    = self.db.find_components(Project, {'type' : 'normal', 'caption' : None})
		typed_e     = self.db.find_components(Project, {'type' : None})
		typeless_e  = self.db.find_components(Project)

		unit_test_id = [e.attrib['id'] for e in unit_test_e]
		normal_id    = [e.attrib['id'] for e in normal_e]
		typed_ids    = [e.attrib['id'] for e in typed_e]
		typeless_ids = [e.attrib['id'] for e in typeless_e]

		expected_typed_ids.sort()
		expected_typeless_ids.sort()

		typed_ids.sort()
		typeless_ids.sort()

		self.assertEquals(expected_unit_test_id, unit_test_id)
		self.assertEquals(expected_normal_id, normal_id)
		self.assertEquals(expected_typed_ids, typed_ids)
		self.assertEquals(expected_typeless_ids, typeless_ids)

	def test_find_invalid_components(self):
		# Ensure that a TypeError is raised if searching for a component
		# which the database is not able to instantiate from ASF XML.
		self.assertRaises(TypeError, self.db.find_components, UnitTestConfigItem)
		self.assertRaises(TypeError, self.db.find_components, UnitTestDummyClass)

	def test_reverse_lookup(self):
		lookupid = "basic"

		expected_proj   =  {
			"myproj" : Module,
			"statusproj1" : Module
		}
		expected_module =  { "avr32.driver.test" : Module }

		# Test reverse lookup of a driver and verify that we get the expected ids and classes
		result = self.db.reverse_lookup_by_id(lookupid)
		ids = {}
		for r in result:
			ids[r.id] = r.__class__

		expect = dict()
		expect.update(expected_proj)
		expect.update(expected_module)
		self.assertEquals(expect, ids)


class AppBuilderTestCase(SetupTestCase):
	xml_filename = "appbuilder.xml"
	devmap_xml_filename = xml_filename

	def test_find_modules_for_mcu_xmega(self):
		expected = ["xmega.driver.1", "xmega.driver.3"]
		mcu = MCU("atxmega128a1", self.db)

		result = [x.id for x in self.db.find_modules_for_mcu(mcu, types=["driver", "service", "component"])]

		self.assertEquals(result, expected)

	def test_find_modules_for_mcu_uc3(self):
		expected = ["uc3.driver.1", "uc3.driver.2", "uc3.driver.3"]

		mcu = MCU("at32uc3l0256", self.db)
		result = [x.id for x in self.db.find_modules_for_mcu(mcu, types=["driver", "service", "component"])]

		self.assertEquals(result, expected)

	def test_hidden(self):
		expected = "xmega.driver.hidden"
		mcu = MCU("atxmega128a1", self.db)

		not_hidden = [x.id for x in self.db.find_modules_for_mcu(mcu, types=["driver", "service", "component"])]
		all = [x.id for x in self.db.find_modules_for_mcu(mcu, types=["driver", "service", "component"], check_hidden=False)]

		# Find the hidden modules:
		hidden = set(all).difference(set(not_hidden)).pop()

		self.assertEquals(hidden, expected)

	def test_types(self):
		expected_dict = {
				"driver" : ["xmega.driver.1"],
				"service" : ["xmega.driver.3"],
				}
		mcu = MCU("atxmega128a1", self.db)

		for types, expected in expected_dict.items():
			result = [x.id for x in self.db.find_modules_for_mcu(mcu, types=types)]
			self.assertEquals(result, expected)

	def test_no_type(self):
		expected = ["xmega.driver.example1", "xmega.driver.1", "xmega.driver.3"]
		mcu = MCU("atxmega128a1", self.db)

		result = [x.id for x in self.db.find_modules_for_mcu(mcu)]
		self.assertEquals(result, expected)


class ConfigItemTestCase(SetupTestCase):
	devmap_xml_filename = SetupTestCase.xml_filename

	def test_module(self):
		module_to_test = "avr32.driver.test"
		expected_dev_list = ["uc3a", "xmega"]
		expected_appnotes = ["AVR123", "AVR234"]
		expected_keyword = { 'testcategory' : ["test1", "test2"] }

		# Load module
		module = self.db.lookup_by_id(module_to_test)

		# Test property functions
		assert module.id is not None
		assert module.type is not None
		assert module.caption is not None
		assert module.toolchain is None

		# Test info element functions
		# Test device-support element functions
		dev_list = module.get_device_support()
		self.assertEquals(dev_list, expected_dev_list)

		# Test appnote element functions
		appnotes = module.get_appnote_list()
		self.assertItemsEqual(appnotes, expected_appnotes)

		# Test keyword element functions
		keywords = module.get_keyword_dict()
		self.assertEquals(keywords, expected_keyword)

		# Test visualization. We do not validate the result, but check that the function runs without errors
		module.visualize_info(self._input_eater)
		module.visualize_requirements(self._input_eater)

	def _input_eater(self, *args):
		# Do nothing, just eat the input
		pass

	def test_board(self):
		board_to_test = "avr32.boards.evk1100"
		expected_mcu = "at32uc3a0512"

		# Load board
		board = self.db.lookup_by_id(board_to_test)


		# Test property functions
		assert board.id is not None
		assert board.type is not None
		assert board.caption is not None
		assert board.toolchain is None

		self.assertEquals(board.mcu.name, expected_mcu)
		self.assertEquals(board.get_device_support(), [expected_mcu])

	def test_project(self):
		project_to_test = "myproj"
		expected_mcu = "at32uc3a0512"
		expected_board = "avr32.boards.evk1100"
		expected_generators = ["verbose", "doxygen"]
		non_existent_config = "nonexistent"
		existent_config = "cookiemonster"
		existent_value = "C is for cookie"

		# Load project
		project = self.db.lookup_by_id(project_to_test)

		# Test property functions
		assert project.id is not None
		assert project.type == Project.default_type
		assert project.caption is not None
		assert project.toolchain is None

		self.assertEquals(project.mcu.name, expected_mcu)
		self.assertEquals(project.board, expected_board)

		# Test generators
		self.assertEquals(project.get_generator_list(), expected_generators)

		# Test group config functions
		self.assertEquals(project.get_config_group(non_existent_config), {})

		# Test config functions
		self.assertEquals(project.get_config(existent_config), existent_value)

		# Test device support
		self.assertEquals(project.get_device_support(), [expected_mcu])

	def test_project_build(self):
		project_to_test = "myproj"
		generators = ["verbose", "doxygen"]

		expected_defines_verbose = [ ('BOARD', 'EVK1100'), ('MYPROJ', '1') ]
		expected_defines_doxygen = [ ('BOARD', 'EVK1100') ]

		# Load project
		project = self.db.lookup_by_id(project_to_test)

		self.assertEquals(project.get_build(BuildDefine, "verbose"), expected_defines_verbose)
		self.assertEquals(project.get_build(BuildDefine, "doxygen"), expected_defines_doxygen)

	def test_project_workspace_name(self):
		my_name_project = self.db.lookup_by_id("my_name_proj")
		expected_my_name = "my_name"

		no_name_project = self.db.lookup_by_id("gen_name_proj")
		expected_no_name = None

		my_name = my_name_project.workspace_name
		no_name = no_name_project.workspace_name

		self.assertEquals(my_name, expected_my_name)
		self.assertEquals(no_name, expected_no_name)

	def test_resolve_device_support_wrong(self):
		module_to_test_1 = self.db.lookup_by_id('devicesuppdriver_wrong')

		mcu = MCU("atxmega128a1", self.db)
		map = mcu.get_group_map()

		expected_not_supported_1 = []
		expected_not_supported_2 = ["devicesuppdriver_wrong"]
		expected_not_supported_3 = ["devicesuppdriver_wrong","devicesuppdriver3"]

		result = [module.id for module in module_to_test_1.resolve_device_support(map, recursive=False, check_self=False)]
		self.assertEquals(result, expected_not_supported_1)

		result = [module.id for module in module_to_test_1.resolve_device_support(map, recursive=False, check_self=True)]
		self.assertEquals(result, expected_not_supported_2)

		result = [module.id for module in module_to_test_1.resolve_device_support(map, recursive=True, check_self=True)]
		self.assertEquals(result, expected_not_supported_3)

	def test_resolve_device_support_correct(self):
		module_to_test_1 = self.db.lookup_by_id('devicesuppdriver_correct')

		mcu = MCU("atxmega128a1", self.db)
		map = mcu.get_group_map()

		expected_not_supported_1 = []
		expected_not_supported_2 = []
		expected_not_supported_3 = ["devicesuppdriver3"]

		result = [module.id for module in module_to_test_1.resolve_device_support(map, recursive=False, check_self=False)]
		self.assertEquals(result, expected_not_supported_1)

		result = [module.id for module in module_to_test_1.resolve_device_support(map, recursive=False, check_self=True)]
		self.assertEquals(result, expected_not_supported_2)

		result = [module.id for module in module_to_test_1.resolve_device_support(map, recursive=True, check_self=True)]
		self.assertEquals(result, expected_not_supported_3)

	def test_description(self):
		module = self.db.lookup_by_id("information")

		expected_summary = "Summary"
		expected_detailed = "Detailed\nDetailed"

		# Check duplicate summary:
		result = module.get_description_summary_text()
		self.assertEquals(result, expected_summary)

		# Check detailed description
		result = module.get_description_detailed_text()
		self.assertEquals(result, expected_detailed)

class DeviceMapTestCase(SetupTestCase):
	xml_filename = "devicemap.xml"
	devmap_xml_filename = xml_filename

	def test_mcu_list(self):
		device_map = self.db.lookup_by_id("map.atmel")

		all_mcus_group = "avr"

		expected_mcus = ['atxmega64a1', 'atxmega128a1', 'atxmega63a3', 'atxmega32a4', 'at32uc3a0512']
		expected_mcus.sort()

		group_a1 = "xmegaa1"
		expected_mcus_group_a1 = ['atxmega64a1', 'atxmega128a1']
		expected_mcus_group_a1.sort()


		# Load all MCUs in devicemap
		supported_list = device_map.get_mcu_list()
		supported_list.sort()

		self.assertEquals(supported_list, expected_mcus)

		# Load all MCUs in the avr group (which should be all)
		group_supported_list = device_map.get_mcu_list(all_mcus_group)
		group_supported_list.sort()

		self.assertEquals(group_supported_list, expected_mcus)

		# Load all MCUs in a group that is split into two
		group_supported_list = device_map.get_mcu_list(group_a1)
		group_supported_list.sort()

		self.assertEquals(group_supported_list, expected_mcus_group_a1)

	def test_mcu_list(self):
		device_map = self.db.lookup_by_id("map.atmel")

		mcu_to_test = "at32uc3a0512"
		expected_selection_order = ["at32uc3a0512", "uc3a0", "uc3a", "ucr2", "uc3", "avr"]

		order = list()
		for m in device_map.iter_selection_list(mcu_to_test):
			order.append(m)

		self.assertEquals(order, expected_selection_order)

	def test_mcu_object(self):
		mcu_name = "atxmega128a1"
		mcu_selection_list = ["atxmega128a1", "xmegaa1", "xmegaa", "xmega", "avr"]
		mcu = MCU(mcu_name, self.db)

		# Check that unknown map cannot be loaded
		self.assertRaises(NotFoundError, MCU, mcu_name, self.db, map_id="unknown_map")

		# Check that the wrong class (driver) cannot be loaded as devicemap
		self.assertRaises(ConfigError, MCU, mcu_name, self.db, map_id="mydriver")

		# Check that we cannot create a MCU with a mcu name that does not exist
		self.assertRaises(DbError, MCU, "nonexistent_mcu", self.db)

		# Check name is correct
		self.assertEquals(mcu.name, mcu_name)

		# Check parents
		self.assertEquals(mcu.get_group_map(), mcu_selection_list)

	def test_mcu_doc_arch_group(self):
		# Expected mapping between MCU name and doc-arch
		expected_doc_arch = {
			'atxmega32a4'  : 'xmegaa',
			'at32uc3a0512' : 'uc3a',
		}

		# Get the MCU names
		mcus_to_test = expected_doc_arch.keys()

		# Generate map from MCU name to doc-arch
		doc_arch_list = [MCU(mcu_name, self.db).get_doc_arch_group() for mcu_name in mcus_to_test]
		doc_arch = dict(zip(mcus_to_test, doc_arch_list))

		# Check that generated map is as expected
		self.assertEquals(doc_arch, expected_doc_arch)

	def test_mcu_doc_arch_name(self):
		# Expected mapping between MCU name and doc-arch
		expected_doc_arch = {
			'atxmega32a4'  : 'XMEGAA',
			'at32uc3a0512' : 'UC3A',
		}

		# Get the MCU names
		mcus_to_test = expected_doc_arch.keys()

		# Generate map from MCU name to doc-arch
		doc_arch_list = [MCU(mcu_name, self.db).get_doc_arch_name() for mcu_name in mcus_to_test]
		doc_arch = dict(zip(mcus_to_test, doc_arch_list))

		# Check that generated map is as expected
		self.assertEquals(doc_arch, expected_doc_arch)

	def test_get_doc_arch_dict(self):
		device_map = self.db.lookup_by_id("map.atmel")

		# Expected map between group and doc-arch name
		expected_doc_arch_dict = {
			'xmegaa' : 'XMEGAA',
			'uc3a' : 'UC3A',
		}

		# Get device map's mapping from groups to their doc-arch names
		doc_arch_dict = device_map.get_doc_arch_dict()

		# Check that generated map is as expected
		self.assertEquals(doc_arch_dict, expected_doc_arch_dict)


class DeviceAliasMapTestCase(SetupTestCase):
	xml_filename = "devicealiasmap.xml"
	devmap_xml_filename = xml_filename

	def test_device_support(self):
		expected_device_support_1 = ['xmegaa1', 'xmegaa3']
		expected_device_support_2 = ['atxmega128a1', 'uc3']
		expected_device_support_3 = expected_device_support_1 + expected_device_support_2

		module_1 = self.db.lookup_by_id("driver_1") 
		module_2 = self.db.lookup_by_id("driver_2") 
		module_3 = self.db.lookup_by_id("driver_3") 

		device_support_1 = module_1.get_device_support()
		device_support_2 = module_2.get_device_support()
		device_support_3 = module_3.get_device_support()

		self.assertEquals(device_support_1, expected_device_support_1)
		self.assertEquals(device_support_2, expected_device_support_2)
		self.assertEquals(device_support_3, expected_device_support_3)


class ProjectGeneratorTestCase(SetupTestCase):
	xml_filename = "project-generator.xml"
	devmap_xml_filename = xml_filename

	def test_get_application_modules(self):
		project = self.db.lookup_by_id("project")
		generator = GenericProject(project, self.db, self.runtime)

		expected_app_modules = [
				self.db.lookup_by_id("testapp#xmegaa"),
				self.db.lookup_by_id("appmeta1"),
				self.db.lookup_by_id("appmeta2"),
			]

		app_modules = generator._get_application_modules()

		expected_app_modules.sort()
		app_modules.sort()

		self.assertEquals(app_modules, expected_app_modules)

	def test_get_api_modules(self):
		project = self.db.lookup_by_id("project")
		generator = GenericProject(project, self.db, self.runtime)

		expected_api_modules = [
				self.db.lookup_by_id("driver1"),
				self.db.lookup_by_id("driver2"),
			]

		api_modules = generator._get_api_modules_files()
		# Get the modules which are stored in the dictionary keys
		api_modules = api_modules.keys()

		api_modules.sort()
		expected_api_modules.sort()


		self.assertEquals(api_modules, expected_api_modules)

	def test_get_workspace_name(self):
		# Project with custom name
		my_name_project = self.db.lookup_by_id("my_name_project")
		my_name_generator = GenericProject(my_name_project, self.db, self.runtime)
		expected_my_name = "my_name"

		# Project with name resolved from driver example
		gen_name_project_1 = self.db.lookup_by_id("gen_name_project_1")
		gen_name_generator_1 = GenericProject(gen_name_project_1, self.db, self.runtime)
		expected_gen_name_1 = "module_example"

		# Project with name resolved from application
		gen_name_project_2 = self.db.lookup_by_id("gen_name_project_2")
		gen_name_generator_2 = GenericProject(gen_name_project_2, self.db, self.runtime)
		expected_gen_name_2 = "some_awesome_example"

		# Project with failing name resolving
		fail_name_project = self.db.lookup_by_id("fail_name_project")
		fail_name_generator = GenericProject(fail_name_project, self.db, self.runtime)
		expected_fail_name = "fail_application"

		my_name = my_name_generator.get_workspace_name()
		gen_name_1 = gen_name_generator_1.get_workspace_name()
		gen_name_2 = gen_name_generator_2.get_workspace_name()
		fail_name = fail_name_generator.get_workspace_name()

		self.assertEquals(my_name, expected_my_name)
		self.assertEquals(gen_name_1, expected_gen_name_1)
		self.assertEquals(gen_name_2, expected_gen_name_2)
		self.assertEquals(fail_name, expected_fail_name)


class ModuleSelectorTestCase(SetupTestCase):
	xml_filename = "module-selector.xml"
	devmap_xml_filename = xml_filename

	def test_select_by_device(self):
		project = self.db.lookup_by_id("project")
		selector = self.db.lookup_by_id("devselect1")

		expected_module = self.db.lookup_by_id("devselect1#xmegaa")
		expected_device_support = ['ucr2','xmegaa']

		# Create a config
		config = ConfigurationHandler()

		# Verify the correct device-support is found by the selector
		# Convert the list to a set to ensure that the order does not matter
		device_support = selector.get_device_support()
		self.assertEquals(set(device_support), set(expected_device_support))

		# Verify that the correct module is selected
		self.assertEquals(selector.select_module(config, project.mcu), expected_module)

	def test_select_by_device_build(self):
		project = self.db.lookup_by_id("project")
		module = self.db.lookup_by_id("testdev")

		expected_headers = [os.path.join(self.xml_basedir,"xmega.h")]

		# Create a config
		config = ConfigurationHandler()

		# Resolve all selections
		project.resolve_all_selections(config, project.mcu)

		self.assertEquals(module.get_build(BuildHeader, None), expected_headers)

	def test_select_by_config(self):
		project = self.db.lookup_by_id("project")
		selector = self.db.lookup_by_id("confselect1")

		configuration_name = "config.selection"
		valid_conf = "speed"
		expected_module = self.db.lookup_by_id("confselect1#speed")

		invalid_conf = "cookiemonster"

		# Create a config
		config = ConfigurationHandler()

		# If no config value is set and there is no default value
		# it should fail:
		self.assertRaises(ConfigError, selector.select_module, config, project.mcu)

		# Set configuration
		config.set_config(configuration_name, valid_conf)

		# Now check that it is able to select the correct module
		self.assertEquals(selector.select_module(config, project.mcu), expected_module)

		# Set invalid configuration
		config.set_config(configuration_name, invalid_conf)

		# Now check it is not able to find a match
		self.assertRaises(NoSelectionError, selector.select_module, config, project.mcu)

	def test_select_by_config_default(self):
		project = self.db.lookup_by_id("project")
		selector = self.db.lookup_by_id("confselect2")

		expected_module = self.db.lookup_by_id("confselect2#speed")

		# Create a config
		config = ConfigurationHandler()

		# Check that we can load the default value when no configuration is set
		self.assertEquals(selector.select_module(config, project.mcu), expected_module)

	def test_select_by_config_and_device(self):
		project = self.db.lookup_by_id("project_confdevselect1")
		selector = self.db.lookup_by_id("confdevselect1")

		expected_headers = [os.path.join(self.xml_basedir,"option2_xmega.h")]
		expected_defines = [("definename", "define22")]
		expected_appnotes = ["avr22"]
		expected_keywords = {"category" : [ "test22-1", "test22-2" ]}

		# Create a config
		conf = ConfigurationHandler()

		# Resolve all selections
		project.resolve_all_selections(conf, project.mcu)

		# Test for the correct header files for the project
		self.assertEquals(project.get_build(BuildHeader, None), expected_headers)

		# Test for the correct header files for the project
		self.assertEquals(project.get_build(BuildDefine, None), expected_defines)

		# Test for the correct appnote list
		self.assertEquals(set(selector.get_appnote_list(recursive=True)), set(expected_appnotes))
		self.assertEquals(set(selector.get_appnote_list(recursive=False)), set([]))

		# Test for the correct keyword list
		self.assertEquals(selector.get_keyword_dict(recursive=True), expected_keywords)
		self.assertEquals(selector.get_keyword_dict(recursive=False), {})

	def test_select_by_config_and_default(self):
		project = self.db.lookup_by_id("project_conf_default")

		expected_module = self.db.lookup_by_id("confdef1#expected")
		expected_header = [os.path.join(self.xml_basedir, 'expected.h')]

		# Create a config
		#config = ConfigurationHandler()
		project.resolve_all_selections(project.get_configuration(), project.mcu)

		header = project.get_build(BuildHeader, None)

		self.assertEquals(header, expected_header)

	def test_subids_device(self):
		module = self.db.lookup_by_id("devselect1")

		expected_ids = ["ucr2", "xmegaa"]
		expected_ids_nostrip = ["devselect1#ucr2", "devselect1#xmegaa"]
		expected_ids_given_mcu = ["xmegaa"]

		self.assertItemsEqual(module.get_subids(strip_ids=False), expected_ids_nostrip)

		self.assertItemsEqual(module.get_subids(strip_ids=True), expected_ids)

		mcu = MCU("atxmega128a1", self.db)

		self.assertItemsEqual(module.get_subids(strip_ids=True, mcu=mcu), expected_ids_given_mcu)

	def test_subids_config(self):
		module = self.db.lookup_by_id("confselect1")

		expected_ids = ["speed", "size"]
		expected_ids_given_mcu = ["size"]

		self.assertEquals(module.get_subids(strip_ids=True), expected_ids)

		mcu = MCU("atxmega128a1", self.db)

		self.assertEquals(module.get_subids(strip_ids=True, mcu=mcu), expected_ids_given_mcu)

class ProjectConfigTestCase(SetupTestCase):
	xml_filename = "configuration.xml"
	devmap_xml_filename = xml_filename

	def test_config_simple(self):
		project = self.db.lookup_by_id("project1")
		config_group = "compiler"
		expected_config = {"conf1" : "val1", "conf2" : "val2", "conf3" : "val3" }

		single_config = "single1"
		expected_single_config = "singleval1"

		self.assertEqual(project.get_config_group(config_group), expected_config)
		self.assertEqual(project.get_config(single_config), expected_single_config)


	def test_config_duplicate(self):
		self.assertRaises(ConfigError, self.db.lookup_by_id, "project2")
		#project = self.db.lookup_by_id("project2")
		#config_group = "duplicate"
		#single_config = "dup_single"

		#self.assertRaises(ConfigError, project.get_config_group, config_group)
		#self.assertRaises(ConfigError, project.get_config, single_config)

	def test_config_inheritance(self):
		project = self.db.lookup_by_id("project3")
		config_group = "compiler"
		expected_config = {"conf1" : "val1", "conf2" : "val2", "conf3" : "board3" }

		config_single_1 = "conf1"
		expected_single_1 = "val_project1"

		config_single_2 = "conf2"
		expected_single_2 = "val_board2"

		self.assertEqual(project.get_config_group(config_group), expected_config)

		self.assertEqual(project.get_config(config_single_1), expected_single_1)

		self.assertEqual(project.get_config(config_single_2), expected_single_2)

	def test_config_override(self):
		project = self.db.lookup_by_id("project3")
		config_group = "compiler"
		expected_config = {"conf1" : "val1", "conf2" : "val2", "conf3" : "board3" }
		change_config = "compiler.conf2"
		change_config_value = "override_group"
		change_expected_config = {"conf1" : "val1", "conf2" : change_config_value, "conf3" : "board3" }

		config_single_1 = "conf1"
		original_single_1 = "val_project1"
		changed_single_1 = "override1"

		# Instansiate toolchain generator to be able to check configuration overriding
		toolchain = GenericProject(project, self.db, self.runtime)

		# Verify values before overriding
		self.assertEqual(toolchain.get_config(config_single_1), original_single_1)
		self.assertEqual(toolchain.get_config_group(config_group), expected_config)

		# Override values
		self.runtime.set_config(config_single_1, changed_single_1)

		# Verify the result
		self.assertEqual(toolchain.get_config(config_single_1), changed_single_1)

		# Override group value
		self.runtime.set_config(change_config, change_config_value)
		# Verify reading back
		self.assertEqual(toolchain.get_config_group(config_group), change_expected_config)


class AVRStudio5TestCase(SetupTestCase):
	xml_filename = "avrstudio5.xml"
	devmap_xml_filename = xml_filename


	def test_get_build_file_dict_from_modules(self):
		project_to_test = self.db.lookup_by_id("xmega.driver.awesome.example1.xplain")
		prereq_to_test = self.db.lookup_by_id("xmega.driver.awesome.example1")

		expected_base_file = os.path.join(self.xml_basedir, 'example1')
		expected_file_dict = {
			BuildC        : {expected_base_file + '.c' : [None, prereq_to_test , None]},
			BuildAssembly : {expected_base_file + '.s' : [None, prereq_to_test , None]},
			BuildHeader   : {expected_base_file + '.h' : [None, prereq_to_test , None]},
			BuildUserLibrary  : {expected_base_file + '.a' : [None, prereq_to_test , None]},
			BuildDistributeFile  : {expected_base_file + '.d' : [None, prereq_to_test , None]},
			BuildDistributeUserFile  : {expected_base_file + '.u' : [None, prereq_to_test , None]},
			BuildDistributeLicense : {expected_base_file + '.l' : [None, prereq_to_test , None]},
		}

		generator = AVRStudio5Project(project_to_test, self.db, self.runtime)
		file_dict = generator._get_build_file_dict_from_modules([prereq_to_test])

		self.assertEqual(file_dict, expected_file_dict)


	def test_add_files_to_vstemplate_project(self):
		project_to_test = self.db.lookup_by_id("xmega.driver.awesome.example1.xplain")

		# Load expected XML output for vstemplate
		input_file = os.path.join(self.xml_basedir, 'avrstudio5_vstemplate_project.xml')
		expected_tree = ET.parse(input_file)
		expected_root = expected_tree.getroot()

		expected_xml_output = get_elementtree_as_string(expected_root)

		# Generate vstemplate output
		generator = AVRStudio5Project(project_to_test, self.db, self.runtime)

		file_list = [
			[os.path.join('dir1', 'sub1')],
			[os.path.join('dir2', 'dir3', 'subsub1')],
			['root'],
			[os.path.join('dir2', 'sub2')],
			[os.path.join('dir2', 'dir4', 'subsub2')],
			[os.path.join('dir2', 'dir4', 'subsub3')],
			[os.path.join('dir2', 'sub2')],
		]

		root_e = ET.Element('Project')
		generator._add_files_to_vstemplate_project(root_e, file_list)

		xml_output = get_elementtree_as_string(root_e)

		self.assertEqual(xml_output, expected_xml_output)


	def test_get_project_data(self):
		# Extension dummy class needed to generate project data
		class DummyExtension(object):
			org = 'Atmel'
			shortname = 'ASF'
			version = '1.2.3'
			root_path = '.'

		project_to_test = self.db.lookup_by_id("xmega.driver.awesome.example1.xplain")

		expected_name = 'AWESOME_EXAMPLE1'
		expected_caption = 'Awesome driver example 1 - Xplain'
		expected_caption_detailed = expected_caption + ' - ATxmega128A1'
		expected_description = 'This is awesome. And this works just fine. [%s]' % (expected_caption_detailed)

		generator = AVRStudio5Project(project_to_test, self.db, self.runtime)
		self.db.extension = DummyExtension()
		generator._get_project_data()

		self.assertEqual(generator.project_name, expected_name)
		self.assertEqual(generator.project_caption, expected_caption)
		self.assertEqual(generator.project_description, expected_description)


	def test_get_toolchain_configuration(self):
		project_to_test = self.db.lookup_by_id("xmega.driver.awesome.example1.xplain")
		generator = AVRStudio5Project(project_to_test, self.db, self.runtime)

		expected_configs = {
			'AvrGcc.Project.Only'      : 'Project',
			'AvrGcc.Board.And.Project' : 'Project',
			'AvrGcc.Board.Only'        : 'Board',
		}
		expected_list_configs = {
			'AvrGcc.Board.Only.List'        : ['Board'],
			'AvrGcc.Board.And.Project.List' : ['Board', 'Project'],
			'AvrGcc.Project.Only.List'      : ['Project_1', 'Project_2']
		}

		(configs, list_configs) = generator.get_toolchain_configuration()

		self.assertEquals(configs, expected_configs)
		self.assertEquals(list_configs, expected_list_configs)


	def test_generate_vstemplate_tree(self):
		pass

class TruncStringTestCase(unittest.TestCase):
	def test_trunc_string(self):
		self.assertEqual(trunc_string("asdf",2), "..")
		self.assertEqual(trunc_string("asdf",4), "asdf")
		self.assertEqual(trunc_string("asdf",5), "asdf")
		self.assertEqual(trunc_string("asdfgh",5), "asd..")
		self.assertEqual(trunc_string(None,5), None)

class AtmelStudioIntegrationTestCase(unittest.TestCase):
	xml_basedir = os.path.normcase("asf/test-input/studio_integration/")
	device_map_file = os.path.join(xml_basedir, "devicemap.xml")
	facade = None

	def setUp(self):
		# Instantiate facade object with test extension and device map
		facade = PythonFacade([self.xml_basedir], self.device_map_file)
		level = facade.runtime.get_log_level_from_text("critical")
		facade.runtime.setup_log(level)

		main_db = facade.get_database("Atmel.ASF")
		main_ext = main_db.extension

		self.facade = facade
		self.main_db = main_db
		self.main_ext = main_ext

	def tearDown(self):
		self.facade = None
		self.main_db = None
		self.main_ext = None

	def test_PythonFacade_get_all_mcus(self):
		expected_result = ['atxmega128a1', 'atxmega128b1', 'at32uc3a0128']
		expected_result.sort()
		result = self.facade.get_all_mcus()
		result.sort()
		self.assertEqual(expected_result, result)

	def test_PythonFacade_get_asf_h_content(self):
		# Find all modules for our project
		project = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained")
		# Resolve all selections before fetching prerequisites
		project.resolve_all_selections(self.facade.configuration, project.mcu)
		modules = project.get_prerequisites(recursive=True)

		# Read expected output
		asf_h_template = os.path.join(self.xml_basedir, "asf_h.template")
		expected_file_name = os.path.join(self.xml_basedir, "asf_h.out")
		file = open(expected_file_name, 'r')
		expected_result = file.read()
		file.close()
		# Remove line ending characters
		expected_result = remove_newlines(expected_result)

		# Generate our asf.h
		result = self.main_ext.get_asf_h_content("as5_8", modules, asf_h_template)
		# Remove line ending characters
		result = remove_newlines(result)
		# Compare
		self.assertEqual(expected_result, result)

	def test_PythonFacade_get_items_for_mcu(self):
		expected_result = [self.main_db.lookup_by_id("avr32.drivers.tc"),
				self.main_db.lookup_by_id("avr32.drivers.tc.example")]
		expected_result.sort()

		result = self.main_ext.get_items_for_mcu(MCU("at32uc3a0128", self.main_db))
		result.sort()
		self.assertEqual(expected_result, result)

	def test_PythonFacade_get_items_for_mcu_filtered(self):
		expected_result = [self.main_db.lookup_by_id("avr32.drivers.tc")]
		expected_result.sort()

		result = self.main_ext.get_items_for_mcu(MCU("at32uc3a0128", self.main_db), mod_types_to_get=["driver"])
		result.sort()
		self.assertEqual(expected_result, result)

	def test_PythonFacade_get_all_modules_and_selectors(self):
		expected_result = [self.main_db.lookup_by_id("avr32.drivers.tc"),
				self.main_db.lookup_by_id("avr32.drivers.tc.example"),
				self.main_db.lookup_by_id("config.board.xmega_a1_xplained.init"),
				self.main_db.lookup_by_id("config.board.xmega_a1_xplained.led")]
		expected_result.sort()

		result = self.main_ext.get_all_modules_and_selectors("at32uc3a0128")
		result.sort()
		self.assertEqual(expected_result, result)

	def test_PythonFacade_get_all_modules_and_selectors_filtered(self):
		expected_result = [self.main_db.lookup_by_id("avr32.drivers.tc"),
				self.main_db.lookup_by_id("config.board.xmega_a1_xplained.init"),
				self.main_db.lookup_by_id("config.board.xmega_a1_xplained.led")]
		expected_result.sort()

		result = self.main_ext.get_all_modules_and_selectors("at32uc3a0128", mod_types_to_ignore=["application"])
		result.sort()
		self.assertEqual(expected_result, result)

	def test_PythonFacade_get_all_project_ids(self):
		expected_result = ['avr32.drivers.tc.example.a', 'xmega.drivers.nvm.example1.xmega_a1_xplained']
		expected_result.sort()

		result = self.main_ext.get_all_project_ids()
		result.sort()

		self.assertEqual(expected_result, result)

	def test_PythonFacade_write_project_data_to_xml_file(self):
		# Load expected XML output for vstemplate
		input_file = os.path.join(self.xml_basedir, "project_data.xml")
		expected_tree = ET.parse(input_file)
		expected_root = expected_tree.getroot()
		expected_result = get_elementtree_as_string(expected_root)
		# Remove line ending characters
		expected_result = expected_result.replace('\r','').replace('\n','');

		# Write xml file
		output_file_name = os.path.join(self.xml_basedir, "delete.me")
		projects = self.main_ext.get_all_project_ids()
		projects.sort()
		self.main_ext.write_project_data_to_xml_file(projects, output_file_name)

		# Read back xml file
		expected_tree = ET.parse(output_file_name)
		expected_root = expected_tree.getroot()
		result = get_elementtree_as_string(expected_root)
		# Remove line ending characters
		result = result.replace('\r','').replace('\n','');

		# Delete newly created file
		os.remove(output_file_name)

		# Compare files
		self.assertEqual(expected_result, result)

	def test_PythonFacade_get_mcu(self):
		my_mcu = MCU("atxmega128a1", self.main_db)
		mcu = self.main_ext.get_mcu("atxmega128a1")
		# Compare each data member in Mcu
		bool_res = (my_mcu.mcu_name == mcu.mcu_name and my_mcu.db == mcu.db
			and my_mcu.map_id == mcu.map_id)
		self.assertEqual(bool_res, True)

	def test_PythonFacade_get_generator_from_project_id(self):
		project = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained")
		expected_result = AVRStudio5Project(project, self.main_db, self.facade.runtime)

		result = self.main_ext.get_generator_from_project_id("xmega.drivers.nvm.example1.xmega_a1_xplained")
		self.assertEqual(expected_result.generator_tag, result.generator_tag)

	def test_MCU_get_group_map(self):
		expected_result =  ['atxmega128a1', 'avr', 'xmega', 'xmegaa', 'xmegaa1']
		result = self.facade.get_mcu("atxmega128a1").get_group_map()
		result.sort()
		self.assertEqual(expected_result, result)

	def test_MCU_get_doc_arch_group(self):
		# Expected mapping between MCU name and doc-arch
		expected_doc_arch = {
			'atxmega128a1'  : 'xmegaa',
			'at32uc3a0128' : 'uc3a',
		}

		# Get the MCU names
		mcus_to_test = expected_doc_arch.keys()

		# Generate map from MCU name to doc-arch
		doc_arch_list = [MCU(mcu_name, self.main_db).get_doc_arch_group() for mcu_name in mcus_to_test]
		doc_arch = dict(zip(mcus_to_test, doc_arch_list))

		# Check that generated map is as expected
		self.assertEquals(doc_arch, expected_doc_arch)

	def test_MCU_get_doc_arch_name(self):
		# Expected mapping between MCU name and doc-arch
		expected_doc_arch = {
			'atxmega128a1'  : 'XMEGAA',
			'at32uc3a0128' : 'UC3A',
		}

		# Get the MCU names
		mcus_to_test = expected_doc_arch.keys()

		# Generate map from MCU name to doc-arch
		doc_arch_list = [MCU(mcu_name, self.main_db).get_doc_arch_name() for mcu_name in mcus_to_test]
		doc_arch = dict(zip(mcus_to_test, doc_arch_list))

		# Check that generated map is as expected
		self.assertEquals(doc_arch, expected_doc_arch)

	def test_ConfigItem_id_caption_type(self):
		result = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained")
		expected_result = "xmega.drivers.nvm.example1.xmega_a1_xplained"
		self.assertEqual(expected_result, result.id)
		expected_result = "Drivers NVM Example 1 for XMEGA-A1 Xplained"
		self.assertEqual(expected_result, result.caption)
		expected_result = "normal"
		self.assertEqual(expected_result, result.type)

	def test_ConfigItem_get_description_summary_text(self):
		expected_result = "Use the XMEGA NVM to read the device and the production signatures and output them on LEDs."
		result = self.main_db.lookup_by_id("xmega.drivers.nvm.example1").get_description_summary_text()
		self.assertEqual(expected_result, result)

	def test_ConfigItem_get_help_url(self):
		expected_result = "http://asf.atmel.com/docs/6.6.6/test/html/index.html"
		result = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained").get_help_url("test", "6.6.6")
		self.assertEqual(expected_result, result)

	def test_ConfigItem_get_quick_start_url(self):
		expected_result = "http://asf.atmel.com/docs/6.6.6/xmegaa/html/xmega_nvm_quickstart.html"
		mcu = MCU("atxmega128a1", self.main_db)
		result = self.main_db.lookup_by_id("xmega.drivers.nvm").get_quick_start_url(mcu.get_doc_arch_group(), "6.6.6")
		self.assertEqual(expected_result, result)

	def test_ConfigItem_get_sub_elements(self):
		elems = self.main_db.lookup_by_id("config.board.xmega_a1_xplained.led#no").get_sub_elements()
		self.assertEqual(len(elems), 1)
		self.assertEqual(elems[0].attrib, {'value': 'xmega'})
		self.assertEqual(elems[0].tag, "device-support")

	def test_ConfigItem_get_configuration(self):
		expected_result = {'config.board.xmega_a1_xplained.led': 'no', 'config.board.xmega_a1_xplained.init': 'yes'}
		result = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained").get_configuration().config
		self.assertEqual(expected_result, result)
		result = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained")._get_configuration_from_element().config
		self.assertEqual(expected_result, result)

	def test_ConfigItem_get_prerequisite_ids(self):
		expected_result = ['board.xmega_a1_xplained', 'xmega.drivers.nvm.example1']
		result = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained").get_prerequisite_ids(recursive=False)
		result.sort()
		self.assertEqual(expected_result, result)

	def test_ConfigItem_has_subids(self):
		result = self.main_db.lookup_by_id("xmega.drivers.nvm.example1.xmega_a1_xplained").has_subids()
		self.assertEqual(False, result)
		result = self.main_db.lookup_by_id("config.board.xmega_a1_xplained.led").has_subids()
		self.assertEqual(True, result)

	def test_ConfigItem__get_property(self):
		expected_result = "yes"
		result = self.main_db.lookup_by_id("config.board.xmega_a1_xplained.init")._get_property('default')
		self.assertEqual(expected_result, result)

	def test_ConfigItem_get_build(self):
		module = self.main_db.lookup_by_id("avr32.drivers.tc")
		self.assertEqual(['avr32/drivers/tc'], separator_replace_list(module.get_build(BuildInclude, "as5_8")))
		self.assertEqual(['avr32/drivers/tc/tc.c'], separator_replace_list(module.get_build(BuildC, "as5_8")))
		self.assertEqual(['avr32/drivers/tc/tc_asm.s'], separator_replace_list(module.get_build(BuildAssembly, "as5_8")))
		self.assertEqual(['avr32/drivers/tc/tc.h'], separator_replace_list(module.get_build(BuildHeader, "as5_8")))
		self.assertEqual(['avr32/drivers/tc/module_config'], separator_replace_list(module.get_build(BuildModuleConfigPath, "as5_8")))
		self.assertEqual(['--section-start=.BOOT=0x20000'], module.get_build(BuildLinkerConfig, "as5_8"))
		self.assertEqual(['conf_board.h'], module.get_build(BuildModuleConfigRequiredHeaderFile, "as5_8"))
		self.assertEqual(['avr32/drivers/tc/tc.h'], separator_replace_list(module.get_build(BuildApiHeader, "as5_8")))
		self.assertEqual([('FOO', 'BAR')], module.get_build(BuildDefine, "as5_8"))
		self.assertEqual(['libavr51g1-4qt-k-0rs.a'], module.get_build(BuildUserLibrary, "as5_8"))
		self.assertEqual(['libc.a'], module.get_build(BuildCompilerLibrary, "as5_8"))
		self.assertEqual(['license.txt'], module.get_build(BuildDistributeLicense, "as5_8"))
		self.assertEqual(['a.file'], module.get_build(BuildDistributeFile, "as5_8"))

	def test_ConfigItem_resolve_all_selections(self):
		module = self.main_db.lookup_by_id("common.services.basic.clock")
		config = ConfigurationHandler()

		mcu = MCU("atxmega128a1", self.main_db)
		module.resolve_all_selections(config, mcu)
		expected_result = ['common/services/clock/xmega/sysclk1.c']
		result = separator_replace_list(module.get_build(BuildC, "as5_8"))
		self.assertEqual(expected_result, result)

		mcu = MCU("atxmega128b1", self.main_db)
		module.resolve_all_selections(config, mcu)
		expected_result = ['common/services/clock/xmega/sysclk2.c']
		result = separator_replace_list(module.get_build(BuildC, "as5_8"))
		self.assertEqual(expected_result, result)

	def test_AVRStudio5Project32(self):
		def fix_separators(dic_to_fix):
			"""
			Fix path separators in dictionary (both keys and values)
			and returns a sorted list.
			"""
			new_dic = {}
			for key, value in dic_to_fix.items():
				new_dic[key.replace("\\",'/')] = [value[0].replace("\\",'/'), value[1], value[2].replace("\\",'/')];
			return sorted(new_dic.items())

		project = self.main_db.lookup_by_id("avr32.drivers.tc.example.a")
		driver = self.main_db.lookup_by_id("avr32.drivers.tc")
		gen = AVRStudio5Project32(project, self.main_db, self.facade.runtime)

		# Test _get_project_data()
		gen._get_project_data()
		expected_result = 'STK600 - AT32UC3A0128'
		self.assertEqual(gen.project_caption, expected_result)

		# Test get_build_from_export for misc build types
		self.assertEqual(['libc.a'], gen.get_build_from_export(BuildCompilerLibrary))
		self.assertEqual([("a.file",['src/ASF/a.file', driver, 'a.file'])], fix_separators(gen.get_build_from_export(BuildDistributeFile)))
		self.assertEqual([('avr32/drivers/tc/tc.h', ['src/ASF/avr32/drivers/tc/tc.h', driver, 'avr32/drivers/tc/tc.h'])], fix_separators(gen.get_build_from_export(BuildHeader)))
		self.assertEqual(['src/asf/../../../avr32/drivers/tc', 'src', 'src/config'], separator_replace_list(gen.get_build_from_export(BuildInclude)))
		self.assertEqual([('avr32/drivers/tc/tc_asm.s', ['src/ASF/avr32/drivers/tc/tc_asm.s', driver, 'avr32/drivers/tc/tc_asm.s'])],
			fix_separators(gen.get_build_from_export(BuildAssembly)))
		self.assertEqual([('avr32/drivers/tc/tc.c', ['src/ASF/avr32/drivers/tc/tc.c', driver, 'avr32/drivers/tc/tc.c'])], fix_separators(gen.get_build_from_export(BuildC)))
		self.assertEqual([('FOO', 'BAR')], gen.get_build_from_export(BuildDefine))
		self.assertEqual([('libavr51g1-4qt-k-0rs.a', ['src/ASF/libavr51g1-4qt-k-0rs.a', driver, 'libavr51g1-4qt-k-0rs.a'])], fix_separators(gen.get_build_from_export(BuildUserLibrary)))

		# Test get_compile_files()
		self.assertEqual([('avr32/drivers/tc/tc.c', ['src/ASF/avr32/drivers/tc/tc.c', driver, 'avr32/drivers/tc/tc.c']),
			('avr32/drivers/tc/tc_asm.s', ['src/ASF/avr32/drivers/tc/tc_asm.s', driver, 'avr32/drivers/tc/tc_asm.s'])], fix_separators(gen.get_compile_files()))

		# Test project_toolchain_settings
		self.assertEqual(True, 'avr51g1-4qt-k-0rs' in gen.project_toolchain_settings['Release']['avr32gcc.linker.libraries.Libraries'])

		# Test get_framework_config()
		# Read expected data
		input_file = os.path.join(self.xml_basedir, "../frameworkconfig.xml")
		expected_tree = ET.parse(input_file)
		expected_root = expected_tree.getroot()
		expected_result = get_elementtree_as_string(expected_root)

		# Generate result
		et = ET.fromstring(gen.get_framework_config())
		result = get_elementtree_as_string(et)
		# Compare
		self.assertEqual(expected_result, result)

	def test_DeviceMap_get_doc_arch_dict(self):
		device_map = self.main_db.lookup_by_id("map.atmel")

		# Expected map between group and doc-arch name
		expected_doc_arch_dict = {
			'xmegaa' : 'XMEGAA',
			'uc3a' : 'UC3A',
		}

		# Get device map's mapping from groups to their doc-arch names
		doc_arch_dict = device_map.get_doc_arch_dict()

		# Check that generated map is as expected
		self.assertEquals(doc_arch_dict, expected_doc_arch_dict)


class FdkExtensionManagerTestCase(unittest.TestCase):
	old_dir = os.path.abspath(os.curdir)
	xml_schema_dir = norm_path_join(sys.path[0], "schemas/")
	ext_basedir = norm_path_join(sys.path[0], "asf/test-input/extensions/")
	# Data for two test extensions
	ext_dirs = [norm_path_join(ext_basedir, d) for d in ["Aaa/Bbb/", "Ccc/", "Eee/"]]
	ext_uuids = ["01234567-89ab-cdef-0123-456789abcdef", "00000000-0000-0000-0000-000000000000"]

	def _assertIsInstance(self, test_object, test_class):
		"""
		Compatibility for Python 2.6. Remove when 2.6 support is not needed.
		"""
		self.assertTrue(isinstance(test_object, test_class))

	def setUp(self):
		class DummyRuntime(object):
			pass

		rel_ext_basedir = os.path.relpath(self.ext_basedir)
		self.extmgr = FdkExtensionManager(DummyRuntime(), rel_ext_basedir)
		self.extmgr.load_schemas(self.xml_schema_dir)

	def tearDown(self):
		# The extension manager changes directory, revert here
		os.chdir(self.old_dir)

	def test_root_path(self):
		expected_result = os.path.normpath(os.path.join(sys.path[0], self.ext_basedir))

		self.assertEqual(self.extmgr.root_path, expected_result)

	def test_scan_for_extensions(self):
		expected_result = self.ext_dirs

		result = self.extmgr.scan_for_extensions()

		self.assertEqual(sorted(result), sorted(expected_result))

	def test_load_extension(self):
		# Bogus directory
		self.assertRaises(ConfigError, self.extmgr.load_extension, 'this_should_fail')
		# Directory missing extension.xml
		self.assertRaises(DbError, self.extmgr.load_extension, self.ext_basedir)

		# Extension register should be empty
		self.assertEqual(self.extmgr.fdk_ext_dict, {})

		new_ext = self.extmgr.load_extension(self.ext_dirs[0])
		self._assertIsInstance(new_ext, FdkExtension)

		# Extension register should still be empty
		self.assertEqual(self.extmgr.fdk_ext_dict, {})

	def test_register_extension_and_clear_extension_register(self):
		new_ext = self.extmgr.load_extension(self.ext_dirs[0])

		# We should start with an empty extension register
		self.assertEqual(self.extmgr.fdk_ext_dict, {})

		# Registering twice should fail
		self.extmgr.register_extension(new_ext)
		self.assertRaises(DbError, self.extmgr.register_extension, new_ext)

		# Check the extension register layout
		self.assertEqual(self.extmgr.fdk_ext_dict, {new_ext.uuid : {new_ext.version : new_ext}})

		# Check that the extension register is cleared
		self.extmgr.clear_extension_register()
		self.assertEqual(self.extmgr.fdk_ext_dict, {})

	def test_load_and_register_then_request_extensions(self):
		# Extension register should be empty
		self.assertEqual(self.extmgr.fdk_ext_dict, {})

		# Load the test extensions
		self.extmgr.load_and_register_extensions(self.ext_dirs)

		# Request the extensions
		ext_0 = self.extmgr.request_extension(self.ext_uuids[0])
		ext_1 = self.extmgr.request_extension(self.ext_uuids[1], 'latest')

		# Check that they are indeed FDK extension objects
		self._assertIsInstance(ext_0, FdkExtension)
		self._assertIsInstance(ext_1, FdkExtension)

		# Request non-existing version of extension
		self.assertRaises(DbError, self.extmgr.request_extension, self.ext_uuids[1], '0.0.0')
		# Request non-existing extension
		self.assertRaises(DbError, self.extmgr.request_extension, self.ext_uuids[1].replace('0', '1'))

		# Clear the extension register before returning
		self.extmgr.clear_extension_register()

	def test_resolve_version_selection(self):
		vers = ["1.0.0", "1.0.10", "1.0.3", "1.1.20", "1.1.2"]

		# Check for very latest version
		selected_ver = self.extmgr.resolve_version_selection(vers, 'latest')
		self.assertEqual(selected_ver, "1.1.20")

		# Check for very latest version using limit notation
		selected_ver = self.extmgr.resolve_version_selection(vers, '(, )')
		self.assertEqual(selected_ver, "1.1.20")

		# Check for latest version in 1.0 range
		selected_ver = self.extmgr.resolve_version_selection(vers, '1.0.+')
		self.assertEqual(selected_ver, "1.0.10")

		# Check for latest version before 1.1.20
		selected_ver = self.extmgr.resolve_version_selection(vers, '(, 1.1.20)')
		self.assertEqual(selected_ver, "1.1.2")

		# Check for latest version not after 1.1.2
		selected_ver = self.extmgr.resolve_version_selection(vers, '(, 1.1.2]')
		self.assertEqual(selected_ver, "1.1.2")

		# Check for latest version between 1.0.3 and 1.1.2
		selected_ver = self.extmgr.resolve_version_selection(vers, '(1.0.3, 1.1.2)')
		self.assertEqual(selected_ver, "1.0.10")

		# Check for latest version equal or larger than 1.0.10, and smaller than 1.1.2
		selected_ver = self.extmgr.resolve_version_selection(vers, '[1.0.10, 1.1.2)')
		self.assertEqual(selected_ver, "1.0.10")

		# Check exception for non-existent version between 1.0.10 and 1.1.2
		self.assertRaises(NoSelectionError, self.extmgr.resolve_version_selection, vers, '(1.0.10, 1.1.2)')

		# Check exception for non-existent version newer than 1.1.20
		self.assertRaises(NoSelectionError, self.extmgr.resolve_version_selection, vers, '(1.1.20, )')


class FdkExtensionTestCase(unittest.TestCase):
	old_dir = os.path.abspath(os.curdir)
	xml_schema_dir = norm_path_join(sys.path[0], "schemas/")

	ext_basedir = norm_path_join(sys.path[0], "asf/test-input/extensions/")
	main_ext_dir = norm_path_join(ext_basedir, "Aaa/Bbb/")
	main_ext_uuid = "01234567-89ab-cdef-0123-456789abcdef"
	alt_ext_eid = "ccc"

	device_map = norm_path_join(ext_basedir, "devmap.xml")
	content_xml = "content.xml"

	def _assertIsInstance(self, test_object, test_class):
		"""
		Compatibility for Python 2.6. Remove when 2.6 support is not needed.
		"""
		self.assertTrue(isinstance(test_object, test_class))

	def setUp(self):
		class DummyLog(object):
			def info(self, log_msg):
				pass

		class DummyRuntime(object):
			log = DummyLog()
			version_postfix = ""

		# Extension manager is needed to test interaction between extensions
		self.extmgr = FdkExtensionManager(DummyRuntime(), self.ext_basedir)

		# Load XSDs, device map, set content XML filename
		self.extmgr.load_schemas(self.xml_schema_dir)
		self.extmgr.load_device_map(self.device_map)
		ConfigDB.xml_filename = self.content_xml

		# Initialize extension register
		ext_dirs = self.extmgr.scan_for_extensions()
		self.extmgr.load_and_register_extensions(ext_dirs)

		# Load the main extension
		self.ext = self.extmgr.request_extension(self.main_ext_uuid)

	def tearDown(self):
		# The extension manager changes directory, revert here
		os.chdir(self.old_dir)

	def test_get_database(self):
		ext_db = self.ext.get_database()
		self._assertIsInstance(ext_db, ConfigDB)

	def test_get_external_database(self):
		# Get an existing database
		alt_ext_db = self.ext.get_external_database(self.alt_ext_eid)
		self._assertIsInstance(alt_ext_db, ConfigDB)

		# Try to dereference a bogus extension ID
		self.assertRaises(DbError, self.ext.get_external_database, "bogus_eid")

	def test_path_attributes(self):
		expected_rel_root_path = os.path.relpath(self.main_ext_dir, self.ext_basedir)

		self.assertEqual(self.ext.root_path, self.main_ext_dir)
		self.assertEqual(self.ext.relative_root_path, expected_rel_root_path)
		self.assertEqual(self.ext.root_xml_path, norm_path_join(self.main_ext_dir, "extension.xml"))
		self.assertEqual(self.ext.xml_cache_path, norm_path_join(self.main_ext_dir, "content.xml.cache"))

	def test_extension_attributes(self):
		self.assertEqual(self.ext.uuid, self.main_ext_uuid)
		self.assertEqual(self.ext.org, "Aaa")
		self.assertEqual(self.ext.shortname, "Bbb")
		self.assertEqual(self.ext.fullname, "Bbb test extension")
		self.assertEqual(self.ext.version, "0.1.2")

	def test_file_attributes(self):
		# self.expand_to_manager_root_path == True, so extension's directory must be added
		self.assertEquals(self.ext.icon_image, os.path.normpath("Aaa/Bbb/docs/bbb_icon.png"))
		self.assertEquals(self.ext.preview_image, os.path.normpath("Aaa/Bbb/docs/bbb_preview.jpg"))
		self.assertEquals(self.ext.license, os.path.normpath("Aaa/Bbb/docs/aaa_license.txt"))

		self.assertEquals(self.ext.license_caption, "Aaa license")
		self.assertEquals(self.ext.release_notes, "http://bbb.aaa.com/release-notes/")
		self.assertEquals(self.ext.release_notes_caption, "Bbb release notes")

	def test_meta_info_attributes(self):
		expected_description = """
		This is a test extension for unit testing of the FdkExtension class.
		"""
		expected_search_terms = ["first", "second"]

		self.assertEquals(self.ext.description, expected_description)
		self.assertEquals(self.ext.search_terms, expected_search_terms)

	def test_author_info_attributes(self):
		self.assertEquals(self.ext.author_name, "Aaa")
		self.assertEquals(self.ext.author_website, "http://www.aaa.com/")
		self.assertEquals(self.ext.author_email, "support@aaa.com")

	def test_help_attributes_for_asf_docs(self):
		expected_scheme = "asf-docs"

		expected_online_help_caption = "Bbb help"
		expected_online_help_index = "http://bbb.aaa.com/help_index"
		expected_online_module_help = "http://bbb.aaa.com/element_help/$VER$/$MODULE$/html/"
		expected_online_module_guide = "http://bbb.aaa.com/element_guide/$VER$/$MODULE$/html/"

		self.assertEquals(self.ext.online_help_index_caption_and_url, (expected_online_help_caption, expected_online_help_index))
		self.assertEquals(self.ext.online_module_help_scheme_and_url, (expected_scheme, expected_online_module_help))
		self.assertEquals(self.ext.online_module_guide_scheme_and_url, (expected_scheme, expected_online_module_guide))

		expected_offline_help_caption = "Bbb help doc"
		# self.expand_to_manager_root_path == True, so extension's directory must be added
		expected_offline_help_index = os.path.normpath("Aaa/Bbb/docs/help.pdf")
		expected_offline_module_help = os.path.normpath("Aaa/Bbb/docs/help/$MODULE$/html/")
		expected_offline_module_guide = os.path.normpath("Aaa/Bbb/docs/guides/$MODULE$/html/")

		self.assertEquals(self.ext.offline_help_index_caption_and_path, (expected_offline_help_caption, expected_offline_help_index))
		self.assertEquals(self.ext.offline_module_help_scheme_and_path, (expected_scheme, expected_offline_module_help))
		self.assertEquals(self.ext.offline_module_guide_scheme_and_path, (expected_scheme, expected_offline_module_guide))

	def test_help_attributes_for_append(self):
		alt_ext_db = self.ext.get_external_database(self.alt_ext_eid)
		alt_ext = alt_ext_db.extension

		expected_scheme = "append"

		expected_online_module_help = "http://ccc.c.com/module-help-pages/"
		expected_online_module_guide = "http://ccc.c.com/module-guide-pages/"

		self.assertEquals(alt_ext.online_module_help_scheme_and_url, (expected_scheme, expected_online_module_help))
		self.assertEquals(alt_ext.online_module_guide_scheme_and_url, (expected_scheme, expected_online_module_guide))

		# self.expand_to_manager_root_path == True, so extension's directory must be added
		expected_offline_module_help = os.path.normpath("Ccc/docs/help-pages/")
		expected_offline_module_guide = os.path.normpath("Ccc/docs/guide-pages/")

		self.assertEquals(alt_ext.offline_module_help_scheme_and_path, (expected_scheme, expected_offline_module_help))
		self.assertEquals(alt_ext.offline_module_guide_scheme_and_path, (expected_scheme, expected_offline_module_guide))

	def test_external_prerequisites(self):
		expected_prerequisite_ids = ["bbb.board", "ccc.application"]

		# Load the test project from main extension database
		db = self.ext.get_database()
		project = db.lookup_by_id("bbb.project")

		# Check the prerequisites IDs
		prerequisites = project.get_prerequisites(recursive=True)
		prerequisite_ids = [p.id for p in prerequisites]

		self.assertEquals(prerequisite_ids, expected_prerequisite_ids)


class FdkExtensionDocsTestCase(unittest.TestCase):
	old_dir = os.path.abspath(os.curdir)
	xml_schema_dir = norm_path_join(sys.path[0], "schemas/")

	ext_basedir = norm_path_join(sys.path[0], "asf/test-input/extensions/")

	first_ext_dir = norm_path_join(ext_basedir, "Aaa/Bbb/")
	second_ext_dir = norm_path_join(ext_basedir, "Ccc/")
	third_ext_dir = norm_path_join(ext_basedir, "Eee/")

	# asf-docs
	first_ext_uuid  = "01234567-89ab-cdef-0123-456789abcdef"
	# append
	second_ext_uuid = "00000000-0000-0000-0000-000000000000"
	# no docs
	third_ext_uuid  = "ffffffff-ffff-ffff-ffff-ffffffffffff"

	device_map = norm_path_join(ext_basedir, "devmap.xml")
	content_xml = "content.xml"

	def _assertIsInstance(self, test_object, test_class):
		"""
		Compatibility for Python 2.6. Remove when 2.6 support is not needed.
		"""
		self.assertTrue(isinstance(test_object, test_class))

	def setUp(self):
		class DummyLog(object):
			def info(self, log_msg):
				pass

		class DummyRuntime(object):
			log = DummyLog()
			version_postfix = ""

		# Extension manager is needed to test interaction between extensions
		self.extmgr = FdkExtensionManager(DummyRuntime(), self.ext_basedir)

		# Load XSDs, device map, set content XML filename
		self.extmgr.load_schemas(self.xml_schema_dir)
		self.extmgr.load_device_map(self.device_map)
		ConfigDB.xml_filename = self.content_xml

		# Initialize extension register
		ext_dirs = self.extmgr.scan_for_extensions()
		self.extmgr.load_and_register_extensions(ext_dirs)

		# Load the extensions
		self.first_ext = self.extmgr.request_extension(self.first_ext_uuid)
		self.second_ext = self.extmgr.request_extension(self.second_ext_uuid)
		self.third_ext = self.extmgr.request_extension(self.third_ext_uuid)

	def tearDown(self):
		# The extension manager changes directory, revert here
		os.chdir(self.old_dir)


	# Test extension with "asf-docs" doc scheme
	def test_online_asf_docs_module_help(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module")

		expected_help_url = "http://bbb.aaa.com/element_help/0.1.2/some_doc_arch/html/group__bbb__module__help.html"
		help_url = module.get_help_url("some_doc_arch")

		self.assertEqual(help_url, expected_help_url)

	def test_online_asf_docs_module_guide(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module")

		expected_guide_url = "http://bbb.aaa.com/element_guide/0.1.2/some_doc_arch/html/bbb_module_guide.html"
		guide_url = module.get_quick_start_url("some_doc_arch")

		self.assertEqual(guide_url, expected_guide_url)

	def test_offline_asf_docs_module_help(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module")

		expected_help_path = os.path.normpath("Aaa\\Bbb\\docs\\help\\some_doc_arch\\html\\group__bbb__module__help.html")
		help_path = module.get_offline_help_path("some_doc_arch")

		self.assertEqual(help_path, expected_help_path)

	def test_offline_asf_docs_module_guide(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module")

		expected_guide_path = os.path.normpath("Aaa\\Bbb\\docs\\guides\\some_doc_arch\\html\\bbb_module_guide.html")
		guide_path = module.get_offline_quick_start_path("some_doc_arch")

		self.assertEqual(guide_path, expected_guide_path)


	# Test extension with "append" doc scheme
	def test_online_append_module_help(self):
		db = self.second_ext.get_database()
		module = db.lookup_by_id("ccc.module_append")

		expected_help_url = "http://ccc.c.com/module-help-pages/driver/ccc_online_module_help.html"
		help_url = module.get_help_url("some_doc_arch")

		self.assertEqual(help_url, expected_help_url)

	def test_online_append_module_guide(self):
		db = self.second_ext.get_database()
		module = db.lookup_by_id("ccc.module_append")

		expected_guide_url = "http://ccc.c.com/module-guide-pages/driver/ccc_online_module_guide.html"
		guide_url = module.get_quick_start_url("some_doc_arch")

		self.assertEqual(guide_url, expected_guide_url)

	def test_offline_append_module_help(self):
		db = self.second_ext.get_database()
		module = db.lookup_by_id("ccc.module_append")

		expected_help_path = os.path.normpath("Ccc\\docs\\help-pagesdriver\\ccc_offline_module_help.html")
		help_path = module.get_offline_help_path("some_doc_arch")

		self.assertEqual(help_path, expected_help_path)

	def test_offline_append_module_guide(self):
		db = self.second_ext.get_database()
		module = db.lookup_by_id("ccc.module_append")

		expected_guide_path = os.path.normpath("Ccc\\docs\\guide-pagesdriver\\ccc_offline_module_guide.html")
		guide_path = module.get_offline_quick_start_path("some_doc_arch")

		self.assertEqual(guide_path, expected_guide_path)


	# Test extensions with defined doc scheme, but URL override in asf.xml
	def test_fixed_online_module_help(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module_fixed_help")

		expected_help_url = "http://fixed_module_help_page.com/"
		help_url = module.get_help_url("some_doc_arch")

		self.assertEqual(help_url, expected_help_url)

	def test_fixed_online_module_guide(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module_fixed_help")

		expected_guide_url = "http://fixed_module_guide_page.com/"
		guide_url = module.get_quick_start_url("some_doc_arch")

		self.assertEqual(guide_url, expected_guide_url)

	def test_fixed_offine_module_help(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module_fixed_help")

		expected_help_path = os.path.normpath("Aaa\\Bbb\\doc\\fixed_module_help.html")
		help_path = module.get_offline_help_path("some_doc_arch")

		self.assertEqual(help_path, expected_help_path)

	def test_fixed_offline_module_guide(self):
		db = self.first_ext.get_database()
		module = db.lookup_by_id("bbb.module_fixed_help")

		expected_guide_path = os.path.normpath("Aaa\\Bbb\\doc\\fixed_module_guide.html")
		guide_path = module.get_offline_quick_start_path("some_doc_arch")

		self.assertEqual(guide_path, expected_guide_path)


	# Test extension with no doc scheme
	def test_online_missing_module_help_and_guide(self):
		db = self.third_ext.get_database()
		no_doc_module = db.lookup_by_id("eee.module_undoc")

		expected_help_url = None
		expected_guide_url = None

		help_url = no_doc_module.get_help_url("some_doc_arch")
		guide_url = no_doc_module.get_quick_start_url("some_doc_arch")

		self.assertEqual(help_url, expected_help_url)
		self.assertEqual(guide_url, expected_guide_url)

	def test_online_fixed_module_help_and_guide(self):
		db = self.third_ext.get_database()
		doc_module = db.lookup_by_id("eee.module_doc")

		expected_help_url = "http://module_help_page.com/"
		expected_guide_url = "http://module_guide_page.com/"

		help_url = doc_module.get_help_url("some_doc_arch")
		guide_url = doc_module.get_quick_start_url("some_doc_arch")

		self.assertEqual(help_url, expected_help_url)
		self.assertEqual(guide_url, expected_guide_url)

	def test_offline_missing_module_help(self):
		db = self.third_ext.get_database()
		no_doc_module = db.lookup_by_id("eee.module_undoc")

		expected_help_path = None
		expected_guide_path = None

		help_path = no_doc_module.get_offline_help_path("some_doc_arch")
		guide_path = no_doc_module.get_offline_quick_start_path("some_doc_arch")

		self.assertEqual(help_path, expected_help_path)
		self.assertEqual(guide_path, expected_guide_path)

	def test_offline_fixed_module_help(self):
		db = self.third_ext.get_database()
		doc_module = db.lookup_by_id("eee.module_doc")

		expected_help_path = os.path.normpath("Eee\\docs\\module_offline_help_page.html")
		expected_guide_path = os.path.normpath("Eee\\docs\\module_offline_guide_page.html")

		help_path = doc_module.get_offline_help_path("some_doc_arch")
		guide_path = doc_module.get_offline_quick_start_path("some_doc_arch")

		self.assertEqual(help_path, expected_help_path)
		self.assertEqual(guide_path, expected_guide_path)
