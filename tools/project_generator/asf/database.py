import asf.etreeabstraction as ET
import os
import os.path
import re
import urllib

from datetime import datetime

import asf.helper

from asf.build import *
from asf.exception import *
from asf.configuration import *


class ConfigItem(object):
	tag = "*"
	# String that is used for indicating that any device is supported
	_device_support_all_devices = "*"

	def __init__(self, element, db):
		self.element = element
		self.db = db
		self.prerequisites = []

		for child in element.iterchildren(tag='require'):
			idref = child.attrib['idref']

			# db raises exception for erroneous idref
			component = db.lookup_by_id(idref)
			self.prerequisites.append(component)

		self._element_configuration = self._get_configuration_from_element()

	@property
	def id(self):
		return self._require_property('id')

	@property
	def type(self):
		return self._get_property('type')

	@property
	def caption(self):
		return self._get_property('caption')

	@property
	def toolchain(self):
		return self._get_property('toolchain')

	@property
	def basedir(self):
		""" Return base directory from which the module has been found.
		"""
		current = self.element
		basedir_attribute = ConfigDB.basedir_tag
		while current is not None:
			if current.attrib.has_key(basedir_attribute):
				return current.attrib[basedir_attribute]
			current = current.getparent()

		return None

	@property
	def fromfile(self):
		""" Return the asf.xml file which the module belongs.
		"""
		current = self.element
		file_attribute = ConfigDB.fromfile_tag
		while current is not None:
			if current.attrib.has_key(file_attribute):
				return current.attrib[file_attribute]
			current = current.getparent()

		return None

	def _get_property(self, property):
		"""
		Return value of a property, or None if the corresponding
		element attribute is not set, i.e., there is no value
		for the property.
		"""
		if self.element.attrib.has_key(property):
			return self.element.attrib[property]
		else:
			return None

	def _require_property(self, property):
		"""
		Return value of a property. Raise a ConfigError if the
		corresponding element attribute is not set, i.e., there
		is no value for the property.
		"""
		if self.element.attrib.has_key(property):
			return self.element.attrib[property]
		else:
			raise ConfigError("`%s' with id `%s' and type `%s' is missing attribute `%s'" % (self.__class__.__name__, self.id, self.type, property))

	def _make_child_xpath(self, tag=None, attribute_list=None, value_list=None):
		"""
		Returns an XPath expression for locating child tags with the
		specified tag name, attributes and corresponding values.
		The lists of attributes and values need not be the same length,
		however, it does not make sense to have more values than
		attributes.
		If there are more attributes than values, the resulting XPath
		expression will only require that the attributes are present.

		All of the parameters are optional.

		For example, to search for all child tags featuring a type-
		attribute with a specific value, and a subtype attribute with
		no specific value:
			self._make_child_xpath(None,
					['type', 'subtype'],
					['test'])
		This results in the following XPath expression:
			*[@type='test'][@subtype]
		"""
		if tag:
			xpath = tag
		else:
			xpath = "*"

		while attribute_list:
			attribute = attribute_list.pop(0)

			xpath += "[@%s" % attribute

			if value_list:
				try:
					value = value_list.pop(0)
					if value:
						xpath += "='%s'" % value
				except IndexError:
					pass

			xpath += "]"

		return xpath

	def get_child_elements(self, tag=None, attribute_list=None, value_list=None):
		"""
		Returns all immediate sub-elements of the ConfigItem's element.
		Elements at deeper levels are excluded.

		All parameters are optional.
		See _make_child_xpath() for details.
		"""
		xpath = "./" + self._make_child_xpath(tag, attribute_list, value_list)
		return self.element.findall(xpath)

	def get_sub_elements(self, tag=None, attribute_list=None, value_list=None):
		"""
		Returns all sub-elements of the ConfigItem's element. Elements
		at all depths are included.

		All parameters are optional.
		See _make_child_xpath() for details.
		"""
		xpath = ".//" + self._make_child_xpath(tag, attribute_list, value_list)
		return self.element.findall(xpath)

	def _get_build_elements(self, build_type):
		attribute_list = ['type']
		value_list = [build_type.type]

		if build_type.subtype != BuildType.subtype:
			attribute_list.append('subtype')
			value_list.append(build_type.subtype)

		return self.get_child_elements(build_type.tag, attribute_list, value_list)

	def get_build(self, build_type, toolchain, recursive=True, sort=False):
		"""
		Get build items of a given build_type.
		You must run resolve_all_selections if module selectors are used.
		If not an Exception will be raised.

		@params build_type Type to get, must be a subclass of BuildType
		@params toolchain  String with name of toolchain in use or None
		@params recursive  Parse recursively from all requirements. True or False
		"""
		if not issubclass(build_type, BuildType):
			raise TypeError("`build_type' must be a subclass of BuildType")

		files = []

		# Get files for this element
		for f in self._get_build_elements(build_type):
			# If toolchain attribute is set for build element, only include it
			# if we generating project for that toolchain
			if f.attrib.has_key('toolchain'):
				if f.attrib["toolchain"] == toolchain:
					files.append(build_type.get_build_item_from_element(f))
			else:
				files.append(build_type.get_build_item_from_element(f))

		# Get files for for all prerequisites, if this is a recursive search
		if recursive:
			for prereq in self.prerequisites:
				for f in prereq.get_build(build_type, toolchain, recursive):
					files.append(f)

		# Remove duplicates
		files = list(set(files))
		# Sort build items?
		if sort:
			files.sort()

		return files

	def get_prerequisites(self, recursive=True):
		prereqs = []

		for prereq in self.prerequisites:
			prereqs.append(prereq)
			if recursive:
				prereqs.extend(prereq.get_prerequisites(recursive))

		return asf.helper.uniquify_list(prereqs)

	def get_prerequisite_ids(self, recursive=True):
		prereq_ids = []

		for prereq in self.prerequisites:
			prereq_ids.append(prereq.id)
			if recursive:
				prereq_ids.extend(prereq.get_prerequisite_ids(recursive))

		return asf.helper.uniquify_list(prereq_ids)

	def get_device_support(self):
		"""
		Return a list of supported devices that is specified for this tag.
		"""
		device_list = []
		for e in self.get_child_elements("device-support"):
			device_list.append(e.attrib["value"])

		return device_list

	def _get_info_list(self, info_type, recursive=False):
		"""
		Returns a list with all info of the specified type that is
		specified for this tag.

		The XML is expected to be in this format:
		<info type="info_type" value="f"/>
		<info type="info_type" value="o"/>
		<info type="info_type" value="o"/>
		Where "f", "o" and "o" will be elements in the list.
		"""
		item_attr = "value"
		log_warning = self.db.log.warning

		info = []

		for e in self.get_child_elements('info', ['type'], [info_type]):
			info.append(e.attrib[item_attr])

		if recursive:
			for prereq in self.prerequisites:
				prereq_info = prereq._get_info_list(info_type, recursive)
				info.extend(prereq_info)

		return sorted(list(set(info)))

	def _get_info_value(self, info_type):
		"""
		Returns the value of a single info item of the specified type
		that is specified for this tag.

		If multiple info items of the same type are found, an error is
		output. If the info type is not found, None is returned.
		"""
		log_error = self.db.log.error
		value = None
		info = self._get_info_list(info_type)

		if len(info) > 1:
			log_error("%s has multiple info-tags of type `%s', expected 1 or less" % (str(self), info_type))

		if info:
			value = info[0]

		return value

	def _get_info_dict(self, info_type, recursive=False):
		"""
		Returns a dictionary of the specified info type, where the
		info tag's value attribute is used as key to get the info lists.

		The XML is expected to be in this format:
		<info type="info_type" value="foo">
			<info_type value="b"/>
			<info_type value="a"/>
			<info_type value="r"/>
		</info>
		Where "foo" will be a key in the dictionary, which will map to
		a list consisting of "b", "a" and "r".
		"""
		key_attr = "value"
		item_attr = "value"
		log_warning = self.db.log.warning

		info = {}

		for top_info in self.get_child_elements('info', ['type'], [info_type]):
			key = top_info.attrib[key_attr]

			item_list = info.get(key, [])
			if item_list:
				log_warning("%s has multiple info-tags of type `%s' with attribute `%s' set to `%s'" % (str(self), info_type, key_attr, key))

			for sub_info in top_info.findall("./%s[@%s]" % (info_type, item_attr)):
				item_list.append(sub_info.attrib[item_attr])

			if item_list:
				info[key] = sorted(item_list)
			else:
				log_warning("%s has info-tag(s) of type `%s' with no valid child `%s' tags" % (str(self), info_type, info_type))

		if recursive:
			for prereq in self.prerequisites:
				prereq_info = prereq._get_info_dict(info_type, recursive)

				# Check for each key in the prerequisite's info dicts
				# whether they already exist in the total info; we
				# want to add to the info lists, not overwrite them.
				for key, item_list in prereq_info.items():
					item_list.extend(info.get(key, []))
					info[key] = sorted(list(set(item_list)))

		return info

	def get_appnote_list(self, recursive=False):
		return self._get_info_list("appnote", recursive)

	def get_keyword_dict(self, recursive=False):
		return self._get_info_dict("keyword", recursive)

	@staticmethod
	def _format_description_text(text):
		"""
		Clean the description text of "formatting artifacts" from the
		ASF XML:
		- remove whitespace at start and end of all lines
		- replace empty lines with a single newline
		- concatenate non-empty lines with space as separator
		"""
		if text == None:
			return None

		formatted = ''
		lines = [l.strip() for l in text.strip().splitlines()]

		format_lines = []
		new_line = True
		for l in lines:
			if l == '':
				format_lines.append('\n')
				new_line = True
			else:
				if new_line == False:
					format_lines.append(' ')
				format_lines.append(l)
				new_line = False

		return ''.join(format_lines)

	def _get_description_text_by_type(self, desc_type):
		"""
		Get the description text of specified type ('summary' or
		'description') from the module's XML element.

		The text is formatted before it is returned.
		"""
		desc_elements = self.get_child_elements('info', ['type', 'value'], ['description', desc_type])
		num_elements = len(desc_elements)

		if num_elements > 1:
			self.db.log.error("Found %s description %s for module `%s', expected 1" % (num_elements, desc_type, self.id))

		try:
			desc = desc_elements[0].text
		except IndexError:
			return None
		else:
			return desc

	def get_description_summary_text(self):
		desc = self._get_description_text_by_type('summary')
		return self._format_description_text(desc)

	# Return summary text as a list of stripped lines
	def get_description_summary_text_list(self):
		desc = self._get_description_text_by_type('summary')
		if desc:
			return [l.strip() for l in desc.strip().splitlines()]
		return None

	def get_description_detailed_text(self):
		desc = self._get_description_text_by_type('detailed')
		return self._format_description_text(desc)

	# Return detailed text as a list of stripped lines
	def get_description_detailed_text_list(self):
		desc = self._get_description_text_by_type('detailed')
		if desc:
			return [l.strip() for l in desc.strip().splitlines()]
		return None

	def get_gui_flag_list(self, recursive=False):
		return self._get_info_list("gui-flag", recursive)

	def has_subids(self):
		"""
		Returns True if module is a module selector that has subids.
		Returns False for all other modules
		"""
		return False

	def get_subids(self, strip_ids=False, mcu=None):
		"""
		Get list of sub ids for the module. (See also has_subids())

		For a normal module an empty list is returned.
		For a select-by-device or select-by-config it returns ids of the submodules.

		If strip_ids is True the common part of the ids are stripped (id after the '#' is returned)
		Set mcu as instance of MCU class to return only ids compatible with the MCU.
		"""
		return []

	def resolve_all_selections(self, config, mcu):
		if not isinstance(config, ConfigurationHandler):
			raise ConfigError("Configuration '%s' is not instance of ConfigurationHandler" % (config))
		if not isinstance(mcu, MCU):
			raise ConfigError("MCU '%s' is not instance of MCU" % (mcu))

		for prereq in self.prerequisites:
			prereq.resolve_all_selections(config, mcu)

		return self

	def _get_highest_priority_value(self, value_list, priority_mapping_function):
		"""
		Returns the value from a list of values which has the highest
		priority according to the supplied priority mapping function.
		A high priority means that the function returns a low number.

		If the list is empty, None is returned.
		"""
		if not value_list:
			current_value = None
		else:
			current_value = value_list.pop()
			current_priority = priority_mapping_function(current_value)

			for new_value in value_list:
				new_priority = priority_mapping_function(new_value)

				if new_priority < current_priority:
					current_value = new_value
					current_priority = new_priority

		return current_value

	def resolve_device_support(self, mcu_search_list, recursive=True, check_self=True):
		"""
		Checks all prerequisites for device-support according to the given
		mcu_search_list. Returns a list of modules that do not meet the
		device-support requirements.

		If check_self is set to True the module that the function is run on
		is checked in addition to the requirements.
		"""
		not_supported_list = []

		modules_to_check = []

		if check_self:
			modules_to_check.append(self)

		modules_to_check.extend(self.get_prerequisites(recursive))

		for prereq in modules_to_check:
			is_supported = False
			support_list = prereq.get_device_support()

			if self._device_support_all_devices in support_list:
				is_supported = True
				# Stop processing prereq and continue on next one
				continue

			for support in support_list:
				if support in mcu_search_list:
					is_supported = True

			if not is_supported:
				not_supported_list.append(prereq)

		return not_supported_list

	def _get_config_elements(self, name=None):
		xpath = './' + self._make_child_xpath('config', ['name'], [name])
		return self.element.findall(xpath)

	def _get_configuration_from_element(self):
		config_e = self._get_config_elements()

		result = ConfigurationHandler()

		for e in config_e:
			name = e.attrib['name']
			value = e.attrib['value']
			if result.get_config(name) is not None:
				raise ConfigError("ConfigItem `%s' contains multiple configs with name `%s'. Multiple definitions not allowed." % (self.id, name))
			result.set_config(name, value)

		return result

	def get_configuration(self):
		return self._element_configuration

	def get_config(self, name):
		return self.get_configuration().get_config(name)

	def get_config_group(self, group):
		return self.get_configuration().get_config_group(group)

	def get_generator_list(self):
		"""
		Find and return all children of the toolchain tag
		"""
		attr = 'value'
		xpath = './' + self._make_child_xpath('generator', [attr])
		generator_e = self.element.findall(xpath)

		generators = []
		for e in generator_e:
			generators.append(e.attrib[attr])

		return generators

	def get_related_modules_dict(self):
		"""
		Return a dictionary of IDs of related modules.
		"""
		module_id_dict = {}
		for e in self.get_child_elements("related-module"):
			type = e.attrib["type"]
			module_id_dict[type] = module_id_dict.get(type, []) + [e.attrib["module-id"]]

		return module_id_dict

	def get_related_module_type_ids(self, type):
		"""
		Return a list of IDs of related modules of given type.
		"""
		return self.get_related_modules_dict().get(type, [])

	def __repr__(self):
		return "ConfigItem[class=%s tag=%s id=%s caption=%s]" % (self.__class__.__name__, self.tag, self.id, self.caption)

	def visualize_info(self, log_function):
		"""
		Output the appnote and keyword types of info,
		if any is set, to the specified log function.
		"""
		indent = 4
		value_topics = {
				'Description summary'     : self.get_description_summary_text,
				'Description details'      : self.get_description_detailed_text,
			}
		# Map info type -> function which returns corresponding list
		list_topics = {
				'Appnote' : self.get_appnote_list,
			}
		# Map info type -> function which returns corresponding dict
		dict_topics = {
				'Keyword' : self.get_keyword_dict,
				'Related modules' : self.get_related_modules_dict,
			}


		output = ''
		# Iterate through topics that return values
		for topic in value_topics:
			value = value_topics[topic]()
			if value:
				output += '%s%s:\n' % (' '.ljust(indent), topic)
				output += '%s%s\n' % (' '.ljust(2 * indent), value)
				output += '\n'

		# Iterate through topics that return lists
		for topic in list_topics:
			# Get list of items for topic
			topic_items = list_topics[topic]()
			# Output topic and its items, if they exist
			if len(topic_items):
				output +=  '%s%s:\n' % (' '.ljust(indent), topic)
				# Output topic items
				for item in topic_items:
					output += "%s'%s'\n" % (' '.ljust(2 * indent), item)
				output += '\n'


		# Iterate through topics that return dictionaries
		for topic in dict_topics:
			# Get dictionary of subtopics
			dict_subtopics = dict_topics[topic]()
			# Output topic, subtopics and their items, if subtopics exist
			if len(dict_subtopics):
				output +=  '%s%s:\n' % (' '.ljust(indent), topic)
				for subtopic in dict_subtopics:
					# Output the subtopic, get its item list
					output +=  "%s'%s':\n" % (' '.ljust(2 * indent), subtopic)
					subtopic_items = dict_subtopics[subtopic]
					# Output subtopic items
					for item in subtopic_items:
						output +=  "%s'%s'\n" % (' '.ljust(3 * indent), item)
					output += '\n'
				output += '\n'

		log_function(output)

	def visualize_requirements(self, log_function, padding=' '):
		if isinstance(self, ModuleSelector):
			log_function("%s+-%s [%s] %s" % (padding[:-1], self.id, self.tag, str(self.get_device_support()) ))
		else:
			log_function("%s+-%s [%s] %s" % (padding[:-1], self.id, self.tag, str(self.get_device_support())))
		padding = padding + ' '

		total = len(self.prerequisites)
		count = 0
		for p in self.prerequisites:
			count += 1
			log_function(padding + '|')
			if count == total:
				p.visualize_requirements(log_function, padding + ' ')
			else:
				p.visualize_requirements(log_function, padding + '|')

	def get_doxygen_entry_point(self):
		"""
		Return the Doxygen entry point associated with this module
		"""
		entry_point_list = self.get_build(BuildDoxygenEntryPoint, None, False)
		if len(entry_point_list) > 0:
			return entry_point_list[0]

		# If the module is a Project, look into its dependencies to find a match
		if isinstance(self, Project):
			project_module_list = self.get_project_modules()
			for project_module in project_module_list:
				entry_point_list = project_module.get_build(BuildDoxygenEntryPoint, None, False)
				if len(entry_point_list) > 0:
					return entry_point_list[0]

		return None

	def get_doxygen_quick_start(self):
		"""
		Return the Doxygen quick start associated with this module
		"""
		quick_start_list = self.get_build(BuildDoxygenQuickStart, None, False)
		if len(quick_start_list) > 0:
			return quick_start_list[0]
		return None

	def get_all_doxygen_quick_start(self):
		"""
		Return all Doxygen quick starts associated with this module
		"""
		quick_start_list = self.get_build(BuildDoxygenQuickStart, None, False)
		if len(quick_start_list) > 0:
			return quick_start_list
		return None

	def get_help_uri(self):
		"""
		Return the URI (Uniform Ressource Identifier) of the help entry point
		"""
		doxygen_ref = self.get_doxygen_entry_point()
		if not doxygen_ref:
			return None
		return "group__" + re.sub("_", "__", doxygen_ref.lower()) + ".html"

	def get_quick_start_uri(self):
		"""
		Return the URI (Uniform Ressource Identifier) of the quick start entry point
		"""
		doxygen_ref = self.get_doxygen_quick_start()
		if not doxygen_ref:
			return None
		return  doxygen_ref.lower() + ".html"

	def get_help_dir(self):
		return os.path.join(self.basedir, "doxygen", self.id)

	def get_help_url(self, module_dir, custom_version = None):
		"""
		Return the help URL for this item.
		"""
		if not custom_version:
			# Use framework version if no version provided
			custom_version = self.db.get_framework_version_number()

		uri = self.get_help_uri()
		if not uri:
			# No documentation available for this module
			return None

		url = self.db.get_documentation_server() + uri

		url = url.replace("$VER$", urllib.quote(custom_version))
		url = url.replace("$MODULE$", urllib.quote(module_dir))

		return url

	def get_quick_start_url(self, doc_arch, custom_version = None):
		"""
		Return the quick start URL for this item.
		"""
		if not custom_version:
			# Use framework version if no version provided
			custom_version = self.db.get_framework_version_number()

		uri = self.get_quick_start_uri()
		if not uri:
			# No quick start available for this module
			return None

		url = self.db.get_documentation_server() + uri

		url = url.replace("$VER$", urllib.quote(custom_version))
		url = url.replace("$MODULE$", urllib.quote(doc_arch))

		return url

class ModuleSelector(ConfigItem):
	tag = "*"

	def has_subids(self):
		return True

	def get_subids(self, strip_ids=False, mcu=None):
		id_attr = "id"

		mcu_search_list = False

		if not mcu is None:
			if isinstance(mcu, MCU):
				mcu_search_list = mcu.get_group_map()
			else:
				raise ConfigError("MCU '%s' is not instance of MCU or None" % (mcu))

		ids = []

		# Find direct children and possible options
		search_tags = [Module.tag, SelectByDevice.tag, SelectByConfig.tag]
		search_elem = []
		for tag in search_tags:
			search_elem.extend(self.get_child_elements(tag, ['id']))

		for elem in search_elem:
			id = elem.attrib[id_attr]
			pos = id.find("#")

			# Check if no "#" is found
			if pos < 0:
				raise ConfigError("Module selector " + self.id + " contains module without '#' " + id)

			add_module = True
			if mcu_search_list:
				module = self.db.lookup_by_id(id)
				not_supported = module.resolve_device_support(mcu_search_list)

				if len(not_supported) > 0:
					add_module = False

			if add_module:
				if strip_ids:
					# Add the id without the '#'
					ids.append(id[(pos + 1):])
				else:
					ids.append(id)

		return ids

	def get_device_support(self):
		return [self._device_support_all_devices]

	def select_module(self, configuration, mcu):
		# Select module based on search list, starting with the first entry
		selection = None

		search_list = self._get_search_list(configuration, mcu)
		for term in search_list:
			try:
				selection = self.db.relative_lookup_by_id(self.element, self.id + '#' + term)
			except NotFoundError:
				# No match, try next search term
				continue

			# Stop searching on first match
			break

		# Treat empty selection as an error
		if selection is None:
			raise NoSelectionError("`%s' with id `%s' found no selection. Tried %s" % (self.__class__.__name__, self.id, ", ".join(search_list)))

		return selection

	def reset_selection(self):
		self.prerequisites = []

	def _get_search_list(self, configuration, mcu):
		raise Exception("Abstract class method called")

	def _get_proxy(self):
		try:
			return self.prerequisites[0]
		except IndexError:
			raise Exception("Selections must be resolved before calling this function")

	def get_build(self, build_type, toolchain=None, recursive=True, sort=False):
		# Proxy function to selected module
		build = []
		if recursive:
			build.extend(self._get_proxy().get_build(build_type, toolchain, recursive, sort))
		return build

	def _get_build_elements(self, build_type):
		return []

	def _get_info_list(self, info_type, recursive=False):
		info_list = super(ModuleSelector, self)._get_info_list(info_type, recursive=False)
		if recursive:
			# Proxy function to selected module
			info_list.extend(self._get_proxy()._get_info_list(info_type, recursive=True))

		return sorted(list(set(info_list)))

	def _get_info_dict(self, info_type, recursive=False):
		info_dict = super(ModuleSelector, self)._get_info_dict(info_type, recursive=False)
		if recursive:
			# Proxy function to selected module
			add_info_dict = self._get_proxy()._get_info_dict(info_type, recursive=True)
			for key, add_list in add_info_dict.items():
				new_list = info_dict.get(key, [])
				new_list.extend(add_list)
				info_dict[key] = sorted(list(set(new_list)))

		return info_dict

	def resolve_all_selections(self, config, mcu):
		if not isinstance(config, ConfigurationHandler):
			raise ConfigError("Configuration '%s' is not instance of ConfigurationHandler" % (config))
		if not isinstance(mcu, MCU):
			raise ConfigError("MCU '%s' is not instance of MCU" % (mcu))

		# Forget previous selection first, in case of failure
		self.prerequisites = []
		# Resolve module selector
		selected = self.select_module(config, mcu)
		# Set the selected module as a prerequisite
		self.prerequisites = [selected]
		# Resolve sub-elements
		selected.resolve_all_selections(config, mcu)

	def get_help_uri(self):
		"""
		Special case for module selectors, they will never have any help documentation:

		Use _get_proxy() and call help_uri on that to find help url
		"""
		return None

class SelectByDevice(ModuleSelector):
	tag = "select-by-device"
	mcu_map_id = "map.atmel"

	def _get_search_list(self, configuration, mcu):
		"""
		Return the map, or list, of the project MCU and what device
		groups it belong to. The list is sorted in order of sinking
		specificity.
		"""
		return mcu.get_group_map()

	def select_module(self, configuration, mcu):
		# Select module based on search list, starting with the first entry
		selection = None

		device_support = self.get_device_support_mapping()

		search_list = self._get_search_list(configuration, mcu)
		for term in search_list:
			if term in device_support.keys():
				# Found a match, set it and return
				selection = self.db.lookup_by_id(device_support[term])

		# Treat empty selection as an error
		if selection is None:
			raise NoSelectionError("`%s' with id `%s' found no selection. Tried %s" % (self.__class__.__name__, self.id, ", ".join(search_list)))

		return selection

	def _get_all_device_support_elements(self):
		return self.element.findall("./%s/%s" % ( Module.tag, "device-support" ))

	def get_device_support_mapping(self):
		"""
		Return a dictionary with device support as key and module id of the corresponding device as value
		"""
		module_elements = self._get_all_device_support_elements()

		support = dict()
		for element in module_elements:
			key = element.attrib["value"]
			value = element.getparent().attrib["id"]
			support[key] = value

		return support

	def get_device_support(self):
		mapping = self.get_device_support_mapping()

		return mapping.keys()


class SelectByConfig(ModuleSelector):
	tag = "select-by-config"

	@property
	def name(self):
		return self._require_property("name")

	@property
	def default(self):
		return self._get_property("default")

	def _get_search_list(self, configuration, mcu):
		"""
		Generates a single-entry search list with the value of the
		selector's specified project config tag.
		If the config tag cannot be found, an optional default value is
		used and a log message is outputted. If no default value exists, a
		ConfigError is raised.
		"""
		search_term = configuration.get_config(self.name)

		if search_term is None:
			search_term = self.default
			if search_term is None:
				raise ConfigError("Project config `%s' missing for `%s' with id `%s' and no default" % (self.name, self.__class__.__name__, self.id))
			else:
				self.db.log.debug("`%s' with id `%s' using default project config value `%s'" % (self.__class__.__name__, self.id, self.default))

		return [search_term]

class Module(ConfigItem):
	tag = "module"

	def __init__(self, element, db):
		ConfigItem.__init__(self, element, db)
		self.filelist = set([])

	def get_device_support(self):

		# Meta or application modules do not require "device-support" tags
		if self.type in ["meta", "application"]:
			return [self._device_support_all_devices]
		else:
			return super(Module, self).get_device_support()


class TypelessConfigItem(ConfigItem):
	tag = "*"

	@property
	def type(self):
		return self.tag

class DeviceMap(TypelessConfigItem):
	tag = "device-map"
	group_tag = "group"
	doc_group_attr = "doc-arch"
	node_tag = "mcu"

	def _get_doc_arch_group_elements(self):
		"""
		"""
		return self.get_sub_elements(tag=self.group_tag, attribute_list=[self.doc_group_attr])

	def get_doc_arch_dict(self):
		"""
		"""
		doc_arch_list = []

		doc_arch_elements = self._get_doc_arch_group_elements()
		for e in doc_arch_elements:
			doc_arch_list.append((e.attrib['name'], e.attrib[self.doc_group_attr]))

		return dict(doc_arch_list)

	def iter_selection_elements(self, node_name):
		"""
		Iterator which yields the tags of the tree within a given node
		lies. The list is generated by searching for the node with the
		specified name in the selection map, then returning the parent
		tags.
		The list is ordered according to reduced specificity, i.e., the
		node name comes first, then the name of the immediate parent
		group, etc.
		"""
		# Find the end node, which is the first item in the search list
		node = self.element.findall(".//%s[@%s='%s']" % (self.node_tag, 'name', node_name))
		if len(node) != 1:
			raise DbError("Device selection map `%s' contains %s nodes with name `%s', expected 1" % (self.id, len(node), node_name))
		else:
			# Iterate through the node and parent tags, yielding their name
			current = node[0]
			while current.tag in (self.group_tag, self.node_tag):
				yield current
				current = current.getparent()

	def iter_selection_list(self, node_name):
		"""
		Iterator which yields items from a device selection list. The list is
		generated by searching for a node with the specified name in
		the selection map, then identifying the groups which lead
		to the node.
		The list is ordered according to reduced specificity, i.e., the
		node name comes first, then the name of the immediate parent
		group, etc.
		"""
		for element in self.iter_selection_elements(node_name):
			yield element.attrib['name']

	def get_mcu_list(self, group = None, check_group_is_mcu = False):
		"""
		Return a list of MCUs. By specifying group it can give all MCUs for that group.
		"""
		if group:
			mcus = self.element.findall(".//group[@name='%s']//mcu" % group)
		else:
			mcus = self.element.findall(".//mcu")
		mcu_list = []
		for mcu in mcus:
			mcu_list.append(mcu.attrib["name"])

		if group and not mcu_list and check_group_is_mcu:
			# We didn't find any mcu's. Check if the given 'group' is actually an mcu
			mcus = self.element.find(".//mcu[@name='%s']" % group)
			if mcus is not None:
				# Found it as mcu, just return it
				mcu_list = [group]

		return mcu_list

class MCU(object):
	tag =  DeviceMap.node_tag

	def __init__(self, mcu_name, db, map_id="map.atmel"):
		self.db = db
		self.mcu_name = mcu_name
		self.map_id = map_id

		try:
			self.map = self.db.lookup_by_id(map_id)
		except NotFoundError:
			raise NotFoundError("Unable to find device-map '%s'" % (self.map_id))

		if not isinstance(self.map, DeviceMap):
			raise ConfigError("Device-map '%s' is not of the correct class " % (self.map_id))

		# This raises DbError if an error occurs
		self._group_map = self._get_group_map()

	@property
	def name(self):
		return self.mcu_name

	def get_group_map(self):
		"""
		Returns a list of the MCU and the groups in which it belongs.
		The list is in order of sinking specificity, meaning the MCU
		itself is the first entry, and AVR is the last entry.
		"""
		return self._group_map

	def _get_group_map(self):
		list = []

		for term in self.map.iter_selection_list(self.mcu_name):
			list.append(term)

		return list

	def search_mcu_selection_list(self, prefix):
		"""
		Searches through the MCU selection list for the first entry
		which starts with the specified prefix. If no match is found,
		the function returns None.
		"""
		for item in self.get_group_map():
			if item[0:len(prefix)] == prefix:
				return item

		return None

	def get_doc_arch_group(self):
		"""
		Returns the doc-arch group of the MCU, i.e., the group which
		defines which MCUs are documented together.

		If no doc-arch is found, None is returned.
		"""
		for tag in self.map.iter_selection_elements(self.mcu_name):
			if tag.attrib.get(DeviceMap.doc_group_attr, None):
				return tag.attrib['name']

		return None

	def get_doc_arch(self):
		"""
		Wrapper function for Studio backwards compatibility.
		"""
		return self.get_doc_arch_group()


	def get_doc_arch_name(self):
		"""
		Returns the doc-arch name of the MCU.

		If no doc-arch is found, None is returned.
		"""
		for tag in self.map.iter_selection_elements(self.mcu_name):
			if tag.attrib.get(DeviceMap.doc_group_attr, None):
				return tag.attrib[DeviceMap.doc_group_attr]

		return None


class Board(TypelessConfigItem):
	tag = "board"

	def __init__(self, element, db):
		ConfigItem.__init__(self, element, db)

		mcu_name = self._require_property('mcu')
		self._mcu = MCU(mcu_name, db)

	@property
	def mcu(self):
		return self._mcu

	def get_device_support(self):
		return [self.mcu.name]

class Project(ConfigItem):
	tag = "project"

	type_attrib  = 'type'
	name_attrib  = 'workspace-name'
	valid_types  = ['normal', 'template', 'unit-test']
	default_type = valid_types[0]

	def __init__(self, element, db):
		ConfigItem.__init__(self, element, db)

		self.filelist = set([])

		if not element.get(self.type_attrib):
			element.set(self.type_attrib, self.default_type)

		self._board = None
		for prereq in self.prerequisites:
			if isinstance(prereq, Board):
				self._board = prereq

		if not self._board:
			raise DbError("Project %s is missing a board definition" % (self.id))

		project_config = self._get_configuration_from_element()
		board_config = self._board.get_configuration()

		self._project_board_configuration = PrioritizedConfigurationHandler(project_config, board_config)

	@property
	def mcu(self):
		return self._board.mcu

	@property
	def board(self):
		return self._board.id

	@property
	def basedir(self):
		current = self.element
		while current is not None:
			if current.attrib.has_key(ConfigDB.basedir_tag):
				return current.attrib[ConfigDB.basedir_tag]
			current = current.getparent()

		raise Exception("No basedir found for project " + self.id)

	@property
	def workspace_name(self):
		return self.element.attrib.get(self.name_attrib, None)

	@staticmethod
	def get_valid_workspace_name_re():
		return re.compile("^[a-z0-9\-_]+$")

	def get_configuration(self):
		return self._project_board_configuration

	def get_device_support(self):
		return self._board.get_device_support()

	def get_project_modules(self):
		"""
		Build a list of the modules directly related to the project.
		It will list all "meta" and "application" modules related to the project.
		"""

		project_module_list = []

		def check_and_add_project_modules(module):

			# List all direct module dependencies
			for prereq in module.get_prerequisites(recursive=False):

				# Handle the SelectByDevice tag
				if isinstance(prereq, SelectByDevice):
					prereq = prereq.select_module(None, self.mcu)

				# TODO: Handle the SelectByConfig tag
				# elif isinstance(prereq, SelectByConfig):

				if prereq.type in ["application", "meta"]:
					if not prereq in project_module_list:
						project_module_list.append(prereq)
						check_and_add_project_modules(prereq)

		check_and_add_project_modules(self)
		return project_module_list

	def get_help_uri(self):
		"""
		Return the URI (Uniform Ressource Identifier) of the help entry point
		"""
		return "index.html"

class DummyProject(Project):

	tag = "project"

	def __init__(self, db, mcu):
		self._element = ET.Element(self.tag)

		ConfigItem.__init__(self, self._element, db)

		self.filelist = set([])

		self._board = None
		self._mcu = mcu
		self._config = {}
		self._db = db

	@property
	def id(self):
		return "dummyproject"

	@property
	def mcu(self):
		map = self.db.lookup_by_id("map.atmel")
		return map.get_mcu_list("uc3a")[0]

	@property
	def board(self):
		raise Exception("Cannot be called on a dummyproject")

	def get_generator_list(self):
		"""
		Find and return all children of the toolchain tag
		"""
		return []

	def get_device_support(self):
		return self._mcu

	def set_config(self, name, value):
		self._db.runtime.set_config(name, value)

	def get_config(self, name):
		return self._db.runtime.get_config(name)

	def get_config_group(self, roup):
		raise Exception("Config_group needed")

class ConfigDB(object):
	basedir_tag = "basedir"
	fromfile_tag = "fromfile"
	show_file_list_in_debug_log = False
	fw_version_attr = "framework-version"
	fw_version_attr_name = "name"
	doc_server_attr = "documentation"
	doc_server_attr_name = "baseurl"

	def __init__(self, runtime, db_file=None):
		self.version_attribute = "xmlversion"
		self.current_version = "1.0"

		self.idmap = {}
		self.circular_reference_map = {}

		self.runtime = runtime
		self.log = runtime.log

		self.fw_version_postfix = ""
		if runtime.version_postfix:
			self.fw_version_postfix = "-" + runtime.version_postfix

		xsd_file = runtime.xml_schema_path
		self.schema = False
		if xsd_file:
			xsd = ET.parse(xsd_file)
			self.schema = ET.XMLSchema(xsd)

		if db_file:
			# Read file
			self.root  = ET.parse(db_file).getroot()
			self.tree = ET.ElementTree(self.root)

			ver = self.root.attrib[self.version_attribute]
			if ver != self.current_version:
				raise DbError("Unsupported ASF XML version " + ver + ". I support only " + self.current_version)
		else:
			# Parse all XML files
			self.root = ET.Element("asf", attrib={self.version_attribute : self.current_version})
			self.tree = ET.ElementTree(self.root)
			self._include_all_subdirs(self.root, '.')

		# Set the default documentation server URL
		result = self.root.findall(".//%s[@%s]" % (self.doc_server_attr, self.doc_server_attr_name))
		if len(result) != 1:
			raise ConfigError("Error getting documentation URL. Found %d but expected 1 element" % (len(result)))

		self.doc_url = "/".join((result[0].attrib[self.doc_server_attr_name].rstrip("/"), "$VER$", "$MODULE$", "html")) + "/"

	def validate_xml(self, tree):
		"""
		Validate the given ElementTree XML tree using the XML schema that is loaded for the ConfigDB.

		The function raises an ET.DocumentInvalid if the tree does not validate.
		"""
		if self.schema:
			self.schema.assertValid(tree)

	def write_xml_tree(self, filename):
		"""
		Write XML tree after includes have been resolved to file for debugging purposes.
		"""
		asf.helper.indent_xml(self.root)
		self.tree.write(filename)

	def _all_subdirs_iter(self, basedir):
		all_files = []

		if self.runtime.use_file_list_cache:
			self.log.info("File-list: Loading from cache: %s last modified %s" %
				(self.runtime.file_list_cache_name, datetime.fromtimestamp(os.path.getatime(self.runtime.file_list_cache_name))))

			list = asf.helper.load_from_file(self.runtime.file_list_cache_name)
			for (dirpath, filename) in list:
				if self.show_file_list_in_debug_log:
					self.log.debug(filename)
				yield (dirpath, filename)
		else:
			match_file = self.runtime.xml_input_filename

			self.log.info("File-list: Parsing %s for %s files" % (os.path.realpath(basedir), match_file))
			skip_folders = ('.svn', '.git')

			for dirpath, dirnames, files in os.walk(basedir):
				# Remove folders that we do not need to walk into
				for skip_folder in skip_folders:
					if skip_folder in dirnames:
						dirnames.remove(skip_folder)

				if match_file in files:
					filename = os.path.join(dirpath, match_file)
					if self.show_file_list_in_debug_log:
						self.log.debug(filename)

					pair = (dirpath, filename)
					all_files.append(pair)
					yield pair

			asf.helper.dump_to_file(all_files, self.runtime.file_list_cache_name)

	def _verify_asf_tag(self, element):
		if not element.tag == "asf":
			return False
		if not element.attrib.has_key(self.version_attribute):
			return False
		if not element.attrib[self.version_attribute] == self.current_version:
			return False
		return True

	def _include_all_subdirs(self, root_element, basedir):
		"""
		Read all asf.xml files in the tree and include them under the root_element
		"""

		for dirpath, filename in self._all_subdirs_iter(basedir):

			if os.path.getsize(filename) == 0:
				continue

			try:
				subtree = ET.parse(filename)
			except Exception as e:
				raise Exception(filename+": "+str(e))
			element = subtree.getroot()

			# Verify that it is an ASF XML file
			if not self._verify_asf_tag(element):
				raise ConfigError("Not compatible XML file " + filename)

			# Validate XML
			try:
				self.validate_xml(element)
			except ET.DocumentInvalid as e:
				raise ConfigError("XML file %s does not validate. Error: %s" % (filename, str(e)))

			element.attrib[self.basedir_tag] = dirpath
			self._expand_compile_paths(element)
			element.attrib[self.fromfile_tag] = filename

			for e in element.findall("*"):
				e.attrib[self.basedir_tag] = dirpath
				e.attrib[self.fromfile_tag] = filename
				root_element.append(e)

	def _expand_compile_paths(self, element):
		"""
		Expand paths of the "build" elements when including an XML file
		"""
		basedir = element.attrib["basedir"]
		# Find all build elements
		for build_entry in element.findall('.//%s' % (BuildType.tag)):

			# Find class for this build type
			type = build_entry.attrib["type"]
			subtype = build_entry.attrib.get("subtype", None)

			c = BuildType.type_to_class(type, subtype)

			# Expand path if build type was found
			if c is not None:
				c.expand_compile_paths(build_entry, basedir)

	def _tag_to_class(self):
		tag_to_class = {
			SelectByDevice.tag : SelectByDevice,
			SelectByConfig.tag : SelectByConfig,
			DeviceMap.tag   : DeviceMap,
			Module.tag      : Module,
			Board.tag       : Board,
			Project.tag     : Project,
		}
		return tag_to_class

	def get_framework_version_number(self):
		"""
		Get ASF version from the database

		Return string with version or raise ConfigError if not found.
		"""

		result = self.root.findall(".//%s[@%s]" % (self.fw_version_attr, self.fw_version_attr_name))
		if len(result) != 1:
			raise ConfigError("Error getting ASF version. Found %d but expected 1 element" % (len(result)))

		return result[0].attrib[self.fw_version_attr_name]

	def get_framework_version(self):
		"""
		Get ASF version including the postfix.

		Return string with ASF framework version + postfix.
		"""
		return self.get_framework_version_number() + self.fw_version_postfix

	def set_documentation_server(self, new_doc_url):
		"""
		Set a custom base URL for the documentation.

		Note that this function only appends $MODULE$/html/ to the URL and thus
		expects that the version is already specified in URL or not required.
		"""
		self.doc_url = "/".join((new_doc_url.rstrip("/"), "$MODULE$", "html")) + "/"

	def get_documentation_server(self):
		"""
		Get the ASF documentation server base URL from the database.

		Unless a custom URL has been set with set_documentation_server(), this will
		be the server that is specified with the documentation tag in the root
		asf.xml file, with some additions:
		http://asf.atmel.com/docs/ + $VER$/$MODULE$/html/
		"""
		return self.doc_url

	def _resolve_idref(self, root, idref, tag="*"):
		return root.find(".//%s[@%s='%s']" % (tag, 'id', idref))

	def lookup_by_id(self, idref, type=ConfigItem):
		"""
		Find a component by id reference from the entire database tree.

		Return an object for the component with the given id.
		Raises a NotFoundError if not found.
		Raises a ConfigError if a circular reference is found
		"""
		return self.relative_lookup_by_id(self.root, idref, type)

	def relative_lookup_by_id(self, root, idref, type=ConfigItem):
		"""
		Find a component by id reference underneath specified root node.

		Return an object for the component with the given id.
		Raises a NotFoundError if not found.
		Raises a ConfigError if a circular reference is found
		"""
		if not issubclass(type, ConfigItem):
			raise TypeError("`type' must be a subclass of ConfigItem")

		if idref in self.idmap:
			item = self.idmap[idref]
		else:
			element = self._resolve_idref(root, idref, type.tag)
			if element is None:
				raise NotFoundError("%s with ID %s not found in the database" % (type.__name__, idref))

			if (idref in self.circular_reference_map):
				raise ConfigError("Circular reference to %s. Involved references were: %s" % (idref, ' '.join(self.circular_reference_map.keys())))
			else:
				self.circular_reference_map[idref] = idref

			if type == ConfigItem:
				tag_to_class = self._tag_to_class()
				type = tag_to_class[element.tag]

			try:
				item = type(element, self)
			except Exception as e:
				self.circular_reference_map.pop(idref)
				# Do not handle exception, but reraise it
				raise

			self.idmap[idref] = item

			self.circular_reference_map.pop(idref)

		assert isinstance(item, type)

		return item

	def _reverse_resolve_idref(self, idref, tag="*"):
		return self.root.findall(".//%s[@%s='%s']" % (tag, 'idref', idref))

	def reverse_lookup_by_id(self, lookup_id):
		"""
		Find a list of components that requires a given id
		"""
		type = ConfigItem

		list = []
		for item in self._reverse_resolve_idref(lookup_id, type.tag):
			element = item.getparent()
			id = element.attrib["id"]

			if id in self.idmap:
				item = self.idmap[id]
			else:
				if type == ConfigItem:
					tag_to_class = self._tag_to_class()
					type = tag_to_class[element.tag]

				item = type(element, self)
				self.idmap[id] = item

			list.append(item)

		return list

	def _reverse_resolve_related_module_id(self, module_id):
		return self.root.findall(".//related-module[@%s='%s']" % ('module-id', module_id))

	def reverse_lookup_by_related_module_id(self, lookup_id):
		"""
		Find a list of components that requires a given id
		"""

		module_id_dict = {}
		for item in self._reverse_resolve_related_module_id(lookup_id):
			type = item.attrib["type"]
			element = item.getparent()
			id = element.attrib["id"]
			module_id_dict[type] = module_id_dict.get(type, []) + [id]

		return module_id_dict

	def find_components(self, type, attribute_dict={}):
		"""
		Find component of the based type and return the XML tree

		A dictionary with required attributes and, optionally, values
		for the attributes can also be specified. If only the presence
		of an attribute is required, its value should be None.

		For example, to search for a database type which has attribute
		'foo' with value 'bar' and the attribute 'foobar' with any
		value, the attribute dictionary must be:

		attr_dict = {
			'foo' : 'bar',
			'foobar' : None,
		}
		"""
		if not issubclass(type, ConfigItem):
			raise TypeError("`type' must be a subclass of ConfigItem")

		tags = self._tag_to_class()

		if not type.tag in tags:
			raise TypeError('Unable to find type ' + type.tag)

		attrs = ''
		if attribute_dict:
			for attr, val in attribute_dict.items():
				attrs += '[@' + attr
				if val is not None:
					attrs += "='%s'" % val
				attrs += ']'

		return self.root.findall(".//" + type.tag + attrs)

	def iter_components(self, type, attribute_dict={}):
		"""
		Iterator for iterating though XML tree items of a specific type. Type must
		be a subclass of ConfigItem
		"""
		if not issubclass(type, ConfigItem):
			raise TypeError("`type' must be a subclass of ConfigItem")

		for c_e in self.find_components(type, attribute_dict):
			# Get the ID and check if component was already instantiated
			c_id = c_e.attrib['id']
			component = self.idmap.get(c_id, None)

			# If no instance was found, create one now and store it
			if not component:
				component = type(c_e, self)
				self.idmap[c_id] = component

			yield component

	def iter_projects(self):
		return self.iter_components(Project)

	def iter_modules(self):
		return self.iter_components(Module)

	def iter_module_selectors(self):
		# Yield all module selectors of any type from database
		subclasses = ModuleSelector.__subclasses__()
		for subclass in subclasses:
			subclasses.extend(subclass.__subclasses__())

			for component in self.iter_components(subclass):
				yield component

	def iter_boards(self):
		return self.iter_components(Board)

	def wildcard_find(self, match_string, type=Project, attribute_dict={}):
		"""
		Find all components of a given type (default Project) using the match_string string.

		"*"'s in the match string will match any character.
		Example: a match_string of "avr32*" will match "avr32.test", "avr32", but not "avr"

		Return a list of component ids that matches.
		"""
		matches = []
		re_str = re.escape(match_string)
		re_str = re_str.replace("\\*", ".*")
		re_match = re.compile(re_str)

		for component in self.find_components(type, attribute_dict):
			id = component.attrib["id"]
			if re_match.match(id):
				matches.append(id)

		return matches

	def _find_selector_for_module(self, module):
		"""
		Helper function for find_modules_for_mcu

		Finds the module-selector "parent" that a module belongs to and returns it.
		If the module does not belong to a module selector, the module is returned.
		"""
		# Check module selectors:
		hash_pos = module.id.find("#")
		if hash_pos >= 0:
			# Find parent module selector. Raises NotFoundError on not found
			parent_module = self.lookup_by_id(module.id[:hash_pos])
			return parent_module
		else:
			return module

	def find_modules_for_mcu(self, mcu, types=None, check_hidden=True):
		"""
		Find all modules for a specific MCU.

		Types is a list of strings with the supported module types.
		Example: ["driver", "service", "component"]

		Set check_hidden to True in order to remove modules set as hidden. False
		will return all modules regardess of hidden status.
		"""
		if not isinstance(mcu, MCU):
			raise ConfigError("MCU '%s' is not instance of MCU" % (mcu))

		hidden_flag = "hidden"
		map = mcu.get_group_map()

		# Reset all selections first
		for selector in self.iter_module_selectors():
			selector.reset_selection()

		supported_modules = []
		for module in self.iter_modules():
			if (types is not None) and (module.type not in types):
				# If not the correct type we skip this module and look for the next
				continue

			if check_hidden:
				if hidden_flag in module.get_gui_flag_list(recursive=False):
					# Hidden module, skip and look for next module
					continue

			not_supported = module.resolve_device_support(map, check_self=True)
			if len(not_supported) == 0:
				# No modules are incompatible, find module selectors and add to supported list
				module = self._find_selector_for_module(module)
				supported_modules.append(module)

		return supported_modules

	def get_valid_id_re(self):
		""" Return compiled regular expression for validating ids """
		return re.compile("^[a-z0-9\.\-_#]+$")

	def sanity_check(self):
		self.log.info("-- Database sanity check")

		total = 0
		errors = 0

		# Get the elements of all components to check
		component_classes_to_check = [
				Board,
				Module,
				Project,
		]

		component_elements = []

		for component_class in component_classes_to_check:
			component_elements.extend(self.find_components(component_class))

		# Need the device map for the next sanity checks
		device_map = self.lookup_by_id("map.atmel")
		mcu_list = device_map.get_mcu_list()

		# Check all specific MCUs for doc-arch
		no_doc_arch_mcus = []
		for mcu_name in mcu_list:
			# Skip the "unspecified-*" devices
			if 'unspecified-' in mcu_name:
				continue

			total += 1
			mcu = MCU(mcu_name, self)
			if not mcu.get_doc_arch_group():
				no_doc_arch_mcus.append(mcu_name)
				errors += 1

		# Print errors for any missing doc-arch
		if no_doc_arch_mcus:
			self.log.error("%s is missing doc-arch for MCUs:" % str(device_map))
			for mcu_name in no_doc_arch_mcus:
				self.log.error("\t" + mcu_name)

		# Check that doc-arch group does not contain another doc-arch
		for doc_arch_e in device_map._get_doc_arch_group_elements():
			total += 1

			# Now find sub-elements that are doc-arch groups
			sub_doc_arch_e = doc_arch_e.findall('.//%s[@%s]' % (DeviceMap.group_tag, DeviceMap.doc_group_attr))
			if sub_doc_arch_e:
				errors += 1
				self.log.error("%s has doc-arch group %s which itself contains doc-arch group(s):" % (str(device_map), doc_arch_e.attrib['name']))
				for e in sub_doc_arch_e:
					self.log.error("\t" + e.attrib['name'])

		# Check that same-named groups have matching doc-arch
		doc_arch_dict = device_map.get_doc_arch_dict()
		doc_arch_list = [(e.attrib['name'], e.attrib[DeviceMap.doc_group_attr]) for e in device_map._get_doc_arch_group_elements()]

		for group, doc_arch in sorted(doc_arch_dict.items()):
			total += 1
			mismatch_arches = [da for g, da in doc_arch_list if g == group and da != doc_arch]

			if mismatch_arches:
				errors += 1
				self.log.error("%s has multiple, differing doc-arch names for group %s:" % (str(device_map), group))
				for arch in [doc_arch] + mismatch_arches:
					self.log.error("\t\"%s\"" % arch)

		for e in component_elements:
			id = e.attrib['id']

			# Load the component and resolve all references
			try:
				total += 1
				c = self.lookup_by_id(id)
			except ConfigError as err:
				errors += 1
				self.log.error("While resolving "+ id +":")
				self.log.error("ConfigError: "+ str(err))
			except NotFoundError as err:
				errors += 1
				self.log.error("While resolving "+ id +":")
				self.log.error("NotFoundError: "+ str(err))

			# Check that "default-mcu"-attribute is set for all "doxygen-module" generator tags,
			# and for absolutely no other generator tags
			for element in c.get_child_elements("generator"):
				gen_name = element.get("value")
				mcu_name = element.get("default-mcu", None)

				if gen_name == "doxygen-module":
					if mcu_name not in mcu_list:
						errors += 1
						if mcu_name == None:
							error_string = 'not set'
						else:
							error_string = 'set to invalid MCU `%s\'' % mcu_name
						self.log.critical("%s: attribute `default-mcu' is %s in generator tag for `doxygen-module'" % (str(c), error_string))
				else:
					if mcu_name != None:
						errors += 1
						self.log.critical("%s: attribute `default-mcu' is erroneously set in generator tag for `%s'" % (str(c), gen_name))

			# Check that "device-support" actually gives support for a device(s)
			for element in c.get_child_elements("device-support"):
				dev_name = element.get("value")
				if not dev_name or not device_map.get_mcu_list(dev_name, True):
					errors += 1
					self.log.critical("%s: `%s' is not a valid MCU or MCU group" % (str(c), dev_name))

			# Make sure the 'doxygen-entry-point' attribute is present for non-hidden modules
			if isinstance(c, Module) and c.type in ["driver", "component", "service"]:
				if not 'hidden' in c.get_gui_flag_list():
					has_doxygen_entry_point = False
					for element in c.get_child_elements("build"):
						if element.attrib["type"] == "doxygen-entry-point":
							has_doxygen_entry_point = True
							break
					if not has_doxygen_entry_point:
						errors += 1
						self.log.error("Module with id %s is missing doxygen-entry-point attribute" % (id))
		
		# Find all select-by-config names, save all possible valid config names and values
		select_by_config_names = {}
		config_names = []
		for element in self.root.findall(".//%s" % (SelectByConfig.tag)):
			name = element.attrib['name']
			id = element.attrib['id']
			module = self.lookup_by_id(id)
			available_options = [x.partition("#")[2] for x in module.get_subids()]
			
			select_by_config_names[name] = dict([("id", id), ("options", available_options)])

		# Verify all config entries against the saved select-by-config names and values
		for element in self.root.findall(".//%s" % ("config")):
			total += 1
		
			config_name = element.attrib['name']
			config_value = element.attrib['value']
			project_id = element.getparent().attrib["id"]

			# TODO: Extract allowable compiler options from the generators, ignore for now
			if config_name.startswith("config.compiler."):
				continue

			if config_name in select_by_config_names.keys():
				# Check if the config value exists in the select-by-config
				available_options = select_by_config_names[config_name]["options"]
				if not config_value in available_options:
					errors += 1
					self.log.critical("Config '%s' with value '%s' in '%s' does not match available select-by-config modules '%s'" % (config_name, config_value, project_id, ', '.join(available_options)))
					# Try to load module to find originating file, if lookup fails we have already given the user enough feedback to find the error:
					self.log.critical("    Offending file: %s" % (self.lookup_by_id(project_id).fromfile))
			else:
				# No matching select-by-config for the current config entry name
				errors += 1
				self.log.critical("Config '%s' in '%s' does not have a corresponding select-by-config module" % (config_name, project_id))
				# Try to load module to find originating file, if lookup fails we have already given the user enough feedback to find the error:
				self.log.critical("    Offending file: %s" % (self.lookup_by_id(project_id).fromfile))
		
		# Find all IDs and check for duplicates
		total += 1
		ids = []
		for element in self.root.findall(".//%s[@%s]" % ("*", 'id')):
			ids.append(element.attrib['id'])

		id_count = len(ids)
		id_unique = len(list(set(ids)))

		if id_count != id_unique:
			errors += 1
			self.log.critical("Elements with the same ID in the database. You must fix this!")

			# Report which ones have the problem:
			count_dict = asf.helper.count_duplicates(ids)
			for id, count in count_dict.items():
				if count > 1:
					self.log.critical("Duplicate id %s occurs %d times." % (id, count))

		if (errors):
			raise DbError("Database sanity check failed with %d error(s)" % (errors))

		self.log.info("-- Check done. Failures:" + str(errors) + "/" + str(total))
