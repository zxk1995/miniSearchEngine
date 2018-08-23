 ///
 /// @file    WebPage.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 20:45:20
 ///

#include "WebPage.h"
#include "Config.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::priority_queue;
using std::make_pair;

namespace zxk666
{

struct WordFreqCompare
{
	bool operator()(const pair<string, int> & lhs, const pair<string, int> & rhs)
	{
		if(lhs.second < rhs.second)
			return true;
		else if(lhs.second == rhs.second && lhs.first > rhs.first)
			return true;
		else
			return false;
	}
};

#if 0
WebPage::WebPage(string & doc, Config & config, WordSegmentation & jieba)
: _doc(doc)
, _config(config)
{
	_topWords.reserve(10);
	processDoc(doc, config, jieba);
}

void zxk666::WebPage::processDoc(const string & doc, Config & config, WordSegmentation &jieba)
{
	string docIdHead = "<docid>";
	string docIdTail = "</docid>";
	string docUrlHead = "<link>";
	string docUrlTail = "</link>";
	string docContentHead = "<content>";
	string docContentTail = "</content>";
	string docTitleHead = "<title>";
	string docTitleTail = "</title>";
	//获取ID
	int bpos = doc.find(docIdHead);
	int epos = doc.find(docIdTail);
	string docId = doc.substr(bpos + docIdHead.size(), epos - bpos - docIdHead.size());
	_docId = atoi(docId.c_str());
	//获取title
	bpos = doc.find(docTitleHead);
	epos = doc.find(docTitleTail);
	_docTitle = doc.substr(bpos + docTitleHead.size(), epos - bpos - docTitleHead.size());
	//content
	bpos = doc.find(docContentHead);
	epos = doc.find(docContentTail);
	_docContent = doc.substr(bpos + docContentHead.size(), epos - bpos - docContentHead.size());
	//url
	bpos = doc.find(docUrlHead);
	epos = doc.find(docUrlTail);
	_docUrl = doc.substr(bpos + docUrlHead.size(), epos - bpos - docUrlHead.size());
	//分词
	vector<string> wordsVec = jieba(_docContent.c_str());
	set<string> stopWordList = config.getStopWordList();
	calcTopK(wordsVec, TOPK_NUMBER, stopWordList);
}
#endif

void WebPage::calcTopK(vector<string> &wordsVec, int k, set<string> & stopWordList)
{
	for(auto iter = wordsVec.begin(); iter != wordsVec.end(); ++iter)
	{
		auto sit = stopWordList.find(*iter);
		if(sit == stopWordList.end())
		{
			++_wordsMap[*iter];
		}
	}

	priority_queue<pair<string, int>, vector<pair<string, int> >, WordFreqCompare> wordFreqQue(_wordsMap.begin(), _wordsMap.end());
	while(!wordFreqQue.empty())
	{
		string top = wordFreqQue.top().first;
		wordFreqQue.pop();
		if(top.size() == 1 && (static_cast<unsigned int>(top[0]) == 10 || static_cast<unsigned int>(top[0]) == 13))
		{
			continue;
		}
		_topWords.push_back(top);
		if(_topWords.size() >= static_cast<size_t>(k))
		{
			break;
		}
	}
}

int WebPage::getDocId()
{
	return _docId;
}

string WebPage::getDoc()
{
	return _doc;
}

map<string, int> & WebPage::getWordsMap()
{
	return _wordsMap;
}

#if 1
bool operator == (const WebPage & lhs, const WebPage & rhs)
{
	int commNum = 0;
	auto iter = lhs._topWords.begin();
	for(;iter != lhs._topWords.end(); ++iter)
	{
		commNum += std::count(rhs._topWords.begin(), rhs._topWords.end(), *iter);
	}

	int lhsNum = lhs._topWords.size();
	int rhsNum = rhs._topWords.size();
	int totalNum = lhsNum < rhsNum ? lhsNum : rhsNum;
	
	if(static_cast<double>(commNum) / totalNum > 0.75)
		return true;
	else
		return false;
}

bool operator < (const WebPage & lhs, const WebPage & rhs)
{
	if(lhs._docId < rhs._docId)
		return true;
	else 
		return false;
}

}
#endif
