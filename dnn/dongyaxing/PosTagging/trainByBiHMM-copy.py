
def add2transDict(pos1, pos2, transDict):
	if pos1 in transDict:
		if pos2 in transDict[pos1]:
			transDict[pos1][pos2] += 1
		else:
			transDict[pos1][pos2]  = 1
	else:
		transDict[pos1] = {pos2:1}
def add2emitDict(pos, word, emitDict):
	if pos in emitDict:
		if word in emitDict[pos]:
			emitDict[pos][word] += 1
		else:
			emitDict[pos][word]  = 1
	else:
		emitDict[pos] = {word:1}
def sta(infile, transDict, emitDict):
	fdi = open(infile)
	for line in fdi:
		infs = line.strip().split()
		wpList = [["__NONE__", "__start__"]] + [s.split("/") for s in infs] + [["__NONE_", "__end__"]]
		for i in range(1, len(wpList)):
			pre_pos = wpList[i-1][1]
			cur_pos = wpList[i][1]
			word    = wpList[i][0]
			if word == "" or cur_pos == "" or pre_pos == "":
				continue
			add2transDict(pre_pos, cur_pos, transDict)
			add2emitDict(cur_pos, word, emitDict)
		add2transDict("__end__", "__end__", transDict)
	fdi.close()
def getPosNumList(transDict):
	pnList = []
	for pos in transDict:
		#if pos == "__start__" or pos == "__end__":
		#	continue
		num = sum(transDict[pos].values())
		pnList.append([pos, num])
	pnList.sort(key=lambda infs:(infs[1]), reverse=True)
	return pnList
def getTotalWordNum(emitDict):
	total_word_num = 0
	for pos in emitDict:
		total_word_num += sum(list(emitDict[pos].values()))
	return total_word_num
def out4model(transDict, emitDict, model_file):
	pnList = getPosNumList(transDict)
	
	# 状态集合
	fdo = open(model_file, "w")
	total = sum([num for pos, num in pnList])
	for pos, num in pnList:
		fdo.write("pos_set\t%s\t%d\t%f\n" % (pos, num, num/total))
	
	# 转移概率
	total_word_num = getTotalWordNum(emitDict)
	for pos1, num1 in pnList:
		if pos1 == "__end__":
			continue
		num1 += len(pnList)-1
		for pos2, _ in pnList:
			if pos2 == "__start__":
				continue
			num2 = 1
			if pos2 in transDict[pos1]:
				num2 = transDict[pos1][pos2] + 1
				fdo.write("trans_prob\t%s\t%s\t%f\n" % (pos1, pos2, math.log(num2/num1)))
			else:
				fdo.write("trans_prob\t%s\t%s\t%f\n" % (pos1, pos2, math.log(1/total_word_num)))
	# 发射概率
	for pos,_ in pnList:
		if pos == "__start__" or pos == "__end__":
			continue
		wnList = list(emitDict[pos].items())
		wnList.sort(key=lambda infs:infs[1], reverse=True)
		num1 = sum([num for _, num in wnList])+len(wnList)+1
		for word, num2 in wnList:
			fdo.write("emit_prob\t%s\t%s\t%f\n" % (pos, word, math.log((num2+1)/num1)))
		fdo.write("emit_prob\t%s\t%s\t%f\n" % (pos, "__NEW__", math.log(1/total_word_num)))
	fdo.close()
import sys
import math
try:
	infile     = sys.argv[1]
	model_file = sys.argv[2]
except:
	sys.stderr.write("\tpython "+sys.argv[0]+" infile model_file\n")
	sys.exit(-1)
transDict = {}
emitDict  = {}
sta(infile, transDict, emitDict)
out4model(transDict, emitDict, model_file)
