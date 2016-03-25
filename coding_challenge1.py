import xml.dom.minidom
import copy
import sys
string = "<p><b><i>Some random test</i> <u>string code</u></b></p>"
input_str = xml.dom.minidom.parseString(string)
input_str = input_str.childNodes
Output = ''
gstack = []
INLINE = ['a','abbr','acronym','b','baseofont','bdo','big','br','cite','code','dfn','em','font',
'i','img','input','kbd','label','q','s','samp','select','small','span','strike','strong','sub',
'sup','textarea','tt','u','var']
def check_inline(tag):
	if tag in INLINE:
		return tag
	return False

def recurse(stack,obj):
	global Output
	#print obj, len(obj.childNodes)
	if obj.nodeName == "#text":
		if obj.data == ' ':
			return
		# Text node
		strings = obj.data.split(" ")
		for sub in strings:
			if stack:
				local_string = "[{"+''.join(stack)+"}]"+sub
			else:
				local_string = sub+" "
			Output += local_string
		# print local_string
	else:
		c = copy.deepcopy(stack)
		if check_inline(obj.nodeName):
			c.append('<'+obj.nodeName+'>')
		for child in obj.childNodes:
			recurse(c,child)

for i in input_str:
	recurse(gstack, i)

print Output
