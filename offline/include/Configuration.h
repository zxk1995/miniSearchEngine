 ///
 /// @file    Configuration.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-21 23:06:17
 ///
 
#ifndef __ZXK666_CONFIGURATION_H__
#define __ZXK666_CONFIGURATION_H__

#include <string>
#include <map>
#include <set>

using std::set;
using std::map;
using std::string;

namespace zxk666
{

class Configuration
{
public:
	Configuration(const string & filepath);
	map<string, string> & getConfigMap();
	set<string> & getStopWordList();
	void debug();

private:
	string _filepath;
	map<string, string> _configMap;
	set<string> _stopWordList;
};

}//end of namespace zxk666

#endif
