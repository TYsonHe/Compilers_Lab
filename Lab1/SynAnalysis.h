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
    string id;              // 产生式编号
    string left;            // 产生式左部
    string right[100];      // 产生式右部，默认最多100个
    int rightNum;           // 产生式右部的个数
};

struct Grammar
{
    int totalNum = 0;
    vector<Generate> g;
    vector<string> nonTerminal;     // 非终结符集合
    vector<string> terminal;        // 终结符集合
};

struct FIRST
{
    string id;              // 符号
    vector<string> first;   // first集合
};

struct CLOSURE_ITEM
{
    string id;              // 产生式编号
    string left;            // 产生式左部
    string right[100];      // 产生式右部，默认最多100个
    int rightNum;           // 产生式右部的个数
    int dot;                // 点的位置
    string lookahead;       // 向前看符号
};

struct CLOSURE
{
    int num_of_item = 0;        // 项目集中项目的个数
    vector<CLOSURE_ITEM> item;  // 项目集
};

struct ITEM_SET
{
    int num_of_closure = 0;     // 项目集族中项目集的个数
    vector<CLOSURE> closure;    // 项目集族
};

struct LR_TABLE_ITEM
{
    string id;                  // 项目集编号
    map<string, string> action; // action表
    map<string, string> go;     // goto表
};

struct LR_TABLE
{
    int num_of_item = 0;        // LR分析表中项目的个数
    vector<LR_TABLE_ITEM> item; // LR分析表
};


class SynAnalysis
{
public:
    SynAnalysis() = default;

    Grammar grammar;                    // 产生式集合
    vector<FIRST> first;                // first集合
    ITEM_SET item_set;                  // 项目集族
    LR_TABLE lr_table;                  // LR分析表
    stack<int> status;                  // 状态栈
    stack<string> symbol;               // 符号栈
    vector<string> input;               // 输入串

    map <string, bool> haveFirst;       // 记录在FIRST运算中，已经遍历到的变量
    map <string, bool> inTerminate;     // 记录终结符
    map <string, bool> unTerminate;     // 记录非终结符

    void ProcessGrammar();

    void GetFirst();
    void GetNonTerminalFirst(string s);
    FIRST GetStringFirst(vector<string> s);

    void GetItemSet();                  // 获取项目集族
    CLOSURE GetClosure(CLOSURE I);      // 获取项目集的闭包
    CLOSURE Goto(CLOSURE I, string X);  // 获取项目集的GOTO集合

    void GetLRTable();                  // 获取LR分析表
    void SynAnalysisProcess();          // 语法分析过程

    void Debug();

    bool addNew = true;                 // addNew变量，在计算FIRST集合的时候用到
};

extern vector <LexResult> lexResults;