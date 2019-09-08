import xml.etree.ElementTree as et
import os

TEST_XML_PATH = "../../Data/Enviromental_Data/Air_Quality_For_Each_Region.xml"

import xml.etree.ElementTree as et
import os

TEST_XML_PATH = "../../Data/Enviromental_Data/Air_Quality_For_Each_Region.xml"

class xml_loader:
	def __init__(): #Load the XML file
		self._root = et.parse(TEST_XML_PATH).getroot()

	def get_attrib_of(keyword):
		# This will return the attribute (Information) of the given keyword
		pass

	def get_tag_of(keyword):
		# Gets the tag of the given keyword
		pass
