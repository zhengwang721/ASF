
import os
import os.path
import re
import sys
import time
import subprocess

from optparse import OptionParser

import asf.helper

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

def svn_user_options(svnuser, svnpass):
	cmd = []
	if svnuser:
		cmd.append("--username=" + svnuser)
		cmd.append("--password=" + svnpass)
		cmd.append("--non-interactive")
		cmd.append("--no-auth-cache")

	return cmd

def get_files_from_cmd(cmd):
		result_filename = "from_cmd.txt"
		result_lines = run_command(cmd, result_filename)

		modified = re.compile("^[AMD]\s+([^\s]+)")

		# Build the file list
		file_list = []
		for line in result_lines:
				match = modified.search(str(line).lstrip())
				if match:
					filename = match.group(1)
					if os.path.isfile(filename):
						debug("DETECT %s" % filename)
						file_list.append(filename)
					else:
						debug("Not found %s" % filename)
		return file_list

def get_files_fromsvnrevision(path, svnuser, revision=0):
	cmd = ["svn", "diff", "-r" + str(revision) + ":HEAD", "--summarize"]
	cmd.extend(svnuser)
	cmd.append(path)
	return get_files_from_cmd(cmd)

def get_files_fromsvnstatus(path, svnuser):
	cmd = ["svn", "status"]
	cmd.extend(svnuser)
	cmd.append(path)
	return get_files_from_cmd(cmd)

def get_files_fromsvncommit(path, svnuser):
	"""
	Return a modified file list (since branch creation)
	"""
	# Run svn 
	revision = re.compile("^r([^\s\|]+)")
	file_list = []

	cmd = ["svn", "log", "--stop-on-copy"]
	cmd.extend(svnuser)
	cmd.append(path)

	result_lines = run_command(cmd, "fromsvncommit.txt")

	ref_revision = 0
	for line in result_lines:
			match = revision.search(str(line))
			if match:
					ref_revision = match.group(1)

	# Check if revision is found
	if (ref_revision == 0):
			print "Cannot find SVN revision to use"
			return file_list

	debug("Found revision", ref_revision)

	# Build the file list
	file_list = get_files_fromsvnrevision(path, svnuser, ref_revision)

	return file_list

if __name__ == "__main__":

	start_time = time.clock()
	parser = OptionParser(usage=
			 """<options>

			Example:
			python svn_parse.py --fromsvncommit
			 """)
	 # General otions
	parser.add_option("--fromsvnrevision", action="store", dest="fromsvnrevision", default=0, help="Get files list from svn revision modifications")
	parser.add_option("--fromsvncommit", action="store_true", dest="fromsvncommit", default=False, help="Get files list from svn commit modifications")
	parser.add_option("--fromsvnstatus", action="store_true", dest="fromsvnstatus", default=False, help="Get files list from svn status modifications")
	default_filename="svn_parse_output.txt"
	parser.add_option("--outfile", action="store", dest="outfile", default=default_filename, help="Filename for storing result. Default: " + default_filename)
	parser.add_option("--svnuser", action="store", dest="svnuser", default='', help="Specify svn user for authentification")
	parser.add_option("--svnpass", action="store", dest="svnpass", default='', help="Specify svn password for authentification")
	parser.add_option("-d", "--debug", action="store_true", dest="debug", default=False, help="Enable debug output")

	(options, args) = parser.parse_args()

	if ((len(args) < 1)
	and (options.fromsvnrevision == 0)
	and (options.fromsvncommit == False)
	and (options.fromsvnstatus == False)
	and (options.all == False)):
		parser.error("Incorrect number of arguments or usage")

	path = "."

	debug_enabled = options.debug

	svnuser = []
	if options.svnuser:
		svnuser = svn_user_options(options.svnuser, options.svnpass)

	files = []
	try:
		if options.fromsvnrevision:
			files = get_files_fromsvnrevision(path, svnuser, options.fromsvnrevision)
		elif options.fromsvnstatus:
			files = get_files_fromsvnstatus(path, svnuser)
		else:
			files = get_files_fromsvncommit(path, svnuser)
	except Exception as e:
		print "Error: ", e
		sys.exit(1)

	result_file = open(options.outfile,'w')
	for filename in files:
		result_file.write(filename + "\n")
	result_file.close()
	debug("Written result to '%s'" % (options.outfile))
