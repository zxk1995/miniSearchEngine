 ///
 /// @file    DirScanner.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-21 23:12:01
 ///
 
#ifndef __ZXK666_DIRSCANNER_H__
#define __ZXK666_DIRSCANNER_H__

#include "Configuration.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace zxk666
{

class DirScanner
{
public:
	DirScanner(Configuration & conf);
	//void operator()();
	vector<string> files();
	void traverse(const string & dirname);

private:
	vector<string> _vecFilesfiles;
	Configuration & _conf;
};

}//end of namespace zxk666

#endif
