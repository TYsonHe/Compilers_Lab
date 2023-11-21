/***********************************************
* 词法分析器
* 编译环境：Visual Studio 2022
* by 何诗锟 康力 顾屹洋
***********************************************/

/***********************************************头文件**************************************************/
#include "LexAnalysis.h"
/**********************************************函数*****************************************************/

/********************************************
* 功能：消除文件中的注释
* code：单词对应的种别码
* token：用于存储单词
* row：单词所在的行数
*********************************************/


/********************************************
* 功能：打印词法分析的结果
* code：单词对应的种别码
* token：用于存储单词
* row：单词所在的行数
*********************************************/
void LexAnalysis::print(TokenCode code)
{

	switch (code)
	{
		/*未识别的符号*/
	case TK_UNDEF:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);	//未识别的符号为红色
		cout << '(' << code << ',' << token << ")" << "未识别的符号在第" << row << "行。" << endl;
		return;
		break;
		/*关键字*/
	case KW_VOID:		//void关键字
	case KW_MAIN:	//main关键字
	case KW_INT:		//int关键字
	case KW_WHILE:	//while关键字
	case KW_IF:		//if关键字
	case KW_ELSE:	//else关键字
	case KW_RETURN:	//return关键字
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);	//关键字为蓝色
		break;

		/* 运算符 */
	case TK_PLUS:	//+加号
	case TK_MINUS:	//-减号
	case TK_STAR:	//*乘号
	case TK_DIVIDE:	///除号
	case TK_ASSIGN:	//=赋值运算符
	case TK_EQ:		//==等于号
	case TK_LT:		//<小于号
	case TK_LEQ:	//<=小于等于号
	case TK_GT:		//>大于号
	case TK_GEQ:		//>=大于等于号

		/* 分隔符 */
	case TK_OPENPA:	//(左圆括号
	case TK_CLOSEPA:	//)右圆括号
	case TK_BEGIN_LEFT:	//{左大括号
	case TK_END_RIGHT:	//}右大括号
	case TK_COMMA:	//,逗号
	case TK_SEMOCOLOM:	//;分号
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);	//运算符和分隔符为绿色
		break;

		/* 常量 */
	case TK_INT:	//整型常量
		/* 标识符 */
	case TK_IDENT:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);	//关键字为灰色
		break;
		/* 结束符 */
	case TK_END:   //#井号
	default:
		break;
	}
	cout << '(' << code << ',' << token << ")" << endl;
}

/********************************************
* 功能：判断是否是关键字
* MAX：关键字数量
* token：用于存储单词
*********************************************/
bool isKey(string token)
{
	for (int i = 0; i < MAX; i++)
	{
		if (token.compare(keyWord[i]) == 0)
			return true;
	}
	return false;
}

/********************************************
* 功能：返回关键字的内码值
* MAX：关键字数量
* token：用于存储单词
*********************************************/
int  getKeyID(string token)
{
	for (int i = 0; i < MAX; i++)
	{	//关键字的内码值为keyWord数组中对应的下标加3
		if (token.compare(keyWord[i]) == 0)
			return i + 3;
	}
	return -1;
}

/********************************************
* 功能：判断一个字符是否是字母
* letter：被判断的字符
*********************************************/
bool isLetter(char letter)
{
	if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
		return true;
	return false;

}

/********************************************
* 功能：判断一个字符是否是数字
* digit：被判断的字符
*********************************************/
bool isDigit(char digit)
{
	if (digit >= '0' && digit <= '9')
		return true;
	return false;
}

/********************************************
* 功能：词法分析
* fp：文件指针
* code：单词对应的种别码
* token：用于存储单词
* row：单词所在的行数
*********************************************/
void LexAnalysis::lexicalAnalysis(FILE* fp)
{
	char ch;			//用于存储从文件中获取的单个字符
	while ((ch = fgetc(fp)) != '#')	//未读取到文件尾，从文件中获取一个字符
	{
		token = ch;									//将获取的字符存入token中
		if (ch == ' ' || ch == '\t' || ch == '\n')	//忽略空格、Tab和回车
		{
			if (ch == '\n')							//遇到换行符，记录行数的row加1
				row++;
			continue;								//继续执行循环
		}
		// 处理单行和多行注释
		else if (ch == '/')
		{
			ch = fgetc(fp);//再读一个
			token = "";
			if(ch == '/') //再读一个是'/' 单行注释
			{
				token+="//";
				while ((ch = fgetc(fp)) != '\n')
					;//一直读到换行
				row++;
				code = TK_SINGAL_LINE_ANNO;
			}
			else if (ch == '*')  //再读一个是'*' 多行注释
			{
				token += "/*";
				while ((ch = fgetc(fp)) != '*')
				{
					if (ch == '\n')
						row++;
				}//一直读到下一个 *号
				if ((ch = fgetc(fp)) == '/') //再下一个是'/'号
				{
					token += " */";
					code = TK_MUTI_LINE_ANNO;
				}
			}
			//都不是，认为是除号
			else
			{
				code = TK_DIVIDE;
				//文件指针后退一个字节，即重新读取上述单词后的第一个字符
				fseek(fp, -1L, SEEK_CUR);
			}

		}
		else if (isLetter(ch))			//以字母开头，关键字或标识符
		{
			token = "";					//token初始化
			while (isLetter(ch) || isDigit(ch))	//非字母或数字时退出，将单词存储在token中
			{
				token.push_back(ch);	//将读取的字符ch存入token中
				ch = fgetc(fp);			//获取下一个字符
			}
			//文件指针后退一个字节，即重新读取上述单词后的第一个字符
			fseek(fp, -1L, SEEK_CUR);
			if (isKey(token))	//关键字
				code = TokenCode(getKeyID(token));
			else	//标识符
				code = TK_IDENT;	//单词为标识符
		}
		else if (isDigit(ch))	//无符号常数以数字开头
		{
			token = "";			//token初始化
			while (isDigit(ch))	//当前获取到的字符为数字
			{
				token.push_back(ch);		//读取数字，将其存入token中
				ch = fgetc(fp);				//从文件中获取下一个字符
			}
			code = TK_INT;				//单词为整型
			//文件指针后退一个字节，即重新读取常数后的第一个字符
			fseek(fp, -1L, SEEK_CUR);
		}
		else switch (ch)
		{
			/*运算符*/
		case '+': code = TK_PLUS;		//+加号			
			break;
		case '-': code = TK_MINUS;		//-减号
			break;
		case '*': code = TK_STAR;		//*乘号		
			break;
		case '=':
		{
			ch = fgetc(fp);				//超前读取'='后面的字符
			if (ch == '=')				//==等于号
			{
				token.push_back(ch);	//将'='后面的'='存入token中
				code = TK_EQ;			//单词为"=="
			}
			else {						//=赋值运算符
				code = TK_ASSIGN;		//单词为"="
				fseek(fp, -1L, SEEK_CUR);	//将超前读取的字符重新读取
			}
		}
		break;
		case '<':
		{
			ch = fgetc(fp);				//超前读取'<'后面的字符
			if (ch == '=')				//<=小于等于号
			{
				token.push_back(ch);	//将'<'后面的'='存入token中
				code = TK_LEQ;			//单词为"<="
			}
			else {						//<小于号
				code = TK_LT;			//单词为"<"
				fseek(fp, -1L, SEEK_CUR);	//将超前读取的字符重新读取
			}
		}
		break;
		case '>':
		{
			ch = fgetc(fp);				//超前读取'>'后面的字符
			if (ch == '=')				//>=大于等于号
			{
				token.push_back(ch);	//将'>'后面的'='存入token中
				code = TK_GEQ;			//单词为">="
			}
			else {						//>大于号
				code = TK_GT;			//单词为">"
				fseek(fp, -1L, SEEK_CUR);	//将超前读取的字符重新读取
			}
		}
		break;
		case '!':
		{
			ch = fgetc(fp);
			if (ch == '=')
			{
				token.push_back(ch);
				code = TK_NEQ;         //!=号
			}
			else
				fseek(fp, -1L, SEEK_CUR);
		}
		/*分界符*/
		case '(': code = TK_OPENPA;		//(左圆括号
			break;
		case ')': code = TK_CLOSEPA;	//)右圆括号
			break;
		case '{': code = TK_BEGIN_LEFT;		//{左大括号
			break;
		case '}': code = TK_END_RIGHT;		//}右大括号
			break;
		case ',': code = TK_COMMA;		//,逗号
			break;
		case ';': code = TK_SEMOCOLOM;	//;分号
			break;
			//未识别符号
		default: code = TK_UNDEF;
		}
		this->print(code);				//打印词法分析结果
	}
}


