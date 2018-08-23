 ///
 /// @file    MainServer.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-07-22 05:03:25
 ///
 
#include "MainServer.h"
#include "WordQuery.h"

namespace zxk666
{

Threadpool *g_threadpool = NULL;
CacheManager *g_cacheMgr = NULL;

void onConnection(const zxk666::TcpConnectionPtr & conn)
{
	cout << conn->toString() << endl;
	string loginfo = conn->toString() + " The link was established.";
	LogInfo(loginfo);
	conn->send("Connection Established.");
}

void onMessage(const zxk666::TcpConnectionPtr & conn)
{
	string query(conn->receive());
	WordQuery * ptask(new WordQuery(query, conn, *g_cacheMgr));
	g_threadpool->addTask(ptask);
}

void onClose(const TcpConnectionPtr & conn)
{
	string loginfo = conn->toString() + " close.";
	LogInfo(loginfo);
	cout << conn->toString() << " closed" << endl;
}

void MainServer::start()
{
	LogInfo("Server Startd");
	_cacheMgr.initCache();
	_timer.start();
	g_threadpool = &_threadpool;
	g_cacheMgr = &_cacheMgr;
	_threadpool.start();
	_tcpserver.setConnectionCallback(&zxk666::onConnection);
	_tcpserver.setMessageCallback(&zxk666::onMessage);
	_tcpserver.setCloseCallback(&zxk666::onClose);
	_tcpserver.start();
}

void MainServer::stop()
{
	_tcpserver.stop();
	_threadpool.stop();
}

}
