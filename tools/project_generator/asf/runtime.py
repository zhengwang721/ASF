import logging
import os
import sys

from asf.database import ConfigDB
from asf.toolchain import *


class Runtime(object):
	log = None
	log_handler = None

	def __init__(self, template_dir, configuration):
		self.debug = False
		self.configuration = configuration
		self.use_file_list_cache = False
		self.file_list_cache_name = "asf_filelist.cache"
		self.template_dir = template_dir
		self.xml_schema_path = None
		self.xml_input_filename = "asf.xml"
		self.args = list()
		self.archiver = None
		self.version_postfix = None
		self.generators = ["all"]
		self.document_version = None
		self.no_zip = False

	def generator_tag_to_class(self, tag):
		generator_to_class = {
			verbose.VerboseProject.toolchain   : verbose.VerboseProject,
			avrgcc.AVRGCCProject.toolchain     : avrgcc.AVRGCCProject,
			avrgcc.AVR32GCCProject.toolchain   : avrgcc.AVR32GCCProject,
			armgcc.ARMGCCProject.toolchain     : armgcc.ARMGCCProject,
			iarew.IAREW8Project.toolchain      : iarew.IAREW8Project.get_generator,
			iarew.IAREW32Project.toolchain     : iarew.IAREW32Project.get_generator,
			iarewarm.IAREWARMProject.toolchain     : iarewarm.IAREWARMProject.get_generator,
			doxygen.DoxygenProject.toolchain   : doxygen.DoxygenProject,
			doxygen.DoxygenModule.toolchain   : doxygen.DoxygenModule,
			avrstudio5.AVRStudio5Project.generator_tag : avrstudio5.AVRStudio5Project,
			avrstudio5.AVRStudio5Project32.generator_tag : avrstudio5.AVRStudio5Project32,
			avrstudio5.AVRStudio5ProjectARM.generator_tag : avrstudio5.AVRStudio5ProjectARM,
			avrstudio5.AVRStudio5ProjectTemplate.generator_tag : avrstudio5.AVRStudio5ProjectTemplate,
			avrstudio5.AVRStudio5Project32Template.generator_tag : avrstudio5.AVRStudio5Project32Template,
			avrstudio5.AVRStudio5ProjectARMTemplate.generator_tag : avrstudio5.AVRStudio5ProjectARMTemplate,
		}
		return generator_to_class.get(tag, None)

	@staticmethod
	def setup_log(level=logging.WARNING, timed_log = False):
		log = logging.getLogger("Log")
		Runtime.log = log

		if Runtime.log_handler:
			log.removeHandler(Runtime.log_handler)
			del Runtime.log_handler

		handler = logging.StreamHandler(sys.stdout)
		Runtime.log_handler = handler

		log_format = "%(levelname)s - %(message)s"
		if timed_log:
			log_format = "%(asctime)s - " + log_format
		formatter = logging.Formatter(log_format)
		handler.setFormatter(formatter)

		log.addHandler(handler)
		log.setLevel(level)

	def get_log_level_from_text(self, text):
		"""
		Convert text level to logging level. Returns level or None if no
		level is found.
		"""
		LEVELS = {
		'debug' : logging.DEBUG,
		'info'  : logging.INFO,
		'warning' : logging.WARNING,
		'error' : logging.ERROR,
		'critical' : logging.CRITICAL,
		}
		level = LEVELS.get(text)
		if not level:
			raise Exception('Invalid log level name "%s"' %text)

		return level


	def set_debug(self, value = True):
		self.debug = value

	def set_config(self, name, value):
		"""
		Sets the value of a config, overwriting any previous value.
		"""
		self.configuration.set_config(name, value)

	def unset_config(self, name):
		"""
		Unsets a config, removing both its name and value if it exists.
		"""
		self.configuration.unset_config(name, value)

	def get_config(self, name):
		"""
		Gets the value of a config, if it exists.
		"""
		return self.configuration.get_config(name)

	def get_config_group(self, group):
		"""
		Gets a dictionary of existing configs that belong to the
		specified group.
		"""
		return self.configuration.get_config_group(group)

	def create_and_set_cache_dir(self, dir):
		# Create folder
		try:
			os.mkdir(dir)
		except OSError:
			# Folder exists
			pass

		# Set all cache files to the correct path
		self.file_list_cache_name = os.path.join(dir, "asf_filelist.cache")

	def set_use_file_list_cache(self, value = True):
		self.use_file_list_cache = value

	def set_xml_schema_path(self, filepath):
		self.xml_schema_path = filepath

	def set_xml_input_filename(self, filename):
		self.xml_input_filename = filename

	def set_version_postfix(self, postfix):
		self.version_postfix = postfix

	def set_commandline_args(self, args):
		self.args = args

	def set_archiver(self, archiver):
		self.archiver = archiver

	def set_generator_string(self, generator_string):
		self.generators = generator_string.split(',')

	def set_document_version(self, document_version):
		self.document_version = document_version

	def set_no_zip(self, no_zip):
		self.no_zip = no_zip

	def get_document_version(self):
		if self.document_version == None:
			raise Exception('Document version fetched before it was set or database loaded.')

		return self.document_version

	def load_db(self, filename=None):
		# Parse all XML files or load from given filename
		self.db = ConfigDB(self, filename)
		if self.document_version == None:
			self.document_version = self.db.get_framework_version_number()

	def visualize_xml(self):
		visualize_xml(self.db.root)

	def write_xml_tree(self, filename):
		self.db.write_xml_tree(filename)

	def validate_xml(self):
		self.db.validate_xml(self.db.root)

	def database_sanity_check(self):
		self.db.sanity_check()

	def run(self):
		pass
