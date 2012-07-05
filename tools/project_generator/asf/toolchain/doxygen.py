
import re
from asf.toolchain.generic import GenericProject,GenericElement
from asf.database import *



class DoxygenProject(GenericProject):
	toolchain = "doxygen"
	custom_html_header = ""
	custom_html_footer = ""
	custom_html_stylesheet = ""
	custom_layout = ""
	custom_use_dot = "YES"
	doxyfile_dir = ""
	ASF = {}
	recursive = True

	def __init__(self, project, db, runtime):
		GenericProject.__init__(self, project, db, runtime)
		self.doxyfile_dir = os.path.join(self.project.basedir, "doxygen/")

	def get_doxygen_mcu(self):
		return self.project.mcu.name

	def get_gcc_mcu_name(self, mcu_name):

		#Dictionary for regular expressions and actions to get the correct GCC name.
		regexp_to_action = {
			'(at)(tiny|x?mega)(\w+)': "'__AVR_' + m.group(1).upper() + m.group(2) + m.group(3).upper() + '__'",
			'(at|at32)(uc\w+)'      : "'__AVR32_' + m.group(2).upper()+ '__'"
		}

		#Iterizing over dictionary
		gcc_mcu_name = None
		if mcu_name:
			for regexp, action in regexp_to_action.iteritems():
				m = re.match(regexp, mcu_name)
				if m:
					gcc_mcu_name = eval(action)
					break

		return gcc_mcu_name

	def get_generated_files_list(self):
		return [self._get_doxyfile_path()]

	def _get_doxyfile_path(self):
		return os.path.join(self.doxyfile_dir, "doxyfile.doxygen")

	def get_generated_dirs_list(self):
		return [self.doxyfile_dir]

	def set_custom_html_header(self, html_header):
		self.custom_html_header = html_header

	def set_custom_html_footer(self, html_footer):
		self.custom_html_footer = html_footer

	def set_custom_html_stylesheet(self, html_stylesheet):
		self.custom_html_stylesheet = html_stylesheet

	def set_custom_layout(self, custom_layout):
		self.custom_layout = custom_layout
		
	def set_custom_use_dot(self, use_dot):
		self.custom_use_dot = use_dot

	def generate_doxygen(self):

		# Create a decent project name
		self.ASF["$ASF_PROJECT$"] = "\"" + self.project.caption + "\""

		# Handle C files, header files and include paths
		type_mapping = {
			'$ASF_INPUT$'        : BuildC,
			'$ASF_HEADERS$'      : BuildHeader,
			'$ASF_INCLUDE_PATH$' : BuildInclude,
			'$ASF_IMAGE_PATH$'   : BuildDoxygenImagePath,
		}

		for declaration, filetype in type_mapping.items():
			str = ""
			for f in self.project.get_build(filetype, self.toolchain, self.recursive):

				# Make relative paths from project dir
				f = os.path.relpath(f, self.doxyfile_dir)

				# Let's replace backslashes with slashes. Yeah!
				f = f.replace('\\','/')

				# Add to list. Separate by space.
				str += " " + f
			self.ASF[declaration] = str

		# Make sure the documentation has a mainpage tag, look into all input files
		pattern_mainpage = re.compile(r"[\\@]mainpage")
		has_mainpage = False
		for f in self.project.get_build(BuildHeader, self.toolchain, self.recursive) + self.project.get_build(BuildC, self.toolchain, self.recursive):

			# Read the content of the input file
			file_content = self._read_file(f)

			# Check if this file has a mainpage tag
			if re.search(pattern_mainpage, file_content):
				has_mainpage = True
				break

		# If no mainpage tag has been found for this documentation, throw an error message
		if not has_mainpage:
			self.log.error("Project `%s' is missing a doxygen mainpage tag" % (self.project.id))

		# Add header files to INPUT (along with c-files)
		self.ASF["$ASF_INPUT$"] += " " + self.ASF["$ASF_HEADERS$"]

		# Use dot (generates images)
		self.ASF["$ASF_USE_DOT$"] = self.custom_use_dot

		# Include sources by default
		self.ASF["$ASF_INCLUDE_SOURCES$"] = "YES"

		# Set the custom html header/footer/css if any
		self.ASF["$ASF_HTML_HEADER$"] = self.custom_html_header
		self.ASF["$ASF_HTML_FOOTER$"] = self.custom_html_footer
		self.ASF["$ASF_HTML_STYLESHEET$"] = self.custom_html_stylesheet

		# Set the custom layout if any
		self.ASF["$ASF_CUSTOM_LAYOUT$"] = self.custom_layout

		# Handle defines
		defines = self.project.get_build(BuildDefine, self.toolchain, self.recursive)

		mcu_gcc_name = self.get_gcc_mcu_name(self.get_doxygen_mcu())
		if mcu_gcc_name:
			defines.append((mcu_gcc_name, None))

		define_text = ""
		if not defines is None:
			for definition, value in defines:
				if value is None:
					define_text += " " + definition
				else:
					define_text += " %s=%s" % ( definition, value )
		self.ASF["$ASF_DEFINES$"] = define_text

	def write_doxygen(self):
		# Create doxyfile.doxygen
		doxyfile_path = os.path.join(self.templatedir, "doxyfile.doxygen" )
		doxyfile = self._read_file(doxyfile_path)
		doxyfile = self._replace_strings(doxyfile, self.ASF)
		if not os.path.exists(self.doxyfile_dir):
			os.makedirs(self.doxyfile_dir)
		doxyfile_path = self._get_doxyfile_path()
		self._write_file(doxyfile_path, doxyfile)
		self.project.filelist.add(doxyfile_path)

		self.log.info("Doxygen doxyfile written:" + doxyfile_path)

	def write(self):
		self.project.resolve_all_selections(self.configuration, self.project.mcu)
		self._check_for_device_support()
		self.generate_doxygen()
		self.write_doxygen()


class DoxygenModule(DoxygenProject):
	toolchain = "doxygen-module"
	module_file = "doxygen_module_mainpage.h"
	ASF = {}
	recursive = False

	def __init__(self, project, db, runtime):
		GenericElement.__init__(self, project, db, runtime)
		self.doxyfile_dir = project.get_help_dir()

	def get_generated_files_list(self):
		return [self._get_doxyfile_path(), os.path.join(self.doxyfile_dir, self.module_file)]

	def get_generated_dirs_list(self):
		return [self.doxyfile_dir, os.path.join(self.project.basedir, "doxygen")]

	def get_doxygen_mcu(self):
		generator_elements = self.project.get_child_elements("generator", ["default-mcu"])
		try:
			return generator_elements[0].attrib["default-mcu"]
		except:
			return None

	def write(self):

		# We may need an mcu to resolve select-by-device-stuff.
		mcu = self.get_doxygen_mcu()
		if mcu:
			self.project.resolve_all_selections(ConfigurationHandler(), MCU(mcu, self.db))
		else:
			raise DbError("Attribute `default-mcu' is not set for doxygen-module generator")

		self.generate_doxygen()

		# Get module configuration file(s)
		for p in self.project.get_build(BuildModuleConfigPath, self.toolchain, self.recursive):
			for f in self.project.get_build(BuildModuleConfigRequiredHeaderFile, self.toolchain, self.recursive):

				# Combine the module config file name and path
				f = p + "\\" + f

				# Sanity check to see that the config file actually exists in the current path
				if not os.path.exists(f):
					continue

				# Make relative paths from project dir
				f = os.path.relpath(f, self.doxyfile_dir)

				# Let's replace backslashes with slashes
				f = f.replace('\\','/')

				# Add to list. Separate by space.
				self.ASF["$ASF_INPUT$"] += " " + f

		# Read module template file
		template_file_path = os.path.join(self.templatedir, self.module_file )
		template = self._read_file(template_file_path)
		# Insert/replace stuff in template here
		pass
		# Insert \mainpage element template file to doxygen file here
		self.ASF["$ASF_INPUT$"] += " "+self.module_file
		self.write_doxygen()
		# Write module template here
		self._write_file(os.path.join(self.doxyfile_dir, self.module_file), template)
