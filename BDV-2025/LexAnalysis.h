#pragma once
#include "IT.h"
#include "LT.h"
#include "In.h"
#include "Log.h"
#include "FST.h"

namespace Lexer
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	idtable;

		LEX() {
			lextable.size = 0; lextable.table = nullptr;
			idtable.size = 0; idtable.table = nullptr;
		}
	};

	struct Graph
	{
		char lexema;
		FST::FST graph;
		Graph() { lexema = 0; }
		Graph(char l, FST::FST g) { lexema = l; graph = g; }
	};

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm);

	int getIndexInLT(LT::LexTable& lextable, int idxTI);

	IT::STDFNC getStandFunction(char* id);
}