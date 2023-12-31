// 总体程序入口
#include "LexAnalysis.h"
#include "SynAnalysis.h"
using namespace std;

/******************************************全局变量*****************************************************/
TokenCode code = TK_UNDEF;		//记录单词的种别码
const int MAX = 7;				//关键字数量
int row = 1;					//记录字符所在的行数
string token = "";				//用于存储单词
char  keyWord[][7] = { "void","main","int","while","if","else","return" };	//存储关键词
vector <LexResult> lexResults;	//存储词法分析结果
bool passLex = true;			//记录词法分析是否通过
/******************************************全局变量*****************************************************/

int main()
{
	string filename;		//文件路径
	FILE* fp;				//文件指针
	cout << "请输入源文件名：" << endl;
	while (true) {
		cin >> filename;
		if ((fopen_s(&fp, filename.c_str(), "r")) == 0)		//打开文件
			break;
		else
			cout << "路径输入错误！" << endl;	//读取失败
	}
	cout << "/=***************************词法分析结果***************************=/" << endl;
	LexAnalysis lexAnalysis = LexAnalysis();
	lexAnalysis.lexicalAnalysis(fp);		//词法分析
	if (passLex == false) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "词法分析未通过！编译终止(>-<)!" << endl;
		return 0;
	}
	fclose(fp);
	SynAnalysis synAnalysis = SynAnalysis();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	//字体恢复原来的颜色
	cout << "/=***************************文法解析结果***************************=/" << endl;
	synAnalysis.ProcessGrammar();				//文法解析
	cout << "/=***************************语法分析结果***************************=/" << endl;
	synAnalysis.GetFirst();						//获取first集
	synAnalysis.GetItemSet();					//获取项目集族
	synAnalysis.GetLRTable();					//获取LR分析表
	synAnalysis.SynAnalysisProcess();			//语法分析
	// synAnalysis.Debug();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	//字体恢复原来的颜色

	return 0;
}