/***********************************************
* �ʷ�������
* ���뻷����Visual Studio 2022
* by ��ʫ� ���� ������
***********************************************/

/***********************************************ͷ�ļ�**************************************************/
#include "LexAnalysis.h"
/**********************************************����*****************************************************/

/********************************************
* ���ܣ������ļ��е�ע��
* code�����ʶ�Ӧ���ֱ���
* token�����ڴ洢����
* row���������ڵ�����
*********************************************/


/********************************************
* ���ܣ���ӡ�ʷ������Ľ��
* code�����ʶ�Ӧ���ֱ���
* token�����ڴ洢����
* row���������ڵ�����
*********************************************/
void LexAnalysis::print(TokenCode code)
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
	case TK_BEGIN_LEFT:	//{�������
	case TK_END_RIGHT:	//}�Ҵ�����
	case TK_COMMA:	//,����
	case TK_SEMOCOLOM:	//;�ֺ�
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);	//������ͷָ���Ϊ��ɫ
		break;

		/* ���� */
	case TK_INT:	//���ͳ���
		/* ��ʶ�� */
	case TK_IDENT:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);	//�ؼ���Ϊ��ɫ
		break;
		/* ������ */
	case TK_END:   //#����
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
	{	//�ؼ��ֵ�����ֵΪkeyWord�����ж�Ӧ���±��3
		if (token.compare(keyWord[i]) == 0)
			return i + 3;
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
void LexAnalysis::lexicalAnalysis(FILE* fp)
{
	char ch;			//���ڴ洢���ļ��л�ȡ�ĵ����ַ�
	while ((ch = fgetc(fp)) != '#')	//δ��ȡ���ļ�β�����ļ��л�ȡһ���ַ�
	{
		token = ch;									//����ȡ���ַ�����token��
		if (ch == ' ' || ch == '\t' || ch == '\n')	//���Կո�Tab�ͻس�
		{
			if (ch == '\n')							//�������з�����¼������row��1
				row++;
			continue;								//����ִ��ѭ��
		}
		// �����кͶ���ע��
		else if (ch == '/')
		{
			ch = fgetc(fp);//�ٶ�һ��
			token = "";
			if(ch == '/') //�ٶ�һ����'/' ����ע��
			{
				token+="//";
				while ((ch = fgetc(fp)) != '\n')
					;//һֱ��������
				row++;
				code = TK_SINGAL_LINE_ANNO;
			}
			else if (ch == '*')  //�ٶ�һ����'*' ����ע��
			{
				token += "/*";
				while ((ch = fgetc(fp)) != '*')
				{
					if (ch == '\n')
						row++;
				}//һֱ������һ�� *��
				if ((ch = fgetc(fp)) == '/') //����һ����'/'��
				{
					token += " */";
					code = TK_MUTI_LINE_ANNO;
				}
			}
			//�����ǣ���Ϊ�ǳ���
			else
			{
				code = TK_DIVIDE;
				//�ļ�ָ�����һ���ֽڣ������¶�ȡ�������ʺ�ĵ�һ���ַ�
				fseek(fp, -1L, SEEK_CUR);
			}

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
			token = "";			//token��ʼ��
			while (isDigit(ch))	//��ǰ��ȡ�����ַ�Ϊ����
			{
				token.push_back(ch);		//��ȡ���֣��������token��
				ch = fgetc(fp);				//���ļ��л�ȡ��һ���ַ�
			}
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
		case '!':
		{
			ch = fgetc(fp);
			if (ch == '=')
			{
				token.push_back(ch);
				code = TK_NEQ;         //!=��
			}
			else
				fseek(fp, -1L, SEEK_CUR);
		}
		/*�ֽ��*/
		case '(': code = TK_OPENPA;		//(��Բ����
			break;
		case ')': code = TK_CLOSEPA;	//)��Բ����
			break;
		case '{': code = TK_BEGIN_LEFT;		//{�������
			break;
		case '}': code = TK_END_RIGHT;		//}�Ҵ�����
			break;
		case ',': code = TK_COMMA;		//,����
			break;
		case ';': code = TK_SEMOCOLOM;	//;�ֺ�
			break;
			//δʶ�����
		default: code = TK_UNDEF;
		}
		this->print(code);				//��ӡ�ʷ��������
	}
}


