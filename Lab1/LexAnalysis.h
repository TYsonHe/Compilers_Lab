/***********************************************
* �ʷ�������
* ���뻷����Visual Studio 2022
* by ��ʫ� ���� ������
***********************************************/
#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <string>
using namespace std;
/* ���ʱ��� */
enum TokenCode
{
	/*δ����*/
	TK_UNDEF = 0,

	/* ע�ͷ� */
	TK_SINGAL_LINE_ANNO, //����ע��
	TK_MUTI_LINE_ANNO,   //����ע��

	/* �ؼ��� */
	KW_VOID,	//void�ؼ���
	KW_MAIN,	//main�ؼ���
	KW_INT,		//int�ؼ���
	KW_WHILE,	//while�ؼ���
	KW_IF,		//if�ؼ���
	KW_ELSE,	//else�ؼ���
	KW_RETURN,	//return�ؼ���

	/* ����� */
	TK_PLUS,	//+�Ӻ�
	TK_MINUS,	//-����
	TK_STAR,	//*�˺�
	TK_DIVIDE,	///����
	TK_ASSIGN,	//=��ֵ�����
	TK_EQ,		//==���ں�
	TK_LT,		//<С�ں�
	TK_LEQ,		//<=С�ڵ��ں�
	TK_GT,		//>���ں�
	TK_GEQ,		//>=���ڵ��ں�

	/* �ָ��� */
	TK_OPENPA,	//(��Բ����
	TK_CLOSEPA,	//)��Բ����
	TK_BEGIN_LEFT,	//{�������
	TK_END_RIGHT,	//}�Ҵ�����
	TK_COMMA,	//,����
	TK_SEMOCOLOM,	//;�ֺ�

	/* ���� */
	TK_INT,		//���ͳ���

	/* ��ʶ�� */
	TK_IDENT,

	/* ������ */
	TK_END
};

class LexAnalysis
{

public:

	// ���캯��
	LexAnalysis() = default;
	// ��Ա����
	void print(TokenCode code);
	void lexicalAnalysis(FILE* fp);
};

// ȫ�ֱ���
extern TokenCode code;
extern const int MAX;
extern int row;
extern string token;
extern char  keyWord[][7];
