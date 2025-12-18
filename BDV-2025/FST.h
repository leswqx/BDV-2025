#pragma once
#include <iostream>

namespace FST
{
	struct RELATION
	{
		char  symbol;
		short nnode;
		RELATION(
			char c = 0x00,
			short ns = 0
		);
	};

	struct NODE
	{
		short n_relation;
		RELATION* relations;
		NODE();
		NODE(short n, RELATION rel, ...);
	};


	struct FST
	{
		char* string;
		short position;
		short nstates;
		NODE* node;
		short* rstates;

		FST() { string = NULL; position = 0; nstates = 0; node = NULL; rstates = NULL; }
		FST(short ns, NODE n, ...);
		FST(char* s, FST& fst);
	};

	bool execute(FST& fst);
};
