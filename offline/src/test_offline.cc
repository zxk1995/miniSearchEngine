 ///
 /// @file    test_offline.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 22:20:31
 ///
 
#include "Configuration.h"
#include "DirScanner.h"
#include "PageLib.h"
#include "PageLibPreprocessor.h"
#include "WebPage.h"
#include "WordSegmentation.h"

#include <stdio.h>
#include <time.h>

int main(void)
{
	zxk666::Configuration conf("/home/zxk666/miniSearchEngine/offline/conf/my.conf");
	
	zxk666::DirScanner dirScanner(conf);

	zxk666::PageLib pagelib(conf, dirScanner);
	cout << "PageLib create success" << endl;

	time_t t1 = time(NULL);
	pagelib.create();
	pagelib.store();
	time_t t2 = time(NULL);
	printf("lib create and store use time: %ld", t2 -t1);

	zxk666::PageLibPreprocessor libPreprocessor(conf);
	libPreprocessor.doProcess();

	return 0;
}


