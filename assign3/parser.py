import xml.etree.ElementTree as ET
from collections import Counter
# print "hello"
poswords = set()
iobeslist = []
wordsfreq = []
# tree = ET.parse('partb/devdata_part_b_iobes.xml')
tree = ET.parse('traindata_part_b_iobes.xml')
# tree = ET.parse('partb/predictPOS.xml')
tree2 = ET.parse('traindata_postag.xml')

# tree2 = ET.parse('devdata_postag.xml')
root = tree.getroot()

for child in root:
    for word in child:
        # print (word.get('type'))
        wordsfreq.append(word.text)
        poswords.add(word.text)
        # if word.text == "create":
        #     iobeslist.append(word.get('type'))
# print(len(wordsfreq))
print (len(poswords))
wordsfreqw = Counter(wordsfreq)
word1000 = wordsfreqw.most_common(5000)
# print (type(word1000))
# print ("new" in word1000)
c = 0
intheset = False
for word, freq  in word1000:
    if word == "create":
        intheset = True
    if freq == 1:
        c = c+ 1
print(intheset)
print (c)
print (wordsfreqw["create"])
# print (word1000["new"])
root2 = tree2.getroot()
for child in root2:
    for word in child:
        if word.text in poswords:
            iobeslist.append(word.get('type'))
# print(poswords)
# # print (len(wordsfreq))
# # for child in root:
# #     for word in child:
# #         if word.text in poswords:
# #             # print (word.text)
# #             iobeslist.append(word.get('type'))
# print (len(iobeslist))
asg = Counter(iobeslist)
print (asg)


