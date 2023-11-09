/***********************************************
* �ʷ�������
* ���뻷����Visual Studio 2022
***********************************************/

/***********************************************ͷ�ļ�**************************************************/
#include <iostream>
#include <string>
#include <Windows.h>
#include "LexSync.h"
/**********************************************����*****************************************************/

/********************************************
* ���ܣ���ӡ�ʷ������Ľ��
* code�����ʶ�Ӧ���ֱ���
* token�����ڴ洢����
* row���������ڵ�����
*********************************************/
void print(TokenCode code)
{

	switch (code)
	{
		/*δʶ��ķ���*/
	case TK_UNDEF:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);	//δʶ��ķ���Ϊ��ɫ
		cout << '(' << code << ',' << token << ")" << "δʶ��ķ����ڵ�" << row << "�С�" << endl;
		return;
		break;
		/*�ؼ���*/
	case KW_VOID:		//void�ؼ���
	case KW_MAIN:	//main�ؼ���
	case KW_INT:		//int�ؼ���
	case KW_WHILE:	//while�ؼ���
	case KW_IF:		//if�ؼ���
	case KW_ELSE:	//else�ؼ���
	case KW_RETURN:	//return�ؼ���
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);	//�ؼ���Ϊ��ɫ
		break;

		/* ����� */
	case TK_PLUS:	//+�Ӻ�
	case TK_MINUS:	//-����
	case TK_STAR:	//*�˺�
	case TK_DIVIDE:	///����
	case TK_ASSIGN:	//=��ֵ�����
	case TK_EQ:		//==���ں�
	case TK_LT:		//<С�ں�
	case TK_LEQ:	//<=С�ڵ��ں�
	case TK_GT:		//>���ں�
	case TK_GEQ:		//>=���ڵ��ں�

		/* �ָ��� */
	case TK_OPENPA:	//(��Բ����
	case TK_CLOSEPA:	//)��Բ����
	case TK_OPENBR:	//[��������
	case TK_CLOSEBR:	//]��������
	case TK_BEGIN:	//{�������
	case TK_END:	//}�Ҵ�����
	case TK_COMMA:	//,����
	case TK_SEMOCOLOM:	//;�ֺ�
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);	//������ͷָ���Ϊ��ɫ
		break;

		/* ���� */
	case TK_INT:	//���ͳ���
	case TK_DOUBLE:	//�����ͳ���
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);	//����Ϊ��ɫ
		if (token.find('.') == token.npos)
			cout << '(' << code << ',' << atoi(token.c_str()) << ")" << endl;						//����Ϊ����
		else
			cout << '(' << code << ',' << atof(token.c_str()) << ")" << endl;							//����Ϊ������
		return;
		break;
		/* ��ʶ�� */
	case TK_IDENT:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);	//�ؼ���Ϊ��ɫ
		break;
	default:
		break;
	}
	cout << '(' << code << ',' << token << ")" << endl;
}

/********************************************
* ���ܣ��ж��Ƿ��ǹؼ���
* MAX���ؼ�������
* token�����ڴ洢����
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
* ���ܣ����عؼ��ֵ�����ֵ
* MAX���ؼ�������
* token�����ڴ洢����
*********************************************/
int  getKeyID(string token)
{
	for (int i = 0; i < MAX; i++)
	{	//�ؼ��ֵ�����ֵΪkeyWord�����ж�Ӧ���±��1
		if (token.compare(keyWord[i]) == 0)
			return i + 1;
	}
	return -1;
}

/********************************************
* ���ܣ��ж�һ���ַ��Ƿ�����ĸ
* letter�����жϵ��ַ�
*********************************************/
bool isLetter(char letter)
{
	if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
		return true;
	return false;

}

/********************************************
* ���ܣ��ж�һ���ַ��Ƿ�������
* digit�����жϵ��ַ�
*********************************************/
bool isDigit(char digit)
{
	if (digit >= '0' && digit <= '9')
		return true;
	return false;
}

/********************************************
* ���ܣ��ʷ�����
* fp���ļ�ָ��
* code�����ʶ�Ӧ���ֱ���
* token�����ڴ洢����
* row���������ڵ�����
*********************************************/
void lexicalAnalysis(FILE* fp)
{
	char ch;			//���ڴ洢���ļ��л�ȡ�ĵ����ַ�
	while ((ch = fgetc(fp)) != EOF)	//δ��ȡ���ļ�β�����ļ��л�ȡһ���ַ�
	{
		token = ch;									//����ȡ���ַ�����token��
		if (ch == ' ' || ch == '\t' || ch == '\n')	//���Կո�Tab�ͻس�
		{
			if (ch == '\n')							//�������з�����¼������row��1
				row++;
			continue;								//����ִ��ѭ��
		}
		else if (isLetter(ch))			//����ĸ��ͷ���ؼ��ֻ��ʶ��
		{
			token = "";					//token��ʼ��
			while (isLetter(ch) || isDigit(ch))	//����ĸ������ʱ�˳��������ʴ洢��token��
			{
				token.push_back(ch);	//����ȡ���ַ�ch����token��
				ch = fgetc(fp);			//��ȡ��һ���ַ�
			}
			//�ļ�ָ�����һ���ֽڣ������¶�ȡ�������ʺ�ĵ�һ���ַ�
			fseek(fp, -1L, SEEK_CUR);
			if (isKey(token))	//�ؼ���
				code = TokenCode(getKeyID(token));
			else	//��ʶ��
				code = TK_IDENT;	//����Ϊ��ʶ��
		}
		else if (isDigit(ch))	//�޷��ų��������ֿ�ͷ
		{
			int isdouble = 0;	//����Ƿ�Ϊ������
			token = "";			//token��ʼ��
			while (isDigit(ch))	//��ǰ��ȡ�����ַ�Ϊ����
			{
				token.push_back(ch);		//��ȡ���֣��������token��
				ch = fgetc(fp);				//���ļ��л�ȡ��һ���ַ�
				//�õ����е�һ�γ���С����
				if (ch == '.' && isdouble == 0)
				{
					//С������һλ������
					if (isDigit(fgetc(fp)))
					{
						isdouble = 1;		//��Ǹó������Ѿ����ֹ�С����
						fseek(fp, -1L, SEEK_CUR);		//����ǰ��ȡ��С�����һλ���¶�ȡ	
						token.push_back(ch);			//��С������token��
						ch = fgetc(fp);				//��ȡС��������һλ����
					}
				}
			}
			if (isdouble == 1)
				code = TK_DOUBLE;	//����Ϊ������
			else
				code = TK_INT;				//����Ϊ����
			//�ļ�ָ�����һ���ֽڣ������¶�ȡ������ĵ�һ���ַ�
			fseek(fp, -1L, SEEK_CUR);
		}
		else switch (ch)
		{
			/*�����*/
		case '+': code = TK_PLUS;		//+�Ӻ�			
			break;
		case '-': code = TK_MINUS;		//-����
			break;
		case '*': code = TK_STAR;		//*�˺�		
			break;
		case '/': code = TK_DIVIDE;		//����
			break;
		case '=':
		{
			ch = fgetc(fp);				//��ǰ��ȡ'='������ַ�
			if (ch == '=')				//==���ں�
			{
				token.push_back(ch);	//��'='�����'='����token��
				code = TK_EQ;			//����Ϊ"=="
			}
			else {						//=��ֵ�����
				code = TK_ASSIGN;		//����Ϊ"="
				fseek(fp, -1L, SEEK_CUR);	//����ǰ��ȡ���ַ����¶�ȡ
			}
		}
		break;
		case '<':
		{
			ch = fgetc(fp);				//��ǰ��ȡ'<'������ַ�
			if (ch == '=')				//<=С�ڵ��ں�
			{
				token.push_back(ch);	//��'<'�����'='����token��
				code = TK_LEQ;			//����Ϊ"<="
			}
			else {						//<С�ں�
				code = TK_LT;			//����Ϊ"<"
				fseek(fp, -1L, SEEK_CUR);	//����ǰ��ȡ���ַ����¶�ȡ
			}
		}
		break;
		case '>':
		{
			ch = fgetc(fp);				//��ǰ��ȡ'>'������ַ�
			if (ch == '=')				//>=���ڵ��ں�
			{
				token.push_back(ch);	//��'>'�����'='����token��
				code = TK_GEQ;			//����Ϊ">="
			}
			else {						//>���ں�
				code = TK_GT;			//����Ϊ">"
				fseek(fp, -1L, SEEK_CUR);	//����ǰ��ȡ���ַ����¶�ȡ
			}
		}
		break;
		/*�ֽ��*/
		case '(': code = TK_OPENPA;		//(��Բ����
			break;
		case ')': code = TK_CLOSEPA;	//)��Բ����
			break;
		case '[': code = TK_OPENBR;		//[��������
			break;
		case ']': code = TK_CLOSEBR;	//]��������
			break;
		case '{': code = TK_BEGIN;		//{�������
			break;
		case '}': code = TK_END;		//}�Ҵ�����
			break;
		case ',': code = TK_COMMA;		//,����
			break;
		case ';': code = TK_SEMOCOLOM;	//;�ֺ�
			break;
			//δʶ�����
		default: code = TK_UNDEF;
		}
		print(code);				//��ӡ�ʷ��������
	}
}

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
	lexicalAnalysis(fp);		//�ʷ�����
	fclose(fp);					//�ر��ļ�
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	//����ָ�ԭ������ɫ
	return 0;
}
