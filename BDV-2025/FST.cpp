#include "pch.h"
#include "FST.h"
#include <cstring>
#include <cstdarg> 

namespace FST
{
	RELATION::RELATION(char c, short ns)
	{
		symbol = c;
		nnode = ns;
	};

	NODE::NODE()
	{
		n_relation = 0;
		relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...)
	{
		n_relation = n;
		relations = new RELATION[n];

		RELATION* ptr = &rel;
		for (int i = 0; i < n; i++)
		{
			relations[i] = ptr[i];
		}
	};

	FST::FST(short ns, NODE n, ...)
	{
		nstates = ns;
		node = new NODE[ns];
		rstates = new short[nstates];

		NODE* ptr = &n;
		for (int k = 0; k < ns; k++)
		{
			node[k] = ptr[k];
		}

		string = NULL;
		position = -1;
	}

	FST::FST(char* s, FST& fst)
	{
		node = fst.node;
		nstates = fst.nstates;
		string = s;
		rstates = new short[nstates];
		position = -1;
	}

	bool execute(FST& fst)
	{
		int i, j;
		memset(fst.rstates, -1, fst.nstates * sizeof(short));
		for (fst.rstates[0] = 0, fst.position = 0; fst.position < (signed)(strlen(fst.string)); fst.position++)
		{
			for (i = 0; i < fst.nstates; i++)
		{
				if (fst.rstates[i] == fst.position)
			{
					for (j = 0; j < fst.node[i].n_relation; j++)
				{
					if (fst.node[i].relations[j].symbol == fst.string[fst.position])
					{
						fst.rstates[fst.node[i].relations[j].nnode] = fst.position + 1;
						}
					};
				};
			};
		};
		return (fst.rstates[fst.nstates - 1] == (strlen(fst.string)));
	}
}