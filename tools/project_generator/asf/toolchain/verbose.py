
from asf.toolchain.generic import GenericProject
from asf.database import *

class VerboseProject(GenericProject):
	toolchain = "verbose"

	def write(self):
		self.log.info("----------------- VERBOSE generator -----------------------")
		self.log.info("Project %s " % (self.project.caption))
		self.log.info("Project ID = %s" % (self.project.id))
		self.log.info("Board = %s MCU = %s" % (self.project.board, self.project.mcu))

		self.log.debug("Requirements after resolving module-selectors:")
		self.project.visualize_requirements(self.log.debug)

		type_mapping = {
			'C files'           : BuildC,
			'Assembly files'    : BuildAssembly,
			'Includes'          : BuildInclude,
			'Header files'      : BuildHeader,
			'Defines'           : BuildDefine,
		}

		for description, build_type in type_mapping.items():
			self.log.info(description + ":")
			for build in self.project.get_build(build_type, toolchain=self.toolchain):
				self.log.info(build)
			self.log.info("")
