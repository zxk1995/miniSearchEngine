 ///
 /// @file    PageLibPreprocessor.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-21 23:19:09
 ///
 
#ifndef __ZXK666_PAGELIBPREPROCESSOR_H__
#define __ZXK666_PAGELIBPREPROCESSOR_H__

#include "WebPage.h"
#include "Configuration.h"
#include "WordSegmentation.h"

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <string>

using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::map;

namespace zxk666
{

class PageLibPreprocessor
{
	friend class WebPage;
public:
	PageLibPreprocessor(Configuration & conf);
	void doProcess();
	void readInfoFromFile();
	void cutRedundantPages();
	void buildInvertIndexTable();
	void storeOnDisk();

private:
	Configuration & _conf;
	WordSegmentation _jieba;
	vector<WebPage> _pageLib;
	unordered_map<int, pair<int, int> > _offsetLib;
	unordered_map<string, vector<pair<int, double>>> _invertIndexTable;
};

}//end of namespace zxk666

#endif
