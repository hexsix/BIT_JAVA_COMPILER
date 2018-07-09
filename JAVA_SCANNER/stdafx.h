#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

/*
EOF								-1

正常运行							0

错误的单词						0x100
注释								0x101
空格								0x102
关键字							0x103
标识符							0x104

布尔型							0x105

字符型							0x106
整型								0x107
浮点型							0x108
字符串							0x109

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
++  --  +(正)  –(负)  !  ~		0x11c
[]  ()  .						0x11d

,								0x120
{}								0x121
;								0x122

*/


// 关键字
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