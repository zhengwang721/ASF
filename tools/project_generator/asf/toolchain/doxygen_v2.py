
from asf.database import *
from asf.toolchain.doxygen import *
import platform

class DoxygenProject_V2(DoxygenProject):
	toolchain = "doxygen-v2"
	template_layout_filename = "doxygen_v2_layout.xml"
	template_filename = "doxyfile_v2.doxygen"
	custom_layout = ""
	dictionary = {}

	def convert_path_for_makefile(self, path):
		return path.replace(os.sep, "/")

	def _get_doxyfile_makefile_path(self):
		return os.path.join(self.doxyfile_dir, "Makefile")

	def _get_docbook_fop_conf_path(self):
		return os.path.join(self.doxyfile_dir, "fop.xconf")

	def _get_doxylayout_path(self):
		return os.path.join(self.doxyfile_dir, "DoxygenLayout.xml")

	def _write_docbook_files(self):
		template_docbook_dir = os.path.join(self.templatedir, "docbook_pdf")

		# Read the Makefile template for generating docbook PDF from doxygen
		docbook_makefile = self._read_file(os.path.join(template_docbook_dir, "doxygen_v2_pdf_Makefile"))
		docbook_fop_config = self._read_file(os.path.join(template_docbook_dir, "doxygen_v2_pdf_fop_xconf"))

		this_path = os.path.abspath(".")

		xslt_tool_path = os.path.normpath(os.path.join(this_path, "..","buildtools-docbook"))

		# Replace the strings for pdf output name and toolpath
		self.dictionary["$DOCBOOK_PATH$"] = self.convert_path_for_makefile(xslt_tool_path)
		self.dictionary["$PDF_NAME$"] = "manual.pdf"
		self.dictionary["$GRAPICS_PATH$"] = self.convert_path_for_makefile(template_docbook_dir)
		self.dictionary["$TRANSFORM_PATH$"] = self.convert_path_for_makefile(template_docbook_dir)

		if platform.system() == "Windows" :
			self.dictionary["$DOCBOOK_FONT_DIR$"] = "C:\\Windows\\Fonts\\"
		elif platform.system() == "Linux" :
			self.dictionary["$DOCBOOK_FONT_DIR$"] = "/Fonts/"
		else :
			raise ConfigError("Project `%s' has unknown Fonts path in the fop.xconf file!" % (self.project.id))

		docbook_makefile = self._replace_strings(docbook_makefile, self.dictionary)
		docbook_fop_config = self._replace_strings(docbook_fop_config, self.dictionary)

		self._write_file(self._get_doxyfile_makefile_path(), docbook_makefile)
		self._write_file(self._get_docbook_fop_conf_path(), docbook_fop_config)

	def write_doxygen(self):
		super(DoxygenProject_V2, self).write_doxygen()

		self._write_docbook_files()

		if self.custom_layout == "":
			doxylayoutfile_path = os.path.join(self.templatedir, self.template_layout_filename)
			doxylayoutfile = self._read_file(doxylayoutfile_path)
			doxylayoutfile_path = self._get_doxylayout_path()
			self._write_file(doxylayoutfile_path, doxylayoutfile)

			self.log.info("Doxygen layout doxyfile written:" + doxylayoutfile_path)

class DoxygenModule_V2(DoxygenModule, DoxygenProject_V2):
	toolchain = "doxygen-module-v2"
	module_file = "doxygen_v2_module_mainpage.h"
