 ///
 /// @file    Configuration.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 13:12:30
 ///

 
#include "Configuration.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ifstream;

using namespace zxk666;

Configuration::Configuration(const string & filepath)
{
	ifstream ifs(filepath);
	if(!ifs.good())
	{
		cout << "config file open error" << endl;
		exit(EXIT_FAILURE);
	}
	string str1;
	string str2;
	while(ifs >> str1 >> str2)
	{
		_configMap.insert(std::make_pair(str1, str2));
	}
	ifs.close();

	ifstream ifs1("../data/stop_words_en.txt");
	while(ifs1 >> str1)
	{
		_stopWordList.insert(str1);
	}
	
	ifstream ifs2("../data/stop_words_cn.txt");
	while(ifs2 >> str2)
	{
		_stopWordList.insert(str2);
	}
}

map<string, string> & Configuration::getConfigMap()
{
	return _configMap;
}

set<string> & Configuration::getStopWordList()
{
	return _stopWordList;
}

void Configuration::debug()
{
	for(auto conf : _configMap)
	{
		cout << conf.first << "\t" << conf.second << endl;
	}
}
