 ///
 /// @file    DirScanner.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 13:52:57
 ///
 
#include "DirScanner.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

using std::ifstream;
using std::cout;
using std::endl;
using namespace zxk666;

DirScanner::DirScanner(Configuration & conf)
: _conf(conf)
{
	map<string, string> & configmap = _conf.getConfigMap();
	_conf.debug();
	string str = configmap["yuliao"];
	cout << str << endl;
	traverse(str);
}

vector<string> DirScanner::files()
{
	return _vecFilesfiles;
}

void DirScanner::traverse(const string & dirname)
{
	DIR * dir = ::opendir(dirname.c_str());
	if(NULL == dir)
	{
		printf("dir open error\n");
		exit(EXIT_FAILURE);
	}
	::chdir(dirname.c_str());
	struct dirent * mydirent;
	struct stat mystat;
	while(NULL != (mydirent = ::readdir(dir)))
	{
		::stat(mydirent->d_name, &mystat);
		if(S_ISDIR(mystat.st_mode))
		{
			if(0 == ::strncmp(mydirent->d_name, ".", 1) || 0 == ::strncmp(mydirent->d_name, ".." , 2))
			{
				continue;
			}
			else
			{
				traverse(mydirent->d_name);
			}
		}
		else
		{
			string filename;
			filename.append(::getcwd(NULL, 0)).append("/").append(mydirent->d_name);
			_vecFilesfiles.push_back(filename);
		}
	}
	::chdir("..");
	::closedir(dir);
}

