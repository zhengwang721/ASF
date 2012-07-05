import sys
import xml.etree.cElementTree as ET

from optparse import OptionParser

from asf.helper import indent_xml, strip_xml


if __name__ == "__main__":
	parser = OptionParser(usage="%prog <input file> - Indent XML file and output to standard out.")

	parser.add_option("-o","--output", action="store", type="string", dest="filename", help="Output to given filename")
	parser.add_option("-s","--strip", action="store_true", dest="strip", default=False, help="Strip existing XML before indenting")
	parser.add_option("-v","--verbose", action="store_true", dest="verbose", default=False, help="Verbose mode")

	(options, args) = parser.parse_args()

	if len(args) < 1:
		parser.print_help()
		sys.exit(1)

	input = args[0]
	output = sys.stdout

	if options.filename:
		output=options.filename

	if options.verbose:
		print "Input file",input
		print "Output file", output

	try:
		tree = ET.parse(input)
	except Exception as e:
		print "Unable to read or parse file. Error:", str(e)
		sys.exit(1)

	root = tree.getroot()

	if options.strip:
		strip_xml(root)

	indent_xml(root)

	tree.write(output)
