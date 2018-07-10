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
	std::string _str[2];		// �԰뻺����
	int _cur_buf;				// ��ǰ���ڵĻ�����
	int _pointer;				// ��ǰָ��λ��
	bool eof;					// �Ƿ��Ѿ�����
	bool trace_back_flag;		// ���˵�ʱ�򷵻��ϸ�����������Ϊtrue, �´�forward��ʱ�򲻴��ļ����µ�
	void _read_file();			// ���ļ�����ǰ��������ȡ_BUF_SIZE���ַ�
public:
	BUFFER();
	BUFFER(std::string filename);
	~BUFFER();
	bool open(std::string filename);	// ���ļ�, �ɹ����� true
	bool close();						// �ر��ļ�, �ɹ����� true
	char get_forward();			// �����һ���ַ�
	void trace_back();			// ����Ԥ�����ַ�
};

class SCANNER {
private:
	BUFFER * buf;

	std::string temp_token;
	int attr;

	int line_number;

	// ״̬����, ���� final �������� DFA �ս�״̬
	TOKEN st_0();
	// �ؼ��� �� ��ʶ��
	void st_identify_final();
	// �ַ���
	void st_string0();
	void st_string1();
	void st_string_final();
	// �ַ�
	void st_char0();
	void st_char1();
	void st_char2();
	void st_char_final();
	// ���ͣ������ͺ͵�ֽ��
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
	// ����� (��ʱ�����ֱ�ʾ״̬, �п��ٲ鵥��Ӣ��)
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

	// ������������
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