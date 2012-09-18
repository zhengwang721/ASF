"""
This is the interface AVR Studio uses when reading data from ASF.

WARNING:
Changing this script can cause AVR Studio to fail. Don't  this script unless
you have informed the the AVR Studio developers.
"""
import logging
import asf.runtime
from asf.configuration import ConfigurationHandler
from asf.runtime import Runtime
from asf.database import *

from asf.toolchain import *



class PythonFacade:
	valid_generator_names = [
		avrstudio5.AVRStudio5Project.generator_tag,
		avrstudio5.AVRStudio5Project32.generator_tag,
		avrstudio5.AVRStudio5ProjectARM.generator_tag,
		avrstudio5.AVRStudio5ProjectTemplate.generator_tag,
		avrstudio5.AVRStudio5Project32Template.generator_tag,
		avrstudio5.AVRStudio5ProjectARMTemplate.generator_tag,
	]

	def __init__(self, db_file, changedir=True):
		self.configuration = ConfigurationHandler()
		self.runtime = Runtime(".", self.configuration)
		self.runtime.setup_log(logging.FATAL)
		self.runtime.set_debug(True)
		self.log = self.runtime.log
		if changedir:
			basedir = "../../"
			os.chdir(basedir)
		# if file exists read db from file
		if os.access(db_file,os.F_OK):
			self.runtime.load_db(db_file)
		else:
			# load DB by recursively reading asf.xml files
			self.runtime.load_db()
			# save to file
			self.runtime.db.write_xml_tree(db_file)

		self.db = self.runtime.db

	def get_all_mcus(self):
		"""
		Return the list of MCUs that are supported in this ASF Distribution
		"""
		mcu_list = []
		for map in self.db.iter_components(DeviceMap):
			mcu_list.extend(map.get_mcu_list())
		return mcu_list

	def get_items_for_mcu(self, mcu, mod_types_to_get=None):
		"""
		Return the list of all modules/select-by-device/select-by-config associated with the mcu

		mod_types_to_get is optional, but can be a list of module types:
		["driver", "component", "service", "library"]

		If unspecified, it defaults to None, and no filtering on type is
		done.
		"""
		mods = self.db.find_modules_for_mcu(mcu, mod_types_to_get, check_hidden=True)
		return mods

	def find_modules(self):
		return self.db.iter_modules()

	def get_mcu(self,mcu_name):
		return MCU(mcu_name,self.db)

	def lookup_by_id(self, id):
		return self.db.lookup_by_id(id)

	def get_remote_ids(self, mcu_name, id):
		"""
		This function should be deleted once Atmel Studio stops using it.
		"""
		return [id]

	def get_all_modules_and_selectors(self, mcuname, mod_types_to_ignore=None):
		"""
		This method returns a list of all modules and module selectors that support the specified MCU.

		mod_types_to_ignore is optional, but can be a list of module types:
		["application", "build-specific"]

		If unspecified, it defaults to None, and no filtering on type is
		done. Note that this filter behaves in the opposite way of how
		the filtering in get_items_for_mcu() does.

		Modules of type meta should not show up in the wizard.
		"""
		retlist = []
		mcu = self.get_mcu(mcuname)
		map = mcu.get_group_map()

		mods = self.db.iter_modules()

		for mod in mods:
			if (mod_types_to_ignore is not None) and (mod.type in mod_types_to_ignore):
				continue

			not_supported = mod.resolve_device_support(map, check_self=True)
			if (len(not_supported) == 0):
				retlist.append(mod)

		sel_iterators = [self.db.iter_components(SelectByDevice), self.db.iter_components(SelectByConfig)]

		for sels in sel_iterators:
			for sel in sels:
				not_supported = sel.resolve_device_support(map, check_self=True)
				if len(not_supported) == 0:
					retlist.append(sel)

		return retlist

	def print_modules_with_empty_caption(self):
		"""
		print modules with empty caption
		"""
		mods = self.db.iter_modules()
		for mod in mods:
			if mod.caption == '':
				print mod.id

	def print_all_modules(self):
		mods = self.get_items_for_mcu( MCU('at32uc3c0512c',self.db))
		print len(mods)
		for mod in mods:
			print mod.id

	def get_generator_to_projects(self):
		"""
		get avrstudio specific example projects

		This function returns a dictionary, which maps the generator
		classes to a list of projects which support it.
		"""
		generator_to_projects = {}

		try:
			for project in self.db.iter_projects():
				for generator_name in project.get_generator_list():
					if generator_name in self.valid_generator_names:
						the_class = self.runtime.generator_tag_to_class(generator_name)

						if the_class is None:
							self.log.warning("Warning: Unknown generator `%s' for project `%s'" % (generator_name, project.id))
							continue

						try:
							generator_to_projects[the_class].append(project)
						except KeyError:
							generator_to_projects[the_class] = [project]
		except:
			pass

		return generator_to_projects

	#helper function for printing out list
	def print_items(self,desc,items):
		print desc,len(items)
		try:
			for src,dest in items.items():
				print src,'=',dest
		except:
			for item in items:
				print item

	def printlog(self,a):
		print a


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


	def get_project_data(self, project_id):
		"""
		Return a dictionary with data for the project with the specified ID:
		id, type, device, caption, name, description and help_url.
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


	def _get_license_to_captions(self, generator):
		"""
		Return dictionary which maps a license file to the captions of
		whichever modules have specified it as their license.
		"""
		license_to_ids = generator._get_license_to_ids()
		license_to_captions = {}

		for license, ids in license_to_ids.items():
			caption_list = []

			for new_id in ids:
				module = self.db.lookup_by_id(new_id)
				caption = module.caption

				if caption:
					caption_list.append(caption)
				else:
					self.log.error("Module `%s' has empty caption" % new_id)

			caption_list.sort()
			license_to_captions[license] = caption_list

		return license_to_captions


	def get_generator_from_project_id(self, project_id):
		"""
		Returns a generator object for the project with the specified ID.
		"""
		project = self.db.lookup_by_id(project_id)

		# Look for a valid generator for the project
		for generator_name in project.get_generator_list():
			if generator_name in self.valid_generator_names:
				generator_class = self.runtime.generator_tag_to_class(generator_name)

				# Instantiate the generator and return it
				return generator_class(project, self.db, self.runtime)

		# No valid generator was found for the project
		return None


	def get_all_project_ids(self):
		"""
		Returns a list of all project ids in the repository.

		The projects are not checked with regards to which generator they support.
		"""
		project_ids = []

		for project in self.db.iter_projects():
			project_ids.append(project.id)

		return project_ids


	def get_asf_h_content(self, toolchain, module_ids, template_file=None):
		"""
		Return the content of asf.h for the supplied list of module IDs.

		The toolchain must be specified, i.e., either avrgcc or avr32gcc.

		The template asf.h can also be specified -- it must then contain
		the string $ASF_HEADER_FILES$ where the includes are to be
		put.
		"""
		# GenericElement expects a project to get config from at init
		class Dummy(object):
			@staticmethod
			def get_configuration():
				return {}

		modules = [self.db.lookup_by_id(module_id) for module_id in module_ids]

		dummy = Dummy()
		generic_generator = generic.GenericElement(dummy, self.db, self.runtime)
		generic_generator.toolchain = toolchain

		asf_h = generic_generator._generate_asf_header_content(modules, template_file)

		return asf_h


	# printing all example project related data
	def print_all_proj_data(self):
		def capitalize_first_letter(string):
			return string[:1].upper() + string[1:]

		gen_to_projs = self.get_generator_to_projects()

		for generator, projects in gen_to_projs.items():
			print "%s projects for generator %s" % (len(projects), generator.generator_tag)

			for project in projects:
				proj = generator(project, self.db, self.runtime)

				proj._get_project_data()
				print "Project Id=", proj.project_id
				print "Device = ", proj.project_device
				print "Project Caption =", proj.project_caption
				print "Project Name=", proj.project_name
				print "Description = ", proj.project_description
				print "Project Documentation URL=", proj.project_help_url

				for category,keywords in proj.get_project_keywords().items():
					print 'Category = ',category
					for keyword in keywords:
						print '		',keyword

				#Build linker flags
				self.print_items('--------- Build linker flags---------',proj.get_build_from_export(BuildLinkerFlags))

				#Build Compiler library
				self.print_items('--------- Build Compiler library---------',proj.get_build_from_export(BuildCompilerLibrary))

				#Build distribute files
				self.print_items('--------- Build distribute files---------',proj.get_build_from_export(BuildDistributeFile))

				#Build header
				self.print_items('--------- Build header ---------',proj.get_build_from_export(BuildHeader))

				#Build includes
				self.print_items('--------- BuildIncludes ---------',proj.get_build_from_export(BuildInclude))

				#Build Linker Script
				self.print_items('--------- Build Linker Script ---------',proj.get_build_from_export(BuildLinkerScript))

				#Build Assembly
				self.print_items('--------- Build Assembly ---------',proj.get_build_from_export(BuildAssembly))

				#csource file
				self.print_items('--------- BuildC ---------',proj.get_build_from_export(BuildC))

				#build define
				self.print_items('--------- BuildDefine ---------',proj.get_build_from_export(BuildDefine))

				#build user library
				self.print_items('--------- BuildUserLibrary ---------',proj.get_build_from_export(BuildUserLibrary))

				#LicenseFiles
				self.print_items('--------- BuildDistributeLicense ---------',proj.get_build_from_export(BuildDistributeLicense))

				#linker script
				total_config = proj.get_total_config()

				self.print_items('------------Device Series------------',[proj._get_framework_arch()])

				self.print_items('--------- Compiler Optimization Flags ---------',proj._get_avrgccproj_optimization_level(total_config))

				self.print_items('--------- Linker Flags ---------',proj._get_avrgccproj_linker_flags(total_config))

				self.print_items('--------- Linker General Settings ---------',proj._get_avrgccproj_stdlib_settings(total_config))

				self.print_items('--------- Compiler Files ---------',proj.get_compile_files())

				print proj.get_framework_config()

				self.print_items('--------- License Files----------',proj.get_license_files())


# For testing we can try out things here
#if __name__ == "__main__":
#	obj = PythonFacade('C:\\masterdb.xml')

#	project_ids = obj.get_all_project_ids()
#	obj.write_project_data_to_xml_file(project_ids, 'C:\\test.xml')

#	obj.print_all_modules()
#	obj.print_all_proj_data()


