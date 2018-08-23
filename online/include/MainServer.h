 ///
 /// @file    MainServer.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-07-21 17:35:32
 ///
 
#ifndef __ZXK666_MAINSERVER_H__
#define __ZXK666_MAINSERVER_H__

#include "CacheManager.h"
#include "TcpServer.h"
#include "Threadpool.h"
#include "Config.h"
#include "TimerThread.h"
#include "Mylog.h"
#include "WordQuery.h"
#include "TcpConnection.h"

#include <iostream>
#include <memory>
#include <string>

void onConnection(const zxk666::TcpConnectionPtr&);
void onMessage(const zxk666::TcpConnectionPtr&);
void onClose(const zxk666::TcpConnectionPtr&);

namespace zxk666
{

class MainServer
{
	friend void onConnection(const zxk666::TcpConnectionPtr&);
	friend void onMessage(const zxk666::TcpConnectionPtr&);
	friend void onClose(const zxk666::TcpConnectionPtr&);

public:
	MainServer(const string & configPath)
	: _config(Config(configPath))
	, _threadpool(1, 1)
	, _tcpserver(_config.getConfigMap()["ip"], (stoi(_config.getConfigMap()["port"])))
	//, _tcpserver("192.168.227.128", 9999)
	, _cacheMgr(_config.getConfigMap()["cachePath"])
	//, _cacheMgr("../data/Cache")
	, _timer(15, 15, bind(&CacheManager::periodcUpdate, &_cacheMgr))
	{}

	void start();
	void stop();

private:
	  Config _config;
	  zxk666::Threadpool _threadpool;
	  zxk666::TcpServer _tcpserver;
	  zxk666::CacheManager _cacheMgr;
	  zxk666::TimerThread _timer;
};

}//end of namespace zxk666

#endif
