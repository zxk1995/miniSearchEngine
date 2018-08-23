 ///
 /// @file    WordQuery.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-23 04:10:27
 ///
 
#include "WordQuery.h"
#include "WebPage.h"
#include "Mylog.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <json/json.h>

using std::stringstream;
using std::ifstream;
using std::cout;
using std::endl;

using namespace zxk666;

struct SimilarityCompare
{
	SimilarityCompare(vector<double> & base)
	: _base(base)
	{}

	bool operator()(const pair<int, vector<double>>& lhs, const pair<int, vector<double>>& rhs)
	{
		double lhsCrossProduct = 0;
		double rhsCrossProduct = 0;
		double lhsVectorLength = 0;
		double rhsVectorLength = 0;

		for(size_t idx = 0; idx != _base.size(); ++idx)
		{
			lhsCrossProduct += (lhs.second)[idx] * _base[idx];
			rhsCrossProduct += (rhs.second)[idx] * _base[idx];
			lhsVectorLength += pow((lhs.second)[idx], 2);
			rhsVectorLength += pow((rhs.second)[idx], 2);
		}

		if(lhsCrossProduct / sqrt(lhsVectorLength) < rhsCrossProduct / sqrt(rhsVectorLength))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	vector<double> _base;	
};

WordQuery::WordQuery(string & queryword, TcpConnectionPtr conn, CacheManager & cacheMgr)
: _conn(conn)
, _cacheMgr(cacheMgr)
, _queryword(queryword.substr(0, queryword.size() - 1))
{
	Config _config("/home/zxk666/miniSearchEngine/online/conf/my.conf");
	//ifstream ifspage(_config.getConfigMap()["newpagelib"]);
	//ifstream ifsoffset(_config.getConfigMap()["newoffsetlib"]);
	ifstream ifspage("/home/zxk666/miniSearchEngine/online/data/newpagelib.dat");
	ifstream ifsoffset("/home/zxk666/miniSearchEngine/online/data/newoffsetlib.dat");

	if((!ifspage.good()) || (!ifsoffset.good()))
	{
		cout << "pagelib or offsetlib file open error" << endl;
		exit(EXIT_FAILURE);
	}

	int docId;
	int offset;
	int docLen;
	string line;
	while(getline(ifsoffset, line))
	{
		stringstream ss(line);
		ss >> docId >> offset >> docLen;

		string doc;
		doc.resize(docLen, ' ');
		ifspage.seekg(offset, ifspage.beg);
		ifspage.read(&*doc.begin(), docLen);

		WebPage webpage(doc, _config, _jieba);
		_pageLib.insert(make_pair(docId, webpage));
		_offsetLib.insert(make_pair(docId, make_pair(offset, docLen)));
	}
	ifsoffset.close();
	ifspage.close();

	//ifstream ifsinvertindex(_config.getConfigMap()["invertindexlib"]);
	ifstream ifsinvertindex("/home/zxk666/miniSearchEngine/online/data/invertindex.dat");

	if(!ifsinvertindex.good())
	{
		cout << "invertindexlib file open error" << endl;
		exit(EXIT_FAILURE);
	}
	string word;
	double weight;
	while(getline(ifsinvertindex, line))
	{
		stringstream ss(line);
		ss >> word;
		set<pair<int, double>> id_weihgt;
		while(ss >> docId >> weight)
		{
			id_weihgt.insert(make_pair(docId, weight));
		}
		_invertIndexTable.insert(make_pair(word, id_weihgt));
		id_weihgt.clear();
	}
	ifsinvertindex.close();
}

void WordQuery::process()
{
	//会输入的词进行分词
	cout << "pthread_self = " << pthread_self() << endl;
	Cache * curCache = &_cacheMgr.getCache();
	string res = curCache->get(_queryword);
	if(res.size() > 0)
	{
		cout << "find through Cache" << endl;
		cout << "缓存查询结果:" << res << endl;
		_conn->sendInLoop(res);
		return;
	}
	vector<string> queryword;
	if(_queryword.size() > 0)
	{
		queryword = _jieba(_queryword.c_str());
	}
	
	for(auto item : queryword)
	{
		cout << "关键词:" << item << '\n';
		auto uit = _invertIndexTable.find(item);
		if(uit == _invertIndexTable.end())
		{
			cout << "can not find " << item << endl;
			_conn->sendInLoop(returnNoAnswer());
		}
	}
	cout << endl;

	//计算每个词的权重
	vector<double> weightList = WordQuery::getQueryWordsWeightVector(queryword);
	SimilarityCompare similarityCmp(weightList);

	//执行查询
	vector<pair<int, vector<double> > > resultVec;
	if(executeQuery(queryword, resultVec))
	{
		//对查询结果进行排序
		stable_sort(resultVec.begin(), resultVec.end(), similarityCmp);
		vector<int> docIdVec;
		for(auto item : resultVec)
		{
			docIdVec.push_back(item.first);
		}
		string jsonfile = createJson(docIdVec, queryword);
		cout << jsonfile << endl;
		_conn->sendInLoop(jsonfile);
		curCache->set(_queryword, jsonfile);
	}
	else
	{
		_conn->sendInLoop(returnNoAnswer());
	}
}

string WordQuery::returnNoAnswer()
{
	Json::Value root;
	Json::Value arr;
	Json::Value elem;
	elem["title"] = "404, not found";
	elem["summary"] = "亲, I cannot found what you want. What a pity!";
	elem["url"] = "";
	arr.append(elem);
	root["files"] = arr;
	Json::StyledWriter writer;
	return writer.write(root);
}

vector<double> WordQuery::getQueryWordsWeightVector(vector<string> & queryword)
{
	//计算查询词的权重
	//统计词频
	map<string, int> wordFreqMap;
	for(auto item : queryword)
	{
		++wordFreqMap[item];
	}
	
	//计算权重
	vector<double> weightList;
	double weightSum = 0;
	int totalPageNum = _offsetLib.size();
	for(auto & item : queryword)
	{
		int df = _invertIndexTable[item].size();
		double idf = log(static_cast<double>(totalPageNum) / (df + 1)) / log(2);
		int tf = wordFreqMap[item];
		double w = idf * tf;
		weightSum += pow(w, 2);
		weightList.push_back(w);
	}

	//归一化
	for(auto & item : weightList)
	{
		item /= sqrt(weightSum);
	}
	cout << "weightList's elem: ";
	for(auto item : weightList)
	{
		cout << item << "\t";
	}
	cout << endl;
	return weightList;
}

bool WordQuery::executeQuery(const vector<string>& queryword, vector<pair<int, vector<double>>>& resultVec)
{
	cout << "executeQuery()" << endl;
	if(queryword.size() == 0)
	{
		cout << "empty string not found" << endl;
		return false;
	}
	typedef set<pair<int, double>>::iterator setIter;
	vector<pair<setIter, int>> iterVec;//保存需要求交集的迭代器
	int minIterNum = 0x7FFFFFFF;
	for(auto item : queryword)//找出出现次数最少的查询词
	{
		int sz = _invertIndexTable[item].size();//词item出现次数
		if(sz == 0)
			return false;
		if(minIterNum > sz)
			minIterNum = sz;
		iterVec.push_back(make_pair(_invertIndexTable[item].begin(), 0));//（词item出现的vector头指针，0）
	}
	cout << "资料库中出现次数最少的查询词:" << minIterNum << endl;

	bool isExiting = false;
	while(!isExiting)
	{
		size_t idx = 0;
		for(; idx != iterVec.size() - 1; ++idx)//循环次数为queryword划分词个数
		{
			if((iterVec[idx].first)->first != iterVec[idx + 1].first->first)//docid的比较
				break;
		}
	
		if(idx == iterVec.size() - 1)
		{//找到相同的docid
			vector<double> weightVec;
			int docid = iterVec[0].first->first;
			for(idx = 0; idx != iterVec.size(); ++idx)
			{
				weightVec.push_back(iterVec[idx].first->second);//权重
				++(iterVec[idx].first);//setIter++
				++(iterVec[idx].second);
				if(iterVec[idx].second == minIterNum)
				{
					isExiting = true;
				}
			}
			resultVec.push_back(make_pair(docid, weightVec));
		}
		else
		{//找到最小的docid，并将其所在的迭代器++
			int minDocId = 0x7FFFFFFF;
			int iterIdx;
			for(idx = 0; idx != iterVec.size(); ++idx)
			{
				if(iterVec[idx].first->first < minDocId)
				{
					minDocId = iterVec[idx].first->first;
					iterIdx = idx;
				}
			}

			++(iterVec[iterIdx].first);
			++(iterVec[iterIdx].second);
			if(iterVec[iterIdx].second == minIterNum)
			{
				isExiting = true;
			}
		}
	}
	return true;
}

string WordQuery::createJson(vector<int>& docIdVec, const vector<string> & queryword)
{
	Json::Value root;
	Json::Value arr;

	int cnt = 0;
	for(auto id : docIdVec)
	{
		string summary = _pageLib[id].summary(queryword);
		string title = _pageLib[id].getTitle();
		string url = _pageLib[id].getUrl();

		Json::Value elem;
		elem["title"] = title;
		elem["summary"] = summary;
		elem["url"] = url;
		arr.append(elem);
		if(++cnt == 100)
			break;
	}

	root["files"] = arr;
	Json::StyledWriter writer;
	return writer.write(root);
}

