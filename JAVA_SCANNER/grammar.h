#pragma once

#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include "scanner.h"
#include <stack>

class GRAMMAR_ANALYSIS {
private:
	TOKEN tokens;
	std::stack<int> _l;	// ����ջ
	std::stack<int> _r;	// �������봮

	bool check();		// �ж��Ƿ��Ƕ����е��﷨�Ӽ�Sw
	void Sw();			// <Sw> �� while(Er)S;
	void Er();			// <Er> �� Vc Er'
	void Er_1();		// <Er'> �� < Vc
	void Er_2();		// <Er'> �� > Vc
	void S();			// <S> �� ��ʶ�� = Ex
	void Vc1();			// <Vc> �� ��ʶ��
	void Vc2();			// <Vc> �� ������
	void Ex();			// <Ex> �� Vc Ex'
	void Ex_1();		// <Ex'> �� P Vc
	void Ex_2();		// <Ex'> �� ��
	void P1();			// <P> �� +
	void P2();			// <P> �� -
	void P3();			// <P> �� *
	void P4();			// <P> �� /
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
��ʶ��	14
=		15
������	16
+		17
-		18
*		19
/		20

*/