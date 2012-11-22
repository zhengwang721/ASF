import os

from asf.database import ConfigDB
from asf.exception import *
from asf.extension import *


class FdkExtensionManager(object):
	_root_path = None
	fdk_ext_dict = {}
	runtime = None
	use_cache = None
	extension_class = FdkExtension

	def __init__(self, runtime, root_path, use_cache=False):
		"""
		Initialize with the runtime to pass on to extensions (for the
		database instantiation), the root path for the extension
		manager, and a specifier whether or not caching should be used
		when loading extensions.
		"""
		self.runtime = runtime

		if not root_path or not os.path.isdir(root_path):
			raise ConfigError("`%s' is not a valid directory path" % root_path)
		self._root_path = os.path.abspath(root_path)
		self.use_cache = use_cache

		# Set this extension manager as the current for extensions
		self.extension_class.manager = self

		# Set the extensions to expand compile paths to the manager's root before
		# starting project generation. This must be done for command line version
		# only, not for the Studio extension.
		self.extension_class.expand_to_manager_root_path = True
		os.chdir(self.root_path)


	@property
	def root_path(self):
		"""
		Return the root path of the extension manager, i.e., the
		directory under which the extensions are located.
		"""
		return self._root_path


	def scan_for_extensions(self):
		"""
		Return a list of directories under the current root path which
		contain extension XML files.
		"""
		# Start walking directories, looking for path to extension definitions
		root_paths = []
		for root_dir, dirs, files in os.walk(self.root_path):
			if self.extension_class.xml_filename in files:
				root_paths.append(root_dir)
				# Don't go into subdirs when an extension XML is found
				del dirs[:]

		return root_paths


	def load_extension(self, root_path, use_cache=None):
		"""
		Load an extension from the specified directory and return the
		corresponding object.
		This does not affect the register of extensions.

		If use_cache is True, the extension will try to use its cache.
		If use_cache is False, the extension will reload the database,
		creating its cache at the same time.
		If use_cache is not defined, the extension manager's default
		behavior -- set during initialization -- is used.
		"""
		if use_cache is None:
			use_cache = self.use_cache
		try:
			new_fdk_ext = self.extension_class(self.runtime, root_path, use_cache)
		except IOError:
			raise IOError("Could not open %s in %s" % (self.extension_class.xml_filename, root_path))

		return new_fdk_ext


	def register_extension(self, new_fdk_ext):
		"""
		Registers the specified extension so it is made available for
		other extensions.

		If the same extension is registered more than once, or different
		extensions with the _same_ UUID and version are attempted
		registered, a DbError is raised.
		"""
		# Make nested dict: { uuid : { version : fdkext }}
		key = new_fdk_ext.uuid
		new_dict = {new_fdk_ext.version : new_fdk_ext}
		old_dict = self.fdk_ext_dict.get(key, {})

		if new_fdk_ext.version in old_dict:
			raise DbError("Extension %s %s was already added" % (new_fdk_ext.uuid, new_fdk_ext.version))

		new_dict.update(old_dict)
		self.fdk_ext_dict[key] = new_dict
	

	def clear_extension_register(self):
		"""
		Clears the extension register, making all previously registered
		extensions unavailable for other extensions.
		"""
		self.fdk_ext_dict = {}


	def load_and_register_extensions(self, root_paths, use_cache=None):
		"""
		Load the extensions from the specified list of root paths and
		register them in the extension map.
		This will first clear all previously registered extensions.
		"""
		self.clear_extension_register()

		# Now instantiate the extension objects
		for new_path in root_paths:
			new_fdk_ext = self.load_extension(new_path, use_cache)
			self.register_extension(new_fdk_ext)


	def load_schemas(self, xsd_dir):
		"""
		Load the XML schema files for FDK extension and content
		definitions from the specified directory.
		"""
		extension_xsd = os.path.join(xsd_dir, 'fdk_extension.xsd')
		content_xsd = os.path.join(xsd_dir, 'fdk_content.xsd')

		self.extension_class.load_schema(extension_xsd)
		ConfigDB.load_schema(content_xsd)


	@staticmethod
	def load_device_map(xml_file):
		"""
		Load the global device map from the specified file.
		"""
		ConfigDB.load_device_map(xml_file)


	@staticmethod
	def resolve_version_selection(ext_vers, version):
		"""
		Try to resolve a version number for the request.

		This function applies a filter on the list of available
		versions, depending on what the requested version is, then sorts
		the results and returns the last version in the list.
		"""
		def int_from_ver(version):
			integers = []
			if version is not '':
				for number in version.split('.'):
					integers.append(int(number))
			return integers

		# If requested version is unspecified or "latest", nothing to do here.
		if version is None or version == 'latest':
			pass

		# If there is a plus in the version request, e.g., "1.3.+", filter out
		# versions from ext_vers which do not start with "1.3."
		elif '+' in version:
			ver_filter = version.partition('+')[0]
			ext_vers = [v for v in ext_vers if v.startswith(ver_filter)]
		
		# If there is a comma in the version request, e.g., "[1.2.3, )", filter
		# out the versions which do not fall within the interval
		elif ',' in version:
			start, end = version.split(',')
			end = end.strip()

			# Get the limit symbols and version limits
			start_l = start[0]
			end_l = end[-1]
			start_v = int_from_ver(start[1:])
			end_v = int_from_ver(end[:-1])

			# Map the limit symbols to lambdas
			limit_to_lambda = {
				# Open bounds
				'(' : lambda x: start_v < int_from_ver(x),
				')' : lambda x: int_from_ver(x) < end_v,
				# Closed bounds
				'[' : lambda x: start_v <= int_from_ver(x),
				']' : lambda x: int_from_ver(x) <= end_v,
			}

			# Now filter the versions according to limits
			if start_v:
				ext_vers = filter(limit_to_lambda[start_l], ext_vers)
			if end_v:
				ext_vers = filter(limit_to_lambda[end_l], ext_vers)

		# Last possibility is that a specific version was specified.
		# If it is available, reduce ext_vers to that single version.
		# If not available, make ext_vers empty
		else:
			if version in ext_vers:
				ext_vers = [version]
			else:
				ext_vers = []

		# If any of the filtering caused an empty ext_vers, resolving
		# has failed
		if not ext_vers:
			raise NoSelectionError("Could not resolve version `%s'" % version)

		# Now sort the available versions
		ext_vers.sort(key=int_from_ver)

		# Return the last entry in list of available versions
		return ext_vers[-1]


	def request_extension(self, uuid, version=None):
		"""
		Return the extension object that corresponds to org, shortname
		and (optionally) version.
		"""
		# TODO: should check that the same extension is not requested
		# in two different versions for the same project. How???
		fdk_exts = self.fdk_ext_dict.get(uuid, None)
		if not fdk_exts:
			raise DbError("Extension %s could not be found" % uuid)

		ext_vers = fdk_exts.keys()

		try:
			sel_ver = self.resolve_version_selection(ext_vers, version)
		except NoSelectionError as e:
			raise DbError("%s of extension %s" % (str(e), uuid))

		return fdk_exts[sel_ver]


class StudioFdkExtensionManager(FdkExtensionManager):
	extension_class = StudioFdkExtension

	def __init__(self, runtime, use_cache=False):
		self.runtime = runtime

		self._root_path = ""
		self.use_cache = use_cache

		# Set this extension manager as the current
		self.extension_class.manager = self

		# Use XML cache ?
		self.extension_class.use_cache = use_cache
