 ///
 /// @file    PageLibPreprocessor.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 16:11:13
 ///
 
#include "PageLibPreprocessor.h"

#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>

using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

using namespace zxk666;

PageLibPreprocessor::PageLibPreprocessor(Configuration & conf)
: _conf(conf)
{}

void PageLibPreprocessor::doProcess()
{
	readInfoFromFile();
	time_t t1 = time(NULL);
	cutRedundantPages();//网页去重
	buildInvertIndexTable();//建立倒排索引
	time_t t2 = time(NULL);
	printf("preprocess time: %ld min\n", (t2 -t1)/60);
	storeOnDisk();//写回磁盘
	time_t t3 = time(NULL);
	printf("store time: %ld min\n", (t3 - t2)/60);
}

void PageLibPreprocessor::readInfoFromFile()
{
	map<string, string> confmap = _conf.getConfigMap();
	ifstream ifspage(confmap["ripepagelib"]);
	ifstream ifsoffset(confmap["offsetlib"]);

	if((!ifspage.good()) || (!ifsoffset.good()))
	{
		cout << "page or offset lib open error" << endl;
	}

	string line;
	int docID, docOffset, docLen;
	while(getline(ifsoffset, line))
	{
		stringstream ss(line);
		ss >> docID >> docOffset >> docLen;
		string doc;
		doc.resize(docLen, ' ');
		ifspage.seekg(docOffset, ifspage.beg);
		ifspage.read(&*doc.begin(), docLen);
		WebPage webpage(doc, _conf, _jieba);
		_pageLib.push_back(webpage);
		_offsetLib.insert(std::make_pair(docID, std::make_pair(docOffset, docLen)));
	}
}

void PageLibPreprocessor::cutRedundantPages()
{
	for(size_t i = 0; i != _pageLib.size() - 1; ++i)
	{
		for(size_t j = i + 1; j != _pageLib.size(); ++j)
		{
			if(_pageLib[i] == _pageLib[j])
			{
				_pageLib[j] = _pageLib[_pageLib.size() - 1];
				_pageLib.pop_back();
				--j;
			}
		}
	}
}

void PageLibPreprocessor::buildInvertIndexTable()
{
	for(auto page : _pageLib)
	{
		map<string, int> & wordsMap = page.getWordsMap();
		for(auto wordFreq : wordsMap)
		{
			_invertIndexTable[wordFreq.first].push_back(std::make_pair(page.getDocId(), wordFreq.second));
		}
	}
	//计算每篇文章中词的权重
	map<int,double> weightSum;
	int totalPageNum = _pageLib.size();
	for(auto & item : _invertIndexTable)
	{
		int df = item.second.size();
		double idf = log(static_cast<double>(totalPageNum)/(df + 1)) / log (2);
		for(auto & sitem : item.second)
		{
			double weight = sitem.second * idf;
			weightSum[sitem.first] += pow(weight, 2);
			sitem.second = weight;
		}
	}

	for(auto & item : _invertIndexTable)
	{
		for(auto & sitem : item.second)
		{
			sitem.second = sitem.second / sqrt(weightSum[sitem.first]);
		}
	}

}

void PageLibPreprocessor::storeOnDisk()
{
	sort(_pageLib.begin(), _pageLib.end());

	ofstream ofsPageLib(_conf.getConfigMap()["newpagelib"]);
	ofstream ofsOffset(_conf.getConfigMap()["newoffsetlib"]);
	ofstream ofsInvertIdexLib(_conf.getConfigMap()["invertindexlib"]);

	if(!ofsPageLib.good() || !ofsOffset.good())
	{
		cout << "new page or offset lib ofstream open error" << endl;
	}

	for(auto & page : _pageLib)
	{
		int id = page.getDocId();
		int length = page.getDoc().size();
		ofstream::pos_type offset = ofsPageLib.tellp();
		ofsPageLib << page.getDoc();
		ofsOffset << id << '\t' << offset << '\t' << length << '\n';
	}

	ofsPageLib.close();
	ofsOffset.close();

	if(!ofsInvertIdexLib.good())
	{
		cout << "invert index table ofstream open error" << endl;
	}
	for(auto & item : _invertIndexTable)
	{
		ofsInvertIdexLib << item.first << '\t';
		for(auto & sitem : item.second)
		{
			ofsInvertIdexLib << sitem.first << '\t' << sitem.second << '\t';
		}
		ofsInvertIdexLib << endl;
	}
	ofsInvertIdexLib.close();
}


