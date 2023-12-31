/***********************************************
* 词法分析器
* 编译环境：Visual Studio 2022
* by 何诗锟 康力 顾屹洋
***********************************************/
#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
using namespace std;
/* 单词编码 */
enum TokenCode
{
	/*未定义*/
	TK_UNDEF = 0,

	/* 注释符 */
	TK_SINGAL_LINE_ANNO, //单行注释
	TK_MUTI_LINE_ANNO,   //多行注释

	/* 关键字 */
	KW_VOID,	//void关键字
	KW_MAIN,	//main关键字
	KW_INT,		//int关键字
	KW_WHILE,	//while关键字
	KW_IF,		//if关键字
	KW_ELSE,	//else关键字
	KW_RETURN,	//return关键字

	/* 运算符 */
	TK_PLUS,	//+加号
	TK_MINUS,	//-减号
	TK_STAR,	//*乘号
	TK_DIVIDE,	///除号
	TK_ASSIGN,	//=赋值运算符
	TK_EQ,		//==等于号
	TK_LT,		//<小于号
	TK_LEQ,		//<=小于等于号
	TK_GT,		//>大于号
	TK_GEQ,		//>=大于等于号
	TK_NEQ,     //!= 不等于号

	/* 分隔符 */
	TK_OPENPA,	//(左圆括号
	TK_CLOSEPA,	//)右圆括号
	TK_BEGIN_LEFT,	//{左大括号
	TK_END_RIGHT,	//}右大括号
	TK_COMMA,	//,逗号
	TK_SEMOCOLOM,	//;分号

	/* 常量 */
	TK_INT,		//整型常量

	/* 标识符 */
	TK_IDENT,

	/* 结束符 */
	TK_END
};

/* 词法分析结果 */
struct LexResult
{
	TokenCode code;
	string token;
};

class LexAnalysis
{
public:
	// 构造函数
	LexAnalysis() = default;
	// 成员函数
	void print(TokenCode code);
	void lexicalAnalysis(FILE* fp);
};

// 全局变量
extern TokenCode code;
extern const int MAX;
extern int row;
extern string token;
extern char  keyWord[][7];
extern vector <LexResult> lexResults;
extern bool passLex;