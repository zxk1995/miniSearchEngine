 ///
 /// @file    WordSegmentation.hpp
 /// @author  zxk666(1332971744@qq.com)
 /// @date    2018-08-22 15:43:06
 ///

#ifndef __ZXK666_WORDSEGMENTATION_H__
#define __ZXK666_WORDSEGMENTATION_H__


#include "cppjieba/Jieba.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace zxk666
{

const char * const DICT_PATH = "/home/zxk666/cppjieba/dict/jieba.dict.utf8";
const char * const HMM_PATH = "/home/zxk666/cppjieba/dict/hmm_model.utf8";
const char * const USER_DICT_PATH = "/home/zxk666/cppjieba/dict/user.dict.utf8";


class WordSegmentation
{
public:
	WordSegmentation()
	: _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH)
	{
		cout << "cppjieba init!" << endl;
	}


	vector<string> operator()(const char * src)
	{
		vector<string> words;
		_jieba.CutAll(src, words);//cut with hmm
		return words;
	}

private:
	cppjieba::Jieba _jieba;
};

}// end of namespace zxk666


#endif
