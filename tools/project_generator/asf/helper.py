import re
import subprocess


def using_ironpython():
	try:
		import clr
	except ImportError:
		return False
	else:
		return True


def indent_xml(elem, level=0):
	""" Indent all ElementTree nodes by adding whitespace to XML tree """

	i = "\n" + level*"\t"
	if len(elem):
		if not elem.text or not elem.text.strip():
			elem.text = i + "\t"
		if not elem.tail or not elem.tail.strip():
			elem.tail = i
		for elem in elem:
			indent_xml(elem, level+1)
		if not elem.tail or not elem.tail.strip():
			elem.tail = i
	else:
		if level and (not elem.tail or not elem.tail.strip()):
			elem.tail = i

def strip_xml(elem):
	""" Remove all whitespace from Elementree nodes using strip() function"""
	if elem.text:
		elem.text = elem.text.strip()
	if elem.tail:
		elem.tail = elem.tail.strip()

	if len(elem):
		for e in elem:
			strip_xml(e)

def visualize_xml(element, level=0):
	""" Visualize all ElementTree nodes by printing them to screen """

	output = '%s<%s>' % ( '| '*level, element.tag )
	output = output.ljust(30)

	attr = "attributes: "
	for key, value in element.attrib.items():
		attr += "%s=%s, " % (key, value)

	print output, attr[:80]
	for child in element:
		visualize_xml(child, level+1)

def dump_to_file(variable, filename):
	if using_ironpython():
		import pickle
	else:
		import cPickle as pickle

	f = file(filename, 'wb')
	pickle.dump(variable, f, True)
	f.close()

def load_from_file(filename):
	if using_ironpython():
		import pickle
	else:
		import cPickle as pickle
	f = file(filename, 'rb')
	v = pickle.load(f)
	f.close()
	return v

def count_duplicates(list):
	"""
	This function counts duplicates in a list and returns them as a dictionary
	with the string as key and count as value
	"""
	count = dict()
	for item in list:
		count[item] = count.get(item, 0) + 1

	return count

def run_command(cmd):

	try:
		process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		process.wait()

		lines = process.communicate()

		return lines
	except:
		return ""

def find_svn_revision():
	commands = ["svn info", "git svn info"]
	revision_re = re.compile("Revision: ([0-9]+)", re.MULTILINE)

	for command in commands:
		output = run_command(command)

		output_str = str(output)

		match = revision_re.search(output_str)

		if match:
			return match.group(1)

	return ""

def trunc_string(str, length, postfix=".."):
	"""
	Truncate a string to "length" characters.

	If string is longer than "length" characters, a postfix is inserted to show that the string is shortened:
	example:
	trunc_string("as", 2) -> ".."
	trunc_string("asdf", 4) -> "asdf"
	trunc_string("asdf", 5) -> "asdf"
	trunc_string("asdfgh",5) ->  "asd.."
	trunc_string(None, 5) -> None
	"""

	try:
		return (str[:(length - len(postfix))] + postfix) if len(str) > length else str
	except TypeError:
		return str

def uniquify_list(in_list):
	"""
	Remove duplicates in list, preserving the order.
	"""
	in_set = set(in_list)
	out_list = [x for x in in_list if x in in_set and not in_set.remove(x)]
	return out_list
