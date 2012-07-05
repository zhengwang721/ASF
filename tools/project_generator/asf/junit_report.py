import asf.etreeabstraction as ET
import time

from socket import gethostname

import asf.helper


def create_new_junit_suite(report, package_name, suite_name):
	"""
	Make a new JUnit suite with specified package and suite names if the
	supplied report is a JUnitReport, then return the new suite.

	If the specified report is None, a dummy suite is returned instead.

	An exception is raised if the supplied report is not None and is not
	of type JUnitReport.
	"""
	if isinstance(report, JUnitReport):
		suite = report.get_new_suite(suite_name, package_name)
	else:
		if report:
			raise Exception("Supplied report is not of type JUnitReport.")

		suite = JUnitNullSuite()

	return suite


class JUnitReport(object):
	root_tag = 'testsuites'

	def __init__(self):
		root_e = ET.Element(self.root_tag)
		self.etree = ET.ElementTree(root_e)
		self.suites = []
		self.root_e = root_e


	def get_new_suite(self, name, package):
		"""
		Create and add a new test suite for the report, then return it.
		"""
		suite = JUnitTestSuite(name, package)
		self.add_suite(suite)

		return suite


	def add_suite(self, suite):
		"""
		Add an existing test suite to the report.
		"""
		if suite not in self.suites:
			# Update the suite ID first
			suite.root_e.set('id', str(len(self.suites)))
			self.root_e.append(suite.root_e)
			self.suites.append(suite)


	def get_last_suite(self):
		"""
		Return the suite which was last added, or None if there is no
		suite.
		"""
		try:
			suite = self.suites[-1]
		except IndexError:
			suite = None

		return suite


	def get_total_metrics(self):
		"""
		Return a dictionary with the total number of test successes,
		failures and errors.
		"""
		total_metrics = {}

		for suite in self.suites:
			metrics = suite.get_metrics()

			for key, value in metrics.items():
				total_metrics[key] = value + total_metrics.get(key, 0)

		return total_metrics


	def write(self, filename):
		"""
		Write the test report to the specified file.
		"""
		asf.helper.indent_xml(self.root_e)
		self.etree.write(filename)


class JUnitTestSuite(object):
	root_tag      = 'testsuite'

	prop_root_tag = 'properties'
	prop_tag      = 'property'

	sys_out_tag   = 'system-out'
	sys_err_tag   = 'system-err'

	case_tag      = 'testcase'
	fail_tag      = 'failure'
	error_tag     = 'error'


	def __init__(self, name, package):
		self.nr_of_successes = 0
		self.nr_of_failures = 0
		self.nr_of_errors = 0
		self.time = 0

		root_e = ET.Element(self.root_tag)
		root_e.set('hostname', gethostname())
		root_e.set('package', package)
		root_e.set('name', name)
		root_e.set('id', '0')
		self.root_e = root_e

		self.prop_root_e = ET.SubElement(root_e, self.prop_root_tag)
		self.sys_out_e = ET.SubElement(root_e, self.sys_out_tag)
		self.sys_err_e = ET.SubElement(root_e, self.sys_err_tag)

		self._update()


	def get_metrics(self):
		"""
		Return a dictionary with the suite's number of test successes,
		failures and errors.
		"""
		metrics = {
			'successes' : self.nr_of_successes,
			'failures'  : self.nr_of_failures,
			'errors'    : self.nr_of_errors,
		}

		return metrics


	def _update(self):
		"""
		Internal function for updating the test suite's attributes.
		"""
		attribs_to_update = [
			('errors',    self.nr_of_errors),
			('failures',  self.nr_of_failures),
			('tests',     self.nr_of_failures + self.nr_of_successes),
			('timestamp', time.asctime()),
			('time',      self._format_seconds(self.time)),
		]
		for name, value in attribs_to_update:
			self.root_e.set(name, str(value))


	def _format_seconds(self, seconds):
		"""
		Internal function for formatting the supplied number of seconds
		to a 3-decimal format, i.e., millisecond granularity.
		"""
		return '%.3f' % seconds


	def add_property(self, name, value):
		"""
		Add a property with specified name and value to the test suite.
		"""
		prop_e = ET.SubElement(self.prop_root_e, self.prop_tag)
		prop_e.set('name', name)
		prop_e.set('value', value)


	def add_system_output(self, text):
		"""
		Add a new section of system output to the test suite.
		"""
		try:
			self.sys_out_e.text += '\n' + text
		except:
			self.sys_out_e.text = text

		self._update()


	def add_system_error(self, text):
		"""
		Add a new section of system error to the test suite.
		"""
		try:
			self.sys_err_e.text += '\n' + text
		except:
			self.sys_err_e.text = text

		self._update()


	def _get_new_case(self, case_class, case_name, case_time):
		"""
		Internal function for creating and adding a new test case to the
		test suite, with the specified class, name and time (duration) in
		seconds.

		This also updates the total test suite time with that of the
		case.
		"""
		case_e = ET.SubElement(self.root_e, self.case_tag)
		case_e.set('classname', case_class)
		case_e.set('name', case_name)
		case_e.set('time', self._format_seconds(case_time))

		self.time += case_time

		return case_e


	def add_success_case(self, case_class, case_name, case_time):
		"""
		Add a successful test case.
		"""
		self.nr_of_successes += 1
		case_e = self._get_new_case(case_class, case_name, case_time)

		self._update()


	def add_failure_case(self, case_class, case_name, case_time, message):
		"""
		Add a failing test case.
		"""
		self.nr_of_failures += 1
		case_e = self._get_new_case(case_class, case_name, case_time)
		fail_e = ET.SubElement(case_e, self.fail_tag)
		fail_e.set('message', message)

		self._update()


	def add_error_case(self, case_class, case_name, case_time, message):
		"""
		Add a test case error, e.g., when test case did not run.
		"""
		self.nr_of_errors += 1
		case_e = self._get_new_case(case_class, case_name, case_time)
		fail_e = ET.SubElement(case_e, self.error_tag)
		fail_e.set('message', message)

		self._update()


	def add_time(self, time):
		"""
		Add seconds to the test suite time (duration).
		"""
		self.time += time

		self._update()


	def set_time(self, time):
		"""
		Set the test suite time (duration) in seconds.
		"""
		self.time = time

		self._update()


class JUnitNullSuite(object):
	"""
	This is a dummy suite which can be used in scripts where a JUnit report
	is optional, removing the need for checking whether or not a suite has
	actually been created before attempting to output information.
	"""


	def add_property(self, name, value):
		pass


	def add_system_output(self, text):
		pass


	def add_system_error(self, text):
		pass


	def add_success_case(self, case_class, case_name, case_time):
		pass


	def add_failure_case(self, case_class, case_name, case_time, message):
		pass


	def add_error_case(self, case_class, case_name, case_time, message):
		pass


	def add_time(self, time):
		pass


	def set_time(self, time):
		pass
