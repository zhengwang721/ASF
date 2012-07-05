import os
import os.path
import re
import sys
import time

from optparse import OptionParser

import asf.helper

from asf.configuration import ConfigurationHandler
from asf.runtime import Runtime
from asf.database import MCU
import asf.exception


class ConsoleRuntime(Runtime):
	def __init__(self, template_dir, configuration):
		Runtime.__init__(self, template_dir, configuration)
		self.quit = False
		self.commands = {
			'q' : { 'func' : self.command_quit,
					'opt' : '',
					'help' : "Quit"},
			'l' : { 'func' : self.command_lookup,
					'opt' : '<id> [mcu]',
					'help' : "Lookup <id> and print relevant information. Optionally resolve select-by-* elements with [mcu]"},
			'r' : { 'func' : self.command_reverse_lookup,
					'opt' : '<id>',
					'help' : "Find all modules that use/require <id>"},
			'find' : { 'func' : self.command_find,
					'opt' : "<str>",
					'help' : "Find module ids that start with <str>. '*' can be used for matching several characters."},
			'h' : { 'func' : self.command_help,
					'opt' : '',
					'help' : "Show this help"},
		}
		self.project = None
		self.generator = None

	def command_help(self, args):
		print "=== Database console (%s)===" % (self.db.get_framework_version())
		print "Syntax format is: <command> <arguments>"
		print ""
		print "Available commands:"
		just1 = 4
		just2 = 10
		for (c, param) in self.commands.iteritems():
			print c.ljust(just1) + ' ' + param['opt'].ljust(just2) + ' - ' + param['help']

	def command_quit(self, arg):
		self.quit = True

	def command_find(self, arg):
		print "Searching for", arg
		matches = []
		re_str = re.escape(arg)
		re_str = re_str.replace("\\*", ".*")
		re_match = re.compile(re_str)

		all = self.db.root.findall(".//*")
		for d in all:
			if d.attrib.has_key('id'):
				id = d.attrib['id']
				if re_match.match(id):
					print id


	def command_lookup(self, arg):
		print "Lookup", arg

		try:
			(id, mcu_name) = arg.split(" ")
		except ValueError:
			id = arg
			mcu_name = None

		try:
			c = self.db.lookup_by_id(id)
			print "Module:",c

			if mcu_name:
				print "\nResolving selections using", mcu_name

				c.resolve_all_selections(self.configuration, MCU(mcu_name, self.db))

			print "\nFrom the xml file", c.fromfile

			print "\nRequirements:"
			c.visualize_requirements(self.output)

			print "\nInformation:"
			c.visualize_info(self.output)

		except asf.exception.NotFoundError:
			print "Not found in db"

	def output(self, str):
		print str

	def command_reverse_lookup(self, arg):
		print "The following modules refer to",arg+":"

		all_modules = list()

		reverse_list = self.db._reverse_resolve_idref(arg)
		if not reverse_list:
			print "No modules"
		else:
			for rev in reverse_list:
				id = rev.getparent().attrib["id"]
				print id
				all_modules.append(id)
			working_modules = list()
			try:
				for c in self.db.reverse_lookup_by_id(arg):
					working_modules.append(c.id)

			except NotFoundError as k:
				pass

			non_working = set(all_modules) - set(working_modules)
			if non_working:
				print "Non working modules", "\n".join(non_working)

		print "\nThe following modules are related to", arg+":"

		related_modules = self.db.reverse_lookup_by_related_module_id(arg)
		if not related_modules:
			print "No modules"
		else:
			for type in related_modules:
				print type+":"
				for type_id in related_modules[type]:
					print "\t",type_id

	def run_command(self, cmd, args):
		if cmd in self.commands:
			try:
				self.commands[cmd]["func"](args)
			except Exception as e:
				print "Error occured during command: ", e
		else:
			print "Unknown command", cmd

	def run_interactive(self):
		self.command_help("")

		while not self.quit:
			print "\n$ ",
			line = sys.stdin.readline()
			# Find command and args
			try:
				(cmd, args) = line.split(" ", 1)
				args = args.strip()
			except ValueError:
				# No parameters given
				cmd = line.strip()
				args = None
				pass
			self.run_command(cmd, args)

	def run(self):
		if len(self.args) > 1:
			self.run_command(self.args[0], self.args[1])
		else:
			self.run_interactive()


if __name__ == "__main__":

	start_time = time.clock()
	parser = OptionParser(usage=
		"""<options> <args>.
		The arguments specifies the ids of the projects to generate.
		The wildcard '*' can be used in the id to match several ids.
		Ex: 'avr32* common*' will match all avr32 and common projects.
		If no ids are given all projects are generated""")
	# General otions
	parser.add_option("-b", "--basedir", dest="basedir", help="Set root directory of ASF installation")
	parser.add_option("-s", "--set-config", action="append", dest="config", help="Set a config value: --set-config <name>=<value>")
	parser.add_option("-c","--cached", action="store_true", dest="cached", default=False, help="Do not rescan all directories, but use the list of XML files from last run")

	# Debug options
	parser.add_option("-d","--debug", action="store_true", dest="debug", default=False, help="Enable debugging. Sets 'level' to debug if not otherwise specified.")
	parser.add_option("-l","--level", dest="level", default=False, help="Log output level, from most detailed to least detailed: debug, info, warning or error")
	parser.add_option("-t","--timestamp", action="store_true", dest="timestamp", default=False, help="Timestamp log")
	parser.add_option("-v","--verbose", action="store_const", dest="level", const="info", help="Verbose output")

	(options, args) = parser.parse_args()

	script_path = sys.path[0]

	basedir = options.basedir
	if basedir is None:
		basedir = "../../"

	# Find paths for input files in same folder as this script
	templatedir = os.path.join(script_path, "templates")
	cachedir = os.path.join(script_path, "cache")
	xml_schema_path = os.path.join(script_path, "asf.xsd")

	# Change directory to the ASF root folder
	os.chdir(basedir)

	# Read command line configuration
	try:
		configuration = ConfigurationHandler.generate_from_option_parser(options)
	except Exception as e:
		print "Error with configuration", e
		parser.print_help()
		sys.exit(2)

	runtime = ConsoleRuntime(templatedir, configuration)

	runtime.create_and_set_cache_dir(cachedir)

	runtime.set_debug(options.debug)
	runtime.set_use_file_list_cache(options.cached)
	runtime.set_xml_schema_path(xml_schema_path)
	runtime.set_commandline_args(args)

	# If debugging is enabled and not the log level, we set it to debug
	if options.debug and not options.level:
		options.level = "debug"

	if options.level:
		level = runtime.get_log_level_from_text(options.level)
		if not level:
			print "ERROR: Invalid debug level: " + options.level
			parser.print_help()
			sys.exit(1)

		runtime.setup_log(level, options.timestamp)
	else:
		runtime.setup_log()

	runtime.log.debug("Starting")

	try:
		# Read XML files
		runtime.load_db()

		# Sanity check of database
		version = runtime.db.get_framework_version()
		runtime.log.info("Finished loading ASF version: " + version)

		if options.cached:
			runtime.log.info("Skipping DB sanity check in cached mode")
		else:
			runtime.log.info("Running DB sanity check")
			runtime.database_sanity_check()

		runtime.run()
	except Exception as e:
		runtime.log.critical("Project generation failed: "+str(e))
		# If debugging we raise the exception to get the backtrace
		if options.debug:
			raise

	# Store debug information (do this last, since it modifies the XML tree)
	if options.debug:
		outputfile = os.path.join(cachedir, "db.xml")
		runtime.log.info("Writing database to " + outputfile)
		runtime.write_xml_tree(outputfile)

	# Temporary code for cleaning up everyones repository, before changing
	# location of stored files
	try:
		os.remove("db.xml")
	except OSError:
		pass
	try:
		os.remove("asf_filelist.cache")
	except OSError:
		pass

	runtime.log.info("Execution finished in %.2f seconds"  %(time.clock() - start_time))
