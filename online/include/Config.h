 ///
 /// @file    Config.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-07-21 15:58:46
 ///
 
#ifndef __ZXK666_CONFIG_H__
#define __ZXK666_CONFIG_H__

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <set>

using std::set;

using namespace std;

namespace zxk666
{

class Config
{
public:
	map<string, string> & getConfigMap() { return _configMap; }
	set<string> & getStopWordList() { return _stopWordList; }
	void debug();

	Config(const string &);
	~Config() {}
private:
	string _filepath;
	map<string, string> _configMap;
	set<string> _stopWordList;
};

}//end of namespace zxk666

#endif
