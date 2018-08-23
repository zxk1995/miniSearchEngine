 ///
 /// @file    testQuery.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-07-22 14:45:13
 ///
 
#include "MainServer.h"

int main()
{
	zxk666::MainServer core("/home/zxk666/miniSearchEngine/online/conf/my.conf");
	core.start();
	return 0;
}

