// ����������
#include "LexAnalysis.h"
using namespace std;

/******************************************ȫ�ֱ���*****************************************************/
TokenCode code = TK_UNDEF;		//��¼���ʵ��ֱ���
const int MAX = 7;				//�ؼ�������
int row = 1;					//��¼�ַ����ڵ�����
string token = "";				//���ڴ洢����
char  keyWord[][7] = { "void","main","int","while","if","else","return" };	//�洢�ؼ���
/******************************************ȫ�ֱ���*****************************************************/

int main()
{
	string filename;		//�ļ�·��
	FILE* fp;				//�ļ�ָ��
	cout << "������Դ�ļ�����" << endl;
	while (true) {
		cin >> filename;		//��ȡ�ļ�·��
		if ((fopen_s(&fp, filename.c_str(), "r")) == 0)		//���ļ�
			break;
		else
			cout << "·���������" << endl;	//��ȡʧ��
	}
	cout << "/=***************************�ʷ��������***************************=/" << endl;
	LexAnalysis lexAnalysis = LexAnalysis();
	lexAnalysis.lexicalAnalysis(fp);		//�ʷ�����
	fclose(fp);					//�ر��ļ�
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	//����ָ�ԭ������ɫ
	return 0;
}