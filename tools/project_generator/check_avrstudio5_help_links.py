import os
import os.path
import zipfile
import StringIO
import lxml.etree as ET
#import xml.etree.ElementTree as ET
from optparse import OptionParser
import urlparse
import httplib

from asf.junit_report import *


def replace_base(url, new_base):
	"""
	Replace baseurl/network location in a string containting an URL. If the new_base
	is empty the original url is returned
	"""
	if not new_base:
		return url

	i = urlparse.urlsplit(url)
	new_parts = (i.scheme, new_base, i.path, i.query, i.fragment)
	output = urlparse.urlunsplit(new_parts)

	return output

def head_request(url, baseurl=None):
	""" Perform HEAD request to a given URL, returns status code (200 for OK) """
	if baseurl:
		url = replace_base(url, baseurl)

	i = urlparse.urlsplit(url)
	conn = httplib.HTTPConnection(i.netloc, timeout=0.1)
	try:
		conn.request("HEAD",i.path)
		res = conn.getresponse()
		return res.status
	except Exception as e:
		return str(e)


class FindLinks(object):
	tag = None
	attr = None
	test_class = "findlink"

	def get_links(self, root):
		help_elements = root.findall(".//%s[@%s]" % (self.tag, self.attr))

		links = []

		for help_elem in help_elements:
			url = help_elem.attrib[self.attr]
			links.append(url)

		return links

	def check_links(self, avr_studio_path):
		return []

	def check_and_report_link(self, link, suite, origin, baseurl=None):
		test_class = self.test_class
		test_name = origin

		status = head_request(link, baseurl)
		if status == httplib.OK:
			suite.add_success_case(test_class, test_name, 0)
		else:
			suite.add_failure_case(test_class, test_name, 0, "Invalid link: "+link+". Request status:"+str(status))

class FindPluginLinks(FindLinks):
	tag = "option"
	attr = "help"
	test_class = "plugin.xml"

	def check_links(self, avr_studio_path, suite, baseurl=None):

		plugin_path = os.path.join(avr_studio_path, r"softwareframework\Plugins")
		input_filename = "plugin.xml"

		filenames = os.listdir(plugin_path)

		for filename in filenames:
			if filename.endswith(".zip"):
				path = os.path.join(plugin_path, filename)
				myzip = zipfile.ZipFile(path, "r")

				p = myzip.open(input_filename)
				buffer = StringIO.StringIO(p.read())

				root = ET.parse(buffer)

				for link in self.get_links(root):
					report_url = os.path.relpath(os.path.join(path, input_filename), avr_studio_path)
					self.check_and_report_link(link, suite, report_url, baseurl)

class FindExampleLinks(FindLinks):
	tag = "Documentation"
	attr = "Help"
	test_class = "example"

	def check_links(self, avr_studio_path, suite, baseurl=None):
		example_path = os.path.join(avr_studio_path, r"softwareframework\ExampleProjectTemplates")

		filenames = os.listdir(example_path)

		for filename in filenames:
			if filename.endswith(".xml"):
				path = os.path.join(example_path, filename)

				root = ET.parse(path)

				for link in self.get_links(root):
					report_url = os.path.relpath(path, avr_studio_path)
					self.check_and_report_link(link, suite, report_url, baseurl)

class FindTemplateLinks(FindLinks):
	tag = "documentation"
	attr = "help"
	test_class = "template"

	def check_links(self, avr_studio_path, suite, baseurl=None):
		template_base_path = os.path.join(avr_studio_path, "projecttemplates")
		template_paths = [
				os.path.join(template_base_path, "Atmel Boards"),
				os.path.join(template_base_path, "User Boards"),
				]
		input_filename = "Framework.config"

		for template_path in template_paths:
			filenames = os.listdir(template_path)
			for filename in filenames:
				if filename.endswith(".zip"):
					path = os.path.join(template_path, filename)

					myzip = zipfile.ZipFile(path, "r")
					p = myzip.open(input_filename)

					buffer = StringIO.StringIO(p.read())

					root = ET.parse(buffer)

					for link in self.get_links(root):
						report_url = os.path.relpath(os.path.join(path, input_filename), avr_studio_path)
						self.check_and_report_link(link, suite, report_url, baseurl)

def main(avr_studio_path, baseurl, reportfilename):

	types = {
			'Plugins' : FindPluginLinks,
			'Examples' : FindExampleLinks,
			'Templates' : FindTemplateLinks
			}

	report = JUnitReport()

	for name, theclass in types.items():
		instance = theclass()

		suite = report.get_new_suite(name=name, package="ASF URL test")

		instance.check_links(avr_studio_path, suite, baseurl)

	report.write(reportfilename)


if __name__=="__main__":
	parser = OptionParser(usage=
		"""<options>
		Find all ASF help URLs for the given AVR Studio 5 installation.
		""")

	avr_studio_path = r"C:\Program Files\Atmel\AVR Studio 5.0"

	parser.add_option("-d","--dir", dest="dir", default=avr_studio_path, help="Path to AVR Studio 5 installation")
	parser.add_option("-b","--baseurl", dest="baseurl", default=None, help="Base URL to use. Will replace exisiting base URL")
	default_report_file = "check_avrstudio5_help_links_result.xml"
	parser.add_option("-o","--output", dest="outfile", default=default_report_file, help="Specify custom report filename. If not specified, report is written to: " + default_report_file)

	(options, args) = parser.parse_args()

	main(options.dir, options.baseurl, options.outfile)
