import os
import sys
import unittest
import xmlrunner
from asf.testcases import *

if __name__ == '__main__':
	# Default output directory
	out_dir = 'test-reports'

	# Switch for specifying custom output dir
	out_switch = "--output-dir="

	# Parse arguments for custom output dir
	try:
		argv_remove_list = []
		for arg in sys.argv:
			# Look for custom output dir switch
			if (out_switch in arg):
				out_dir = arg[len(out_switch):]
				argv_remove_list.append(arg)

			# Check for missing =
			elif out_switch[:-1] in arg:
				raise Exception

		# Check that specified path was not blank
		if not len(out_dir):
			raise Exception

	except Exception:
		# If parsing found errors -- quit with error message
		print "Error: output directory must be specified with: --output-dir=<path>"
		sys.exit(1)

	else:
		# If parsing succeeded, consumed arguments must be removed
		for arg in argv_remove_list:
			sys.argv.remove(arg)

	print "Outputting test results to directory: " + out_dir

	# Change directory to this script's location
	script_dir = os.path.dirname(__file__) or os.path.curdir
	os.chdir(script_dir)

	unittest.main(testRunner=xmlrunner.XMLTestRunner(output=out_dir))
