 ///
 /// @file    RssReader.h
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 14:51:58
 ///
 
#ifndef __ZXK666_RSSREADER_H__
#define __ZXK666_RSSREADER_H__

#include "tinyxml2.h"
#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;
using namespace tinyxml2;

namespace zxk666
{

struct RssItem
{
	string _title;
	string _link;
	string _pubDate;
	string _content;
};

struct RssData
{
	string _version;
	string _title;
	string _ink;
	string _description;
	string _language;
	vector<shared_ptr<RssItem> > _items;
};

class RssReader
{
public:
	RssReader(vector<string> & files);
	void loadFeedFiles();
	void loadFeedFiles(const string & filename);
	void initWithRssString(const string & rss);
	void makePages(vector<string> & pages);

private:
	void parseRss(XMLDocument & doc);

private:
	vector<shared_ptr<RssItem> > _items;
	vector<string> & _files;
};

}//end of namespace zxk666

#endif
