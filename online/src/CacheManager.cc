 ///
 /// @file    CacheManager.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-07-22 13:26:32
 ///
 
#include "CacheManager.h"

using namespace zxk666;

void CacheManager::initCache()
{
	cout << "initCache" << endl;
	Cache mainCache;
	mainCache.readFromFile(_cacheDiskPath);
	_cacheList.push_back(mainCache);
	cout << "The main Cache was initialize." << endl;
}

Cache & CacheManager::getCache()
{
	for(auto & item : _Thread2Cache)
	{
		cout << "_Thread2Cache" << endl;
		cout << item.first << endl;
	}
	cout << "查找的thread_self = " << pthread_self() << endl;
	auto it = _Thread2Cache.find(pthread_self());
	if(it == _Thread2Cache.cend())
	{
		_Thread2Cache.insert(make_pair(pthread_self(), _cacheList.size()));
		Cache newCache;
		newCache.readFromFile(_cacheDiskPath);
		_cacheList.push_back(newCache);
		it = _Thread2Cache.find(pthread_self());
		cout << "A new Cache create." << endl;
	}
	for(auto & item : _Thread2Cache)
	{
		cout << "_Thread2Cache" << endl;
		cout << item.first << endl;
	}
	return _cacheList[it->second];
}

void CacheManager::periodcUpdate()
{
	size_t updated = 0;
	for(auto &c : _cacheList)
	{
		c.setUpdateTag();
		c.waitSet();
		updated += c._newCacheCnt;
		cout << c._newCacheCnt << endl;
	}
	cout << "-----------------------------------Cache Update Start---------------------------------" << endl;
	int total = updated < _cacheList[0]._size ? updated : _cacheList[0]._size;//更新数目
	cout << "需要更新缓存条数:" << total << endl;
	if(total == 0)
	{
		cout << "没有缓存需要更新" << endl;
		cout << "-----------------------------------Cache Update end----------------------------------" << endl;
		return;
	}
	int ret = total;
	int seq = 0;
	while(total)
	{
		for(auto & c : _cacheList)
		{
			if(c._newCacheCnt == 0)
				continue;
			else{
				int in_seq = seq;
				Node* nodePending = c._head;
				while(in_seq--)
				{
					nodePending = nodePending->next;
					cout << nodePending << endl;
				}
				cout << nodePending->_query << " " << nodePending->_jsonfile << endl;
				string getFromMain = _cacheList[0].get(nodePending->_query);
				if(getFromMain.size() == 0)//不在主缓存中
				{
					_cacheList[0].removeUpdateTag();
					_cacheList[0].set(nodePending->_query, nodePending->_jsonfile);
					_cacheList[0].setUpdateTag();
				}
				--c._newCacheCnt;
				--total;
				if(0 == total)
				{
					break;
				}
			}
		}
		++seq;
	}
	for(auto & c : _cacheList)
	{
		c._newCacheCnt = 0;
		c.removeUpdateTag();
	}
	_cacheList[0].writeToFile(_cacheDiskPath);
	string logwarn = to_string(ret) + " cache on disk updated.";
	LogWarn(logwarn);
	cout << "-----------------------------------Cache Updated end-------------------------" << endl;
}

