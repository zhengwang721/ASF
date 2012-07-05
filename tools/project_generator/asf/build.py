import os
from asf.exception import *

class BuildType(object):
	tag = 'build'
	type = "*"
	subtype = "*"

	@staticmethod
	def get_build_item_from_element(element):
		if not element.attrib.has_key('value'):
			raise ConfigError("Build define element '%s' is missing value attribute" % (element.tag))

		# Replace directory separator with the OS' default
		return element.attrib["value"].replace("/", os.sep)

	@staticmethod
	def type_to_class(type, subtype, base_class=None):
		"""
		Returns the subclass of the specified base class which has the
		specified type and subtype attributes.
		The search is recursive to allow for several "generations" of
		class inheritance, i.e., it will not find direct child-classes
		only, but any sub-class.

		Both type and subtype _must_ be specified. However, the subtype
		may be set to None -- this would cause the default subtype,
		found in the base class BuildType, to be used.

		If no matching class can be found, a DbError is raised and None
		is returned.

		Typically, users will want to call this function without
		specifying the base class as it is meant for use in the
		recursive search.
		"""
		if not base_class:
			base_class = BuildType
		if not subtype:
			subtype = BuildType.subtype

		for c in base_class.__subclasses__():
			# Look for first class with matching type- and subtype-attributes.
			if (c.type == type) and (c.subtype == subtype):
				return c
			# Do recursive lookup since __subclasses__() actually reports _children_
			# Must catch DbErrors or the search can stop prematurely -- we only know
			# if the search has failed once the initial iteration of this function
			# has searched through all subclasses.
			else:
				try:
					sub_c = c.type_to_class(type, subtype, c)
					if sub_c:
						return sub_c
				except DbError:
					pass

		raise DbError("Could not find subclass of BuildType for type %s, subtype %s" % (type, subtype))

		return None

	@staticmethod
	def expand_compile_paths(element, basedir):
		# First convert any backslash into slash
		value = element.attrib["value"].replace('\\', '/')
		# Expand compile path for element
		element.attrib["value"] = os.path.normpath(os.path.join(basedir, value))

class BuildInclude(BuildType):
	type = "include-path"

class BuildC(BuildType):
	type = "c-source"

class BuildAssembly(BuildType):
	type = "asm-source"

class BuildHeader(BuildType):
	type = "header-file"

class BuildApiHeader(BuildHeader):
	subtype = "api"

class BuildDistribute(BuildType):
	type = "distribute"

class BuildDistributeFile(BuildDistribute):
	subtype = "file"

class BuildDistributeDirectory(BuildDistribute):
	subtype = "directory"

class BuildDistributeUserFile(BuildDistribute):
	subtype = "user-file"

class BuildDistributeLicense(BuildDistribute):
	subtype = "license"

class BuildModuleConfig(BuildType):
	type = "module-config"

class BuildModuleConfigPath(BuildModuleConfig):
	subtype = "path"

class BuildModuleConfigRequiredHeaderFile(BuildModuleConfig):
	subtype = "required-header-file"

	@staticmethod
	def expand_compile_paths(element, basedir):
		# No path expansions for configuration headers.
		pass

class BuildLibrary(BuildType):
	type = "library"

class BuildUserLibrary(BuildLibrary):
	subtype = "user"

class BuildCompilerLibrary(BuildLibrary):
	subtype = "compiler"

	@staticmethod
	def expand_compile_paths(element, basedir):
		# No path expansion, leave value as is
		pass

class BuildLinkerConfig(BuildType):
	type = "linker-config"

class BuildLinkerScript(BuildLinkerConfig):
	subtype = "script"

class BuildAuxLinkerScript(BuildLinkerConfig):
	subtype = "aux-script"

class BuildMacroConfig(BuildType):
	type = "macro-config"

class BuildMacroFile(BuildMacroConfig):
	subtype = "script"

class BuildAuxMacroFile(BuildMacroConfig):
	subtype = "aux-script"

class BuildLinkerFlags(BuildLinkerConfig):
	subtype = "flags"

	@staticmethod
	def expand_compile_paths(element, basedir):
		# No path expansion, leave value as is
		pass

class BuildBuildConfig(BuildType):
	type = "build-config"

	@staticmethod
	def expand_compile_paths(element, basedir):
		# No path expansion, leave value as is
		pass

class BuildAssemblerFlags(BuildBuildConfig):
	subtype = "assembler-flags"

class BuildCCompilerFlags(BuildBuildConfig):
	subtype = "c-compiler-flags"

class BuildCppCompilerFlags(BuildBuildConfig):
	subtype = "c++-compiler-flags"

class BuildPreprocessorFlags(BuildBuildConfig):
	subtype = "preprocessor-flags"

class BuildDefine(BuildType):
	type = "define"

	@staticmethod
	def get_build_item_from_element(element):
		name = None
		value = None
		if not element.attrib.has_key('name'):
			raise ConfigError("Build define element '%s' is missing name type" % (element.id))

		name = element.attrib["name"]

		if element.attrib.has_key('value'):
			value = element.attrib["value"]

		return (name, value)

	@staticmethod
	def expand_compile_paths(element, basedir):
		# No path expansion for defines
		pass

class BuildDoxygenImagePath(BuildType):
	type = "doxygen-image-path"

class BuildDoxygenEntryPoint(BuildType):
	type = "doxygen-entry-point"

	@staticmethod
	def expand_compile_paths(element, basedir):
		# No path expansion for doygen entry points
		pass

class BuildDoxygenQuickStart(BuildDoxygenEntryPoint):
	type = "doxygen-quick-start"
