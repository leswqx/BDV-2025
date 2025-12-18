#include "pch.h"
#include "PolishNotation.h"
#include "Header.h"
#include <stack>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

namespace Polish
{
	int getPriority(LT::Entry& e)
	{
		switch (e.lexema)
		{
		case LEX_LEFTHESIS:
		case LEX_RIGHTTHESIS:
			return 0;

		case LEX_MORE:
		case LEX_LESS:
		case LEX_EQUALS:
		case LEX_NOTEQUALS:
		case LEX_GREATEREQ:
		case LEX_LESSEQ:
			return 1;

		case LEX_PLUS:
		case LEX_MINUS:
			return 2;

		case LEX_STAR:
		case LEX_DIRSLASH:
		case LEX_PERSENT:
			return 3;

		case LEX_POWER2: 
			return 4;

		case LEX_SQUARE:
			return 5;

		default:
			return -1;
		}
	}


	struct ExprInfo
	{
		int start;
		char terminator;
	};

	typedef std::vector<LT::Entry> ltvec;
	typedef std::vector<ExprInfo> exprvec;

	exprvec getExprPositions(Lexer::LEX& tbls);
	int fillVector(int posExprBegin, char terminator, LT::LexTable& lextable, ltvec& v);
	bool setPolishNotation(IT::IdTable& idtable, Log::LOG& log, ltvec& v);
	void addToTable(LT::LexTable& new_table, IT::IdTable& idtable, ltvec& v);

	static int polishExprCounter = 0;

	std::string lexToString(const LT::Entry& e, IT::IdTable& idtable)
	{
		switch (e.lexema)
		{
		case LEX_ID:
		case LEX_LITERAL:
			return std::string(idtable.table[e.idxTI].id);
		case LEX_EQUAL:      return "=";
		case LEX_PLUS:       return "+";
		case LEX_MINUS:      return "-";
		case LEX_STAR:       return "*";
		case LEX_DIRSLASH:   return "/";
		case LEX_PERSENT:    return "%";
		case LEX_POWER2:     return "^";
		case LEX_SQUARE:     return "&";
		case LEX_MORE:       return ">";
		case LEX_LESS:       return "<";
		case LEX_GREATEREQ:  return ">=";
		case LEX_LESSEQ:     return "<=";
		case LEX_EQUALS:     return "==";
		case LEX_NOTEQUALS:  return "!=";
		case LEX_LEFTHESIS:  return "(";
		case LEX_RIGHTTHESIS:return ")";
		default:
			return std::string(1, e.lexema);
		}
	}

	bool PolishNotation(Lexer::LEX& tbls, Log::LOG& log)
	{
		unsigned curExprBegin = 0;
		ltvec v;
		LT::LexTable new_table = LT::Create(tbls.lextable.maxsize);
		exprvec vpositions = getExprPositions(tbls);

		for (int i = 0; i < tbls.lextable.size; i++)
		{
			if (curExprBegin < vpositions.size() && i == vpositions[curExprBegin].start)
			{
				int lexcount = fillVector(vpositions[curExprBegin].start, vpositions[curExprBegin].terminator, tbls.lextable, v);
				if (lexcount > 1) {
					if (!setPolishNotation(tbls.idtable, log, v)) return false;
				}
				addToTable(new_table, tbls.idtable, v);
				i += lexcount - 1;
				curExprBegin++;
				continue;	
			}
			if (tbls.lextable.table[i].lexema == LEX_ID || tbls.lextable.table[i].lexema == LEX_LITERAL)
			{
				int firstind = Lexer::getIndexInLT(new_table, tbls.lextable.table[i].idxTI);
				if (firstind == -1) firstind = new_table.size;
				tbls.idtable.table[tbls.lextable.table[i].idxTI].idxfirstLE = firstind;
			}
			LT::Add(new_table, tbls.lextable.table[i]);
		}
		delete[] tbls.lextable.table;
		tbls.lextable = new_table;
		return true;
	}

	exprvec getExprPositions(Lexer::LEX& tbls)
	{
		exprvec v;
		for (int i = 0; i < tbls.lextable.size; i++)
		{
			char lex = tbls.lextable.table[i].lexema;
			switch (lex)
			{
			case LEX_EQUAL:
			case LEX_RETURN:
			case LEX_WRITE:
				v.push_back({ i + 1, LEX_SEPARATOR });
				break;
			case LEX_WHILE:
			case LEX_SWITCH:
				if (i + 2 < tbls.lextable.size)
					v.push_back({ i + 2, LEX_RIGHTTHESIS });
				break;
			default:
				break;
			}
		}
		return v;
	}

	int fillVector(int posExprBegin, char terminator, LT::LexTable& lextable, ltvec& v)
	{
		v.clear();
		for (int i = posExprBegin; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema == terminator) break;
			v.push_back(lextable.table[i]);
		}
		return (int)v.size();
	}

	bool setPolishNotation(IT::IdTable& idtable, Log::LOG& log, ltvec& v)
	{
		ltvec result;
		stack<LT::Entry> s;

		for (unsigned i = 0; i < v.size(); i++)
		{
			char lex = v[i].lexema;

			if (lex == LEX_ID)
			{
				IT::Entry id = idtable.table[v[i].idxTI];

				if (id.idtype == IT::IDTYPE::F || id.idtype == IT::IDTYPE::S)
				{
					result.push_back(v[i]);
					int depth = 0;
					for (unsigned k = i + 1; k < v.size(); k++)
					{
						result.push_back(v[k]);
						if (v[k].lexema == LEX_LEFTHESIS) depth++;
						else if (v[k].lexema == LEX_RIGHTTHESIS)
						{
							if (depth == 0) { i = k; break; }
							depth--;
							if (depth == 0) { i = k; break; }
						}
					}
					continue;
				}
			}

			if (lex == LEX_ID || lex == LEX_LITERAL)
				result.push_back(v[i]);

			else if (lex == LEX_POWER2 || lex == LEX_SQUARE || lex == LEX_PLUS || lex == LEX_MINUS ||
				lex == LEX_STAR || lex == LEX_DIRSLASH || lex == LEX_PERSENT ||
				lex == LEX_MORE || lex == LEX_LESS || lex == LEX_EQUALS || lex == LEX_NOTEQUALS ||
				lex == LEX_GREATEREQ || lex == LEX_LESSEQ ||
				lex == LEX_LEFTHESIS || lex == LEX_RIGHTTHESIS)
			{
				if (s.empty() || lex == LEX_LEFTHESIS) s.push(v[i]);
				else if (lex == LEX_RIGHTTHESIS)
				{
					while (!s.empty() && s.top().lexema != LEX_LEFTHESIS) { 
						result.push_back(s.top());
						s.pop();
					}

					if (!s.empty()) s.pop();
				}
				else
				{
					if (lex == LEX_SQUARE)
					{
						result.push_back(v[i]);
					}
					else
					{
						while (!s.empty() && getPriority(s.top()) >= getPriority(v[i])) { 
							result.push_back(s.top());
							s.pop();
						}
						s.push(v[i]);
					}
				}
			}
		}
		while (!s.empty()) { result.push_back(s.top()); s.pop(); }
		v = result;

		if (log.stream != nullptr)
		{
			std::ostringstream oss;
			oss << "\n[POLISH " << (++polishExprCounter) << "] ";
			for (size_t k = 0; k < v.size(); k++)
			{
				if (k) oss << ' ';
				oss << lexToString(v[k], idtable);
			}
			*log.stream << oss.str() << std::endl;
		}
		return true;
	}

	void addToTable(LT::LexTable& new_table, IT::IdTable& idtable, ltvec& v)
	{
		for (unsigned i = 0; i < v.size(); i++)
		{
			LT::Add(new_table, v[i]);
			if (v[i].lexema == LEX_ID || v[i].lexema == LEX_LITERAL)
			{
				int firstind = Lexer::getIndexInLT(new_table, v[i].idxTI);
				idtable.table[v[i].idxTI].idxfirstLE = firstind;
			}
		}
	}
}