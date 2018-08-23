 ///
 /// @file    WordQuery.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-23 03:39:21
 ///
 
#ifndef __ZXK666_WORDQUERY_H__
#define __ZXK666_WORDQUERY_H__

#include "Config.h"
#include "WebPage.h"
#include "CacheManager.h"
#include "TcpConnection.h"
#include "Threadpool.h"
#include "TcpServer.h"
#include "Cache.h"
#include "Mylog.h"
#include <json/json.h>

#include <map>
#include <vector>
#include <string>

using std::vector;
using std::unordered_map;
using std::string;
using std::pair;
using std::make_pair;


namespace zxk666
{

class WordQuery
: public zxk666::Task
{
public:
	WordQuery(string &, TcpConnectionPtr, CacheManager &);
	void process();
	void loadLibrary();
	vector<double> getQueryWordsWeightVector(vector<string>& queryWords);
	bool executeQuery(const vector<string> & quwetWords, vector<pair<int, vector<double>>>& queryWords);
	string createJson(vector<int>&docIdVec, const vector<string>& queryWords);
	string returnNoAnswer();

private:
	TcpConnectionPtr _conn;
	CacheManager _cacheMgr;
	string _queryword;
	WordSegmentation _jieba;
	unordered_map<int, WebPage> _pageLib;
	unordered_map<int, pair<int, int> > _offsetLib;
	unordered_map<string, set<pair<int, double> > > _invertIndexTable;
};

}//end of namespace zxk666


#endif

