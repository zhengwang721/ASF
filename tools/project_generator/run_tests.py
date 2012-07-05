import os
import unittest
from asf.testcases import *

if __name__ == '__main__':
	# Change directory to this script's location
	script_dir = os.path.dirname(__file__) or os.path.curdir
	os.chdir(script_dir)

	unittest.main()
