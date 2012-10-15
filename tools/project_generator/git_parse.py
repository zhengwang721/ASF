
import os
import sys
import time
import subprocess

from optparse import OptionParser

debug_enabled = False

def debug(*args):
	if debug_enabled:
		print "DEBUG: ",
		for arg in args:
			print str(arg),
		print ""

def run_command(cmd, output_filename, unlink=True):
		result_file = open(output_filename,'w')
		debug("Running ", cmd)

		# Run command
		p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=result_file, stderr=subprocess.PIPE)
		(stdoutdata, stderrdata) = p.communicate()
		result_file.close()

		# Handle errors
		if len(stderrdata) > 0:
			raise Exception("Subprocess error, stderr=" + stderrdata)

		if p.returncode > 0:
			raise Exception("Subprocess error, return code=" + p.returncode)

		# Read result
		result_file = open(output_filename,'r')
		lines = result_file.readlines()
		result_file.close()

		# Split result up in several lines
		fixed = []
		for x in lines:
			fixed.extend(x.splitlines())

		debug("Result from cmd=", fixed)

		# Delete temp file if needed
		if unlink:
			debug("Deleting ",output_filename)
			os.unlink(output_filename)

		return fixed


def get_files_fromgitcommit(path, main_branch, dev_branch):
	"""
	Return a list of modified files in specified development branch,
	relative to its common ancestor with the specified main branch.
	"""
	# Find common ancestor of main and dev branch, i.e., the branch point
	cmd = ["git", "merge-base", main_branch, dev_branch]
	result_lines = run_command(cmd, "branchcreate.txt")

	# Return list of modified files in dev branch, relative to the common ancestor
	cmd = ["git", "diff", "--name-only", result_lines[0] + ".." + dev_branch]
	return run_command(cmd, "delete.me")


if __name__ == "__main__":

	start_time = time.clock()
	parser = OptionParser(usage=
			"""<options>
			Creates a list of all files that have been modified in the specified branch
			of a Git repository, with respect to its common ancestor with a different
			main branch -- basically the branch point.
			The main branch can be specified by user, but defaults to "main" if left
			unspecified.

			This script assumes that the current working directory is inside the repo.

			Usage:
			python git_parse.py --fromgitcommit -b <dev branch> [-m <main branch>] [--outfile <filename>] [-d]

			Example:
			python git_parse.py --fromgitcommit -b bug123 --outfile bug123_modified_files.txt
			""")
	 # General otions
	parser.add_option("--fromgitcommit", action="store_true", dest="fromgitcommit", default=True, help="Get files list from Git commit history")
	default_filename="git_parse_output.txt"
	parser.add_option("--outfile", action="store", dest="outfile", default=default_filename, help="Filename for storing result. Default: " + default_filename)
	parser.add_option("-b", "--branchname", action="store", dest="branchname", default=None, help="Specify name of branch")
	parser.add_option("-m", "--mainbranch", action="store", dest="mainbranch", default="main", help="Specify name of main branch")
	parser.add_option("-d", "--debug", action="store_true", dest="debug", default=False, help="Enable debug output")

	(options, args) = parser.parse_args()

	if (options.fromgitcommit == False) or not options.branchname:
		parser.error("Incorrect number of arguments or usage")

	path = "."

	debug_enabled = options.debug

	files = []
	try:
		files = get_files_fromgitcommit(path, options.mainbranch, options.branchname)
	except Exception as e:
		print "Error: ", e
		sys.exit(1)

	result_file = open(options.outfile,'w')
	for filename in files:
		result_file.write(filename + "\n")
	result_file.close()
	debug("Written result to '%s'" % (options.outfile))
