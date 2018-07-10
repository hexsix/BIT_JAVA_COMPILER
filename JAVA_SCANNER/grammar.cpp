#include "grammar.h"

GRAMMAR_ANALYSIS::GRAMMAR_ANALYSIS()
{

}

GRAMMAR_ANALYSIS::~GRAMMAR_ANALYSIS()
{

}

void GRAMMAR_ANALYSIS::set_tokens(std::vector<TOKEN> p)
{
	// 最后一个是 EOF
	for (auto i = 0; i < p.size() - 1; i++)
	{
		tokens.push_back(p[i]);
	}
}

void GRAMMAR_ANALYSIS::run()
{
	if (check())
	{
		build();
		out_asm();
	}
	else
	{
		std::cout << "ILLEGAL_WHILE_STATEMENT" << std::endl;
		exit(0);
	}
}

bool GRAMMAR_ANALYSIS::check()
{
	// init _l and _r
	_l.push(0);
	for (auto i = tokens.size() - 1; i >= 0 && i < tokens.size(); --i)
	{
		std::string word = tokens[i].word();
		int attr = tokens[i].attr();
		int _ = -1;
		if (word == "while")
		{
			_ = 8;
		}
		else if (word == "(")
		{
			_ = 9;
		}
		else if (word == ")")
		{
			_ = 10;
		}
		else if (word == ";")
		{
			_ = 11;
		}
		else if (word == "<")
		{
			_ = 12;
		}
		else if (word == ">")
		{
			_ = 13;
		}
		else if (attr == 0x104)
		{
			_ = 14;
		}
		else if (word == "=")
		{
			_ = 15;
		}
		else if (attr == 0x105 || attr == 0x107)
		{
			_ = 16;
		}
		else if (word == "+")
		{
			_ = 17;
		}
		else if (word == "-")
		{
			_ = 18;
		}
		else if (word == "*")
		{
			_ = 19;
		}
		else if (word == "//")
		{
			_ = 20;
		}
		else
		{
			std::cout << "UNEXPECTED VT in grammar analysis" << std::endl;
			exit(0);
		}
		_r.push(_);
	}

	while (true)
	{
		if (_l.empty() && _r.empty())
			break;
		else if (_l.empty() || _r.empty())
			return false;

		int l = _l.top(), r = _r.top();
		if (l == r)
		{
			_l.pop();
			_r.pop();
		}
		else if (l == 0 && r == 8)
		{
			Sw();
		}
		else if (l == 1 && (r == 14 || r == 16))
		{
			Er();
		}
		else if (l == 2 && r == 12)
		{
			Er_1();
		}
		else if (l == 2 && r == 13)
		{
			Er_2();
		}
		else if (l == 3 && r == 14)
		{
			S();
		}
		else if (l == 4 && r == 14)
		{
			Vc1();
		}
		else if (l == 4 && r == 16)
		{
			Vc2();
		}
		else if (l == 5 && (r == 14 || r == 16))
		{
			Ex();
		}
		else if (l == 6 && r == 11)
		{
			Ex_2();
		}
		else if (l == 6 &&
			(r == 17 || r == 18 || r == 19 || r == 20))
		{
			Ex_1();
		}
		else if (l == 7 && r == 17)
		{
			P1();
		}
		else if (l == 7 && r == 18)
		{
			P2();
		}
		else if (l == 7 && r == 19)
		{
			P3();
		}
		else if (l == 7 && r == 20)
		{
			P4();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void GRAMMAR_ANALYSIS::Sw()
{
	// Sw → while(Er)S;
	_l.pop();	// pop Sw
	_l.push(11);	// push ;
	_l.push(3);	// push S
	_l.push(10);	// push )
	_l.push(1);	// push Er
	_l.push(9);	// push (
	_l.push(8);	// push while
}
void GRAMMAR_ANALYSIS::Er()
{
	// <Er> → Vc Er'
	_l.pop();	// pop Er
	_l.push(2);	// push Er'
	_l.push(4);	// push Vc
}
void GRAMMAR_ANALYSIS::Er_1()
{
	// <Er'> → < Vc
	_l.pop();	// pop Er'
	_l.push(4);	// push Vc
	_l.push(12); // push <
}
void GRAMMAR_ANALYSIS::Er_2()
{
	// <Er'> → > Vc
	_l.pop();	// pop Er'
	_l.push(4);	// push Vc
	_l.push(13);	// push >
}
void GRAMMAR_ANALYSIS::S()
{
	// <S> → 标识符 = Ex
	_l.pop();	// pop S
	_l.push(5);	// push Ex
	_l.push(15);	// push =
	// 因为标识符是不定的, 直接pop _r右边
	_r.pop();	// push var
}
void GRAMMAR_ANALYSIS::Vc1()
{
	// <Vc> → 标识符
	_l.pop();	// pop Vc
	// 因为标识符是不定的, 直接pop _r右边
	_r.pop();	// push var
}
void GRAMMAR_ANALYSIS::Vc2()
{
	// <Vc> → 整常数
	_l.pop();	// pop Vc
	// 因为整常数是不定的, 直接pop _r右边
	_r.pop();	// push const
}
void GRAMMAR_ANALYSIS::Ex()
{
	// <Ex> → Vc Ex'
	_l.pop();	// pop Ex
	_l.push(6);	// push Ex'
	_l.push(4);	// push Vc
}
void GRAMMAR_ANALYSIS::Ex_1()
{
	// <Ex'> → P Ex
	_l.pop();	// pop Ex'
	_l.push(5);	// push Ex
	_l.push(7);	// push P
}
void GRAMMAR_ANALYSIS::Ex_2()
{
	// <Ex'> → ε
	_l.pop();	// pop Ex'
}
void GRAMMAR_ANALYSIS::P1()
{
	// <P> → +
	_l.pop();	// pop P
	_l.push(17);	// push +
}
void GRAMMAR_ANALYSIS::P2()
{
	// <P> → -
	_l.pop();	// pop P
	_l.push(18);	// push -
}
void GRAMMAR_ANALYSIS::P3()
{
	// <P> → *
	_l.pop();	// pop P
	_l.push(19);	// push *
}
void GRAMMAR_ANALYSIS::P4()
{
	// <P> → /
	_l.pop();	// pop P
	_l.push(20);	// push /
}

void GRAMMAR_ANALYSIS::build()
{
	while (!op.empty())
		op.pop();
	while (!tp.empty())
		tp.pop();
	// while ( Vc </> Vc )     S          ;
	//   0   1 2   3  4  5 6-size()-2 size()-1
	for (auto i = 6; i < tokens.size() - 1; ++i)
	{
		if (isOperator(tokens[i]))
		{
			while (true)
			{
				if (op.empty())
				{
					op.push(tokens[i]);
					break;
				}
				else if (getPrio(tokens[i]) > getPrio(op.top()))
				{
					op.push(tokens[i]);
					break;
				}
				else
				{
					tp.push(op.top());
					op.pop();
				}
			}
		}
		else
		{
			tp.push(tokens[i]);
		}
	}
	while (!tp.empty())
	{
		op.push(tp.top());
		tp.pop();
	}
}

bool GRAMMAR_ANALYSIS::isOperator(TOKEN x)
{
	return (x.attr() <= 0x11c && x.attr() >= 0x110);
}

int GRAMMAR_ANALYSIS::getPrio(TOKEN x)
{
	return x.attr();
}

void GRAMMAR_ANALYSIS::out_asm()
{
	std::ofstream result;
	result.open("result.asm");
	// while(Vc<Vc)...;
	//   0  1 234 5
	// .while Vc<Vc
	result << ".while " << tokens[2].word()
		<< tokens[3].word() << tokens[4].word() << std::endl;
	// 
	TOKEN a, b, oprt;
	// a + - * / = b
	while (!op.empty())
	{
		TOKEN cur = op.top();
		op.pop();
		if (isOperator(cur))
		{
			TOKEN b = tp.top();
			tp.pop();
			TOKEN a = tp.top();
			tp.pop();
			std::string oprt = cur.word();
			if (oprt == "=")
			{
				/*
					mov		eax, b
					mov		a, eax
				*/
				result << "\tmov\t\teax, " << b.word() << std::endl;
				result << "\tmov\t\t" << a.word() << ", eax\n";
				break;
			}
			else if (oprt == "+")
			{
				/*
					mov		eax, b
					add		eax, a
					mov		ebx, eax
				*/
				result << "\tmov\t\teax, " << b.word() << std::endl;
				result << "\tadd\t\teax, " << a.word() << std::endl;
				result << "\tmov\t\tebx, eax\n\n";
				tp.push(TOKEN("ebx", 0x104));
			}
			else if (oprt == "-")
			{
				/*
					mov		eax, b
					sub		eax, a
					mov		ebx, eax
				*/
				result << "\tmov\t\teax, " << b.word() << std::endl;
				result << "\tsub\t\teax, " << a.word() << std::endl;
				result << "\tmov\t\tebx, eax\n\n";
				tp.push(TOKEN("ebx", 0x104));
			}
			else if (oprt == "*")
			{
				/*
					imul	eax, a, b
					mov		ebx, eax
				*/
				result << "\timul\teax, " << a.word() << ", " << b.word() << std::endl;
				result << "\tmov\t\tebx, eax\n\n";
				tp.push(TOKEN("ebx", 0x104));
			}
			else if (oprt == "//")
			{
				/*
					mov		edx, 0
					mov		eax, a
					cdq
					mov		ebx, b
					idiv	ebx
					mov		ebx, eax
				*/
				result << "\tmov\t\tedx, 0\n";
				result << "\tmov\t\teax, " << a.word() << std::endl;
				result << "\tcdq\n";
				result << "\tmov\t\tebx, " << b.word() << std::endl;
				result << "\tidiv\tebx\n";
				result << "\tmov\t\tebx, eax\n\n";
				tp.push(TOKEN("ebx", 0x104));
			}
			else
			{
				std::cout << "unknown operator in grammar analysis" << std::endl;
				exit(0);
			}
		}
		else
		{
			tp.push(cur);
		}
	}
	// .endw
	result << ".endw";
	result.close();
}