import os
import sys
import time

from asf.configuration import ConfigurationHandler
from asf.database import *
from asf.extensionmanager import *
from asf.runtime import *


class PythonFacade:
	def __init__(self, root_paths, device_map=None):
		self.configuration = ConfigurationHandler()

		# Construct needed paths
		script_path = os.path.dirname(__file__)

		templatedir = os.path.join(script_path, "templates")
		xml_schema_dir = os.path.join(script_path, "schemas")
		if device_map is None:
			device_map = os.path.join(script_path, "device_maps", "atmel.xml")

		# Create runtime
		self.runtime = Runtime(templatedir, self.configuration)
		self.runtime.setup_log(logging.FATAL)
		self.runtime.set_debug(True)
		self.log = self.runtime.log

		# Create extension manager and scan for extensions
		try:
			self.ext_manager = StudioFdkExtensionManager(self.runtime, use_cache=True)
		except:
			raise

		self.ext_manager.load_schemas(xml_schema_dir)
		self.ext_manager.load_device_map(device_map)
		self.ext_manager.load_and_register_extensions(root_paths)

		self.database = ConfigDB(self.runtime, None, device_map)


	def get_extensions(self):
		extensions = []

		for key in self.ext_manager.fdk_ext_dict.keys():
			exts = self.ext_manager.fdk_ext_dict[key]

			for ext in exts.values():
				extensions.append(ext)

		return extensions

	def get_database(self,ext_uuid,ext_version=None):
		"""
		Get the database of the extension requested
		"""
		try:
			ext = self.ext_manager.request_extension(ext_uuid, ext_version)
		except:
			raise

		return ext.get_database()

	def get_mcu(self,mcu_name):
		return MCU(mcu_name,self.database)

	def get_all_mcus(self):
		"""
		Return the list of MCUs that are supported
		"""
		return ConfigDB.device_map.get_mcu_list();

	def check_extension_database_sanity(self, fdk_proj_fdk_xml_path):
		"""
		Perform basic sanity checks for FDK Project

		This loads the extension from the specified path, then runs a
		sanity check on its database.
		"""
		extension = self.ext_manager.load_extension(fdk_proj_fdk_xml_path)
		# Only relevant sanity checks (not all of them) should be called on a FDK project
		# TODO: update once the ConfigDB sanity check is split into multiple functions
		return extension.get_database().sanity_check(fdk_check = True)

	def generate_extension_cache_files(self, fdk_proj_fdk_xml_path):
		"""
		Generate necessary cache files that will be used when extension is packaged

		This loads the extension from the specified path, forcing it to
		reload the database and recreate its cache file. The project
		data cache for the extension is then written.
		"""
		extension = self.ext_manager.load_extension(fdk_proj_fdk_xml_path, use_cache=False)
		project_ids = extension.get_all_project_ids()
		extension.write_project_data_to_xml_file(project_ids,os.path.join(fdk_proj_fdk_xml_path, 'exampleProjects.xml'))

#testing
#if __name__ == "__main__":
	#generate_db_xml();


#	#test printing all mcus
#	#mcus = objext.get_all_mcus()
#	#for mcu in mcus:
#	#	print mcu

#	exts = objext.get_extensions()
#	for ext in exts:
#		example_xml = os.path.join(ext.root_path, 'ExampleProjects.xml')
#		pids = ext.get_all_project_ids()
#		print 'no of projects=',len(pids)
#		ext.write_project_data_to_xml_file(pids,example_xml)
