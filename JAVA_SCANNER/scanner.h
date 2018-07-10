#pragma once

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "stdafx.h"

class TOKEN {
private:
	std::string _word;
	int _attr;
public:
	TOKEN();
	TOKEN(std::string, int);
	~TOKEN();
	void set_token(std::string _, int __);
	std::string word();
	int attr();
	friend std::ostream & operator<<(std::ostream &out, TOKEN &obj);
};

class BUFFER {
private:
	enum { _BUF_SIZE = 128 };
	std::ifstream _file;
	std::string _str[2];		// 对半缓冲区
	int _cur_buf;				// 当前所在的缓冲区
	int _pointer;				// 当前指针位置
	bool eof;					// 是否已经读完
	bool trace_back_flag;		// 回退的时候返回上个缓冲区设置为true, 下次forward的时候不从文件读新的
	void _read_file();			// 从文件中向当前缓冲区读取_BUF_SIZE个字符
public:
	BUFFER();
	BUFFER(std::string filename);
	~BUFFER();
	bool open(std::string filename);	// 打开文件, 成功返回 true
	bool close();						// 关闭文件, 成功返回 true
	char get_forward();			// 获得下一个字符
	void trace_back();			// 回退预读的字符
};

class SCANNER {
private:
	BUFFER * buf;

	std::string temp_token;
	int attr;

	int line_number;

	// 状态函数, 含有 final 字样的是 DFA 终结状态
	TOKEN st_0();
	// 关键字 和 标识符
	void st_identify_final();
	// 字符串
	void st_string0();
	void st_string1();
	void st_string_final();
	// 字符
	void st_char0();
	void st_char1();
	void st_char2();
	void st_char_final();
	// 整型，浮点型和点分界符
	void st_int_final();
	void st_zero_final();
	void st_dot_final();
	void st_float_final();
	void st_scFloat0();
	void st_scFloat1();
	void st_scFloat_final();
	void st_FFloat_final();
	void st_hex0();
	void st_hex_final();
	void st_oct_final();
	void st_long_final();
	// 运算符 (暂时用数字表示状态, 有空再查单词英文)
	void st01();	// +
	void st02();	// ++
	void st03();	// +=
	void st04();	// -
	void st05();	// --
	void st06();	// -=
	void st07();	// *
	void st08();	// *=
	void st09();	// /
	void st10();	// /=
	void st11();	// //
	void st12();	// /*
	void st13();	// %
	void st14();	// %=
	void st15();	// =
	void st16();	// ==
	void st17();	// >
	void st18();	// >=
	void st19();	// >>
	void st20();	// >>=
	void st21();	// >>>
	void st22();	// >>>=
	void st23();	// <
	void st24();	// <=
	void st25();	// <<
	void st26();	// <<=
	void st27();	// !
	void st28();	// !=
	void st29();	// ^
	void st30();	// ^=
	void st31();	// |
	void st32();	// ||
	void st33();	// |=
	void st34();	// &
	void st35();	// &&
	void st36();	// &=

	// 其他辅助函数
	bool isKeyword(std::string a);
	std::string oct2dec(std::string oct);
	std::string hex2dec(std::string hex);
	std::string scfloat2float(std::string scfloat);
	TOKEN getPreviousToken();
	void error_report();
	void output_result();
public:
	std::vector<TOKEN> tokens;
	SCANNER();
	SCANNER(std::string filename);
	~SCANNER();
	void run();
};

#endif /* _SCANNER_H_ */