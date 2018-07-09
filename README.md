# BIT_JAVA_COMPILER

这是北理工编译原理的词法分析和语法分析作业

一.	词法分析

	(1) 内容
		用C语言或C++作为宿主语言完成:
		Java语言词法分析器的设计和实现

	(2) 具体要求
		a.	使用DFA实现词法分析器的设计;
		b.	实现对Java源程序中注释的过滤;
		c.	利用对半缓冲区从文件中逐一读取单词;
		d.	词法分析结果属性字流存放在独立文件(文件名:scan_out)中;
		e.	具有报告词法错误和出错位置(源程序行号和该行字符)的功能;

二.	语法分析
	
	(1) 内容
		用C语言或C++作为宿主语言完成:
		Java语言子集语法、语义分析器

	(2)	具体要求
		a.	完成对附录一中文法描述的Java源代码的语法分析、语义分析及代码生成。
			代码生成的目标代码为汇编代码;
		b.	词法分析使用实验一完成的词法分析器(即与词法分析器有直接接口);

	(3)	子集语法
		<Sw>	→	while(Er)S;
		<Er>	→	Vc < Vc | Vc > Vc
		<S>	→	标识符 = Ex;			// 标识符使用词法分析识别结果
		<Vc>	→	标识符 | 整常数			// 标识符、整场数使用词法分析识别结果
		<Ex>	→	Vc P Ex | Vc
		<P>	→	+ | - | * | /                   // 运算符必须符合优先级和结合性
