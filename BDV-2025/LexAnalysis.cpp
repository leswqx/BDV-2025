#include "pch.h"
#include "LexAnalysis.h"
#include "Graphs.h"
#include "Error.h"
#include "Log.h"
#include <stack>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cerrno>

#pragma warning(disable : 4996)

namespace Lexer
{
	Graph graphs[N_GRAPHS] =
	{
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS) },
		{ LEX_EQUALS,    FST::FST(GRAPH_EQUAL) },
		{ LEX_NOTEQUALS, FST::FST(GRAPH_NOTEQUAL) },
		{ LEX_GREATEREQ, FST::FST(GRAPH_GREATEREQ) },
		{ LEX_LESSEQ,    FST::FST(GRAPH_LESSEQ) },
		{ LEX_LITERAL, FST::FST(GRAPH_INT_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
		{ LEX_NEW,     FST::FST(GRAPH_VAR) },
		{ LEX_MAIN,    FST::FST(GRAPH_MAIN) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_UINT) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_STRING) },
		{ LEX_FUNCTION,FST::FST(GRAPH_FUNCTION) },
		{ LEX_RETURN,  FST::FST(GRAPH_RETURN) },
		{ LEX_WRITE,   FST::FST(GRAPH_WRITE) },
		{ LEX_SWITCH,  FST::FST(GRAPH_SWITCH) },
		{ LEX_CASE,    FST::FST(GRAPH_CASE) },
		{ LEX_DEFAULT, FST::FST(GRAPH_DEFAULT) },
		{ LEX_WHILE,   FST::FST(GRAPH_WHILE) },
		{ LEX_ID,      FST::FST(GRAPH_STRTOUINT) },
		{ LEX_ID,      FST::FST(GRAPH_UINTTOSTR) },
		{ LEX_ID,      FST::FST(GRAPH_STRLEN) },
		{ LEX_ID,      FST::FST(GRAPH_STRCAT) },
		{ LEX_LITERAL_HEX, FST::FST(GRAPH_HEX_LITERAL) },
		{ LEX_ID,      FST::FST(GRAPH_ID) }
	};

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm)
	{
		bool lex_ok = true;
		tables.lextable = LT::Create(MAXSIZE_LT);
		tables.idtable = IT::Create(MAXSIZE_TI);

		for (int i = 0; i < In::InWord::size; i++)
		{
			char* curword = in.words[i].word;
			int curline = in.words[i].line;
			bool found = false;

			if (strlen(curword) > STR_MAXSIZE + 2)
			{
				if (curword[0] == '\"')
				{
					throw ERROR_THROW_IN(205, curline, -1);
				}
				else if (strlen(curword) > MAXSIZE_ID)
				{
					throw ERROR_THROW_IN(204, curline, -1);
				}
			}

			for (int j = 0; j < N_GRAPHS; j++)
			{
				FST::FST fst(curword, graphs[j].graph);
				if (FST::execute(fst))
				{
					char lexema = graphs[j].lexema;

					if (lexema == LEX_ID || lexema == LEX_LITERAL || lexema == LEX_LITERAL_HEX)
					{
						if (lexema == LEX_ID && strlen(curword) > MAXSIZE_ID)
						{
							throw ERROR_THROW_IN(204, curline, -1);
						}
						if (lexema == LEX_LITERAL && curword[0] == '\"')
						{
							if (strlen(curword) > STR_MAXSIZE + 2)
							{
								throw ERROR_THROW_IN(205, curline, -1);
							}
						}

						if (lexema == LEX_LITERAL_HEX)
						{
							errno = 0;
							unsigned long val = std::strtoul(curword, NULL, 16);
							if (errno == ERANGE || val > UINT_MAXSIZE)
							{
								throw ERROR_THROW_IN(325, curline, -1);
							}
							sprintf_s(curword, 256, "%lu", val);
							lexema = LEX_LITERAL;
						}

						if (lexema == LEX_LITERAL && curword[0] == '0' && strlen(curword) > 1)
						{
							throw ERROR_THROW_IN(327, curline, -1);
						}

						IT::IDDATATYPE type = IT::UNDEF;
						if (curword[0] == '\"') type = IT::STR;
						else if (isdigit(curword[0])) type = IT::UINT;

						if (lexema == LEX_ID && i > 0)
						{
							if (strcmp(in.words[i - 1].word, "uint") == 0) type = IT::UINT;
							else if (strcmp(in.words[i - 1].word, "string") == 0) type = IT::STR;
							else if (strcmp(in.words[i - 1].word, "function") == 0 && i >= 2)
							{
								if (strcmp(in.words[i - 2].word, "uint") == 0) type = IT::UINT;
								else if (strcmp(in.words[i - 2].word, "string") == 0) type = IT::STR;
							}
						}

						int idx = IT::isId(tables.idtable, curword);
						if (idx == NULLIDX_TI)
						{
							IT::Entry newE;
							if (type == IT::STR && lexema == LEX_LITERAL) {
								static int litCounter = 0;
								char tmpName[32];
								sprintf_s(tmpName, "LIT_%d", ++litCounter);
								strcpy_s(newE.id, SCOPED_ID_MAXSIZE, tmpName);
							}
							else {
								if (strlen(curword) > MAXSIZE_ID)
								{
									throw ERROR_THROW_IN(204, curline, -1);
								}
								strcpy_s(newE.id, SCOPED_ID_MAXSIZE, curword);
							}

							newE.iddatatype = type;
							newE.idxfirstLE = tables.lextable.size;
							
							if (lexema == LEX_ID) {
								IT::STDFNC stdfnc = getStandFunction(curword);
								if (stdfnc != IT::F_NOT_STD) {
									newE.idtype = IT::S;
									if (stdfnc == IT::F_STR_TO_INT) {
										newE.iddatatype = STR_TO_INT_TYPE;
										newE.value.params.count = STR_TO_INT_PARAMS_CNT;
										newE.value.params.types = new IT::IDDATATYPE[STR_TO_INT_PARAMS_CNT];
										for (int k = 0; k < STR_TO_INT_PARAMS_CNT; k++)
											newE.value.params.types[k] = IT::STR_TO_INT_PARAMS[k];
									}
									else if (stdfnc == IT::F_INT_TO_STR) {
										newE.iddatatype = INT_TO_STR_TYPE;
										newE.value.params.count = INT_TO_STR_PARAMS_CNT;
										newE.value.params.types = new IT::IDDATATYPE[INT_TO_STR_PARAMS_CNT];
										for (int k = 0; k < INT_TO_STR_PARAMS_CNT; k++)
											newE.value.params.types[k] = IT::INT_TO_STR_PARAMS[k];
									}
									else if (stdfnc == IT::F_STR_LEN) {
										newE.iddatatype = STR_LEN_TYPE;
										newE.value.params.count = STR_LEN_PARAMS_CNT;
										newE.value.params.types = new IT::IDDATATYPE[STR_LEN_PARAMS_CNT];
										for (int k = 0; k < STR_LEN_PARAMS_CNT; k++)
											newE.value.params.types[k] = IT::STR_LEN_PARAMS[k];
									}
									else if (stdfnc == IT::F_STR_CAT) {
										newE.iddatatype = STR_CAT_TYPE;
										newE.value.params.count = STR_CAT_PARAMS_CNT;
										newE.value.params.types = new IT::IDDATATYPE[STR_CAT_PARAMS_CNT];
										for (int k = 0; k < STR_CAT_PARAMS_CNT; k++)
											newE.value.params.types[k] = IT::STR_CAT_PARAMS[k];
									}
								}
								else {
									newE.idtype = IT::V;
								}
							}
						else {
							newE.idtype = IT::L;
						}
						
						if (newE.idtype != IT::F && newE.idtype != IT::S)
						{
							if (!IT::SetValue(&newE, curword))
							{
								throw ERROR_THROW_IN(313, curline, -1);
							}
						}
						IT::Add(tables.idtable, newE);
							idx = tables.idtable.size - 1;
						}

						LT::Entry lt(lexema, curline, idx);
						LT::Add(tables.lextable, lt);
					}
				else if (strlen(curword) == 1 && lexema == LEX_SEPARATORS)
				{
					if (curword[0] == '^')
						LT::Add(tables.lextable, LT::Entry(LEX_POWER2, curline));
					else if (curword[0] == '&')
						LT::Add(tables.lextable, LT::Entry(LEX_SQUARE, curline));
					else
						LT::Add(tables.lextable, LT::Entry(curword[0], curline));
				}
					else
					{
						LT::Add(tables.lextable, LT::Entry(lexema, curline));
					}

					found = true;
					break;
				}
			}

			if (!found) {
				Log::writeError(log.stream, Error::GetError(201, curline, 0));
				lex_ok = false;
			}
		}
		return lex_ok;
	}

	int getIndexInLT(LT::LexTable& lextable, int idxTI)
	{
		for (int i = 0; i < lextable.size; i++)
			if (lextable.table[i].idxTI == idxTI) return i;
		return -1;
	}

	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp("stringToUnsigned", id))
			return IT::STDFNC::F_STR_TO_INT;
		if (!strcmp("unsignedToString", id))
			return IT::STDFNC::F_INT_TO_STR;
		if (!strcmp("stringLength", id))
			return IT::STDFNC::F_STR_LEN;
		if (!strcmp("stringConcat", id))
			return IT::STDFNC::F_STR_CAT;
		return IT::STDFNC::F_NOT_STD;
	}
}