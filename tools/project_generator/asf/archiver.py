import os
import shutil
from zipfile import ZipFile, ZIP_DEFLATED
import subprocess
from asf.toolchain.generic import GenericProject
import helper
import asf.etreeabstraction as ET


class Archiver(GenericProject):
	def __init__(self, runtime, archive_dir, pack_zip = False, pack_7zip = False, add_doxygen = False, export_flat=False):
		self.runtime = runtime
		self.archive_dir = archive_dir
		self.pack_zip = pack_zip
		self.pack_7zip = pack_7zip
		self.add_doxygen = add_doxygen
		# Always add xml file when generating doxygen:
		self.create_xml = add_doxygen
		self.destination = ""
		self.zipfile = ""
		self.doxygen_dir = ""
		self.export_flat = export_flat
		# Initialize our XML tree
		if self.create_xml:
			self.xml_file_name = os.path.join(self.archive_dir, "asf_packages.xml")
			self.tree = ET.ElementTree()
			self.root_element = ET.Element("packages")
			self.tree._setroot(self.root_element)

		self.archive_counter = 0

	@property
	def log(self):
		return self.runtime.log

	def write_zip(self, path, archive):
		paths = os.listdir(path)
		for p in paths:
			# Make the path relative
			p = os.path.join(path, p)
			if os.path.isdir(p): # Recursive case
				self.write_zip(p, archive)
			else:
				# Write the file to the zipfile
				archive.write(p, os.path.relpath(p, self.destination))
		return

	# Add project info to XML tree
	def add_xml_info(self):
		# Generate XML archive descriptor
		self.log.info("Generating XML archive descriptor")

		# Architecture
		group_map = self.project.mcu.get_group_map()
		if group_map == None:
			self.log.warning("Skipping XML generation for %s - unable to fetch architecture", self.project.id)
			return
		arch_found = False
		for arch in group_map:
			if arch in ["uc3", "xmega", "sam"]:
				arch_found = True
				break
		if not arch_found:
			self.log.warning("Skipping XML generation for %s - no valid architecture found", self.project.id)
			return
		# Let's check we already has a section for this architecture
		next_element = self.root_element.find(arch)
		if next_element == None:
			# No we don't, just add it.
			next_element = ET.SubElement(self.root_element, arch)

		# Module type
		module = self.project.id.split('.')
		if len(module) > 1:
			module = module[1]
		else:
			module = self.project.id
		if module not in ["applications", "boards", "components", "drivers", "services"]:
			self.log.warning("Skipping XML generation for %s - '%s' is not a valid module type", self.project.id, module)
			return
		next_element = ET.SubElement(next_element, module)

		# Name
		name = self.project.caption
		new_element = ET.SubElement(next_element, "name")
		new_element.text = name

		# MCU
		mcu = self.project.mcu
		new_element = ET.SubElement(next_element, "mcu")
		new_element.text = mcu.name

		# Board
		board = self.project.board
		split_board = board.split('.')
		if len(split_board) > 1:
			board = split_board[1-len(split_board)]
		new_element = ET.SubElement(next_element, "board")
		new_element.text = board

		# File
		zipfile = os.path.basename(self.zipfile)
		new_element = ET.SubElement(next_element, "file", attrib={"name":zipfile} )
		new_element.text = zipfile

		# Doc
		docfile = os.path.relpath(self.doxygen_dir, self.destination)
		docfile = os.path.join(self.project.id, docfile)
		# We're assuming documentation is generated in the './doxygen/html/' sub-directory.
		# Strictly we should be parsing doxyfile.doxygen to be sure, but.. uhm.. maybe later.
		docfile = os.path.join(docfile, "doxygen"+os.sep+"html"+os.sep+"index.html")
		new_element = ET.SubElement(next_element, "doc", attrib={"name":"Readme"} )
		new_element.text = docfile

	# Write the XML to file
	def xml_finalize(self):
		if not self.create_xml:
			return
		helper.indent_xml(self.root_element)
		self.tree.write_c14n(self.xml_file_name)

	def create(self, project, db):
		if not self.archive_dir:
			return

		# If we are exporting in different folders, increase the directory name counter.
		if not self.export_flat:
			self.archive_counter += 1

		self.project = project
		self.db = db
		# Temporary bugfix for shortening path names:
		self.destination = os.path.join(self.archive_dir,
			"%03d" % (self.archive_counter))
		# Original line:
		#self.destination = os.path.join(self.archive_dir, project.id)
		zipfile_destination = os.path.join(self.archive_dir, project.id)

		# Remove any previous created project dir
		if os.path.isdir(self.destination) and not self.export_flat:
			shutil.rmtree(self.destination)

		# On empty file list do nothing
		if not len(project.filelist):
			self.log.debug("No files found for project %s" % project.id)
			return False

		self.log.debug("Creating archive dir %s" % project.id)
		if not os.path.exists(self.destination):
			os.mkdir(self.destination)

		for f in project.filelist:
			path = os.path.join(self.destination, os.path.dirname(f))
			try:
				os.makedirs(path)
			except OSError as e:
				pass
			try:
				shutil.copy(f, path)
			except IOError as e:
				self.log.debug("Failed to copy file %s" % f)
				# Remove whole arhive dir so we don't have any partial stuff stored
				if not self.export_flat:
					shutil.rmtree(self.destination)
				raise e

		if self.add_doxygen:
			# Generate doxygen documentation
			for f in project.filelist:
				# Locate doxyfile.doxygen-file
				if os.path.basename(f) == "doxyfile.doxygen":
					# Found it. We need to run doxygen from the directory where it's located.
					self.doxygen_dir = self.destination+os.sep+os.path.dirname(f)
					# We don't wanna polute the screen, create a file to direct doxygen output
					error_file_name = self.doxygen_dir+os.sep+"doxygen.out"
					error_file = open(error_file_name, 'w')
					# Run doxygen
					self.log.info("Running doxygen...")
					p = subprocess.Popen(["doxygen", "doxyfile.doxygen"], cwd = self.doxygen_dir, stdout = error_file, stderr = error_file)
					p.wait()
					error_file.close()
					# We don't wanna keep the output file if it's empty
					if os.path.getsize(error_file_name) == 0:
						os.remove(error_file_name)
						self.log.info("Completed running Doxygen without any errors")
					else:
						self.log.info("Doxygen generated errors, see %s", error_file_name)

		if self.pack_zip:
			self.zipfile = zipfile_destination + ".zip"
			zip = ZipFile(self.zipfile, "w", ZIP_DEFLATED)
			self.write_zip(self.destination, zip)
			zip.close()

		if self.create_xml:
			self.add_xml_info()

		if self.pack_7zip:
			p = subprocess.Popen(["7z", "a", self.destination + ".7z", self.project.id], cwd = self.archive_dir, stdout = subprocess.PIPE)
			p.wait()

		return True
