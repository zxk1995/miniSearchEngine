 ///
 /// @file    Config.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-07-21 21:36:22
 ///
 
#include "Config.h"

using namespace zxk666;

Config::Config(const string & path)
{
	//获取配置文件
	ifstream is(path);
	if(!is.good())
	{
		cout << "config file open error" << endl;
		exit(EXIT_FAILURE);
	}
	
	string section; 
	string content;
	while(is >> section >> content)
	{
		_configMap.insert(std::make_pair(section, content));
	}
	is.close();

	//获取停用词集
	ifstream ifsStopWordList(_configMap["stopword"]);
	if(!ifsStopWordList.good())
	{
		cout << "stopword file open error" << endl;
		exit(EXIT_FAILURE);
	}
	string stopword;
	while(ifsStopWordList >> stopword)
	{
		_stopWordList.insert(stopword);
	}
	ifsStopWordList.close();
}

void Config::debug()
{
	for(auto confmap : _configMap)
	{
		cout << confmap.first << "\t" << confmap.second << endl;
	}
	cout << endl;

	for(auto stopword : _stopWordList)
	{
		cout << stopword << endl;
	}
	cout << endl;
}

