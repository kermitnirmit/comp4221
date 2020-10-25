import xml.etree.ElementTree as ET
import collections
# print "hello"
trainwords = set()
testwords = set()
tree = ET.parse('traindata.xml')
root = tree.getroot()
for child in root:
    for a in child:
        for word in a:
            trainwords.add(word.text)
other = ET.parse('devdata.xml')
otheroot = other.getroot()
for child in otheroot:
    for a in child:
        for word in a:
            testwords.add(word.text)
print len(testwords)
print len(trainwords)
print testwords.difference(trainwords)