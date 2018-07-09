#include "scanner.h"

/*********************************************************/
/*
 * TOKEN
 */
TOKEN::TOKEN() {}
TOKEN::TOKEN(std::string _, int __)
{
	_word = _, _attr = __;
}
TOKEN::~TOKEN() {}

void TOKEN::set_token(std::string _, int __)
{
	_word = _, _attr = __;
}

std::string TOKEN::word()
{
	return _word;
}

int TOKEN::attr()
{
	return _attr;
}

std::ostream & operator<<(std::ostream &out, TOKEN &obj)
{
	out << "<" << obj._word << ", 0x" << std::hex << obj._attr << ">";
	return out;
}

/*********************************************************/
/**
 *	BUFFER
 */
BUFFER::BUFFER()
{

}

BUFFER::BUFFER(std::string filename)
{
	if (!open(filename))
	{
		std::cout << "file not exist" << std::endl;
		exit(0);
	}
	_cur_buf = 0;
	_pointer = 0;
	_read_file();
}

BUFFER::~BUFFER()
{

}

void BUFFER::_read_file()
{
	_str[_cur_buf].clear();
	char tp;
	int cnt = 0;
	while (_file.peek() != EOF)
	{
		_file.get(tp);
		_str[_cur_buf].append(1, tp);
		cnt++;
		if (cnt >= _BUF_SIZE)
		{
			return;
		}
	}
	eof = true;
}

bool BUFFER::open(std::string filename)
{
	_file.open(filename);
	return _file.is_open();
}

bool BUFFER::close()
{
	if (_file.is_open())
	{
		_file.close();
		return true;
	}
	else
	{
		return false;
	}
}

char BUFFER::get_forward()
{
	char ret;
	int size = _str[_cur_buf].size();
	if (_pointer < size)
	{
		ret = _str[_cur_buf][_pointer];
		
		/** ָ���ƶ�����һλ, ���������ǰ��������С, 
		 *	���û�����������, ���ò���
		 *	������, ���ж�eof
		 *		���eofΪtrue, �Ͳ����ٴ��ļ������������
		 *		����, ���л�������, ���ļ��ж�ȡ�ַ������������, Ȼ��ָ������
		 */
		_pointer++;
		if (_pointer >= size && !eof)
		{
			_cur_buf = (_cur_buf + 1) % 2;
			if (!trace_back_flag)
			{
				_read_file();
			}
			_pointer = 0;
			trace_back_flag = false;
		}

		return ret;
	}
	else
	{
		return EOF;
	}
}

void BUFFER::trace_back()
{
	--_pointer;
	if (_pointer < 0)
	{
		_cur_buf = (_cur_buf + 1) % 2;
		_pointer = _str[_cur_buf].size() - 1;
		trace_back_flag = true;
	}
	if (_pointer < 0 || _pointer >= _str[_cur_buf].size())
	{
		std::cout << "trace back error" << std::endl;
		exit(0);
	}
}

/*********************************************************/
/*
 * SCANNER
 */
SCANNER::SCANNER() 
{
	attr = 0;
	line_number = 1;
	temp_token = "";
	tokens.clear();
}

SCANNER::SCANNER(std::string filename)
{
	buf = new BUFFER(filename);
	attr = 0;
	line_number = 1;
	temp_token = "";
	tokens.clear();
}

SCANNER::~SCANNER() 
{
	delete buf;
}


TOKEN SCANNER::getPreviousToken()
{
	if (tokens.size() == 0)
	{	
		std::cout << "tokens�ǿյ�, ����getPreviousToken()�����ʲô" << std::endl;
		exit(0);
		return TOKEN("tokens�ǿյ�, ����getPreviousToken()�����ʲô", 0x3f3f3f3f);
	}
	return tokens[tokens.size() - 1];
}

void SCANNER::run()
{
	tokens.clear();

	for (auto i = 0; i < 1e7; i++)
	{
		TOKEN temp = st_0();
		tokens.push_back(temp);
		if (attr == 262)
		{
			int break_point = 0;
		}
		if (attr == 0x100)					// ����������ʱ, ��������
		{
			error_report();
			char tp;
			while (true)
			{
				tp = buf->get_forward();
				if (tp == '\n')
				{
					++line_number;
					break;
				}
				else if (tp == (char)EOF)
				{
					attr = EOF;
					break;
				}
			}
		}
		if (attr == EOF)
		{
			break;
		}
	}

	output_result();
}

void SCANNER::error_report()
{
	std::cout << line_number << " : " << temp_token << std::endl;
}

void SCANNER::output_result()
{
	std::ofstream scan_out;
	scan_out.open("scan_out");
	for (auto i = 0; i < tokens.size(); ++i)
	{
		scan_out << tokens[i] << std::endl;
	}
}

TOKEN SCANNER::st_0()
{
	if (attr == EOF)
		return TOKEN("EOF", attr);

	temp_token = "";
	attr = 0x0;

	char cur = buf->get_forward();
	if (cur == EOF)
	{
		attr = EOF;
		temp_token = "EOF";
	}

	// ��ʶ�� �� �ؼ���
	else if ((cur >= 'a' && cur <= 'z') ||
		(cur >= 'A' && cur <= 'Z') ||
		cur == '$' || cur == '_')
	{
		temp_token.append(1, cur);
		st_identify_final();
	}
	// �ַ���
	else if (cur == '\"')
	{
		st_string0();
	}
	// �ַ�
	else if (cur == '\'')
	{
		st_char0();
	}
	// ���ֳ�����
	else if (cur == '0')
	{
		temp_token.append(1, '0');
		st_zero_final();
	}
	else if (cur >= '1' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_int_final();
	}
	else if (cur == '.')
	{
		temp_token = "0.";
		st_dot_final();
	}
	// ���޷�
	else if (cur == '{')
	{
		temp_token = "{";
		attr = 0x121;
	}
	else if (cur == '}')
	{
		temp_token = "}";
		attr = 0x121;
	}
	else if (cur == '[')
	{
		temp_token = "[";
		attr = 0x11d;
	}
	else if (cur == ']')
	{
		temp_token = "]";
		attr = 0x11d;
	}
	else if (cur == ',')
	{
		temp_token = ",";
		attr = 0x120;
	}
	else if (cur == '(')
	{
		temp_token = "(";
		attr = 0x11d;
	}
	else if (cur == ')')
	{
		temp_token = ")";
		attr = 0x11d;
	}
	else if (cur == ';')
	{
		temp_token = ";";
		attr = 0x122;
	}
	// ����� �� ע��
	else if (cur == '+')
	{
		st01();
	}
	else if (cur == '-')
	{
		st04();
	}
	else if (cur == '*')
	{
		st07();
	}
	else if (cur == '/')
	{
		st09();
	}
	else if (cur == '%')
	{
		st13();
	}
	else if (cur == '=')
	{
		st15();
	}
	else if (cur == '>')
	{
		st17();
	}
	else if (cur == '<')
	{
		st23();
	}
	else if (cur == '!')
	{
		st27();
	}
	else if (cur == '^')
	{
		st29();
	}
	else if (cur == '~')
	{
		temp_token = "~";
		attr = 0x11c;
	}
	else if (cur == '|')
	{
		st31();
	}
	else if (cur == '&')
	{
		st34();
	}
	else if (cur == '?')
	{
		temp_token = "?";
		attr = 0x111;
	}
	else if (cur == ':')
	{
		temp_token = ":";
		attr = 0x111;
	}
	// �ո�س�TAB�ȷָ���, �����Ͳ�����
	else if (cur == ' ')
	{
		attr = 0x102;
		temp_token = "(SPACE)";
	}
	else if (cur == '\n')
	{
		++line_number;
		attr = 0x102;
		temp_token = "(NEXTLINE)";
	}
	else if (cur == '\t')
	{
		attr = 0x102;
		temp_token = "(TAB)";
	}
	// ����
	else
	{
		buf->trace_back();
		temp_token = "UNEXPECTED_CHAR:";
		temp_token.append(1, cur);
		attr = 0x100;
	}
	return TOKEN(temp_token, attr);
}

/*********************************************************/
/*
 * SCANNER
 * + �ؼ���
 * + ��ʶ��
 */
void SCANNER::st_identify_final()
{
	char cur = buf->get_forward();

	// ��ĸ | ���� | '$' | '_'
	if ((cur >= 'a' && cur <= 'z') || (cur >= 'A' && cur <= 'Z')
		|| cur == '$' || cur == '_' || (cur >= '0' && cur <= '9'))
	{
		temp_token.append(1, cur);
		st_identify_final();
	}
	// ����
	else
	{
		buf->trace_back();			// Ϊ���ж� ��ʶ��/�ؼ��� �Ľ�β�����һλ, ���˻�ȥ
		if (isKeyword(temp_token))
		{
			if (temp_token == "true" || temp_token == "false")
			{
				attr = 0x105;
			}
			else
			{
				attr = 0x103;
			}
		}
		else
		{
			attr = 0x104;
		}
	}
}

bool SCANNER::isKeyword(std::string a)
{
	for (auto i = 0; i < 50; i++)
	{
		if (a == key_word[i])
		{
			return true;
		}
	}
	return false;
}

/*********************************************************/
/*
 * SCANNER
 * + �ַ���
 */
void SCANNER::st_string0()
{
	char cur = buf->get_forward();
	if (cur == EOF || cur == '\n')
	{
		buf->trace_back();
		temp_token = "UNCLOSED_CHAR";
		attr = 0x100;
		return;
	}

	// ˫���� '\"'
	else if (cur == '\"')
	{
		st_string_final();
	}
	// ת�� '\\'
	else if (cur == '\\')
	{
		st_string1();
	}
	// ����
	else
	{
		temp_token.append(1, cur);
		st_string0();
	}
}

void SCANNER::st_string1()
{
	char cur = buf->get_forward();

	// \ddd 1 �� 3 λ 8 ������������ʾ���ַ� (ddd)
	if (cur >= '0' && cur <= '7')
	{
		char nex = buf->get_forward();
		if (nex < '0' || nex > '7')
		{
			buf->trace_back();		// Ϊ���ж��Ƿ��� \dd �����һλ
			temp_token.append(1, (char)(cur - '0'));
		}
		else
		{
			char nextnex = buf->get_forward();
			if (nextnex < '0' || nextnex > '7')
			{
				buf->trace_back();	// Ϊ���ж��Ƿ��� \ddd �����һλ
				temp_token.append(1,
					(char)((cur - '0') * 8 + (nex - '0')));
			}
			else
			{
				temp_token.append(1,
					(char)((cur - '0') * 64 + (nex - '0') * 8 + (nextnex - '0')));
			}
		}
	}
	// \uxxxx
	else if (cur == 'u')
	{
		// TODO
	}
	// \'
	else if (cur == '\'')
	{
		temp_token.append(1, '\'');
	}
	// 
	else if (cur == '\\')
	{
		temp_token.append(1, '\\');
	}
	// \r
	else if (cur == 'r')
	{
		temp_token.append(1, '\r');
	}
	// \n
	else if (cur == 'n')
	{
		temp_token.append(1, '\n');
	}
	// \f
	else if (cur == 'f')
	{
		temp_token.append(1, '\f');
	}
	// \t
	else if (cur == 't')
	{
		temp_token.append(1, '\t');
	}
	// \b
	else if (cur == 'b')
	{
		temp_token.append(1, '\b');
	}
	else if (cur == '\"')
	{
		temp_token.append(1, '\"');
	}
	// UNDO ����ת��
	// �����ڵ�ת���ַ�
	else
	{
		buf->trace_back();
		temp_token = "ILLEGAL_zhuan_yi";
		attr = 0x100;
	}
	st_string0();
}

void SCANNER::st_string_final()
{
	attr = 0x109;
}

/*********************************************************/
/*
 * SCANNER
 * + �ַ�
 */
void SCANNER::st_char0()
{
	char cur = buf->get_forward();
	if (cur == EOF)
	{
		buf->trace_back();
		temp_token = "UNCLOSED_CHAR";
		attr = 0x100;
	}

	else if (cur == '\\')
	{
		st_char2();
	}
	else
	{
		temp_token.append(1, cur);
		st_char1();
	}
}

void SCANNER::st_char1()
{
	char cur = buf->get_forward();
	if (cur == '\'')
	{
		st_char_final();
	}
	else
	{
		buf->trace_back();
		temp_token = "UNCLOSED_CHAR";
		attr = 0x100;
	}
}

void SCANNER::st_char2()
{
	// TODO ���� st_string1()
	char cur = buf->get_forward();

	// \ddd 1 �� 3 λ 8 ������������ʾ���ַ� (ddd)
	if (cur >= '0' && cur <= '7')
	{
		char nex = buf->get_forward();
		if (nex < '0' || nex > '7')
		{
			buf->trace_back();		// Ϊ���ж��Ƿ��� \dd �����һλ
			temp_token.append(1, (char)(cur - '0'));
		}
		else
		{
			char nextnex = buf->get_forward();
			if (nextnex < '0' || nextnex > '7')
			{
				buf->trace_back();	// Ϊ���ж��Ƿ��� \ddd �����һλ
				temp_token.append(1,
					(char)((cur - '0') * 8 + (nex - '0')));
			}
			else
			{
				temp_token.append(1,
					(char)((cur - '0') * 64 + (nex - '0') * 8 + (nextnex - '0')));
			}
		}
	}
	// \uxxxx
	else if (cur == 'u')
	{
		// TODO
	}
	// \'
	else if (cur == '\'')
	{
		temp_token.append(1, '\'');
	}
	// 
	else if (cur == '\\')
	{
		temp_token.append(1, '\\');
	}
	// \r
	else if (cur == 'r')
	{
		temp_token.append(1, '\r');
	}
	// \n
	else if (cur == 'n')
	{
		temp_token.append(1, '\n');
	}
	// \f
	else if (cur == 'f')
	{
		temp_token.append(1, '\f');
	}
	// \t
	else if (cur == 't')
	{
		temp_token.append(1, '\t');
	}
	// \b
	else if (cur == 'b')
	{
		temp_token.append(1, '\b');
	}
	else if (cur == '\"')
	{
		temp_token.append(1, '\"');
	}
	// UNDO ����ת��
	// �����ڵ�ת���ַ�
	else
	{
		buf->trace_back();
		temp_token = "ILLEGAL_zhuan_yi";
		attr = 0x100;
	}
	st_char1();
}

void SCANNER::st_char_final()
{
	attr = 0x106;
}

/*********************************************************/
/*
 * SCANNER
 * + ����
 * + ������
 * + �� �ֽ��
 */
std::string SCANNER::oct2dec(std::string oct)
{
	// UNDO
	return oct;
}

std::string SCANNER::hex2dec(std::string hex)
{
	// UNDO
	return hex;
}

std::string SCANNER::scfloat2float(std::string scfloat)
{
	// UNDO
	return scfloat;
}

void SCANNER::st_zero_final()
{
	char cur = buf->get_forward();
	
	if (cur == 'L' || cur == 'l')
	{
		st_long_final();
	}
	else if (cur >= '0' || cur <= '7')
	{
		temp_token.append(1, cur);
		st_oct_final();
	}
	else if (cur == 'X' || cur == 'x')
	{
		temp_token.append(1, 'x');
		st_hex0();
	}
	else if (cur == 'F' || cur == 'f')
	{
		st_FFloat_final();
	}
	else if (cur == '.')
	{
		temp_token.append(1, '.');
		st_float_final();
	}
	else if (cur == '8' || cur == '9')
	{
		buf->trace_back();
		temp_token = "ILLEGAL_OCT";
		attr = 0x100;
	}
	else
	{
		buf->trace_back();
		attr = 0x107;
	}
}

void SCANNER::st_int_final()
{
	char cur = buf->get_forward();
	
	if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_int_final();
	}
	else if (cur == 'E' || cur == 'e')
	{
		temp_token.append(1, 'e');
		st_scFloat0();
	}
	else if (cur == '.')
	{
		temp_token.append(1, '.');
		st_float_final();
	}
	else if (cur == 'L' || cur == 'l')
	{
		st_long_final();
	}
	else
	{
		buf->trace_back();
		attr = 0x107;
	}
}

void SCANNER::st_hex0()
{
	char cur = buf->get_forward();
	if ((cur >= '0' && cur <= '9') ||
		(cur >= 'A' && cur <= 'F') ||
		(cur >= 'a' && cur <= 'f'))
	{
		temp_token.append(1, cur);
		st_hex_final();
	}
	else
	{
		buf->trace_back();
		temp_token = "ILLEGAL_HEX";
		attr = 0x100;
	}
}

void SCANNER::st_hex_final()
{
	char cur = buf->get_forward();
	if ((cur >= '0' && cur <= '9') ||
		(cur >= 'A' && cur <= 'F') ||
		(cur >= 'a' && cur <= 'f'))
	{
		temp_token.append(1, cur);
		st_hex_final();
	}
	else if (cur == 'L' || cur == 'l')
	{
		temp_token = hex2dec(temp_token);
		st_long_final();
	}
	else
	{
		buf->trace_back();
		attr = 0x107;
	}
}

void SCANNER::st_oct_final()
{
	char cur = buf->get_forward();
	if (cur >= '0' && cur <= '7')
	{
		temp_token.append(1, cur);
		st_oct_final();
	}
	else if (cur == 'L' || cur == 'l')
	{
		temp_token = oct2dec(temp_token);
		st_long_final();
	}
	else
	{
		buf->trace_back();
		attr = 0x107;
	}
}

void SCANNER::st_long_final()
{
	attr = 0x107;
}

void SCANNER::st_dot_final()
{
	char cur = buf->get_forward();
	if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_float_final();
	}
	else
	{
		buf->trace_back();
		attr = 0x11d;
	}
}

void SCANNER::st_float_final()
{
	char cur = buf->get_forward();
	if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_float_final();
	}
	else if (cur == 'E' || cur == 'e')
	{
		temp_token.append(1, 'e');
		st_scFloat0();
	}
	else if (cur == 'F' || cur == 'f')
	{
		st_FFloat_final();
	}
	else
	{
		buf->trace_back();
		attr = 0x108;
	}
}

void SCANNER::st_scFloat0()
{
	char cur = buf->get_forward();
	if (cur == '-')
	{
		temp_token.append(1, cur);
		st_scFloat1();
	}
	else if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_scFloat_final();
	}
	else
	{
		buf->trace_back();
		temp_token = "ILLEGAL_SIENCE";
		attr = 0x100;
	}
}

void SCANNER::st_scFloat1()
{
	char cur = buf->get_forward();
	if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_scFloat_final();
	}
	else
	{
		buf->trace_back();
		temp_token = "ILLEGAL_SIENCE";
		attr = 0x100;
	}
}

void SCANNER::st_scFloat_final()
{
	char cur = buf->get_forward();
	if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_scFloat_final();
	}
	else if (cur == 'F' || cur == 'f')
	{
		st_FFloat_final();
	}
	else
	{
		buf->trace_back();
		temp_token = scfloat2float(temp_token);
		attr = 0x108;
	}
}

void SCANNER::st_FFloat_final()
{
	attr = 0x108;
}

/*********************************************************/
/*
 * SCANNER
 * + �����
 * + ע��
 */

/* +
 * �Ӻ� �� ����
 */
void SCANNER::st01()
{
	char cur = buf->get_forward();
	int pre_attr = getPreviousToken().attr();
	if (cur == '+')
	{
		st02();
	}
	else if (cur == '=')
	{
		st03();
	}
	else
	{
		buf->trace_back();
		if (pre_attr != 0x104 && pre_attr != 0x105 &&
			pre_attr != 0x106 && pre_attr != 0x107 &&
			pre_attr != 0x108 && pre_attr != 0x109)
		{
			// ���Ǳ�ʶ��, �ַ�, ����, �ַ���
			attr = 0x11c;
			temp_token = "+";
		}
		else
		{
			attr = 0x11a;
			temp_token = "+";
		}
	}
}
/* ++
 * ���� 1
 */
void SCANNER::st02()
{
	attr = 0x11c;
	temp_token = "++";
}
/* +=
 * �Ӹ�ֵ
 */
void SCANNER::st03()
{
	attr = 0x110;
	temp_token = "+=";
}
/* -
 * �� �� ����
 */
void SCANNER::st04()
{
	char cur = buf->get_forward();
	int pre_attr = getPreviousToken().attr();
	if (cur == '-')
	{
		st05();
	}
	else if (cur == '=')
	{
		st06();
	}
	else
	{
		buf->trace_back();
		if (pre_attr != 0x104 && pre_attr != 0x105 &&
			pre_attr != 0x107 && pre_attr != 0x108)
		{
			// ���Ǳ�ʶ��, ����
			attr = 0x11c;
			temp_token = "-";
		}
		else
		{
			attr = 0x11a;
			temp_token = "-";
		}
	}
}
/* --
 * �Լ� 1
 */
void SCANNER::st05()
{
	attr = 0x11c;
	temp_token = "--";
}
/* -=
 * ����ֵ
 */
void SCANNER::st06()
{
	attr = 0x110;
	temp_token = "+=";
}
/* *
 * ��
 */
void SCANNER::st07()
{
	char cur = buf->get_forward();
	
	if (cur == '=')
	{
		st08();
	}
	else
	{
		buf->trace_back();
		attr = 0x11b;
		temp_token = "*";
	}
}
/* *=
 * �˸�ֵ
 */
void SCANNER::st08()
{
	attr = 0x110;
	temp_token = "*=";
}
/* /
 * �� ��
 */
void SCANNER::st09()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st10();
	}
	else if (cur == '/')
	{
		st11();
	}
	else if (cur == '*')
	{
		st12();
	}
	else
	{
		buf->trace_back();
		attr = 0x11b;
		temp_token = "//";
	}
}
/* /=
 * ����ֵ
 */
void SCANNER::st10()
{
	attr = 0x110;
	temp_token = "/=";
}
/* //
 * ��ע��
 */
void SCANNER::st11()
{
	char cur = buf->get_forward();
	while (true)
	{
		if (cur == '\n' || cur == EOF)
		{
			buf->trace_back();
			break;
		}
		temp_token.append(1, cur);
		cur = buf->get_forward();
	}
	attr = 0x101;
}
/* /*
 * ��ע��
 */
void SCANNER::st12()
{
	char cur = buf->get_forward();
	while (true)
	{
		if (cur == '*')
		{
			char nex = buf->get_forward();
			if (nex == '/')
			{
				break;
			}
			else
			{
				buf->trace_back();
			}
		}
		if (cur == '\n')
		{
			++line_number;
			temp_token.append(1, '\\');
			temp_token.append(1, 'n');
		}
		else
		{
			temp_token.append(1, cur);
		}
		cur = buf->get_forward();
	}
	attr = 0x101;
}
/* %
 * ��ģ mod
 */
void SCANNER::st13()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st14();
	}
	else
	{
		buf->trace_back();
		attr = 0x11b;
		temp_token = "%";
	}
}
/* %=
 * ģ��ֵ
 */
void SCANNER::st14()
{
	attr = 0x110;
	temp_token = "%=";
}
/* =
 * ��ֵ
 */
void SCANNER::st15()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st16();
	}
	else
	{
		buf->trace_back();
		attr = 0x110;
		temp_token = "=";
	}
}
/* ==
 * ����
 */
void SCANNER::st16()
{
	attr = 0x117;
	temp_token = "==";
}
/* >
 * ����
 */
void SCANNER::st17()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st18();
	}
	else if (cur == '>')
	{
		st19();
	}
	else
	{
		buf->trace_back();
		attr = 0x118;
		temp_token = ">";
	}
}
/* >=
 * ���ڵ���
 */
void SCANNER::st18()
{
	attr = 0x118;
	temp_token = ">=";
}
/* >>
 * ����λ
 */
void SCANNER::st19()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st20();
	}
	else if (cur == '>')
	{
		st21();
	}
	else
	{
		buf->trace_back();
		attr = 0x119;
		temp_token = ">>";
	}
}
/* >>=
 * ����λ��ֵ
 */
void SCANNER::st20()
{
	attr = 0x118;
	temp_token = ">>=";
}
/* >>>
 * ���������λ
 */
void SCANNER::st21()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st22();
	}
	else
	{
		buf->trace_back();
		attr = 0x119;
		temp_token = ">>>";
	}
}
/* >>>=
 * ���������λ��ֵ
 */
void SCANNER::st22()
{
	attr = 0x110;
	temp_token = ">>>=";
}
/* <
 * С��
 */
void SCANNER::st23()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st24();
	}
	else if (cur == '<')
	{
		st25();
	}
	else
	{
		buf->trace_back();
		attr = 0x118;
		temp_token = "<";
	}
}
/* <=
 * С�ڵ���
 */
void SCANNER::st24()
{
	attr = 0x119;
	temp_token = "<=";
}
/* <<
 * ����λ
 */
void SCANNER::st25()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st26();
	}
	else
	{
		buf->trace_back();
		attr = 0x119;
		temp_token = "<<";
	}
}
/* <<=
 * ����λ��ֵ
 */
void SCANNER::st26()
{
	attr = 0x110;
	temp_token = "<<=";
}
/* !
 * ��
 */
void SCANNER::st27()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st28();
	}
	else
	{
		buf->trace_back();
		attr = 0x11c;
		temp_token = "!";
	}
}
/* !=
 * ������
 */
void SCANNER::st28()
{
	attr = 0x117;
	temp_token = "!=";
}
/* ^
 * ��λ���
 */
void SCANNER::st29()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st30();
	}
	else
	{
		buf->trace_back();
		attr = 0x115;
		temp_token = "^";
	}
}
/* ^=
 * �Ǹ�ֵ
 */
void SCANNER::st30()
{
	attr = 0x110;
	temp_token = "^=";
}
/* |
 * �߼���
 */
void SCANNER::st31()
{
	char cur = buf->get_forward();
	if (cur == '=')
	{
		st33();
	}
	else if (cur == '|')
	{
		st32();
	}
	else
	{
		buf->trace_back();
		attr = 0x114;
		temp_token = "|";
	}
}
/* ||
 * ��·��
 */
void SCANNER::st32()
{
	attr = 0x112;
	temp_token = "||";
}
/* |=
 * ��ֵ
 */
void SCANNER::st33()
{
	attr = 0x110;
	temp_token = "|=";
}
/* &
 * �߼���
 */
void SCANNER::st34()
{
	char cur = buf->get_forward();
	if (cur == '&')
	{
		st35();
	}
	else if (cur == '=')
	{
		st36();
	}
	else
	{
		buf->trace_back();
		attr = 0x116;
		temp_token = "&";
	}
}
/* &&
 * ��·��
 */
void SCANNER::st35()
{
	attr = 0x113;
	temp_token = "&&";
}
/* &=
 * �븳ֵ
 */
void SCANNER::st36()
{
	attr = 0x110;
	temp_token = "&=";
}