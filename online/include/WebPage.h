 ///
 /// @file    WebPage.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-21 23:36:23
 ///
 
#ifndef __ZXK666_WEBPAGE_H__
#define __ZXK666_WEBPAGE_H__

#include "Config.h"
#include "WordSegmentation.h"
#include "cppjieba/Jieba.hpp"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>

using std::vector;
using std::map;
using std::string;
using std::set;
using std::istringstream;

namespace zxk666
{

class WebPage
{
	friend bool operator == (const WebPage & lhs, const WebPage & rhs);
	friend bool operator < (const WebPage & lhs, const WebPage & rhs);
public:
	WebPage(string & doc, Config & config, WordSegmentation & jieba)
	: _doc(doc)
	{
		processDoc(doc, config, jieba);
	}
	WebPage(){}
	int getDocId();
	string getDoc();
	string getTitle() { return _docTitle; }
	string getUrl() { return _docUrl; }
	string summary(const vector<string>& queryword)
	{
		vector<string> summaryVec;
		istringstream iss(_docContent);
		string line;
		while(iss >> line)
		{
			for(auto word : queryword)
			{
				if(line.find(word) != string::npos)
				{
					summaryVec.push_back(line);
					break;
				}
			}
			if(summaryVec.size() >= 5)
				break;
		}
		string summary;
		for(auto line :summaryVec)
		{
			summary.append(line).append("\n");
		}
		return summary;
	}
	map<string, int> & getWordsMap();
	void processDoc(const string & doc, Config & config, WordSegmentation & jieba)
	{
		string docIdHead = "<docid>";
		string docIdTail = "</docid>";
		string docUrlHead = "<link>";
		string docUrlTail = "</link>";
		string docContentHead = "<content>";
		string docContentTail = "</content>";
		string docTitleHead = "<title>";
		string docTitleTail = "</title>";

		int bpos = doc.find(docIdHead);
		int epos = doc.find(docIdTail);
		string docId = doc.substr(bpos + docIdHead.size(), epos - bpos - docIdHead.size());
		_docId = atoi(docId.c_str());
		bpos = doc.find(docTitleHead);
		epos = doc.find(docTitleTail);
		_docTitle = doc.substr(bpos + docIdHead.size(), epos - bpos - docTitleHead.size());
		bpos = doc.find(docContentHead);
		epos = doc.find(docContentTail);
		_docContent = doc.substr(bpos + docIdHead.size(), epos - bpos - docContentHead.size());
		bpos = doc.find(docUrlHead);
		epos = doc.find(docUrlTail);
		_docUrl = doc.substr(bpos + docIdHead.size(), epos - bpos - docUrlHead.size());
		vector<string> wordsVec = jieba(_docContent.c_str());
		set<string> stopWordList = config.getStopWordList();
		calcTopK(wordsVec, TOPK_NUMBER, stopWordList);
	}
	void calcTopK(vector<string> & wordsVec, int k, set<string> & stopWordList);

	const static int TOPK_NUMBER = 20;

private:
	string _doc;
	int _docId;
	string _docTitle;
	string _docUrl;
	string _docContent;
	string _docSummary;
	vector<string> _topWords;
	map<string, int> _wordsMap;
};

#if 0
bool operator == (const WebPage &lhs, const WebPage & rhs)
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

#endif

}//end of namespace zxk666

#endif
