import xml.etree.ElementTree as ET 
import collections 


predwords = []
pred_freq = {}
wordpos = {}
poslist = [] 
posword = {}
tree = ET.parse('traindata_part_c_iobes.xml')
tree_2 = ET.parse('traindata_postag.xml')
tree_3 = ET.parse('traindata_part_b_iobes.xml')
root = tree.getroot()
root_2 = tree_2.getroot() 
root_3 = tree_3.getroot()

wordschunk = {}
chunkword = {}
for child in root_3: 
    for a in child: 
        chunk = a.get('type')
        train_word = a.text

        if wordschunk.get(a.text) is None:
            wordschunk[a.text] = []
            wordschunk[a.text].append(chunk)
        else:
            wordschunk[a.text].append(chunk)

        if chunkword.get(chunk) is None:
            chunkword[chunk] = []
            chunkword[chunk].append(train_word)
        else:
            chunkword[chunk].append(train_word)


word_pred = {}
for child in root_2: 
    for a in child: 
        pos = a.get('type')
        train_word = a.text

        if posword.get(pos) is None: 
            posword[pos] = {}
            posword[pos][train_word] = 1
        else: 
            if posword[pos].get(train_word) is None: 
                posword[pos][train_word] = 1
            else: 
                posword[pos][train_word] += 1

        if wordpos.get(train_word) is None: 
            wordpos[train_word] = {}
            wordpos[train_word][pos] = 1
        else: 
            if wordpos[train_word].get(pos) is None: 
                wordpos[train_word][pos] = 1
            else: 
                wordpos[train_word][pos] += 1
        
        

dist_freq = {}

for child in root: 
    for a in child: 
        pred_position_str = a.get('pred_position')
        pred_position = int(pred_position_str)
        count = 0
        for word in a: 
            distance = count - pred_position
            arg_type = word.get('type')
            og_arg_type = word.get('type')
            train_word = word.text
            only_arg = arg_type.split('-')
            spec_arg = ""
            if len(only_arg) > 1: 
                spec_arg = only_arg[1]
            else: 
                spec_arg = only_arg[0]
            if dist_freq.get(distance) is None: 
                dist_freq[distance] = {}
                dist_freq[distance][spec_arg] = 1
            else: 
                if dist_freq[distance].get(spec_arg) is None: 
                    dist_freq[distance][spec_arg] = 1
                else: 
                    dist_freq[distance][spec_arg] += 1
                

            if word_pred.get(train_word) is None: 
                word_pred[train_word] = {}
                word_pred[train_word][og_arg_type] = 1
            else: 
                if word_pred[train_word].get(og_arg_type) is None: 
                    word_pred[train_word][og_arg_type] = 1
                else: 
                    word_pred[train_word][og_arg_type] += 1

            if pred_position == count: 
                predwords.append(arg_type)
            count += 1
            if pred_freq.get(arg_type) is None: 
                pred_freq[arg_type] = {}
                pred_freq[arg_type][train_word] = 1
            else: 
                if pred_freq[arg_type].get(train_word) is None: 
                    pred_freq[arg_type][train_word] = 1
                else: 
                    pred_freq[arg_type][train_word] += 1
            count += 1
        

# print "word_pred: ", word_pred     
print "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------"   
# print "dist_freq: ", dist_freq
# print "pred_freq: ", pred_freq 
# for key_val in pred_freq: 
#     print(key_val, sorted(pred_freq[key_val].items(), key = 
#              lambda kv:(kv[0], kv[1])))  
print "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------"   

# print "wordpos: ", wordpos

countdict = {}
totalwords_2 = 0
allwords = []
allwords_2 = []

for k in wordpos: 
    if k == "by": 
        print k, wordpos[k]
        for tag in wordpos[k]:
            if tag in posword: 
                for words in posword[tag]:
                    if words in word_pred: 
                        for tag_2 in word_pred[words]:
                            allwords.append(tag_2)
        if k in wordschunk:
            for tag in wordschunk[k]:
                # print "tag: ", tag 
                visited = set()
                for words in chunkword[tag]: 
                    if words in word_pred and words not in visited:
                        visited.add(words)
                        for tag_2 in word_pred[words]:
                            allwords_2.append(tag_2)

sumcounter = 0
visited_3 = set()
for tag in allwords: 
    if tag not in visited_3: 
        visited_3.add(tag)
        sumcounter += round((float(allwords.count(tag)) / len(allwords) * 100), 2)
        print tag, round((float(allwords.count(tag)) / len(allwords) * 100), 2)

print "------------------"

newcountlist = []
sumcounter = 0
visited_2 = set()
for tag in allwords_2: 
    if tag not in visited_2: 
        visited_2.add(tag)
        sumcounter += round((float(allwords_2.count(tag)) / len(allwords_2) * 100), 2)
        print tag, round((float(allwords_2.count(tag)) / len(allwords_2) * 100), 2)



print "sumcounter: ", sumcounter
print "--------------"

counter = collections.Counter(predwords)
print(counter.most_common(50))

list_2 = []
word_pred_dict_2 = {}
totalwords_3 = 0
for k in word_pred: 
    if k == "by":
        print word_pred[k]
        for tag in word_pred[k]:
            list_2.append(tag)
        # print k, word_pred[k]
        # totalwords_3 += len(word_pred[k])
        # for tag in word_pred[k]: 
        #     if word_pred_dict_2.get(tag) is None: 
        #         word_pred_dict_2[tag] = 1
        #     else: 
        #         word_pred_dict_2[tag] += 1

counter = collections.Counter(list_2)
# print("word_pred: ", counter.most_common(10))


# for tag in word_pred_dict_2: 
#         print tag, word_pred_dict_2[tag], round((float(word_pred_dict_2[tag]) / totalwords_3 * 100), 2)