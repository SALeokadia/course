import sys
def getPosList(infs):
	return [s.split("/")[1] for s in infs]
def add2staDict(pos, indx, staDict):
	if pos not in staDict:
		staDict[pos] = [pos, 0, 0, 0]
	staDict[pos][indx] += 1
def add2errDict(mykey, errDict):
	if mykey in errDict:
		errDict[mykey] += 1
	else:
		errDict[mykey]  = 1
def sta(label_file, predict_file, staDict, errDict):
	fdi1 = open(label_file)
	fdi2 = open(predict_file)
	while True:
		line1 = fdi1.readline()
		line2 = fdi2.readline()
		if line1 == "" and line2 == "":
			break
		elif line1 == "" or line2 == "":
			sys.stderr.write("the number of lines is not equal between %s and %s!\n" % (
				label_file, predict_file))
			sys.exit(-1)
		else:
			labelList = getPosList(line1.strip().split())
			predictList = getPosList(line2.strip().split())
			if len(labelList) != len(predictList):
				sys.stderr.write("the number of words is not equal between %s and %s!\n" % (
					label_file, predict_file))
				sys.exit(-1)
			else:
				for i in range(len(labelList)):
					label = labelList[i]
					predict = predictList[i]
					add2staDict(label, 1, staDict)
					add2staDict(predict, 2, staDict)
					add2staDict("all", 1, staDict)
					add2staDict("all", 2, staDict)
					if label == predict:
						add2staDict(label, 3, staDict)
						add2staDict("all", 3, staDict)
					else:
						add2errDict("%s-->%s" % (label, predict), errDict)
						add2errDict("all-->all", errDict)
	fdi2.close()
	fdi1.close()
def out(staDict, errDict, outfile):
	staList = list(staDict.values())
	staList.sort(key=lambda infs:(infs[1]), reverse=True)
	errList = list(errDict.items())
	errList.sort(key=lambda infs:(infs[1]), reverse=True)
	fdo = open(outfile, "w")
	total = staList[0][1]
	for pos, nlabel, npredict, nright in staList:
		fdo.write("pos_%s\t%.4f\t%.4f\t%.4f\n" % (pos, 
			nlabel/total, 
			nright/(npredict if npredict > 0 else 100), 
			nright/(nlabel if nlabel > 0 else 100)))
	total = errList[0][1]
	for errKey, num in errList:
		fdo.write("err_%s\t%.4f\n" % (errKey, num/total))
	fdo.close()
try:
	label_file   = sys.argv[1]
	predict_file = sys.argv[2]
	outfile      = sys.argv[3]
except:
	sys.stderr.write("\tpython "+sys.argv[0]+" label_file predict_file outfile\n")
	sys.exit(-1)
staDict = {}
errDict = {}
sta(label_file, predict_file, staDict, errDict)
out(staDict, errDict, outfile)
