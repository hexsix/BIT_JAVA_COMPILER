#pragma once

#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include "scanner.h"
#include <stack>

class GRAMMAR_ANALYSIS {
private:
	TOKEN tokens;
	std::stack<int> _l;	// 分析栈
	std::stack<int> _r;	// 余留输入串

	bool check();		// 判断是否是定义中的语法子集Sw
	void Sw();			// <Sw> → while(Er)S;
	void Er();			// <Er> → Vc Er'
	void Er_1();		// <Er'> → < Vc
	void Er_2();		// <Er'> → > Vc
	void S();			// <S> → 标识符 = Ex
	void Vc1();			// <Vc> → 标识符
	void Vc2();			// <Vc> → 整常数
	void Ex();			// <Ex> → Vc Ex'
	void Ex_1();		// <Ex'> → P Vc
	void Ex_2();		// <Ex'> → ε
	void P1();			// <P> → +
	void P2();			// <P> → -
	void P3();			// <P> → *
	void P4();			// <P> → /
public:
	GRAMMAR_ANALYSIS();
	~GRAMMAR_ANALYSIS();
	void run();
};

#endif

/*

Sw		0
Er		1
Er'		2
S		3
Vc		4
Ex		5
Ex'		6
P		7
while	8
(		9
)		10
;		11
<		12
>		13
标识符	14
=		15
整常数	16
+		17
-		18
*		19
/		20

*/