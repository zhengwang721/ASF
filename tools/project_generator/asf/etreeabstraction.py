
use_clr = False

try:
	from lxml.etree import *
except:
	import clr
	clr.AddReference("System.Xml.Linq")
	clr.AddReference("System.Core")
	clr.AddReference("System.Xml")
	from System.Linq import Enumerable
	from System.Xml import XmlNodeType
	from System.Xml.Linq import *
	from System.Xml.XPath import Extensions
	from System.IO import StringWriter
	from System.Xml import NameTable
	from System.Xml import XmlNamespaceManager
	use_clr = True

if use_clr:



	class Attributes(object):
		def __init__(self, xelement):
			self.xelement = xelement

		def __getitem__(self, name):
			attr = self.get_attribute(name)
			if (attr is None):
				raise KeyError(name + ' is not present')
			return attr.Value

		def __setitem__(self, name, value):
			self.xelement.SetAttributeValue(name, value)

		def has_key(self, name):
			return self.get_attribute(name) is not None

		def get_attribute(self, name):
			for attr in self.xelement.Attributes():
				if attr.Name.ToString() == name:
					return attr
			return None

		def get(self, name, default_value):
			attr = self.get_attribute(name)
			if attr is not None:
				return attr.Value
			else:
				return default_value

	class Element(object):
		def __init__(self, tag=None, node=None, attrib={}, **extra):
			if node:
				self.node=node
			else:
				self.node = XElement(XName.Get(self._get_node_name(tag)))
				self.node.Value = ""
				for key, value in attrib.items():
					self.node.SetAttributeValue(key, value)
			self.attributes = Attributes(self.node)

		@staticmethod
		def _get_node_name(tag):
			"""
			if tag is of format [namespace-uri()='uri' and local-name()='name'] , then we need to convert to {uri}name
			"""
			sep = '\''

			start = tag.find(sep, 0)
			end = tag.find(sep, start + 1)

			if (start > 0) and (end > 0):
				uri = tag[start + 1 : end]
			else:
				return tag

			start = tag.find(sep, end + 1)
			end = tag.find(sep, start + 1)

			if (start > 0) and (end > 0):
				name = tag[start + 1 : end]
			else:
				return tag

			return '{%s}%s' % (uri, name)



		def iterchildren(self,tag=None,reversed=False):
			children = []
			if tag:
				children = [Element(node=child) for child in self.node.Elements(XName.Get(tag))]
			else:
				children = [Element(node=child) for child in self.node.Elements()]

			if (reversed):
				children.reverse()

			return children

		@property
		def tag(self):
			return self.node.Name.ToString()

		@property
		def attrib(self):
			return self.attributes

		def getfirsttextnode(self, collection):
			for element in collection:
				if element.NodeType == XmlNodeType.Text and element.Value != '':
					return element
			return None

		@property
		def text(self):
			textnode = self.getfirsttextnode(self.node.Nodes())
			if textnode:
				return textnode.Value
			else:
				return None

		@text.setter
		def text(self, value):
			value = value or ''
			textnode = self.getfirsttextnode(self.node.Nodes())
			if textnode:
				textnode.Value = value
			elif self.node.HasElements == False:
				self.node.Value = value

		@property
		def tail(self):
			textnode = self.getfirsttextnode(self.node.NodesAfterSelf())
			if textnode:
				return textnode.Value
			else:
				return None

		@tail.setter
		def tail(self, value):
			textnode = self.getfirsttextnode(self.node.NodesAfterSelf())
			if textnode:
				textnode.value = value

		def get(self,attrib):
			return self.attributes.get_attribute(attrib)

		def set(self,attrib,value):
			self.node.SetAttributeValue(attrib,value)


		def __iter__(self):
			return (Element(node=e) for e in self.node.Elements())

		def __len__(self):
			return Enumerable.Count(self.node.Elements())

		def append(self, element):
			self.node.Add(element.get_node())

		def findall(self, xpath_expression):
			elements = []

			try:
				selected_elements = Extensions.XPathSelectElements(self.node, xpath_expression)
				elements = [Element(node=element) for element in selected_elements]
			except:
				pass

			return elements


		def find(self, xpath_expression):
			return_element = None
			selected_element = None

			try:
				selected_element = Extensions.XPathSelectElement(self.node, xpath_expression)
			except:
				pass

			if selected_element:
				return_element = Element(node=selected_element)

			return return_element

		def getparent(self):
			return Element(node = self.node.Parent)

		def get_node(self):
			return self.node

		def remove(self,element):
			element.get_node().Remove()

		def getroottree(self):
			return ElementTree(self)



	class ElementTree(object):
		def __init__(self, element=None, file=None, document=None):
			if element is not None:
				self.document = XDocument(element.get_node())
			elif document is None:
				self.document = XDocument.Load(str(file))
			else:
				self.document = document

		def getroot(self):
			return Element(node=self.document.Root)

		def write_c14n(self, file, exclusive=False, with_comments=True, compression=0):
			stringWriter = StringWriter();
			self.document.Save(stringWriter)
			file.write(stringWriter.ToString())
		def write(self,fileName):
			self.document.Save(fileName)

		@staticmethod
		def parse(source, parser=None):
			doc = XDocument.Load(source)
			et = ElementTree(document=doc)
			return et




	class XMLSchema(object):
		def __init__(self, schema):
			pass

		def assertValid(self, tree):
			return True


	def parse(source, parser=None):
		doc = XDocument.Load(source)
		et = ElementTree(document=doc)
		return et

	def fromstring(source):
		doc = XDocument.Parse(source)
		return Element(node=doc.Root)

	def tostring(source):
		return source.getroot().get_node().ToString()


	def SubElement(_parent, _tag, attrib={},**_extra):
		node = Element(_tag,None,attrib)
		if _parent is not None:
			_parent.append(node)
			return node



	class DocumentInvalid(Exception):
		pass
