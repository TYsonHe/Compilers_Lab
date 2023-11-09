#pragma once
#include <string>
using namespace std;
/* 单词编码 */
enum TokenCode
{
	/*未定义*/
	TK_UNDEF = 0,

	/* 关键字 */
	KW_VOID,	//void关键字
	KW_MAIN,	//main关键字
	KW_INT,		//int关键字
	KW_DOUBLE,	//double关键字
	KW_FOR,		//for关键字
	KW_WHILE,	//while关键字
	KW_CASE,	//case关键字
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

	/* 分隔符 */
	TK_OPENPA,	//(左圆括号
	TK_CLOSEPA,	//)右圆括号
	TK_OPENBR,	//[左中括号
	TK_CLOSEBR,	//]右中括号
	TK_BEGIN,	//{左大括号
	TK_END,		//}右大括号
	TK_COMMA,	//,逗号
	TK_SEMOCOLOM,	//;分号

	/* 常量 */
	TK_INT,		//整型常量
	TK_DOUBLE,	//浮点型常量

	/* 标识符 */
	TK_IDENT
};

/******************************************全局变量*****************************************************/
TokenCode code = TK_UNDEF;		//记录单词的种别码
const int MAX = 11;				//关键字数量
int row = 1;					//记录字符所在的行数
string token = "";				//用于存储单词
char  keyWord[][7] = { "void","main","int","while","if","else","return" };	//存储关键词
