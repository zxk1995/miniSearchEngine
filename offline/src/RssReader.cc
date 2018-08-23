 ///
 /// @file    RssReader.cc
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 15:03:49
 ///
 
#include "RssReader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>

using std::cout;
using std::endl;
using std::ostringstream;
using std::ofstream;

using namespace zxk666;

RssReader::RssReader(vector<string> & files)
: _files(files)
{}

void RssReader::loadFeedFiles()
{
	for(size_t idx = 0; idx != _files.size(); ++idx)
	{
		loadFeedFiles(_files[idx]);
		cout << "has load " << idx + 1 << " rss files" << endl;
	}
}

void RssReader::loadFeedFiles(const string & filename)
{
	XMLDocument doc;
	doc.LoadFile(filename.c_str());
	cout << "loadFile: " << filename << endl;
	XMLError errorID = doc.ErrorID();
	if(errorID)
	{
		cout << "XMLDocument LoadFile error" << endl;
		return ;
	}
	else
	{
		parseRss(doc);
	}
}

void RssReader::makePages(vector<string> & pages)
{
	cout << "pages's size = " << _items.size() << endl;
	for(size_t idx = 0; idx != _items.size(); ++idx)
	{
		ostringstream oss;
		oss << "<doc>\n"
			<< " <docid>" << (idx + 1) << "</docid>\n"
			<< " <title>" << _items[idx]->_title << "</title>\n"
			<< " <link>" << _items[idx]->_link << "</link>\n"
			<< " <content>" << _items[idx]->_content << "</content>\n"
			<< "/<doc>\n";

		string page = oss.str();
		pages.push_back(page);
	}
}

void RssReader::parseRss(XMLDocument & doc)
{
	cout << "parsRss(XMLDocument & doc)" << endl;
	XMLElement * root = doc.FirstChildElement();
	XMLElement * channel = root->FirstChildElement("channel");	
	XMLElement * item = channel->FirstChildElement("item");
	for(; item; item = item->NextSiblingElement())
	{
		shared_ptr<RssItem> rssItemPtr(new RssItem);
		const char * itemTitle = item->FirstChildElement("title")->GetText();
		const char * itemLink = item->FirstChildElement("link")->GetText();
		const char * itemDescription = item->FirstChildElement("description")->GetText();
		const char * iContent = nullptr;
		XMLElement * contentEncoded = item->FirstChildElement("content:encoded");
		if(contentEncoded)
		{
			iContent = contentEncoded->GetText();
		}
		else
		{
			iContent = itemDescription;
		}
		string szReg = "<.*?>";
		boost::regex fmt(szReg);
		string content = boost::regex_replace(string(iContent), fmt, string(""));
		rssItemPtr->_title = itemTitle ? itemTitle : string("");
		rssItemPtr->_link = itemLink ? itemLink : string("");
		rssItemPtr->_content = content;
		_items.push_back(rssItemPtr);
	}
}


