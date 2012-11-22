import httplib
import os.path
import re
import sys
import time
import urllib
import urlparse

from optparse import OptionParser

import asf.helper

from asf.configuration import ConfigurationHandler
from asf.database import Module, Project
from asf.extensionmanager import *
from asf.junit_report import *
from asf.runtime import Runtime
from asf.toolchain.doxygen import DoxygenProject
from asf.toolchain.generic import GenericProject


class UrlTester(object):
	"""
	Class for testing URLs via HEAD requests, with support for proxy servers.
	"""
	proxy = None

	def __init__(self, proxy_url=None):
		# Set up proxy, if specified
		if proxy_url:
			url_info = urlparse.urlsplit(proxy_url)

			if url_info.scheme == 'http':
				conn_class = httplib.HTTPConnection
			elif url_info.scheme == 'https':
				conn_class = httplib.HTTPSConnection

			self.proxy = conn_class(url_info.netloc, timeout=3)

	def _do_head_request(self, url):
		"""
		Do a HEAD request for the specified URL and return the response.

		The request is done via the optional proxy, if one was specified for the
		object at instantiation time.
		"""
		url_info = urlparse.urlsplit(url)

		if url_info.scheme != 'http':
			raise Exception("Only HTTP is supported for HEAD requests.")

		if self.proxy:
			conn = self.proxy
			req_url = url
		else:
			conn = httplib.HTTPConnection(url_info.netloc, timeout=0.5)
			req_url = url_info.path

		conn.request("HEAD", req_url)

		return conn.getresponse()

	def test_url(self, url):
		"""
		Test the specified URL with a HEAD request, raise an Exception if the response
		from server was anything but OK (code 200).
		"""
		response = self._do_head_request(url)

		if response.status != httplib.OK:
			raise Exception(response.reason)


class BaseLinkChecker(object):
	"""
	This is the base class for all documentation link checkers.

	To create a new link checker, the following class members must be defined:
		class_to_handle  = <database class to handle>
		types_to_handle  = <list of types of the class to handle>
		junit_suite_name = <name of the documentation link>

	In addition the method _get_component_urls() must be defined, and should
	return a list of all URLs for the relevant documentation link for a specified
	module.
	If no URLs are found or the module should be skipped, the returned list must be
	empty.

	A function for testing URLs must be specified, since this class does not do it
	directly. The URL tester function must raise an Exception if there is a problem
	with the tested URL.
	"""
	junit_package_name = "Atmel Software Framework Documentation"

	def __init__(self, db, junit_report, url_tester):
		"""
		Save the database (db) to use, create a new JUnit suite in the specified
		report (junit_report) and store the specified function for testing the
		documentation links (url_tester).
		"""
		self.db = db
		self.junit_suite = create_new_junit_suite(junit_report, self.junit_package_name, self.junit_suite_name)
		self.url_tester = url_tester

	def _iter_components(self):
		"""
		Iterate through the type of database components that are supported by this
		class, as specified by class_to_handle and types_to_handle.
		"""
		for c in self.db.iter_components(self.class_to_handle):
			if c.type in self.types_to_handle:
				yield c

	def _get_interval(self):
		"""
		Get the number of elapsed seconds since last time this function was called.
		For the first call, 0 is returned.
		"""
		current_time = time.time()

		try:
			split_time = self.split_time
		except AttributeError:
			elapsed_time = 0
		else:
			elapsed_time = current_time - split_time

		self.split_time = current_time

		return  elapsed_time

	def _test_and_report_url(self, url_to_test, junit_class):
		"""
		Test the specified URL (url_to_test) and add the success or failure case to
		the JUnit suite. The case class is specified with junit_class, while the case
		name is always the tested URL.

		Failure of the URL is detected if the tester function raises an Exception.
		"""
		# Jenkins doesn't like hash in class name
		junit_class = junit_class.replace('#', '_')

		# Hack to get Jenkins to structure the results as we want them
		junit_class = '.'.join([self.junit_suite_name, junit_class.replace('.', '_')])

		# Reset interval timer, then test the URL
		self._get_interval()

		try:
			self.url_tester(url_to_test)
		except Exception as e:
			self.junit_suite.add_failure_case(junit_class, url_to_test, self._get_interval(), str(e))
		else:
			self.junit_suite.add_success_case(junit_class, url_to_test, self._get_interval())

	def _get_component_urls(self, module):
		raise Exception("Abstract class method called")

	def check_urls(self):
		"""
		Iterate through all the relevant database components, fetch all their URLs then
		test and report the result for each of them.
		"""
		for component in self._iter_components():
			for url in self._get_component_urls(component):
				self._test_and_report_url(url, component.id)


class ModuleGroupLinkChecker(BaseLinkChecker):
	"""
	Class for checking doxygen module group URLs.
	"""
	class_to_handle = Module
	types_to_handle = ["driver", "component", "service"]

	junit_suite_name   = "Module Group"

	def __init__(self, db, junit_report, url_tester):
		"""
		In addition to the init for BaseLinkChecker, save the database's device map
		and a dictionary that maps each doc-arch to its respective list of MCUs.
		"""
		super(ModuleGroupLinkChecker, self).__init__(db, junit_report, url_tester)
		# For modules, we also need the device map, and mapping from doc-arch to MCUs
		self.mcu_map = db.lookup_by_id(GenericProject.mcu_map_id)
		doc_arch = self.mcu_map.get_doc_arch_dict().keys()
		self.doc_arch_to_mcus = dict([(a, self.mcu_map.get_mcu_list(group=a)) for a in doc_arch])

	@staticmethod
	def _get_build_fetcher_for_module(module):
		"""
		Get the module's function that returns build items for the relevant
		documentation type.
		"""
		return module.get_doxygen_entry_point

	@staticmethod
	def _get_url_fetcher_for_module(module):
		"""
		Get the module's function that returns URL for the relevant documentation
		type.
		"""
		return module.get_help_url

	def _get_module_doc_arch(self, module):
		"""
		Get a list of the doc-arch groups that the specified module has support for.
		"""
		module_mcus = []
		module_doc_arch = []

		# Get list of all supported MCUs for the module
		for dev_support in module.get_device_support():
			module_mcus.extend(self.mcu_map.get_mcu_list(group=dev_support, check_group_is_mcu=True))

		# For each doc-arch group..
		for doc_arch, doc_arch_mcus in self.doc_arch_to_mcus.items():
			# ..check for matching MCU with those supported by module
			for module_mcu in module_mcus:
				# Break on first hit, to go to next doc-arch group
				if module_mcu in doc_arch_mcus:
					module_doc_arch.append(doc_arch)
					break

		return module_doc_arch

	def _get_component_urls(self, module):
		"""
		For a non-hidden module, return a list of URLs for the relevant documentation
		type, if it is specified for the module, for all doc-arch groups.
		"""
		# Skip hidden modules
		if 'hidden' in module.get_gui_flag_list():
			return []

		# Get the relevant functions for module's documentation
		fetch_build = self._get_build_fetcher_for_module(module)
		fetch_url = self._get_url_fetcher_for_module(module)

		# Does the module have any docs at all?
		if not fetch_build():
			return []

		module_doc_arch = self._get_module_doc_arch(module)

		urls = []
		for doc_arch in module_doc_arch:
			urls.append(fetch_url(doc_arch))
		return urls


class QuickStartLinkChecker(ModuleGroupLinkChecker):
	"""
	Class for checking quick start guide URLs.
	"""
	junit_suite_name   = "Quick Start Guide"

	@staticmethod
	def _get_build_fetcher_for_module(module):
		return module.get_doxygen_quick_start

	@staticmethod
	def _get_url_fetcher_for_module(module):
		return module.get_quick_start_url


class ProjectLinkChecker(BaseLinkChecker):
	"""
	Class for checking project documentation URLs.
	"""
	class_to_handle = Project
	types_to_handle = ["normal", "unit-test"]

	junit_suite_name = "Project documentation"

	def _get_component_urls(self, project):
		"""
		For a project that has the doxygen generator specified, return the URL of its
		documentation.
		"""
		# Is doxygen among generators for project?
		if not DoxygenProject.toolchain in project.get_generator_list():
			return []

		url = project.get_help_url(urllib.quote(project.id))
		return [url]


class LinkCheckerRuntime(Runtime):
	"""
	Class for executing the various documentation link checks.
	"""
	def __init__(self, template_dir, configuration):
		super(LinkCheckerRuntime, self).__init__(template_dir, configuration)

		self.base_url     = None
		self.junit_report = None
		self.proxy_url    = None

	def set_junit_report(self, report):
		self.junit_report = report

	def set_proxy_url(self, proxy_server):
		self.proxy_server = proxy_server

	def set_base_url(self, base_url):
		self.base_url = base_url

	def run(self):
		# If a custom doc URL has been set, override the one in database
		if self.base_url:
			self.db.set_documentation_server(self.base_url)

		url_tester = UrlTester(proxy_url=self.proxy_url)

		# Now execute _all_ defined link checkers
		classes_to_run = BaseLinkChecker.__subclasses__()
		for c in classes_to_run:
			# Add any subclasses of current checker class
			classes_to_run.extend(c.__subclasses__())

			# Instantiate and run checker
			checker = c(self.db, self.junit_report, url_tester.test_url)
			checker.check_urls()


if __name__ == "__main__":
	class ReturnCode:
		SUCCESS       = 0
		ERROR         = 1
		# Invalid user input
		ERROR_INVALID = 2


	script_path = sys.path[0]

	parser = OptionParser(usage=
		"""<options>
		""")

	# parser.add_option("","--ext-root", dest="ext_root", default=os.path.join(script_path, '..', '..'), help="FDK extension root directory")
	parser.add_option("-b","--base-dir", dest="ext_root", default=os.path.join(script_path, '..', '..'), help="FDK extension root directory")
	parser.add_option("","--main-ext-uuid", dest="main_ext_uuid", default="Atmel.ASF", help="Main FDK extension UUID")
	parser.add_option("","--main-ext-version", dest="main_ext_version", help="Main FDK extension version")

	parser.add_option("-c","--cached", action="store_true", dest="cached", default=False, help="Do not rescan all directories, but use the list of XML files from last run")
	parser.add_option("-u", "--base-url", dest="asf_baseurl", default=None, help="Specify the base URL for the online documentation: http://test.com/asf_docs/")
	parser.add_option("-p", "--proxy-server", dest="proxy_server", default=None, help="Specify the URL for proxy server to use: http://user:password@myproxy.com:123")
	parser.add_option("-j", "--junit-file", dest="junit_file", default=None, help="Specify the file to which the J-Unit report shall be saved")
	parser.add_option("-d", "--debug", action="store_true", dest="debug", default=False, help="Set debug mode")

	(options, args) = parser.parse_args()

	# Get paths that runtime needs to know
	template_dir = os.path.join(script_path, "templates")
	xml_schema_dir = os.path.join(script_path, "schemas")
	device_map = os.path.join(script_path, "device_maps", "atmel.xml")

	if options.junit_file:
		junit_filename = os.path.abspath(options.junit_file)
	else:
		junit_filename = os.path.join(script_path, 'testReport.xml')

	# Instantiate required classes and options
	configuration = ConfigurationHandler()
	junit_report  = JUnitReport()
	runtime       = LinkCheckerRuntime(template_dir, configuration)

	# Set up the runtime
	runtime.setup_log()
	runtime.set_junit_report(junit_report)
	runtime.set_commandline_args(args)
	runtime.set_base_url(options.asf_baseurl)
	runtime.set_proxy_url(options.proxy_server)

	# Assume success for now
	return_code = ReturnCode.SUCCESS

	try:
		# Load DB and check its links
		# Create the extension manager
		ext_manager = FdkExtensionManager(runtime, options.ext_root, use_cache=options.cached)

		# Load prerequisites like XML schemas and device map
		ext_manager.load_schemas(xml_schema_dir)
		ext_manager.load_device_map(device_map)

		# Now scan for extensions and load them
		ext_paths = ext_manager.scan_for_extensions()
		ext_manager.load_and_register_extensions(ext_paths)

		# Get the database of the main extension
		main_ext = ext_manager.request_extension(options.main_ext_uuid, options.main_ext_version)
		main_db = main_ext.get_database()

		# Do a sanity check unless in cached mode
		if not options.cached:
			main_db.sanity_check()

		# Set the database for the runtime and let the testing commence
		runtime.set_db(main_db)
		runtime.run()

	except Exception as e:
		# Runtime failed!
		return_code = ReturnCode.ERROR

		# Add the exception to the last suite that was added to the JUnit report
		suite = junit_report.get_last_suite() or JUnitNullSuite()
		suite.add_system_error(str(e))

		# Re-raise the exception if debugging
		if options.debug:
			raise

	else:
		# Return failure if any links failed
		metrics = junit_report.get_total_metrics()
		if metrics['failures'] or metrics['errors']:
			return_code = ReturnCode.ERROR

	finally:
		# Write the J-Unit report to file
		junit_report.write(junit_filename)

	sys.exit(return_code)
