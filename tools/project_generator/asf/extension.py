import os
import urllib

import asf.etreeabstraction as ET

from asf.database import *
from asf.exception import *
from asf.toolchain import *


class FdkExtension(object):
	_root_path = None

	eid_to_fdk_ext = {}
	database = None
	manager = None
	runtime = None
	schema = None
	use_cache = None

	expand_to_manager_root_path = False

	doc_schemes = ['asf-docs', 'append']
	asf_docs_url_postfix = '$VER$/$MODULE$/html/'
	asf_docs_path_postfix = '$MODULE$/html/'.replace('/', os.sep)

	xml_filename = 'extension.xml'
	# Replace with ConfigDB.xml_filename when asf.xml is renamed to content.xml
	xml_cache_filename = 'content.xml' + '.cache'

	def __init__(self, runtime, root_path, use_cache=False):
		"""
		Initialize with a runtime to pass to the database upon its
		instantiation, the root directory path for the extension, and a
		specifier whether or not the cache file should be used for the
		database.
		"""
		self.runtime = runtime

		# Check that root path is valid
		if not root_path or not os.path.isdir(root_path):
			raise ConfigError("`%s' is not a valid directory path" % root_path)
		root_path = os.path.abspath(root_path)
		self._root_path = root_path

		# Check that extension XML file exists in root path
		if not os.path.isfile(self.root_xml_path):
			raise DbError("`%s' does not exist" % self.root_xml_path)

		# Store cache usage specifier
		self.use_cache = use_cache

		# Parse and, if a scheme is loaded, validate the extension XML
		tree = ET.parse(self.root_xml_path)
		try:
			self.validate_xml(tree)
		except Exception as e:
			raise DbError("Validation of %s in `%s' failed: %s" % (self.xml_filename, self.root_path, str(e)))

		# Find the extension element and store it for later use
		root_element = tree.getroot()
		ext_element = root_element.find('./extension')
		self.element = ext_element


	def __str__(self):
		"""
		Stringize extension using org, shortname and version.
		Example: [Atmel ASF 1.2.3]

		The UUID is not included due to its length.
		"""
		return '[%s %s %s]' % (self.org, self.shortname, self.version)


	def get_external_database(self, eid):
		"""
		Get the database of a different extension, specified with the
		eid parameter.

		The specified eid is resolved to a UUID reference and version,
		with a look-up in the extension XML. The extension then requests
		the corresponding extension from the extension manager.
		Assuming the extension manager returns an extension, this
		function returns its database.

		If the eid cannot be resolved, a DbError is raised.
		"""
		fdk_ext = self.eid_to_fdk_ext.get(eid, None)

		if fdk_ext is None:
			# Resolve extension data from the eid
			dep_e = self.element.find("./dependencies/fdk-extension[@eid='%s']" % eid)

			if dep_e is None:
				raise DbError("Could not find FDK extension dependency with eid=`%s'" % eid)

			ext_uuid = dep_e.get('uuidref')
			ext_version = dep_e.get('version')

			# Request the extension from manager and return it
			fdk_ext = self.manager.request_extension(ext_uuid, ext_version)

			self.eid_to_fdk_ext[eid] = fdk_ext

		ext_db = fdk_ext.get_database()

		return ext_db


	def get_database(self):
		"""
		Return the database belonging to the extension.

		If the database is not already loaded, this function will
		attempt to load it according to the cache usage setting, if a
		cache file is available at all.

		If the cache is not used or the file is not available, a cache
		file is saved when the database has been created.

		If the attribute expand_to_manager_root_path has been set to
		True, the build paths in the extension are extended so that they
		becomes relative to the extension manager's root path, i.e., the
		base directory of all extensions.
		"""
		if not self.database:
			if self.use_cache and os.path.isfile(self.xml_cache_path):
				# Load cache
				database = ConfigDB(self.runtime, self, self.xml_cache_path)
			else:
				# Load all content XML
				database = ConfigDB(self.runtime, self)
				# Write cache to disk
				database.write_xml_tree(self.xml_cache_path)

			# At this point, all paths in the database should be relative
			# to the directory that the extension XML file resides in.

			# Adding a path prefix is needed for inter-extension builds to be possible
			if self.expand_to_manager_root_path:
				path_prefix = self.relative_root_path
				database._expand_paths_from_root(path_prefix)

			self.database = database
		else:
			database = self.database

		return database


	@staticmethod
	def load_schema(xsd_file):
		"""
		Load the XML schema from specified file.
		"""
		if xsd_file is not None:
			xsd = ET.parse(xsd_file)
			schema = ET.XMLSchema(xsd)
		else:
			schema = None

		FdkExtension.schema = schema


	@staticmethod
	def validate_xml(tree):
		"""
		Validate the given ElementTree XML tree using the XML schema
		that is loaded for the FdkExtension.

		The function raises an ET.DocumentInvalid if the tree does not
		validate.
		"""
		if FdkExtension.schema is not None:
			FdkExtension.schema.assertValid(tree)


	# Important file paths

	@property
	def root_path(self):
		"""
		Root path of the extension, i.e., the directory with its
		extension XML
		"""
		return self._root_path


	@property
	def relative_root_path(self):
		"""
		Root path of the extension relative to extension manager's root
		path
		"""
		return os.path.relpath(self.root_path, self.manager.root_path)


	def _normalize_path(self, path):
		"""
		Prefixes the supplied path with extension's relative path from
		the extension manager's root directory, then replaces all
		backward and forward slashes to the OS' default path separator.
		"""
		if self.expand_to_manager_root_path:
			path = os.path.join(self.relative_root_path, path)
			# Convert all slashes to OS' default path separator
			path = path.replace('\\', os.sep)
			path = path.replace('/', os.sep)
		return path


	@property
	def root_xml_path(self):
		"""
		The path of the extension XML
		"""
		return os.path.join(self.root_path, self.xml_filename)


	@property
	def xml_cache_path(self):
		"""
		The path of the content XML cache
		"""
		return os.path.join(self.root_path, self.xml_cache_filename)


	# Extension identifier properties

	@property
	def uuid(self):
		"""
		Return the UUID of the extension.

		This is what the extension manager uses to uniquely identify an
		extension.
		"""
		return self.element.get('uuid')


	@property
	def org(self):
		"""
		Return the org of the extension, basically a name to organize
		the extension under -- usually a shortened form of the author
		name.
		"""
		return self.element.get('org')


	@property
	def shortname(self):
		"""
		Return the short name of the extension, e.g., abbreviation or
		acronym.
		"""
		return self.element.get('shortname')


	@property
	def version(self):
		"""
		Return the version number of the extension.
		"""
		return self.element.get('version')


	@property
	def fullname(self):
		"""
		Return the full name of the extension.
		"""
		return self.element.get('fullname')


	# Extension detail properties

	@property
	def _author(self):
		"""
		Helper function which returns the author attributes, i.e., a
		dictionary with name, website and email keys.
		"""
		e = self.element.find('./author')
		return e.attrib


	@property
	def author_name(self):
		"""
		Return the author's name.
		"""
		return self._author.get('name')


	@property
	def author_website(self):
		"""
		Return the author's website address.
		"""
		return self._author.get('website')


	@property
	def author_email(self):
		"""
		Return the author's email address.
		"""
		return self._author.get('email')


	@property
	def description(self):
		"""
		Return the description text for the extension.
		"""
		e = self.element.find('./description')
		return e.text


	@property
	def icon_image(self):
		"""
		Return the path to the icon image.
		"""
		e = self.element.find('./icon-image')
		return self._normalize_path(e.get('path'))


	@property
	def preview_image(self):
		"""
		Return the path to the preview image.
		"""
		e = self.element.find('./preview-image')
		return self._normalize_path(e.get('path'))


	@property
	def search_terms(self):
		"""
		Return a list of search terms, if any are defined.
		"""
		terms = []

		for term_e in self.element.findall('./search-terms/term'):
			terms.append(term_e.text)

		return terms


	@property
	def license(self):
		"""
		Return the path to the license file.
		"""
		e = self.element.find('./license')
		return self._normalize_path(e.get('path'))


	@property
	def license_caption(self):
		"""
		Return the caption with which to present the license.
		"""
		e = self.element.find('./license')
		return e.get('caption')


	@property
	def release_notes(self):
		"""
		Return the URL to the release notes.
		"""
		e = self.element.find('./release-notes')
		return e.get('url')


	@property
	def release_notes_caption(self):
		"""
		Return the caption with which to present the release notes.
		"""
		e = self.element.find('./release-notes')
		return e.get('caption')


	# Help documentation properties

	@property
	def online_help_index_caption_and_url(self):
		"""
		Return a tuple with the caption and URL to the online help
		index.
		"""
		caption = None
		url = None

		page_e = self.element.find('./online-help/index-page')
		if page_e is not None:
			caption = page_e.get('caption')
			# Set version by force here?
			url = page_e.get('baseurl')

		return (caption, url)


	def _get_online_element_doc_scheme_and_url(self, tag):
		"""
		Helper function which retrieves the scheme and baseurl from a
		specified tag.
		"""
		scheme = None
		url = None

		page_e = self.element.find('./online-help/%s' % tag)
		if page_e is not None:
			scheme = page_e.get('scheme')
			# Set version by force here?
			url = page_e.get('baseurl')

		return (scheme, url)


	@property
	def online_module_help_scheme_and_url(self):
		"""
		Return a tuple with the scheme and URL (processed according to
		the scheme) to the online module-specific help.

		For scheme == asf-docs, $VER$/$MODULE$/html/ is appended to the
		base URL.

		If the scheme is unknown, a DbError is raised.
		"""
		(scheme, url) = self._get_online_element_doc_scheme_and_url('module-help-page')

		if url is not None:
			if scheme == 'asf-docs':
				url =  url.rstrip('/') + '/' + self.asf_docs_url_postfix
			elif scheme not in self.doc_schemes:
				raise DbError("Unknown scheme `%s' for online module help in %s" % (scheme, self.root_xml_path))

		return (scheme, url)


	@property
	def online_module_guide_scheme_and_url(self):
		"""
		Return a tuple with the scheme and URL (processed according to
		the scheme) to the online module-specific guides.

		For scheme == asf-docs, $VER$/$MODULE$/html/ is appended to the
		base URL.

		If the scheme is unknown, a DbError is raised.
		"""
		(scheme, url) = self._get_online_element_doc_scheme_and_url('module-guide-page')

		if url is not None:
			if scheme == 'asf-docs':
				url =  url.rstrip('/') + '/' + self.asf_docs_url_postfix
			elif scheme not in self.doc_schemes:
				raise DbError("Unknown scheme `%s' for online module guide in %s" % (scheme, self.root_xml_path))

		return (scheme, url)


	@property
	def offline_help_index_caption_and_path(self):
		"""
		Returns a tuple with the caption and path to the offline help
		index page.
		"""
		caption = None
		path = None

		page_e = self.element.find('./offline-help/index-page')
		if page_e is not None:
			caption = page_e.get('caption')
			path = self._normalize_path(page_e.get('path'))

		return (caption, path)


	def _get_offline_element_doc_scheme_and_path(self, tag):
		"""
		Helper function which retrieves the scheme and path from a
		specified tag.
		"""
		scheme = None
		path = None

		page_e = self.element.find('./offline-help/%s' % tag)
		if page_e is not None:
			scheme = page_e.get('scheme')
			path = self._normalize_path(page_e.get('path'))

		return (scheme, path)


	@property
	def offline_module_help_scheme_and_path(self):
		"""
		Return a tuple with the scheme and path (processed according to
		the scheme) to the offline module-specific help.

		For scheme == asf-docs, $MODULE$\html\ is appended to the
		path.

		If the scheme is unknown, a DbError is raised.
		"""
		(scheme, path) = self._get_offline_element_doc_scheme_and_path('module-help-page')

		if path is not None:
			if scheme == 'asf-docs':
				path = os.path.join(path, self.asf_docs_path_postfix)
			elif scheme not in self.doc_schemes:
				raise DbError("Unknown scheme `%s' for online module help in %s" % (scheme, self.root_xml_path))

		return (scheme, path)


	@property
	def offline_module_guide_scheme_and_path(self):
		"""
		Return a tuple with the scheme and path (processed according to
		the scheme) to the offline module-specific guides.

		For scheme == asf-docs, $MODULE$\html\ is appended to the
		path.

		If the scheme is unknown, a DbError is raised.
		"""
		(scheme, path) = self._get_offline_element_doc_scheme_and_path('module-guide-page')

		if path is not None:
			if scheme == 'asf-docs':
				path = os.path.join(path, self.asf_docs_path_postfix)
			elif scheme not in self.doc_schemes:
				raise DbError("Unknown scheme `%s' for online module guide in %s" % (scheme, self.root_xml_path))

		return (scheme, path)


class StudioFdkExtension(FdkExtension):
	valid_generator_names = [
		avrstudio5.AVRStudio5Project.generator_tag,
		avrstudio5.AVRStudio5Project32.generator_tag,
		avrstudio5.AVRStudio5ProjectARM.generator_tag,
		avrstudio5.AVRStudio5ProjectTemplate.generator_tag,
		avrstudio5.AVRStudio5Project32Template.generator_tag,
		avrstudio5.AVRStudio5ProjectARMTemplate.generator_tag,
	]

	def __init__(self, runtime, root_path, use_cache=False):
		super(self.__class__, self).__init__(runtime, root_path, use_cache)

		# Load the database instantly
		self.get_database()


	def get_asf_h_content(self, toolchain, modules, template_file=None):
		"""
		Return the content of asf.h for the supplied list of modules.

		The toolchain must be specified, i.e., either avrgcc or
		avr32gcc.

		The template asf.h can also be specified -- it must then contain
		the string $ASF_HEADER_FILES$ where the includes are to be
		put.
		"""
		# GenericElement expects a project to get config from at init
		class Dummy(object):
			@staticmethod
			def get_configuration():
				return {}

		dummy = Dummy()
		generic_generator = generic.GenericElement(dummy, self.database, self.runtime)
		generic_generator.toolchain = toolchain

		asf_h = generic_generator._generate_asf_header_content(modules, template_file)

		return asf_h

	def get_all_project_ids(self):
		"""
		Returns a list of all project ids in the extension's database.

		The projects are not checked with regards to which generator
		they support.
		"""
		project_ids = []

		for project in self.database.iter_projects():
			project_ids.append(project.id)

		return project_ids

	def get_generator_from_project_id(self, project_id):
		"""
		Returns a generator object for the project with the specified
		ID in the extension's database.
		"""
		project = self.database.lookup_by_id(project_id)

		# Look for a valid generator for the project
		for generator_name in project.get_generator_list():
			if generator_name in self.valid_generator_names:
				generator_class = self.runtime.generator_tag_to_class(generator_name)

				# Instantiate the generator and return it
				return generator_class(project, self.database, self.runtime)

		# No valid generator was found for the project
		return None

	def _get_license_to_captions(self, generator):
		"""
		Return dictionary which maps a license file to the captions of
		whichever modules have specified it as their license.
		"""
		license_to_mods = generator._get_license_to_modules()
		license_to_captions = {}

		for license, mods in license_to_mods.items():
			caption_list = []

			for module in mods:
				caption = module.caption

				if caption:
					caption_list.append(caption)
				else:
					self.runtime.log.error("Module `%s' has empty caption" % module.id)

			caption_list.sort()
			license_to_captions[license] = caption_list

		return license_to_captions

	def get_project_data(self, project_id):
		"""
		Return a dictionary with data for the project with the specified
		ID:
		id, type, board vendor, board addons, device, device series,
		caption, name, description, help_url, keywords and licenses.
		"""
		generator = self.get_generator_from_project_id(project_id)

		if generator:
			generator._get_project_data()

			project_data = {
				'id'            : generator.project_id,
				'type'          : generator.project_type,
				'board_vendor'  : generator.project_board_vendor,
				'board_addons'  : generator.project_board_addons,
				'device'        : generator.project_device,
				'device_series' : generator.project_device_series,
				'caption'       : generator.project_caption,
				'name'          : generator.project_name,
				'description'   : generator.project_description,
				'help_url'      : generator.project_help_url,
				'keywords'      : generator.project_keywords,
				'licenses'      : self._get_license_to_captions(generator),
			}

			return project_data

		# No generator was found
		return None

	def write_project_data_to_xml_file(self, project_ids, filename):
		"""
		Writes data about all the specified projects to the specified file in the following XML format.
		The data is stored in the text node of the element: <ProjectId>..</ProjectId>

		<AsfProjects>
			<Project>
				<ProjectId/>
				<ProjectType/>
				<ProjectBoardVendor/>
				<ProjectDevice/>
				<ProjectDeviceSeries/>
				<ProjectCaption/>
				<ProjectName/>
				<ProjectDescription/>
				<ProjectHelpUrl/>
				<ProjectKeywords>
					<Category/>
					<Keyword/>
					...
				</ProjectKeywords>
				...
				<ProjectLicense>
					<LicenseFile/>
					<LicensedCaption/>
					<LicensedCaption/>
					...
				</ProjectLicense>
				...
				<ProjectBoardAddon>
					<Caption/>
					<Vendor/>
					<Position/>
				</ProjectBoardAddon>
				...
			</Project>
		</AsfProjects>
		"""
		# Define the tags
		root_tag = 'AsfProjects'
		project_tag = 'Project'
		project_data_key_to_tag = {
			'id'            : 'ProjectId',
			'type'          : 'ProjectType',
			'board_vendor'  : 'ProjectBoardVendor',
			'device'        : 'ProjectDevice',
			'device_series' : 'ProjectDeviceSeries',
			'caption'       : 'ProjectCaption',
			'name'          : 'ProjectName',
			'description'   : 'ProjectDescription',
			'help_url'      : 'ProjectHelpUrl',
		}
		project_data_key_to_dict_tag_maps = {
			# These are for dicts which map to a list, where each entry should have the same tag
			'keywords'      : ('ProjectKeywords', 'Category', 'Keyword'),
			'licenses'      : ('ProjectLicense', 'LicenseFile', 'LicensedCaption'),
			# These are for dicts which map to another dict, where each key-value pair has its own tag
			'board_addons'  : ('ProjectBoardAddon', 'Caption', [('vendor', 'Vendor'), ('position', 'Position')]),
		}

		# Create the top level XML element
		asf_projects_e = ET.Element(root_tag)

		# Add XML for the individual projects
		for project_id in project_ids:

			project_data = self.get_project_data(project_id)

			# Add the project to XML if its data could be extracted
			if project_data:
				project_e = ET.SubElement(asf_projects_e, project_tag)

				# Add the simple project data to XML
				for key, tag in project_data_key_to_tag.items():
					tag_e = ET.SubElement(project_e, tag)
					tag_e.text = project_data[key]

				# Add dict-ified project data to XML
				for key, (top_tag, key_tag, data_tag_map) in project_data_key_to_dict_tag_maps.items():
					for data_key, data_obj in project_data[key].items():
						top_tag_e = ET.SubElement(project_e, top_tag)

						key_tag_e = ET.SubElement(top_tag_e, key_tag)
						key_tag_e.text = data_key

						if isinstance(data_tag_map, str):
							data_tag = data_tag_map

							for list_entry in data_obj:
								data_tag_e = ET.SubElement(top_tag_e, data_tag)
								data_tag_e.text = list_entry

						else:
							assert(isinstance(data_tag_map, list))
							for data_obj_key, data_tag in data_tag_map:
								data_tag_e = ET.SubElement(top_tag_e, data_tag)
								data_tag_e.text = data_obj.get(data_obj_key)

		# Now write to file
		etree = ET.ElementTree(asf_projects_e)
		etree.write(filename)

	def get_items_for_mcu(self, mcu, mod_types_to_get=None):
		"""
		Return the list of all modules/select-by-device/select-by-config
		associated with the MCU.

		mod_types_to_get is optional, but can be a list of module types:
		["driver", "component", "service", "library"]

		If unspecified, it defaults to None, and no filtering on type is
		done.
		"""
		mods = self.database.find_modules_for_mcu(mcu, mod_types_to_get, check_hidden=True)
		return mods

	def get_mcu(self,mcu_name):
		"""
		Return an MCU object for the specified name.
		"""
		return MCU(mcu_name,self.database)


	def get_remote_ids(self, mcu_name, id):
		"""
		This function should be deleted once Atmel Studio stops using it.
		"""
		return [id]

	def get_all_modules_and_selectors(self, mcuname, mod_types_to_ignore=None):
		"""
		This method returns a list of all modules and module selectors
		that support the specified MCU.

		mod_types_to_ignore is optional, but can be a list of module
		types:
		["application", "build-specific"]

		If unspecified, it defaults to None, and no filtering on type is
		done. Note that this filter behaves in the opposite way of how
		the filtering in get_items_for_mcu() does.

		Modules of type meta should not show up in the wizard.
		"""
		retlist = []
		mcu = self.get_mcu(mcuname)
		map = mcu.get_group_map()

		mods = self.database.iter_modules()

		for mod in mods:
			if (mod_types_to_ignore is not None) and (mod.type in mod_types_to_ignore):
				continue

			not_supported = mod.resolve_device_support(map, check_self=True)
			if (len(not_supported) == 0):
				retlist.append(mod)

		sel_iterators = [self.database.iter_components(SelectByDevice), self.database.iter_components(SelectByConfig)]

		for sels in sel_iterators:
			for sel in sels:
				not_supported = sel.resolve_device_support(map, check_self=True)
				if len(not_supported) == 0:
					retlist.append(sel)

		return retlist
