#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

/*
EOF								-1

��������							0

����ĵ���						0x100
ע��								0x101
�ո�								0x102
�ؼ���							0x103
��ʶ��							0x104

������							0x105

�ַ���							0x106
����								0x107
������							0x108
�ַ���							0x109

=  +=  -=  *=  /=  %=  &=
^=  |=  >>=  <<=  >>>=			0x110
?:								0x111
||								0x112
&&								0x113
|								0x114
^								0x115
&								0x116
==  !=							0x117
<  >  <=  >=					0x118
<<  >>  >>>						0x119
+  -  							0x11a
*  /  %							0x11b
++  --  +(��)  �C(��)  !  ~		0x11c
[]  ()  .						0x11d

,								0x120
{}								0x121
;								0x122

*/


// �ؼ���
const std::string key_word[] = {
	"abstract",

	"boolean",
	"break",
	"byte",

	"case",
	"catch",
	"char",
	"class",
	"const",
	"continue",

	"default",
	"do",
	"double",

	"else",
	"extends",

	"false",
	"final",
	"finally",
	"float",
	"for",

	"goto",

	"if",
	"implements",
	"import",
	"instanceof",
	"int",
	"interface",

	"long",

	"native",
	"new",
	"null",

	"package",
	"private",
	"protected",
	"public",

	"return",

	"short",
	"static",
	"super",
	"switch",
	"synchronized",

	"this",
	"throw",
	"throws",
	"transient",
	"true",
	"try",

	"void",
	"volatile",

	"while"
};