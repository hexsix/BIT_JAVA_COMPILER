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
	out << "<" << obj._word << ", " << obj._attr << ">" << std::endl;
	return out;
}

/*********************************************************/
/*
 * SCANNER
 */
SCANNER::SCANNER() 
{
	p = 0;
	attr = 0;
	line_number = 1;
	temp_token = "";
	tokens.clear();
}
SCANNER::~SCANNER() {}

void SCANNER::init(std::string _)
{
	buf = _;
	p = 0;
	attr = 0;
	line_number = 1;
	temp_token = "";
	tokens.clear();
}

char SCANNER::getNextChar()
{
	p++;
	if (p >= buf.size())
		return EOF;
	return buf[p];
}

TOKEN SCANNER::getPreviousToken()
{
	if (tokens.size() == 0)
		return TOKEN("乌拉!!!!", 0x3f3f3f3f);
	return tokens[tokens.size() - 1];
}

void SCANNER::run()
{
	tokens.clear();

	while (attr != 0x100 || attr != EOF)
	{
		TOKEN temp = st_0();
		tokens.push_back(temp);
	}

	if (attr == 0x100)
	{
		error_report();
	}
	else
	{
		output_result();
	}
}

TOKEN SCANNER::st_0()
{
	if (attr == 0x100 || attr == EOF)
		return;

	temp_token = "";
	attr = 0x0;

	char cur = getNextChar();
	if (cur == EOF)
	{
		attr = EOF;
		temp_token = "EOF";
	}

	// 标识符 和 关键字
	else if ((cur >= 'a' && cur <= 'z') ||
		(cur >= 'A' && cur <= 'Z') ||
		cur == '$' || cur == '_')
	{
		temp_token.append(1, cur);
		st_identify_final();
	}
	// 字符串
	else if (cur == '\"')
	{
		st_string0();
	}
	// 字符
	else if (cur == '\'')
	{
		st_char0();
	}
	// 数字常量等
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
	// 界限符
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
	// 运算符 和 注释
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
	// 空格回车TAB等分隔符, 其他就不管了
	else if (cur == ' ')
	{
		attr = 0x102;
		temp_token = " ";
	}
	else if (cur == '\n')
	{
		line_number++;
		attr = 0x102;
		temp_token = "\n";
	}
	else if (cur == '\t')
	{
		attr = 0x102;
		temp_token = "TAB";
	}
	// 错误
	else
	{
		temp_token = "错误的单词";
		attr = 0x100;
	}
	return TOKEN(temp_token, attr);
}

/*********************************************************/
/*
 * SCANNER
 * + 关键字
 * + 标识符
 */
void SCANNER::st_identify_final()
{
	char cur = getNextChar();

	// 字母 | 数字 | '$' | '_'
	if ((cur >= 'a' && cur <= 'z') || (cur >= 'A' && cur <= 'Z')
		|| cur == '$' || cur == '_' || (cur >= '0' && cur <= '9'))
	{
		temp_token.append(1, cur);
		st_identify_final();
	}
	// 其他
	else
	{
		p--;			// 为了判断 标识符/关键字 的结尾多读了一位, 回退回去
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
 * + 字符串
 */
void SCANNER::st_string0()
{
	char cur = getNextChar();
	if (cur == EOF || cur == '\n')
	{
		temp_token = "未关闭的字符串";
		attr = 0x100;
		return;
	}

	// 双引号 '\"'
	else if (cur == '\"')
	{
		st_string_final();
	}
	// 转义 '\\'
	else if (cur == '\\')
	{
		st_string1();
	}
	// 其他
	else
	{
		temp_token.append(1, cur);
		st_string0();
	}
}

void SCANNER::st_string1()
{
	char cur = getNextChar();

	// \ddd 1 到 3 位 8 进制数据所表示的字符 (ddd)
	if (cur >= '0' && cur <= '7')
	{
		char nex = getNextChar();
		if (nex < '0' || nex > '7')
		{
			p--;		// 为了判断是否是 \dd 多读了一位
			temp_token.append(1, (char)(cur - '0'));
		}
		else
		{
			char nextnex = getNextChar();
			if (nextnex < '0' || nextnex > '7')
			{
				p--;	// 为了判断是否是 \ddd 多读了一位
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
	// UNDO 其他转义
	// 不存在的转义字符
	else
	{
		temp_token = "非法的转义字符";
		attr = 0x100;
	}
}

void SCANNER::st_string_final()
{
	attr = 0x109;
}

/*********************************************************/
/*
 * SCANNER
 * + 字符
 */
void SCANNER::st_char0()
{
	char cur = getNextChar();
	if (cur == EOF)
	{
		temp_token = "未关闭的字符";
		attr = 0x100;
	}

	else if (cur == '\'')
	{
		st_char_final();
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
	char cur = getNextChar();
	if (cur == '\'')
	{
		st_char_final();
	}
	else
	{
		temp_token = "未关闭的字符";
		attr = 0x100;
	}
}

void SCANNER::st_char2()
{
	// TODO 复制 st_string1()
}

void SCANNER::st_char_final()
{
	attr = 0x106;
}

/*********************************************************/
/*
 * SCANNER
 * + 整型
 * + 浮点型
 * + 点 分界符
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
	char cur = getNextChar();
	
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
		temp_token = "非法的八进制整型";
		attr = 0x100;
	}
	else
	{
		p--;
		attr = 0x107;
	}
}

void SCANNER::st_int_final()
{
	char cur = getNextChar();
	
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
		p--;
		attr = 0x107;
	}
}

void SCANNER::st_hex0()
{
	char cur = getNextChar();
	if ((cur >= '0' && cur <= '9') ||
		(cur >= 'A' && cur <= 'F') ||
		(cur >= 'a' && cur <= 'f'))
	{
		temp_token.append(1, cur);
		st_hex_final();
	}
	else
	{
		temp_token = "非法的十六进制整型";
		attr = 0x100;
	}
}

void SCANNER::st_hex_final()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x107;
	}
}

void SCANNER::st_oct_final()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x107;
	}
}

void SCANNER::st_long_final()
{
	attr = 0x107;
}

void SCANNER::st_dot_final()
{
	char cur = getNextChar();
	if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_float_final();
	}
	else
	{
		p--;
		attr = 0x11d;
	}
}

void SCANNER::st_float_final()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x108;
	}
}

void SCANNER::st_scFloat0()
{
	char cur = getNextChar();
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
		temp_token = "非法的科学计数法";
		attr = 0x100;
	}
}

void SCANNER::st_scFloat1()
{
	char cur = getNextChar();
	if (cur >= '0' && cur <= '9')
	{
		temp_token.append(1, cur);
		st_scFloat_final();
	}
	else
	{
		temp_token = "非法的科学计数法";
		attr = 0x100;
	}
}

void SCANNER::st_scFloat_final()
{
	char cur = getNextChar();
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
		p--;
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
 * + 运算符
 * + 注释
 */

/* +
 * 加号 或 正号
 */
void SCANNER::st01()
{
	char cur = getNextChar();
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
		p--;
		if (pre_attr != 0x104 && pre_attr != 0x105 &&
			pre_attr != 0x106 && pre_attr != 0x107 &&
			pre_attr != 0x108 && pre_attr != 0x109)
		{
			// 不是标识符, 字符, 数字, 字符串
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
 * 自增 1
 */
void SCANNER::st02()
{
	attr = 0x11c;
	temp_token = "++";
}
/* +=
 * 加赋值
 */
void SCANNER::st03()
{
	attr = 0x110;
	temp_token = "+=";
}
/* -
 * 减 或 负号
 */
void SCANNER::st04()
{
	char cur = getNextChar();
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
		p--;
		if (pre_attr != 0x104 && pre_attr != 0x105 &&
			pre_attr != 0x107 && pre_attr != 0x108)
		{
			// 不是标识符, 数字
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
 * 自减 1
 */
void SCANNER::st05()
{
	attr = 0x11c;
	temp_token = "--";
}
/* -=
 * 减赋值
 */
void SCANNER::st06()
{
	attr = 0x110;
	temp_token = "+=";
}
/* *
 * 乘
 */
void SCANNER::st07()
{
	char cur = getNextChar();
	
	if (cur == '=')
	{
		st08();
	}
	else
	{
		p--;
		attr = 0x11b;
		temp_token = "*";
	}
}
/* *=
 * 乘赋值
 */
void SCANNER::st08()
{
	attr = 0x110;
	temp_token = "*=";
}
/* /
 * 除 ÷
 */
void SCANNER::st09()
{
	char cur = getNextChar();
	if (cur == '=')
	{
		st10();
	}
	else if (cur == '//')
	{
		st11();
	}
	else if (cur == '*')
	{
		st12();
	}
	else
	{
		p--;
		attr = 0x11b;
		temp_token = "//";
	}
}
/* /=
 * 除赋值
 */
void SCANNER::st10()
{
	attr = 0x110;
	temp_token = "/=";
}
/* //
 * 行注释
 */
void SCANNER::st11()
{
	char cur = getNextChar();
	while (true)
	{
		if (cur == '\n' || cur == EOF)
		{
			p--;
			break;
		}
		temp_token.append(1, cur);
		cur = getNextChar();
	}
	attr = 0x101;
}
/* /*
 * 块注释
 */
void SCANNER::st12()
{
	char cur = getNextChar();
	while (true)
	{
		if (cur == '*')
		{
			char nex = getNextChar();
			if (nex = '/')
			{
				break;
			}
			else
			{
				p--;
			}
		}
		temp_token.append(1, cur);
		cur = getNextChar();
	}
	attr = 0x101;
}
/* %
 * 求模 mod
 */
void SCANNER::st13()
{
	char cur = getNextChar();
	if (cur == '=')
	{
		st14();
	}
	else
	{
		p--;
		attr = 0x11b;
		temp_token = "%";
	}
}
/* %=
 * 模赋值
 */
void SCANNER::st14()
{
	attr = 0x110;
	temp_token = "%=";
}
/* =
 * 赋值
 */
void SCANNER::st15()
{
	char cur = getNextChar();
	if (cur == '=')
	{
		st16();
	}
	else
	{
		p--;
		attr = 0x110;
		temp_token = "=";
	}
}
/* ==
 * 等于
 */
void SCANNER::st16()
{
	attr = 0x117;
	temp_token = "==";
}
/* >
 * 大于
 */
void SCANNER::st17()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x118;
		temp_token = ">";
	}
}
/* >=
 * 大于等于
 */
void SCANNER::st18()
{
	attr = 0x118;
	temp_token = ">=";
}
/* >>
 * 右移位
 */
void SCANNER::st19()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x119;
		temp_token = ">>";
	}
}
/* >>=
 * 右移位赋值
 */
void SCANNER::st20()
{
	attr = 0x118;
	temp_token = ">>=";
}
/* >>>
 * 零填充右移位
 */
void SCANNER::st21()
{
	char cur = getNextChar();
	if (cur == '=')
	{
		st22();
	}
	else
	{
		p--;
		attr = 0x119;
		temp_token = ">>>";
	}
}
/* >>>=
 * 零填充右移位赋值
 */
void SCANNER::st22()
{
	attr = 0x110;
	temp_token = ">>>=";
}
/* <
 * 小于
 */
void SCANNER::st23()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x118;
		temp_token = "<";
	}
}
/* <=
 * 小于等于
 */
void SCANNER::st24()
{
	attr = 0x119;
	temp_token = "<=";
}
/* <<
 * 左移位
 */
void SCANNER::st25()
{
	char cur = getNextChar();
	if (cur == '=')
	{
		st26();
	}
	else
	{
		p--;
		attr = 0x119;
		temp_token = "<<";
	}
}
/* <<=
 * 左移位赋值
 */
void SCANNER::st26()
{
	attr = 0x110;
	temp_token = "<<=";
}
/* !
 * 非
 */
void SCANNER::st27()
{
	char cur = getNextChar();
	if (cur == '=')
	{
		st28();
	}
	else
	{
		p--;
		attr = 0x11c;
		temp_token = "!";
	}
}
/* !=
 * 不等于
 */
void SCANNER::st28()
{
	attr = 0x117;
	temp_token = "!=";
}
/* ^
 * 按位异或
 */
void SCANNER::st29()
{
	char cur = getNextChar();
	if (cur == '=')
	{
		st30();
	}
	else
	{
		p--;
		attr = 0x115;
		temp_token = "^";
	}
}
/* ^=
 * 非赋值
 */
void SCANNER::st30()
{
	attr = 0x110;
	temp_token = "^=";
}
/* |
 * 逻辑或
 */
void SCANNER::st31()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x114;
		temp_token = "|";
	}
}
/* ||
 * 短路或
 */
void SCANNER::st32()
{
	attr = 0x112;
	temp_token = "||";
}
/* |=
 * 或赋值
 */
void SCANNER::st33()
{
	attr = 0x110;
	temp_token = "|=";
}
/* &
 * 逻辑与
 */
void SCANNER::st34()
{
	char cur = getNextChar();
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
		p--;
		attr = 0x116;
		temp_token = "&";
	}
}
/* &&
 * 短路与
 */
void SCANNER::st35()
{
	attr = 0x113;
	temp_token = "&&";
}
/* &=
 * 与赋值
 */
void SCANNER::st36()
{
	attr = 0x110;
	temp_token = "&=";
}