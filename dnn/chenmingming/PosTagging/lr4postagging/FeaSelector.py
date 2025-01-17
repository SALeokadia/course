def readSentences(train_file, sentences):
    # 文件转成sentences[[[w,p][w,p][w,p]],[[w,p][w,p][w,p]]]
    fdi = open(train_file, 'r', encoding='utf-8')
    for line in fdi:
        wps = []
        for s in line.strip().split():
            word, pos = s.split("/")
            wps.append([word, pos])
        sentences.append(wps)
    fdi.close()


def getPosDict(sentences, posDict):
    tmpDict = {}
    for wps in sentences:
        for word, pos in wps:
            if pos not in tmpDict:
                tmpDict[pos] = 1
            else:
                tmpDict[pos] += 1
    tmpList = list(tmpDict.items())
    # tempList [('n', 77189), ('v', 59762), ...]
    tmpList.sort(key=lambda infs: infs[1], reverse=True)
    for pos, _ in tmpList:
        posDict[pos] = len(posDict)
    # posDict {'n': 0, 'v': 1, 'w': 2, 'u': 3, 'm': 4, ...}


# 获取词性的对应下标


def sta4one(cfeaDict, total, indx, feaDict):
    for fea in cfeaDict:
        if fea in feaDict:
            feaDict[fea][indx] += 1
        else:
            feaDict[fea] = [0 for i in range(total)]
            feaDict[fea][indx] += 1
    if "__all__" in feaDict:
        feaDict["__all__"][indx] += 1
    else:
        feaDict["__all__"] = [0 for i in range(total)]
        feaDict["__all__"][indx] += 1


def sta(sentences, posDict, feaDict):
    fe = FeaExtractor()
    for wps in sentences:
        for i in range(len(wps)):
            cfeaDict = {}  # {'cw_充满': 1, 'cwsc_充': 1, 'cwec_满': 1, 'p1w_迈向': 1, 's1w_希望': 1}
            fe.getFeaDict(wps, i, cfeaDict)  #
            _, pos = wps[i]
            sta4one(cfeaDict, len(posDict), posDict[pos], feaDict)


def compute4entropy(lst):
    total = sum(lst)
    if total == 0:
        return 0.0
    entropy = 0.0
    for f in lst:
        if f > 0:
            prob = f / total
            entropy -= prob * math.log(prob)
    return entropy


def compute4one(feaDict, fea, entropy4all):
    lst1 = feaDict["__all__"]
    lst2 = feaDict[fea]
    lst3 = [lst1[i] - lst2[i] for i in range(len(lst1))]
    total = sum(lst1)
    tprob = sum(lst2) / total
    _tprob = sum(lst3) / total
    return entropy4all - tprob * compute4entropy(lst2) - _tprob * compute4entropy(lst3)


def compute(feaDict, igDict):
    entropy4all = compute4entropy(feaDict["__all__"])
    for fea in feaDict:
        if sum(feaDict[fea]) > 0 and fea != "__all__":
            igDict[fea] = compute4one(feaDict, fea, entropy4all)


def out1(igDict, fig_file):
    igList = list(igDict.items())
    igList.sort(key=lambda infs: infs[1], reverse=True)
    fdo = open(fig_file, "w", encoding='utf-8')
    for fea, ig in igList:
        fdo.write("%s\t%f\n" % (fea, ig))
    fdo.close()


def out2(posDict, pos_file):
    tmpList = list(posDict.items())
    tmpList.sort(key=lambda infs: infs[1])
    fdo = open(pos_file, "w", encoding='utf-8')
    for pos, indx in tmpList:
        fdo.write("%s\t%d\n" % (pos, indx))
    fdo.close()


import sys
from FeaExtractor import *
import math

try:
    train_file = sys.argv[1]
    fig_file = sys.argv[2]
    pos_file = sys.argv[3]
except:
    sys.stderr.write("\tpython " + sys.argv[0] + " train_file fig_file pos_file\n")
    sys.exit(-1)
sentences = []
readSentences(train_file, sentences)
# 从训练文件读取句子
posDict = {}
getPosDict(sentences, posDict)
# 获取词性的频数，并按频数降序，生成词性映射下标
feaDict = {}
sta(sentences, posDict, feaDict)
# 计数 当前词汇、及其开始字符、结束字符、前字符串、后字符串 在当前pos下的频次+1
igDict = {}
compute(feaDict, igDict)
# 计算特征的熵值
out1(igDict, fig_file)
# 熵值写入文件, 特征很多，选取信息增益比较大的特征作为后序输入
out2(posDict, pos_file)
# pos 与 映射下标写入文件
