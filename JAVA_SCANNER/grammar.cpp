#include "grammar.h"

GRAMMAR_ANALYSIS::GRAMMAR_ANALYSIS()
{
	// TODO
}

GRAMMAR_ANALYSIS::~GRAMMAR_ANALYSIS()
{

}

void GRAMMAR_ANALYSIS::run()
{
	// TODO
	if (check())
	{

	}
	else
	{
		std::cout << "ILLEGAL_WHILE_STATEMENT" << std::endl;
		exit(0);
	}
}

bool GRAMMAR_ANALYSIS::check()
{
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
	_l.push(14);	// push var
}
void GRAMMAR_ANALYSIS::Vc1()
{
	// <Vc> → 标识符
	_l.pop();	// pop Vc
	_l.push(14);	// push var
}
void GRAMMAR_ANALYSIS::Vc2()
{
	// <Vc> → 整常数
	_l.pop();	// pop Vc
	_l.push(16);	// push const
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
	// <Ex'> → P Vc
	_l.pop();	// pop Ex'
	_l.push(4);	// push Vc
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