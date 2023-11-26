#pragma once
#include "LexAnalysis.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stack>
#include <map>
#include <string>

using namespace std;

struct Generate
{
    string id;              // ����ʽ���
    string left;            // ����ʽ��
    string right[100];      // ����ʽ�Ҳ���Ĭ�����100��
    int rightNum;           // ����ʽ�Ҳ��ĸ���
};

struct Grammar
{
    int totalNum = 0;
    vector<Generate> g;
    vector<string> nonTerminal;     // ���ս������
    vector<string> terminal;        // �ս������
};

struct FIRST
{
    string id;              // ����
    vector<string> first;   // first����
};

struct CLOSURE_ITEM
{
    string id;              // ����ʽ���
    string left;            // ����ʽ��
    string right[100];      // ����ʽ�Ҳ���Ĭ�����100��
    int rightNum;           // ����ʽ�Ҳ��ĸ���
    int dot;                // ���λ��
    string lookahead;       // ��ǰ������
};

struct CLOSURE
{
    int num_of_item = 0;        // ��Ŀ������Ŀ�ĸ���
    vector<CLOSURE_ITEM> item;  // ��Ŀ��
};

struct ITEM_SET
{
    int num_of_closure = 0;     // ��Ŀ��������Ŀ���ĸ���
    vector<CLOSURE> closure;    // ��Ŀ����
};

struct LR_TABLE_ITEM
{
    string id;                  // ��Ŀ�����
    map<string, string> action; // action��
    map<string, string> go;     // goto��
};

struct LR_TABLE
{
    int num_of_item = 0;        // LR����������Ŀ�ĸ���
    vector<LR_TABLE_ITEM> item; // LR������
};


class SynAnalysis
{
public:
    SynAnalysis() = default;

    Grammar grammar;                    // ����ʽ����
    vector<FIRST> first;                // first����
    ITEM_SET item_set;                  // ��Ŀ����
    LR_TABLE lr_table;                  // LR������
    stack<int> status;                  // ״̬ջ
    stack<string> symbol;               // ����ջ
    vector<string> input;               // ���봮

    map <string, bool> haveFirst;       // ��¼��FIRST�����У��Ѿ��������ı���
    map <string, bool> inTerminate;     // ��¼�ս��
    map <string, bool> unTerminate;     // ��¼���ս��

    void ProcessGrammar();

    void GetFirst();
    void GetNonTerminalFirst(string s);
    FIRST GetStringFirst(vector<string> s);

    void GetItemSet();                  // ��ȡ��Ŀ����
    CLOSURE GetClosure(CLOSURE I);      // ��ȡ��Ŀ���ıհ�
    CLOSURE Goto(CLOSURE I, string X);  // ��ȡ��Ŀ����GOTO����

    void GetLRTable();                  // ��ȡLR������
    void SynAnalysisProcess();          // �﷨��������

    void Debug();

    bool addNew = true;                 // addNew�������ڼ���FIRST���ϵ�ʱ���õ�
};

extern vector <LexResult> lexResults;