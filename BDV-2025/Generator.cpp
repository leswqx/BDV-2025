#include "pch.h"
#include "Generator.h"
#include "Parm.h"
#include "LexAnalysis.h"
#include "IT.h"
#include "LT.h"
#include <sstream>
#include <cstring>
#include <fstream>
#include <stack>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

namespace Gener
{
	static std::unordered_set<int> currentFuncParamsTI;
	static bool inFunctionScope = false;
	static int funcBraceDepth = 0;

	inline string fmtId(const IT::Entry& e, int idxTI = -1)
	{
		if (idxTI >= 0 && inFunctionScope && currentFuncParamsTI.count(idxTI))
			return string("p_") + e.id;
		if (e.idtype == IT::IDTYPE::V || e.idtype == IT::IDTYPE::P)
			return string("v_") + e.id;
		if (e.idtype == IT::IDTYPE::L && !(e.id[0] == 'L' && e.id[1] == '_'))
			return string("L_") + e.id;
		return string(e.id);
	}

	inline string fmtFunc(const IT::Entry& e)
	{
		if (e.idtype == IT::IDTYPE::S)
			return string(e.id);
		return string("fn_") + e.id;
	}

	static int whilenum = 0;
	static int switchnum = 0;
	static int currentFuncParamBytes = 0;
	static std::string currentFuncName;
	string numberOfPoints = "k";
	string itoS(int x) { stringstream r;  r << x;  return r.str(); }

	string genCallFuncCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		IT::Entry e = ITENTRY(i);
		stack <int> temp;
		bool stnd = (e.idtype == IT::IDTYPE::S);

		for (int j = i + 1; LEXEMA(j) != LEX_RIGHTTHESIS; j++)
		{
			if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)
				temp.push(j);
		}
		str += "\n";
		while (!temp.empty())
		{
			int idxLex = temp.top();
			int idxTI = tables.lextable.table[idxLex].idxTI;
			IT::Entry ent = ITENTRY(idxLex);
			string id = fmtId(ent, idxTI);
			if (ent.idtype == IT::IDTYPE::L && ent.iddatatype == IT::IDDATATYPE::STR)
				str = str + "push offset " + id + "\n";
			else   str = str + "push " + id + "\n";
			temp.pop();
		}
		if (stnd)
			str += "push offset buffer\n";
		str = str + "call " + fmtFunc(e) + "\n";
		return str;
	}

	string genEqualCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		IT::Entry e1 = ITENTRY(i - 1);
		switch (e1.iddatatype)
		{
		case IT::IDDATATYPE::UINT:
		{
			for (int j = i + 1; LEXEMA(j) != LEX_SEPARATOR; j++)
			{
				switch (LEXEMA(j))
				{
				case LEX_LITERAL:
				case LEX_ID:
				{
					if (ITENTRY(j).idtype == IT::IDTYPE::F || ITENTRY(j).idtype == IT::IDTYPE::S)
					{
						str += genCallFuncCode(tables, log, j);
						str += "push eax\n";
						while (LEXEMA(j) != LEX_RIGHTTHESIS && j < tables.lextable.size) j++;
					}
					else  str += "push " + fmtId(ITENTRY(j), tables.lextable.table[j].idxTI) + "\n";
					break;
				}
				case LEX_PLUS:
					str += "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
				case LEX_MINUS:
					str += "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
				case LEX_STAR:
					str += "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
				case LEX_DIRSLASH:
				{
					string divLabel = "divsafe_" + itoS(numberOfPoints.length());
					numberOfPoints.push_back('d');
					str += "pop ebx\npop eax\ncmp ebx, 0\nje " + divLabel + "_zero\nxor edx, edx\ndiv ebx\npush eax\njmp " + divLabel + "_end\n" + divLabel + "_zero:\npush 0\n" + divLabel + "_end:\n";
					break;
				}
				case LEX_PERSENT:
				{
					string modLabel = "modsafe_" + itoS(numberOfPoints.length());
					numberOfPoints.push_back('m');
					str += "pop ebx\npop eax\ncmp ebx, 0\nje " + modLabel + "_zero\nxor edx, edx\ndiv ebx\npush edx\njmp " + modLabel + "_end\n" + modLabel + "_zero:\npush 0\n" + modLabel + "_end:\n";
					break;
				}
				case LEX_POWER2:
				{
					string powLabel = "pow_" + itoS(numberOfPoints.length());
					numberOfPoints.push_back('p');
					str += "pop ecx\npop eax\n";
					str += "mov ebx, eax\n";
					str += "cmp ecx, 0\n";
					str += "jne " + powLabel + "_start\n";
					str += "mov eax, 1\n";
					str += "jmp " + powLabel + "_end\n";
					str += powLabel + "_start:\n";
					str += "dec ecx\n";
					str += powLabel + "_loop:\n";
					str += "cmp ecx, 0\n";
					str += "je " + powLabel + "_end\n";
					str += "imul eax, ebx\n";
					str += "dec ecx\n";
					str += "jmp " + powLabel + "_loop\n";
					str += powLabel + "_end:\n";
					str += "push eax\n";
					break;
				}
				case LEX_SQUARE:
					str += "pop eax\nimul eax, eax\npush eax\n"; break;
				case LEX_MORE:
					str += "pop ebx\npop eax\ncmp eax, ebx\njg cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_LESS:
					str += "pop ebx\npop eax\ncmp eax, ebx\njl cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_EQUALS:
					str += "pop ebx\npop eax\ncmp eax, ebx\nje cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_NOTEQUALS:
					str += "pop ebx\npop eax\ncmp eax, ebx\njne cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_GREATEREQ:
					str += "pop ebx\npop eax\ncmp eax, ebx\njge cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_LESSEQ:
					str += "pop ebx\npop eax\ncmp eax, ebx\njle cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				}
			}

			str += "pop eax\nmov " + fmtId(e1, i - 1) + ", eax\n";
			break;
		}
		case IT::IDDATATYPE::STR:
		{
			char lex = LEXEMA(i + 1);
			IT::Entry e2 = ITENTRY(i + 1);
			if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S))
			{
				str += genCallFuncCode(tables, log, i + 1);
				if (e2.idtype == IT::IDTYPE::S)
				{
					string varName = fmtId(e1, i - 1);
					string bufName = varName + "_buf";
					string copyLabel = "copy_" + itoS(numberOfPoints.length());
					numberOfPoints.push_back('s');
					str += "; Copy string from buffer to variable's buffer\n";
					str += "push esi\npush edi\n";
					str += "mov esi, eax\n";
					str += "mov edi, offset " + bufName + "\n";
					str += copyLabel + "_loop:\n";
					str += "mov al, [esi]\n";
					str += "mov [edi], al\n";
					str += "cmp al, 0\n";
					str += "je " + copyLabel + "_end\n";
					str += "inc esi\ninc edi\n";
					str += "jmp " + copyLabel + "_loop\n";
					str += copyLabel + "_end:\n";
					str += "pop edi\npop esi\n";
					str += "mov " + varName + ", offset " + bufName + "\n";
				}
				else
				{
					str = str + "mov " + fmtId(e1, i - 1) + ", eax\n";
				}
			}
			else if (lex == LEX_LITERAL)
			{
				str = str + "mov " + fmtId(e1, i - 1) + ", offset " + fmtId(e2, i + 1) + "\n";
			}
			else
			{
				str = str + "mov ecx, " + fmtId(e2, i + 1) + "\nmov " + fmtId(e1, i - 1) + ", ecx\n";
			}
			break;
		}
		}
		return str;
	}

	string genWriteCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;

		std::vector<int> exprIdx;
		int j = i + 1;
		while (j < tables.lextable.size && LEXEMA(j) != LEX_SEPARATOR)
		{
			exprIdx.push_back(j);
			++j;
		}

		if (exprIdx.size() == 1 && !(ITENTRY(exprIdx[0]).idtype == IT::IDTYPE::F || ITENTRY(exprIdx[0]).idtype == IT::IDTYPE::S))
		{
			IT::Entry e = ITENTRY(exprIdx[0]);
			if (e.iddatatype == IT::IDDATATYPE::UINT)
				str = "push " + fmtId(e, tables.lextable.table[exprIdx[0]].idxTI) + "\ncall outUint\n";
			else
			{
				if (e.idtype == IT::IDTYPE::L)
					str = "push offset " + fmtId(e, tables.lextable.table[exprIdx[0]].idxTI) + "\ncall outString\n";
				else
					str = "push " + fmtId(e, tables.lextable.table[exprIdx[0]].idxTI) + "\ncall outString\n";
			}
			str += "push offset newline\ncall outString\n";
			return str;
		}

		if (exprIdx.size() == 1 && ITENTRY(exprIdx[0]).idtype == IT::IDTYPE::S)
		{
			IT::Entry e = ITENTRY(exprIdx[0]);
			str += genCallFuncCode(tables, log, exprIdx[0]);

			if (e.iddatatype == IT::IDDATATYPE::STR)
			{
				string copyLabel = "cout_copy_" + itoS(numberOfPoints.length());
				numberOfPoints.push_back('o');
				str += "; Copy string from buffer to temp buffer for output\n";
				str += "push esi\npush edi\n";
				str += "mov esi, eax\n";
				str += "mov edi, offset temp_str_buf\n";
				str += copyLabel + "_loop:\n";
				str += "mov al, [esi]\n";
				str += "mov [edi], al\n";
				str += "cmp al, 0\n";
				str += "je " + copyLabel + "_end\n";
				str += "inc esi\ninc edi\n";
				str += "jmp " + copyLabel + "_loop\n";
				str += copyLabel + "_end:\n";
				str += "pop edi\npop esi\n";
				str += "push offset temp_str_buf\ncall outString\n";
			}
			else
			{
				str += "push eax\ncall outUint\n";
			}
			str += "push offset newline\ncall outString\n";
			return str;
		}

		bool hasStringStdFunc = false;
		bool hasUintStdFunc = false;
		bool resultIsString = false;
		for (size_t k = 0; k < exprIdx.size(); ++k)
		{
			int idxLex = exprIdx[k];
			char lx = LEXEMA(idxLex);
			if (lx == LEX_ID || lx == LEX_LITERAL)
			{
				IT::Entry ent = ITENTRY(idxLex);
				if (ent.idtype == IT::IDTYPE::S)
				{
					if (ent.iddatatype == IT::IDDATATYPE::STR)
					{
						hasStringStdFunc = true;
						resultIsString = true;
					}
					else if (ent.iddatatype == IT::IDDATATYPE::UINT)
					{
						hasUintStdFunc = true;
						resultIsString = false;
					}
				}
				else if (ent.idtype == IT::IDTYPE::F && ent.iddatatype == IT::IDDATATYPE::STR)
				{
					resultIsString = true;
				}
				else if (ent.iddatatype == IT::IDDATATYPE::STR && ent.idtype != IT::IDTYPE::S)
				{
					if (!hasUintStdFunc)
						resultIsString = true;
				}
			}
		}

		for (size_t k = 0; k < exprIdx.size(); ++k)
		{
			int idxLex = exprIdx[k];
			char lx = LEXEMA(idxLex);
			if (lx == LEX_ID || lx == LEX_LITERAL)
			{
				IT::Entry ent = ITENTRY(idxLex);
				if (ent.idtype == IT::IDTYPE::F || ent.idtype == IT::IDTYPE::S)
				{
					str += genCallFuncCode(tables, log, idxLex);
					str += "push eax\n";
					while (k + 1 < exprIdx.size() && LEXEMA(exprIdx[k + 1]) != LEX_RIGHTTHESIS) ++k;
				}
				else
				{
					if (ent.iddatatype == IT::IDDATATYPE::STR && ent.idtype == IT::IDTYPE::L)
						str += "push offset " + fmtId(ent, tables.lextable.table[idxLex].idxTI) + "\n";
					else
						str += "push " + fmtId(ent, tables.lextable.table[idxLex].idxTI) + "\n";
				}
			}
			else
			{
				switch (lx)
				{
				case LEX_PLUS:      str += "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
				case LEX_MINUS:     str += "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
				case LEX_STAR:      str += "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
				case LEX_DIRSLASH:
				{
					string divLabel = "divsafe_" + itoS(numberOfPoints.length());
					numberOfPoints.push_back('d');
					str += "pop ebx\npop eax\ncmp ebx, 0\nje " + divLabel + "_zero\nxor edx, edx\ndiv ebx\npush eax\njmp " + divLabel + "_end\n" + divLabel + "_zero:\npush 0\n" + divLabel + "_end:\n";
					break;
				}
				case LEX_PERSENT:
				{
					string modLabel = "modsafe_" + itoS(numberOfPoints.length());
					numberOfPoints.push_back('m');
					str += "pop ebx\npop eax\ncmp ebx, 0\nje " + modLabel + "_zero\nxor edx, edx\ndiv ebx\npush edx\njmp " + modLabel + "_end\n" + modLabel + "_zero:\npush 0\n" + modLabel + "_end:\n";
					break;
				}
				case LEX_POWER2:
				{
					string powLabel = "pow_" + itoS(numberOfPoints.length());
					numberOfPoints.push_back('p');
					str += "pop ecx\npop eax\nmov ebx, eax\ncmp ecx, 0\njne " + powLabel + "_start\nmov eax, 1\njmp " + powLabel + "_end\n" + powLabel + "_start:\ndec ecx\n" + powLabel + "_loop:\ncmp ecx, 0\nje " + powLabel + "_end\nimul eax, ebx\ndec ecx\njmp " + powLabel + "_loop\n" + powLabel + "_end:\npush eax\n";
					break;
				}
				case LEX_SQUARE:
					str += "pop eax\nimul eax, eax\npush eax\n"; break;
				case LEX_MORE:
					str += "pop ebx\npop eax\ncmp eax, ebx\njg cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_LESS:
					str += "pop ebx\npop eax\ncmp eax, ebx\njl cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_EQUALS:
					str += "pop ebx\npop eax\ncmp eax, ebx\nje cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_NOTEQUALS:
					str += "pop ebx\npop eax\ncmp eax, ebx\njne cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_GREATEREQ:
					str += "pop ebx\npop eax\ncmp eax, ebx\njge cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				case LEX_LESSEQ:
					str += "pop ebx\npop eax\ncmp eax, ebx\njle cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
					numberOfPoints.push_back('c'); break;
				}
			}
		}

		str += "pop eax\n";

		if (hasUintStdFunc)
		{
			str += "push eax\ncall outUint\n";
		}
		else if (resultIsString && hasStringStdFunc)
		{
			string copyLabel = "cout_copy_" + itoS(numberOfPoints.length());
			numberOfPoints.push_back('o');
			str += "; Copy string from buffer to temp buffer for output\n";
			str += "push esi\npush edi\n";
			str += "mov esi, eax\n";
			str += "mov edi, offset temp_str_buf\n";
			str += copyLabel + "_loop:\n";
			str += "mov al, [esi]\n";
			str += "mov [edi], al\n";
			str += "cmp al, 0\n";
			str += "je " + copyLabel + "_end\n";
			str += "inc esi\ninc edi\n";
			str += "jmp " + copyLabel + "_loop\n";
			str += copyLabel + "_end:\n";
			str += "pop edi\npop esi\n";
			str += "push offset temp_str_buf\ncall outString\n";
		}
		else if (resultIsString)
		{
			str += "push eax\ncall outString\n";
		}
		else
		{
			str += "push eax\ncall outUint\n";
		}

		str += "push offset newline\ncall outString\n";
		return str;
	}

	string genWhileCode(Lexer::LEX& tables, Log::LOG& log, int i, int whileId)
	{
		string str;
		string whileLabel = "while_" + itoS(whileId);

		str += whileLabel + "_start:\n";

		for (int j = i + 2; LEXEMA(j) != LEX_RIGHTTHESIS; j++)
		{
			switch (LEXEMA(j))
			{
			case LEX_LITERAL:
			case LEX_ID:
				if (ITENTRY(j).idtype == IT::IDTYPE::F || ITENTRY(j).idtype == IT::IDTYPE::S)
				{
					str += genCallFuncCode(tables, log, j);
					str += "push eax\n";
					while (LEXEMA(j) != LEX_RIGHTTHESIS) j++;
				}
				else
					str += "push " + fmtId(ITENTRY(j), tables.lextable.table[j].idxTI) + "\n";
				break;
			case LEX_PLUS:
				str += "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
			case LEX_MINUS:
				str += "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
			case LEX_STAR:
				str += "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
			case LEX_DIRSLASH:
			{
				string divLabel = "divsafe_" + itoS(numberOfPoints.length());
				numberOfPoints.push_back('d');
				str += "pop ebx\npop eax\ncmp ebx, 0\nje " + divLabel + "_zero\nxor edx, edx\ndiv ebx\npush eax\njmp " + divLabel + "_end\n" + divLabel + "_zero:\npush 0\n" + divLabel + "_end:\n";
				break;
			}
			case LEX_PERSENT:
			{
				string modLabel = "modsafe_" + itoS(numberOfPoints.length());
				numberOfPoints.push_back('m');
				str += "pop ebx\npop eax\ncmp ebx, 0\nje " + modLabel + "_zero\nxor edx, edx\ndiv ebx\npush edx\njmp " + modLabel + "_end\n" + modLabel + "_zero:\npush 0\n" + modLabel + "_end:\n";
				break;
			}
			case LEX_POWER2:
			{
				string powLabel = "pow_" + itoS(numberOfPoints.length());
				numberOfPoints.push_back('p');
				str += "pop ecx\npop eax\n";
				str += "mov ebx, eax\n";
				str += "cmp ecx, 0\n";
				str += "jne " + powLabel + "_start\n";
				str += "mov eax, 1\n";
				str += "jmp " + powLabel + "_end\n";
				str += powLabel + "_start:\n";
				str += "dec ecx\n";
				str += powLabel + "_loop:\n";
				str += "cmp ecx, 0\n";
				str += "je " + powLabel + "_end\n";
				str += "imul eax, ebx\n";
				str += "dec ecx\n";
				str += "jmp " + powLabel + "_loop\n";
				str += powLabel + "_end:\n";
				str += "push eax\n";
				break;
			}
			case LEX_SQUARE:
				str += "pop eax\nimul eax, eax\npush eax\n"; break;
			case LEX_MORE:
				str += "pop ebx\npop eax\ncmp eax, ebx\njg cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_LESS:
				str += "pop ebx\npop eax\ncmp eax, ebx\njl cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_EQUALS:
				str += "pop ebx\npop eax\ncmp eax, ebx\nje cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_NOTEQUALS:
				str += "pop ebx\npop eax\ncmp eax, ebx\njne cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_GREATEREQ:
				str += "pop ebx\npop eax\ncmp eax, ebx\njge cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_LESSEQ:
				str += "pop ebx\npop eax\ncmp eax, ebx\njle cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			}
		}

		str += "pop eax\ncmp eax, 0\nje " + whileLabel + "_end\n";

		return str;
	}

	string genFunctionCode(Lexer::LEX& tables, int i, string& funcname)
	{
		string str;
		IT::Entry e = ITENTRY(i + 1);
		funcname = fmtFunc(e);
		currentFuncName = funcname;
		str = SEPSTR(funcname) + funcname + " PROC";

		currentFuncParamsTI.clear();
		inFunctionScope = true;
		funcBraceDepth = 0;

		int j = i + 3;
		vector<string> params;
		while (LEXEMA(j) != LEX_RIGHTTHESIS)
		{
			if (LEXEMA(j) == LEX_ID)
			{
				IT::Entry param = ITENTRY(j);
				int ti = tables.lextable.table[j].idxTI;
				currentFuncParamsTI.insert(ti);
				params.push_back(fmtId(param, ti) + (param.iddatatype == IT::IDDATATYPE::UINT ? " : dword" : " : dword"));
			}
			j++;
		}

		currentFuncParamBytes = static_cast<int>(params.size()) * 4;

		if (!params.empty())
		{
			str += " ";
			for (size_t k = 0; k < params.size(); k++)
			{
				str += params[k];
				if (k < params.size() - 1) str += ", ";
			}
		}
		str += "\n; --- save registers ---\npush ebx\npush edx\n; ----------------------\n";
		return str;
	}

	string genReturnCode(Lexer::LEX& tables, Log::LOG& log, int i, const string& funcname)
	{
		string str;

		for (int j = i + 1; j < tables.lextable.size
			&& LEXEMA(j) != LEX_SEPARATOR
			&& LEXEMA(j) != LEX_EQUAL
			&& LEXEMA(j) != LEX_WRITE
			&& LEXEMA(j) != LEX_WHILE
			&& LEXEMA(j) != LEX_FUNCTION
			&& LEXEMA(j) != LEX_MAIN
			&& LEXEMA(j) != LEX_RETURN; j++)
		{
			switch (LEXEMA(j))
			{
			case LEX_LITERAL:
			case LEX_ID:
			{
				if (ITENTRY(j).idtype == IT::IDTYPE::F || ITENTRY(j).idtype == IT::IDTYPE::S)
				{
					str += genCallFuncCode(tables, log, j);
					str += "push eax\n";
					while (LEXEMA(j) != LEX_RIGHTTHESIS) j++;
				}
				else
				{
					IT::Entry ent = ITENTRY(j);
					if (ent.iddatatype == IT::IDDATATYPE::STR && ent.idtype == IT::IDTYPE::L)
						str += "push offset " + fmtId(ent, tables.lextable.table[j].idxTI) + "\n";
					else
						str += "push " + fmtId(ent, tables.lextable.table[j].idxTI) + "\n";
				}
				break;
			}
			case LEX_PLUS:
				str += "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
			case LEX_MINUS:
				str += "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
			case LEX_STAR:
				str += "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
			case LEX_DIRSLASH:
			{
				string divLabel = "divsafe_" + itoS(numberOfPoints.length());
				numberOfPoints.push_back('d');
				str += "pop ebx\npop eax\ncmp ebx, 0\nje " + divLabel + "_zero\nxor edx, edx\ndiv ebx\npush eax\njmp " + divLabel + "_end\n" + divLabel + "_zero:\npush 0\n" + divLabel + "_end:\n";
				break;
			}
			case LEX_PERSENT:
			{
				string modLabel = "modsafe_" + itoS(numberOfPoints.length());
				numberOfPoints.push_back('m');
				str += "pop ebx\npop eax\ncmp ebx, 0\nje " + modLabel + "_zero\nxor edx, edx\ndiv ebx\npush edx\njmp " + modLabel + "_end\n" + modLabel + "_zero:\npush 0\n" + modLabel + "_end:\n";
				break;
			}
			case LEX_POWER2:
			{
				string powLabel = "pow_" + itoS(numberOfPoints.length());
				numberOfPoints.push_back('p');
				str += "pop ecx\npop eax\n";
				str += "mov ebx, eax\n";
				str += "cmp ecx, 0\n";
				str += "jne " + powLabel + "_start\n";
				str += "mov eax, 1\n";
				str += "jmp " + powLabel + "_end\n";
				str += powLabel + "_start:\n";
				str += "dec ecx\n";
				str += powLabel + "_loop:\n";
				str += "cmp ecx, 0\n";
				str += "je " + powLabel + "_end\n";
				str += "imul eax, ebx\n";
				str += "dec ecx\n";
				str += "jmp " + powLabel + "_loop\n";
				str += powLabel + "_end:\n";
				str += "push eax\n";
				break;
			}
			case LEX_SQUARE:
				str += "pop eax\nimul eax, eax\npush eax\n"; break;
			case LEX_MORE:
				str += "pop ebx\npop eax\ncmp eax, ebx\njg cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_LESS:
				str += "pop ebx\npop eax\ncmp eax, ebx\njl cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_EQUALS:
				str += "pop ebx\npop eax\ncmp eax, ebx\nje cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_NOTEQUALS:
				str += "pop ebx\npop eax\ncmp eax, ebx\njne cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_GREATEREQ:
				str += "pop ebx\npop eax\ncmp eax, ebx\njge cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			case LEX_LESSEQ:
				str += "pop ebx\npop eax\ncmp eax, ebx\njle cmp_true_" + numberOfPoints + "\nmov eax, 0\njmp cmp_end_" + numberOfPoints + "\ncmp_true_" + numberOfPoints + ":\nmov eax, 1\ncmp_end_" + numberOfPoints + ":\npush eax\n";
				numberOfPoints.push_back('c');
				break;
			}
		}

		str += "pop eax\n";

		str += "; --- restore registers ---\npop edx\npop ebx\n; -------------------------\n";
		str += "ret " + itoS(currentFuncParamBytes) + "\n";
		return str;
	}

	void CodeGeneration(Lexer::LEX& tables, Parm::PARM& parm, Log::LOG& log)
	{
		ofstream asmfile;
		std::wstring wfname = parm.out;
		std::string filename(wfname.begin(), wfname.end());
		asmfile.open(filename);

		if (!asmfile.is_open())
		{
			Log::writeLine(log.stream, const_cast<char*>("Ошибка: Не удалось создать файл ассемблера"), const_cast<char*>(""));
			return;
		}

		asmfile << BEGIN;
		asmfile << EXTERN;
		asmfile << CONST;

		asmfile << "\n" << DATA;
		for (int i = 0; i < tables.idtable.size; i++)
		{
			IT::Entry e = tables.idtable.table[i];
			std::string name = fmtId(e);
			if (e.idtype == IT::IDTYPE::V || e.idtype == IT::IDTYPE::P)
			{
				if (e.iddatatype == IT::IDDATATYPE::UINT)
					asmfile << "\n\t\t" << name << " dword ?";
				else if (e.iddatatype == IT::IDDATATYPE::STR)
				{
					asmfile << "\n\t\t" << name << " dword ?";
					asmfile << "\n\t\t" << name << "_buf byte 256 dup(0)";
				}
			}
			else if (e.idtype == IT::IDTYPE::L)
			{
				if (e.iddatatype == IT::IDDATATYPE::UINT)
					asmfile << "\n\t\t" << name << " dword " << e.value.vint;
				else if (e.iddatatype == IT::IDDATATYPE::STR)
				{
					if (e.value.vstr.len == 0 || strlen(e.value.vstr.str) == 0)
						asmfile << "\n\t\t" << name << " byte 0";
					else
						asmfile << "\n\t\t" << name << " byte \"" << e.value.vstr.str << "\", 0";
				}
			}
		}

		asmfile << "\n" << CODE;

		string funcname;
		stack<int> whileStack;
		stack<int> switchStack;
		std::vector<char> scopeStack;
		map<int, vector<pair<unsigned, string>>> switchCases;
		std::unordered_map<int, bool> switchCaseOpened;

		for (int i = 0; i < tables.lextable.size; i++)
		{
			char lex = LEXEMA(i);
			switch (lex)
			{
			case LEX_LEFTBRACE:
				if (inFunctionScope) funcBraceDepth++;
				break;

			case LEX_MAIN:
				asmfile << SEPSTR("MAIN") << "main PROC\n";
				break;

			case LEX_FUNCTION:
				asmfile << genFunctionCode(tables, i, funcname);
				break;

			case LEX_RETURN:
				asmfile << genReturnCode(tables, log, i, funcname);
				break;

			case LEX_EQUAL:
				asmfile << genEqualCode(tables, log, i);
				break;

			case LEX_WRITE:
				asmfile << genWriteCode(tables, log, i);
				break;

			case LEX_WHILE:
			{
				whilenum++;
				whileStack.push(whilenum);
				asmfile << genWhileCode(tables, log, i, whilenum);
				scopeStack.push_back('w');
				break;
			}

			case LEX_SWITCH:
			{
				switchnum++;
				switchStack.push(switchnum);
				IT::Entry expr = ITENTRY(i + 2);
				int exprIdx = tables.lextable.table[i + 2].idxTI;
				asmfile << "; --- switch ---\nmov eax, " << fmtId(expr, exprIdx) << "\n";

				int j = i;
				while (LEXEMA(j) != LEX_LEFTBRACE) j++;
				j++;

				vector<pair<unsigned, string>> cases;
				int caseNum = 0;
				while (LEXEMA(j) != LEX_BRACELET || j < tables.lextable.size)
				{
					if (LEXEMA(j) == LEX_CASE)
					{
						unsigned val = ITENTRY(j + 1).value.vint;
						string label = "case_" + itoS(switchnum) + "_" + itoS(caseNum++);
						cases.push_back({ val, label });
						asmfile << "cmp eax, " << val << "\nje " << label << "\n";
					}
					else if (LEXEMA(j) == LEX_DEFAULT)
					{
						string label = "default_" + itoS(switchnum);
						cases.push_back({ 0xFFFFFFFF, label });
					}
					else if (LEXEMA(j) == LEX_BRACELET)
						break;
					j++;
				}

				if (cases.back().first == 0xFFFFFFFF)
					asmfile << "jmp " << cases.back().second << "\n";
				else
					asmfile << "jmp switch_" << switchnum << "_end\n";

				switchCases[switchnum] = cases;
				switchCaseOpened[switchnum] = false;
				scopeStack.push_back('s');
				break;
			}

			case LEX_CASE:
			{
				if (!switchStack.empty())
				{
					int curSwitch = switchStack.top();
					if (switchCaseOpened[curSwitch])
						asmfile << "jmp switch_" << curSwitch << "_end\n";
					for (auto& c : switchCases[curSwitch])
					{
						if (c.first == ITENTRY(i + 1).value.vint)
						{
							asmfile << c.second << ":\n";
							break;
						}
					}
					switchCaseOpened[curSwitch] = true;
				}
				break;
			}

			case LEX_DEFAULT:
			{
				if (!switchStack.empty())
				{
					int curSwitch = switchStack.top();
					if (switchCaseOpened[curSwitch])
						asmfile << "jmp switch_" << curSwitch << "_end\n";
					asmfile << "default_" << curSwitch << ":\n";
					switchCaseOpened[curSwitch] = true;
				}
				break;
			}

			case LEX_BRACELET:
			{
				if (inFunctionScope && funcBraceDepth > 0)
					funcBraceDepth--;

				if (!scopeStack.empty())
				{
					char top = scopeStack.back();
					scopeStack.pop_back();
					if (top == 's' && !switchStack.empty())
					{
						int sw = switchStack.top();
						switchStack.pop();
						asmfile << "switch_" << sw << "_end:\n";
						switchCaseOpened.erase(sw);
						break;
					}
					if (top == 'w' && !whileStack.empty())
					{
						int w = whileStack.top();
						whileStack.pop();
						asmfile << "jmp while_" << w << "_start\nwhile_" << w << "_end:\n";
					}
				}
				else if (!switchStack.empty())
				{
					int sw = switchStack.top();
					switchStack.pop();
					asmfile << "switch_" << sw << "_end:\n";
					switchCaseOpened.erase(sw);
				}
				else if (!whileStack.empty())
				{
					int w = whileStack.top();
					whileStack.pop();
					asmfile << "jmp while_" << w << "_start\nwhile_" << w << "_end:\n";
				}
				if (inFunctionScope)
				{
					if (funcBraceDepth == 0)
					{
						asmfile << currentFuncName << " ENDP" << SEPSTREMP;
						inFunctionScope = false;
						currentFuncParamsTI.clear();
						currentFuncName.clear();
					}
				}
				break;
			}
			}
		}

		asmfile << "\n" << END;
		asmfile.close();

		Log::writeLine(log.stream, const_cast<char*>("Генерация ассемблерного кода завершена"), const_cast<char*>(""));
		Log::writeLine(&std::cout, const_cast<char*>("Файл ассемблера создан:"), const_cast<char*>(filename.c_str()), const_cast<char*>(""));
	}
}