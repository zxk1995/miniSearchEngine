 ///
 /// @file    WebPage.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-21 23:36:23
 ///
 
#ifndef __ZXK666_WEBPAGE_H__
#define __ZXK666_WEBPAGE_H__

#include "Configuration.h"
#include "WordSegmentation.h"

#include <string>
#include <vector>
#include <map>
#include <set>

using std::vector;
using std::map;
using std::string;
using std::set;

namespace zxk666
{

class WebPage
{
	friend bool operator == (const WebPage & lhs, const WebPage & rhs);
	friend bool operator < (const WebPage & lhs, const WebPage & rhs);
public:
	WebPage(string & doc, Configuration & config, WordSegmentation & jieba);
	int getDocId();
	string getDoc();
	map<string, int> & getWordsMap();
	void processDoc(const string & doc, Configuration & config, WordSegmentation &);
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
