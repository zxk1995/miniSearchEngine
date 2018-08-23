 ///
 /// @file    PageLib.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-21 22:57:06
 ///
 
#ifndef __ZXK666_PAGELIB_H__
#define __ZXK666_PAGELIB_H__

#include "Configuration.h"
#include "DirScanner.h"

#include <map>
#include <vector>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::pair;


namespace zxk666
{

class Configuration;
class DirScanner;

class PageLib
{
public:
	PageLib(Configuration& conf, DirScanner& dirScanner);
	void create();
	void store();

private:
	Configuration & _conf;
	DirScanner & _dirScanner;
	vector<string> _vecPages;
	map<int, pair<int, int> > _offsetLib;
};

}//end of namespace zxk666

#endif
