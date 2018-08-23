 ///
 /// @file    PageLib.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 14:27:34
 ///

#include "PageLib.h"
#include "RssReader.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ofstream;

using namespace zxk666;

PageLib::PageLib(Configuration & conf, DirScanner & dirScanner)
: _conf(conf)
, _dirScanner(dirScanner)
{}

void PageLib::create()
{
	vector<string> vecFiles = _dirScanner.files();
	RssReader reader(vecFiles);
	reader.loadFeedFiles();
	reader.makePages(_vecPages);
}

void PageLib::store()
{
	map<string, string> & confmap = _conf.getConfigMap();
	string pageLibPath = confmap["ripepagelib"];
	string offsetLibPath = confmap["offsetlib"];
	cout << pageLibPath << "\t" << offsetLibPath << endl;

	ofstream ofsPage(pageLibPath.c_str());
	ofstream ofsOffset(offsetLibPath.c_str());
	if(!ofsPage.good() && !ofsOffset.good())
	{
		cout << "ofstream open error" << endl;
		exit(EXIT_FAILURE);
	}

	for(size_t idx = 0; idx != _vecPages.size(); ++idx)
	{
		int id = idx + 1;
		int length = _vecPages[idx].size();
		auto offset = ofsPage.tellp();
		ofsPage << _vecPages[idx];
		ofsOffset << id << '\t' << offset << '\t' << length << '\n';
	}
	ofsPage.close();
	ofsOffset.close();
}


