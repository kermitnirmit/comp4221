import xml.etree.ElementTree as ET
import collections
from collections import OrderedDict
# print "hello"
allwords = []
wordspos = {}
poslist = []
trainwords = set()
other = ET.parse('traindata.xml')
otheroot = other.getroot()
for child in otheroot:
    for a in child:
        for word in a:
            trainwords.add(word.text)




tree = ET.parse('traindata.xml')
root = tree.getroot()
for child in root:
    for a in child:
        for word in a:
            # print(word.text, word.get('value'))
            allwords.append(word.text)
            pos = word.get('value')
            poslist.append(pos)
            # wordspos[word.text] = pos
            # print (wordspos[word.text])
            if wordspos.get(word.text) is None:
                wordspos[word.text] = []
                # wordspos[word.text].add(pos)
                wordspos[word.text].append(pos)
                
            else:
                wordspos[word.text].append(pos)
                # print(wordspos[word.text])
counter=collections.Counter(allwords)
countertwo = collections.Counter(poslist)
# print(counter.most_common(10))
# print(countertwo.most_common(10))
totalwords = 0
posdict = {}
for k in wordspos:
    if k == "worry":
        totalwords = len(wordspos[k])
        for tag in wordspos[k]:
            if posdict.get(tag) is None:
                posdict[tag] = 1
            else:
                posdict[tag] = posdict[tag] + 1

# print others

for tag in posdict:
    print tag, posdict[tag], round((float(posdict[tag]) / totalwords * 100), 2)
print totalwords
        


