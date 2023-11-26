#include "SynAnalysis.h"

void SynAnalysis::ProcessGrammar()
{
    string file_name = "Grammar.txt";
    ifstream file(file_name);

    if (!file.is_open()) {
        cout << "文法文件打开失败！" << endl;
        return;
    }

    // 逐行读取处理
    string line;
    while (getline(file, line))
    {
        string str = line;
        if (str.length() == 0)
            continue;
        // 去掉行头的空格
        int i = 0;
        while (str[i] == ' ')
            i++;
        str = str.substr(i);
        // 以 # 开头的行为注释
        if (str[0] == '#')
            continue;
        // 以 @ 开头的行为终结符集合，以空格隔开
        if (str[0] == '@')
        {
            str = str.substr(1);
            int i = 0;
            while (str[i] == ' ')
                i++;
            str = str.substr(i);
            while (str.length() > 0)
            {
                i = 0;
                while (i < str.length() && str[i] != ' ' && str[i] != '\n' && str[i] != '\r')
                    i++;
                string temp = str.substr(0, i);
                grammar.terminal.push_back(temp);
                str = str.substr(i);
                while (str[0] == ' ')
                    str = str.substr(1);
            }
            continue;
        }
        Generate generate;
        int pos = str.find(':');
        generate.id = str.substr(0, pos);
        str = str.substr(pos + 1);
        // 寻找 -> 的位置
        pos = str.find("->");
        i = 0;
        while (i < pos && str[i] == ' ')
            i++;
        int jj = pos - 1;
        while (jj >= i && str[jj] == ' ')
            jj--;
        generate.left = str.substr(i, jj);
        if (find(grammar.nonTerminal.begin(), grammar.nonTerminal.end(), generate.left) == grammar.nonTerminal.end() && find(grammar.terminal.begin(), grammar.terminal.end(), generate.left) == grammar.terminal.end())
            grammar.nonTerminal.push_back(generate.left);
        str = str.substr(pos + 2);
        // 处理右部
        int rightNum = 0;
        while (str.length() > 0)
        {
            while (str[0] == ' ')
                str = str.substr(1);
            if (str.length() == 0)
                break;
            i = 0;
            while (i < str.length() && str[i] != ' ' && str[i] != '\n' && str[i] != '\r')
                i++;
            string temp = str.substr(0, i);
            int j = 0;
            while (j < temp.length() && temp[j] == ' ')
                j++;
            temp = temp.substr(j);
            str = str.substr(i);
            generate.right[rightNum++] = temp;
            if (find(grammar.nonTerminal.begin(), grammar.nonTerminal.end(), temp) == grammar.nonTerminal.end() && find(grammar.terminal.begin(), grammar.terminal.end(), temp) == grammar.terminal.end())
                grammar.nonTerminal.push_back(temp);
        }
        if (rightNum == 1 && generate.right[0] == "$")
            rightNum = 0;           // 空串
        generate.rightNum = rightNum;
        grammar.g.push_back(generate);
        grammar.totalNum++;
    }
    file.close();
    // 关掉输出颜色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "Generate num: " << grammar.totalNum << endl;
    cout << "Non-terminal: " << endl;
    for (int i = 0; i < grammar.nonTerminal.size(); i++)
        cout << grammar.nonTerminal[i] << endl, unTerminate[grammar.nonTerminal[i]] = true;
    cout << endl;
    cout << "Terminal: " << endl;
    for (int i = 0; i < grammar.terminal.size(); i++)
        cout << grammar.terminal[i] << " ", inTerminate[grammar.terminal[i]] = true;
    cout << endl;
    cout << "Grammar: " << endl;
    for (int i = 0; i < grammar.totalNum; i++)
    {
        cout << grammar.g[i].id << ": " << grammar.g[i].left << " -> ";
        for (int j = 0; j < grammar.g[i].rightNum; j++)
            cout << grammar.g[i].right[j] << " ";
        cout << endl;
    }
    cout << endl;
}

void SynAnalysis::GetFirst()
{
    // 先求终结符的first集合
    for (int i = 0; i < grammar.terminal.size(); i++)
    {
        FIRST temp;
        temp.id = grammar.terminal[i];
        temp.first.push_back(grammar.terminal[i]);
        first.push_back(temp);
    }
    // 再求非终结符的first集合
    for (int i = 0; i < grammar.nonTerminal.size(); i++)
    {
        FIRST temp;
        temp.id = grammar.nonTerminal[i];
        first.push_back(temp);
    }
    while (addNew)
    {
        addNew = false;
        for (int i = 0; i < grammar.nonTerminal.size(); i++)
            GetNonTerminalFirst(grammar.nonTerminal[i]);
    }
}

void SynAnalysis::GetNonTerminalFirst(string s)
{
    if (haveFirst[s] == true)
        return;
    haveFirst[s] = true;
    int nowid = -1;
    for (int i = 0; i < first.size(); i++)
        if (first[i].id == s)
        {
            nowid = i;
            break;
        }
    // 遍历所有产生式
    for (int i = 0; i < grammar.totalNum; i++)
    {
        if (grammar.g[i].left != s)
            continue;
        int nowpos = 0;
        // 考虑产生式右部为空的情况
        if (grammar.g[i].rightNum == 0)
        {
            if (find(first[nowid].first.begin(), first[nowid].first.end(), "$") == first[nowid].first.end())
            {
                first[nowid].first.push_back("$");
                addNew = true;
            }
            continue;
        }
        for (nowpos = 0; nowpos < grammar.g[i].rightNum; nowpos++)
        {
            string temp = grammar.g[i].right[nowpos];
            int itsid = -1;
            for (int j = 0; j < first.size(); j++)
                if (first[j].id == temp)
                {
                    itsid = j;
                    break;
                }
            if (inTerminate[temp] == true)
            {
                // 如果是终结符，直接加入first集合
                if (find(first[nowid].first.begin(), first[nowid].first.end(), temp) == first[nowid].first.end())
                {
                    first[nowid].first.push_back(temp);
                    addNew = true;
                }
            }
            else if (unTerminate[temp] == true)
            {
                if (temp == s && find(first[i].first.begin(), first[i].first.end(), "$") == first[i].first.end())
                    continue;
                else if (temp != s)
                {
                    GetNonTerminalFirst(temp);
                    // 将temp的first集合加入s的first集合，$除外
                    for (int j = 0; j < first[itsid].first.size(); j++)
                    {
                        if (first[itsid].first[j] == "$")
                            continue;
                        if (find(first[nowid].first.begin(), first[nowid].first.end(), first[itsid].first[j]) == first[nowid].first.end())
                        {
                            first[nowid].first.push_back(first[itsid].first[j]);
                            addNew = true;
                        }
                    }
                }
            }
            if (find(first[itsid].first.begin(), first[itsid].first.end(), "$") == first[itsid].first.end())
                break;
            if (nowpos == grammar.g[i].rightNum - 1)
            {
                if (find(first[nowid].first.begin(), first[nowid].first.end(), "$") == first[nowid].first.end())
                {
                    first[nowid].first.push_back("$");
                    addNew = true;
                }
            }
        }
    }
    haveFirst[s] = false;
}

FIRST SynAnalysis::GetStringFirst(vector<string> s)
{
    FIRST result;
    result.id = "";
    int ok = 1;
    if (s.size() == 0)
        return result;
    for (int i = 0; i < s.size(); i++)
    {
        if (inTerminate[s[i]] == true && s[i] != "$")
        {
            ok = 0;
            result.first.push_back(s[i]);
            break;
        }
        else if (unTerminate[s[i]] == true)
        {
            int nowid = -1;
            for (int j = 0; j < first.size(); j++)
                if (first[j].id == s[i])
                {
                    nowid = j;
                    break;
                }
            for (int j = 0; j < first[nowid].first.size(); j++)
            {
                int flag = 0;
                if (first[nowid].first[j] == "$")
                    continue;
                for (int k = 0; k < result.first.size(); k++)
                    if (result.first[k] == first[nowid].first[j])
                    {
                        flag = 1;
                        break;
                    }
                if (flag == 0)
                    result.first.push_back(first[nowid].first[j]);
            }
            if (find(first[nowid].first.begin(), first[nowid].first.end(), "$") == first[nowid].first.end())
            {
                ok = 0;
                break;
            }
        }
    }
    if (ok == 1)
        result.first.push_back("$");
    return result;
}

void SynAnalysis::GetItemSet()
{
    item_set.num_of_closure = 1;
    CLOSURE_ITEM temp;
    temp.id = grammar.g[0].id;
    temp.left = grammar.g[0].left;
    temp.dot = 0;
    temp.rightNum = grammar.g[0].rightNum;
    temp.lookahead = "#";
    for (int i = 0; i < grammar.g[0].rightNum; i++)
        temp.right[i] = grammar.g[0].right[i];
    CLOSURE temp_closure;
    temp_closure.item.push_back(temp);
    temp_closure.num_of_item = 1;
    item_set.closure.push_back(GetClosure(temp_closure));
    int old_num = 0, new_num = item_set.num_of_closure;
    while (1)
    {
        if (old_num == new_num)
            break;
        for (int i = old_num; i < new_num; i++)
        {
            for (int j = 0; j < grammar.terminal.size(); j++)
            {
                CLOSURE temp = Goto(item_set.closure[i], grammar.terminal[j]);
                if (temp.num_of_item == 0)
                    continue;
                int flag = 0;
                // 判断是否已经存在
                for (int k = 0; k < item_set.num_of_closure; k++)
                    if (item_set.closure[k].num_of_item == temp.num_of_item)
                    {
                        int l = 0;
                        for (l = 0; l < temp.num_of_item; l++)
                            if (item_set.closure[k].item[l].id != temp.item[l].id || item_set.closure[k].item[l].left != temp.item[l].left || item_set.closure[k].item[l].dot != temp.item[l].dot || item_set.closure[k].item[l].rightNum != temp.item[l].rightNum || item_set.closure[k].item[l].lookahead != temp.item[l].lookahead)
                                break;
                        if (l == temp.num_of_item)
                        {
                            flag = 1;
                            break;
                        }
                    }
                if (flag == 0)
                {
                    item_set.closure.push_back(temp);
                    item_set.num_of_closure++;
                }
            }
            for (int j = 0; j < grammar.nonTerminal.size(); j++)
            {
                CLOSURE temp = Goto(item_set.closure[i], grammar.nonTerminal[j]);
                if (temp.num_of_item == 0)
                    continue;
                int flag = 0;
                // 判断是否已经存在
                for (int k = 0; k < item_set.num_of_closure; k++)
                    if (item_set.closure[k].num_of_item == temp.num_of_item)
                    {
                        int l = 0;
                        for (l = 0; l < temp.num_of_item; l++)
                            if (item_set.closure[k].item[l].id != temp.item[l].id || item_set.closure[k].item[l].left != temp.item[l].left || item_set.closure[k].item[l].dot != temp.item[l].dot || item_set.closure[k].item[l].rightNum != temp.item[l].rightNum || item_set.closure[k].item[l].lookahead != temp.item[l].lookahead)
                                break;
                        if (l == temp.num_of_item)
                        {
                            flag = 1;
                            break;
                        }
                    }
                if (flag == 0)
                {
                    item_set.closure.push_back(temp);
                    item_set.num_of_closure++;
                }
            }
        }
        old_num = new_num;
        new_num = item_set.num_of_closure;
    }
    // 输出项目集族到文件
    string file_name = "ItemSet.txt";
    ofstream file(file_name);
    if (!file.is_open()) {
        cout << "项目集族文件打开失败！" << endl;
        return;
    }
    for (int i = 0; i < item_set.num_of_closure; i++)
    {
        file << "I" << i << ":" << endl;
        for (int j = 0; j < item_set.closure[i].num_of_item; j++)
        {
            file << item_set.closure[i].item[j].id << ": " << item_set.closure[i].item[j].left << " -> ";
            for (int k = 0; k < item_set.closure[i].item[j].rightNum; k++)
            {
                if (k == item_set.closure[i].item[j].dot)
                    file << "." << " ";
                file << item_set.closure[i].item[j].right[k] << " ";
            }
            if (item_set.closure[i].item[j].dot == item_set.closure[i].item[j].rightNum)
                file << "." << " ";
            file << "lookahead: " << item_set.closure[i].item[j].lookahead << endl;
        }
        file << endl;
    }
    file.close();
}

CLOSURE SynAnalysis::GetClosure(CLOSURE I)
{
    CLOSURE result;
    // I的任何项目都属于 CLOSURE(I)
    for (int i = 0; i < I.num_of_item; i++)
        result.item.push_back(I.item[i]);
    result.num_of_item = I.num_of_item;
    int old_num = 0, new_num = result.num_of_item;
    while (1)
    {
        if (old_num == new_num)
            break;
        for (int i = old_num; i < new_num; i++)
        {
            int pos = result.item[i].dot;
            if (pos == result.item[i].rightNum)
                continue;
            if (unTerminate[result.item[i].right[pos]] == true)
            {
                for (int j = 0; j < grammar.totalNum; j++)
                {
                    if (grammar.g[j].left != result.item[i].right[pos])
                        continue;
                    vector <string> now_right;
                    for (int k = pos + 1; k < result.item[i].rightNum; k++)
                        now_right.push_back(result.item[i].right[k]);
                    now_right.push_back(result.item[i].lookahead);
                    FIRST temp = GetStringFirst(now_right);
                    for (int k = 0; k < temp.first.size(); k++)
                    {
                        CLOSURE_ITEM temp_item;
                        temp_item.id = grammar.g[j].id;
                        temp_item.left = grammar.g[j].left;
                        temp_item.dot = 0;
                        temp_item.rightNum = grammar.g[j].rightNum;
                        for (int l = 0; l < grammar.g[j].rightNum; l++)
                            temp_item.right[l] = grammar.g[j].right[l];
                        temp_item.lookahead = temp.first[k];
                        // if (temp_item.lookahead == "$")
                        //     continue;
                        int flag = 0;
                        for (int l = 0; l < result.num_of_item; l++)
                            if (result.item[l].id == temp_item.id && result.item[l].left == temp_item.left && result.item[l].dot == temp_item.dot && result.item[l].rightNum == temp_item.rightNum && result.item[l].lookahead == temp_item.lookahead)
                            {
                                flag = 1;
                                break;
                            }
                        if (flag == 0)
                        {
                            result.item.push_back(temp_item);
                            result.num_of_item++;
                        }
                    }
                }
            }
        }
        old_num = new_num;
        new_num = result.num_of_item;
    }
    return result;
}

CLOSURE SynAnalysis::Goto(CLOSURE I, string X)
{
    CLOSURE_ITEM temp;
    CLOSURE result;
    for (int i = 0; i < I.num_of_item; i++)
    {
        if (I.item[i].dot == I.item[i].rightNum)
            continue;
        int pos = I.item[i].dot;
        if (I.item[i].right[pos] == X)
        {
            temp.id = I.item[i].id;
            temp.left = I.item[i].left;
            temp.dot = I.item[i].dot + 1;
            temp.rightNum = I.item[i].rightNum;
            for (int j = 0; j < I.item[i].rightNum; j++)
                temp.right[j] = I.item[i].right[j];
            temp.lookahead = I.item[i].lookahead;
            result.item.push_back(temp);
            result.num_of_item++;
        }
    }
    return GetClosure(result);
}

void SynAnalysis::GetLRTable()
{
    lr_table.num_of_item = item_set.num_of_closure;
    // 初始化
    for (int i = 0; i < item_set.num_of_closure; i++)
    {
        LR_TABLE_ITEM temp;
        temp.id = "I" + to_string(i);
        lr_table.item.push_back(temp);
    }
    // 遍历所有项目集
    for (int i = 0; i < item_set.num_of_closure; i++)
    {
        // 遍历项目集中的所有项目
        for (int j = 0; j < item_set.closure[i].num_of_item; j++)
        {
            // 判断ACC的情况
            if (item_set.closure[i].item[j].left == "S" && item_set.closure[i].item[j].dot == item_set.closure[i].item[j].rightNum && item_set.closure[i].item[j].lookahead == "#")
            {
                lr_table.item[i].action["#"] = "ACC";
                continue;
            }
            // 判断移入的情况
            if (item_set.closure[i].item[j].dot < item_set.closure[i].item[j].rightNum)
            {
                string temp = item_set.closure[i].item[j].right[item_set.closure[i].item[j].dot];   // 点后面的符号
                if (inTerminate[temp] == true)
                {
                    CLOSURE temp_closure = Goto(item_set.closure[i], temp);
                    for (int k = 0; k < item_set.num_of_closure; k++)
                    {
                        int flag = 0;
                        if (temp_closure.num_of_item != item_set.closure[k].num_of_item)    // 项目集中项目个数不同，肯定不是同一个项目集
                            continue;
                        // 去比较每一个项目内容是否相同
                        for (int l = 0; l < temp_closure.num_of_item; l++)
                        {
                            // 取出该项目集的第l个项目
                            CLOSURE_ITEM temp_item;
                            temp_item.id = item_set.closure[k].item[l].id;
                            temp_item.left = item_set.closure[k].item[l].left;
                            temp_item.dot = item_set.closure[k].item[l].dot;
                            temp_item.rightNum = item_set.closure[k].item[l].rightNum;
                            for (int m = 0; m < item_set.closure[k].item[l].rightNum; m++)
                                temp_item.right[m] = item_set.closure[k].item[l].right[m];
                            temp_item.lookahead = item_set.closure[k].item[l].lookahead;
                            // 观察temp_closure中是否存在该项目
                            int flag2 = 0;
                            for (int m = 0; m < temp_closure.num_of_item; m++)
                                if (temp_closure.item[m].id == temp_item.id && temp_closure.item[m].left == temp_item.left && temp_closure.item[m].dot == temp_item.dot && temp_closure.item[m].rightNum == temp_item.rightNum && temp_closure.item[m].lookahead == temp_item.lookahead)
                                {
                                    flag2 = 1;
                                    break;
                                }
                            if (flag2 == 0)
                            {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0)
                        {
                            lr_table.item[i].action[temp] = "S" + to_string(k);
                            break;
                        }
                    }
                }
            }
            // 判断归约的情况
            if (item_set.closure[i].item[j].dot == item_set.closure[i].item[j].rightNum)
            {
                // 遍历所有产生式
                for (int k = 0; k < grammar.totalNum; k++)
                {
                    if (grammar.g[k].left != item_set.closure[i].item[j].left)
                        continue;
                    // 文法的右边应该和项目的右边相同
                    int flag = 0;
                    if (grammar.g[k].rightNum != item_set.closure[i].item[j].rightNum)
                        continue;
                    for (int l = 0; l < grammar.g[k].rightNum; l++)
                    {
                        if (grammar.g[k].right[l] != item_set.closure[i].item[j].right[l])
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1)
                        continue;
                    // 遍历所有终结符
                    for (int l = 0; l < grammar.terminal.size(); l++)
                    {
                        if (grammar.terminal[l] == item_set.closure[i].item[j].lookahead)
                        {
                            lr_table.item[i].action[grammar.terminal[l]] = "r" + to_string(k);
                            break;
                        }
                    }
                }
            }
            // 判断goto的情况
            for (int k = 0; k < grammar.nonTerminal.size(); k++)
            {
                CLOSURE temp_closure = Goto(item_set.closure[i], grammar.nonTerminal[k]);
                for (int l = 0; l < item_set.num_of_closure; l++)
                {
                    int flag = 0;
                    if (temp_closure.num_of_item != item_set.closure[l].num_of_item)    // 项目集中项目个数不同，肯定不是同一个项目集
                        continue;
                    // 去比较每一个项目内容是否相同
                    for (int m = 0; m < temp_closure.num_of_item; m++)
                    {
                        // 取出该项目集的第m个项目
                        CLOSURE_ITEM temp_item;
                        temp_item.id = item_set.closure[l].item[m].id;
                        temp_item.left = item_set.closure[l].item[m].left;
                        temp_item.dot = item_set.closure[l].item[m].dot;
                        temp_item.rightNum = item_set.closure[l].item[m].rightNum;
                        for (int n = 0; n < item_set.closure[l].item[m].rightNum; n++)
                            temp_item.right[n] = item_set.closure[l].item[m].right[n];
                        temp_item.lookahead = item_set.closure[l].item[m].lookahead;
                        // 观察temp_closure中是否存在该项目
                        int flag2 = 0;
                        for (int n = 0; n < temp_closure.num_of_item; n++)
                            if (temp_closure.item[n].id == temp_item.id && temp_closure.item[n].left == temp_item.left && temp_closure.item[n].dot == temp_item.dot && temp_closure.item[n].rightNum == temp_item.rightNum && temp_closure.item[n].lookahead == temp_item.lookahead)
                            {
                                flag2 = 1;
                                break;
                            }
                        if (flag2 == 0)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        lr_table.item[i].go[grammar.nonTerminal[k]] = "I" + to_string(l);
                        break;
                    }
                }
            }
        }
    }
    // 输出LR分析表到文件
    string file_name = "LRTable.txt";
    ofstream file(file_name);
    if (!file.is_open()) {
        cout << "LR分析表文件打开失败！" << endl;
        return;
    }
    for (int i = 0; i < lr_table.num_of_item; i++)
    {
        file << "I" << i << ":" << endl;
        for (auto it = lr_table.item[i].action.begin(); it != lr_table.item[i].action.end(); it++)
            file << "action[" << it->first << "] = " << it->second << endl;
        for (auto it = lr_table.item[i].go.begin(); it != lr_table.item[i].go.end(); it++)
            file << "go[" << it->first << "] = " << it->second << endl;
        file << endl;
    }
}

void SynAnalysis::SynAnalysisProcess()
{
    // 初始化相关栈
    status.push(0);
    symbol.push("#");
    // 读取token序列
    for (int i = 0; i < lexResults.size(); i++)
        input.push_back(lexResults[i].token);
    input.push_back("#");
    int now_pos = 0;
    while (1)
    {
        // 输出当前栈的状态
        cout << "status: ";
        stack<int> temp_status = status;
        while (!temp_status.empty())
        {
            cout << temp_status.top() << " ";
            temp_status.pop();
        }
        cout << endl;
        cout << "symbol: ";
        stack<string> temp_symbol = symbol;
        while (!temp_symbol.empty())
        {
            cout << temp_symbol.top() << " ";
            temp_symbol.pop();
        }
        cout << endl;
        cout << "input: ";
        for (int i = now_pos; i < input.size(); i++)
            cout << input[i] << " ";
        cout << endl;
        // 获取当前状态
        int now_status = status.top();
        // 获取当前输入符号
        string now_input = input[now_pos];
        // 忽略注释和结束符
        if (lexResults[now_pos].code == TK_SINGAL_LINE_ANNO || lexResults[now_pos].code == TK_MUTI_LINE_ANNO || lexResults[now_pos].code == TK_END)
        {
            now_pos++;
            continue;
        }
        // 特别的，如果当前输入符号为标识符或者常数，需要将其种别码也加入
        if (lexResults[now_pos].code == TK_IDENT)
            now_input = "id";
        else if (lexResults[now_pos].code == TK_INT)
            now_input = "num";
        // 获取当前状态的action表和goto表
        map<string, string> now_action = lr_table.item[now_status].action;
        map<string, string> now_go = lr_table.item[now_status].go;
        // cout << "action: ";
        // for (auto it = now_action.begin(); it != now_action.end(); it++)
        //     cout << it->first << " " << it->second << " ";
        // cout << endl;
        // cout << "go: ";
        // for (auto it = now_go.begin(); it != now_go.end(); it++)
        //     cout << it->first << " " << it->second << " ";
        // cout << endl;
        cout << "now_input: " << now_input << endl;
        // 判断是否为移入
        if (now_action.find(now_input) != now_action.end() && now_action[now_input][0] == 'S')
        {
            // 移入
            int next_status = stoi(now_action[now_input].substr(1));
            status.push(next_status);
            symbol.push(now_input);
            now_pos++;
        }
        // 判断是否为归约
        else if (now_action.find(now_input) != now_action.end() && now_action[now_input][0] == 'r')
        {
            // 归约
            int num = stoi(now_action[now_input].substr(1));
            int rightNum = grammar.g[num].rightNum;
            // 弹出rightNum个状态
            for (int i = 0; i < rightNum; i++)
                status.pop();
            // 弹出rightNum个符号
            for (int i = 0; i < rightNum; i++)
                symbol.pop();
            // 获取当前状态
            int now_status = status.top();
            // 获取当前输入符号
            string now_input = grammar.g[num].left;
            // 获取当前状态的goto表
            map<string, string> now_go = lr_table.item[now_status].go;
            // 将归约的产生式左部加入符号栈
            symbol.push(now_input);
            // 将goto表中的状态加入状态栈
            status.push(stoi(now_go[now_input].substr(1)));
        }
        // 判断是否为接受
        else if (now_action.find(now_input) != now_action.end() && now_action[now_input] == "ACC")
        {
            // 接受
            cout << "Accept!" << endl;
            break;
        }
        else
        {
            cout << "Error!" << endl;
            break;
        }
    }
}

void SynAnalysis::Debug()
{
    cout << "FIRST:" << endl;
    for (int i = 0; i < first.size(); i++)
    {
        cout << first[i].id << ": ";
        for (int j = 0; j < first[i].first.size(); j++)
            cout << first[i].first[j] << " ";
        cout << endl;
    }
    vector <string> s;
    s.push_back("$");
    s.push_back(")");
    vector <string> v;
    v = GetStringFirst(s).first;
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
}



