#include <iostream>
#include <stdlib.h>
#include <string>
#include <queue>
using namespace std;

/************************************************************************/
/* Trie树结构
* 查找字符串
*/
/************************************************************************/
typedef struct TrieNode
{
	char data;
	TrieNode *children[26];
	bool isEndingChar;
	int count;	// 记录此结点被多少个单词占用
	int freq;	// 记录单词被插入多少次
	TrieNode()
	{
		isEndingChar = false;
		for (int i = 0; i < 26; ++i)
		{
			children[i] = NULL;
		}
		count = 0;
		freq = 0;
	}
}TrieNode;

/************************************************************************/
/* Trie类
*/
/************************************************************************/
class Trie
{
public:
	TrieNode *root;		// 根结点

public:
	//构造函数
	Trie()
	{
		root = NULL;
	}

	// 析构
	~Trie()
	{
		delete root;
		root = NULL;
	}
	
	/****************************************************************
	* @brief : 		插入字符串
	* @author : 	dyx
	* @date : 		2019/7/4 15:34
	* @version : 	ver 1.0
	* @inparam :	输入string类型的字符串
	* @outparam : 
	*****************************************************************/
	void insert(string text)
	{
		if (!root)
		{
			root = new TrieNode;
		}
		TrieNode *p = root;
		for (int i = 0; i < text.length(); ++i)
		{
			int index = text[i] - 'a';
			if (p->children[index] == NULL)
			{
				p->children[index] = new TrieNode;
				p->children[index]->data = text[i];
			}
			p->count++;
			p = p->children[index];
		}
		p->count++;
		p->isEndingChar = true;
	}


	/****************************************************************
	* @brief : 		查找是否有此字符串
	* @author : 	dyx
	* @date : 		2019/7/4 15:35
	* @version : 	ver 1.0
	* @inparam :	
	* @outparam :	成功返回true
	*****************************************************************/
	bool find(string pattern)
	{
		TrieNode *p = root;
		for (int i = 0; i < pattern.length(); ++i)
		{
			int index = pattern[i] - 'a';
			if (p->children[index] == NULL)
			{
				return false;
			}
			p = p->children[index];
		}
		return p->isEndingChar;
	}


	/****************************************************************
	* @brief : 		打印所有的字典
	* @author : 	dyx
	* @date : 		2019/7/4 15:36
	* @version : 	ver 1.0
	* @inparam : 
	* @outparam : 
	*****************************************************************/
	void print(TrieNode *Root, string prefix, int &order) const
	{
		if (!Root->isEndingChar)
		{
			for (int i = 0; i < 26; ++i)
			{
				if (Root->children[i] != NULL)
				{
					print(Root->children[i], prefix + (Root->children[i]->data), order);	// 通过prefix+root->children[i]->data, 链接所有单词
				}
			}
		}
		else
			cout << ++order << " " << prefix << endl;// ", frequency: " << Root->freq << endl;
	}
};



int main()
{
	Trie *trie = new Trie();
	trie->insert("are");
	trie->insert("arrg");
	trie->insert("hello");
	trie->insert("her");
	trie->insert("his");
	trie->insert("history");
	cout << trie->find("him") << endl;
	cout << trie->find("his") << endl;
	int id = 0;
	trie->print(trie->root, " ", id);
//	system("pause");
	return 0;
}
