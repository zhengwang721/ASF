import re

class ConfigurationHandler(object):
	"""
	Class for managing configurations from XML and configuration overrides
	from command line. Typically <config> elements in XML and --config
	parameters from command line.
	"""

	def __init__(self):
		self.config = {}

	def set_config(self, name, value):
		"""
		Sets the value of a config, overwriting any previous value.
		"""
		self.config[name] = value

	def unset_config(self, name):
		"""
		Unsets a config, removing both its name and value if it exists.
		"""
		if name in self.config:
			del self.config[name]

	def get_config(self, name):
		"""
		Gets the value of a config, if it exists.
		"""
		return self.config.get(name, None)

	def get_config_group(self, group):
		"""
		Gets a dictionary of existing configs that belong to the
		specified group.
		"""
		if group[-1:] == '.':
			name_prefix = group
		else:
			name_prefix = group + '.'

		group_config = {}

		for name in self.config.iterkeys():
			if name[:len(name_prefix)] == name_prefix:
				group_config[name[len(name_prefix):]] = self.config[name]

		return group_config

	@staticmethod
	def generate_from_option_parser(optionparser):
		result = ConfigurationHandler()

		# Parse configs that are set on command line
		if optionparser.config:
			for c in optionparser.config:
				# Must be on the format <name>=<value>
				try:
					(name, value) = re.split('=', c)
					# Force an actual value
					if value == '':
						raise ValueError
				except ValueError:
					raise Exception("ERROR: Value is missing for config: " + c)

				if result.get_config(name):
					raise Exception("ERROR: Config was already set: " + c)

				result.set_config(name, value)

		return result

class PrioritizedConfigurationHandler(ConfigurationHandler):
	def __init__(self, pri1, pri2=None, pri3=None, pri4=None):
		self.config = {}
		# Update the dictionary with the lowest priority first, and then
		# overwrite with higher priority items
		if pri4:
			self.config.update(pri4.config)
		if pri3:
			self.config.update(pri3.config)
		if pri2:
			self.config.update(pri2.config)

		self.config.update(pri1.config)
