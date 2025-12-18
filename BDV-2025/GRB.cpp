#include "pch.h"
#include "Header.h"
#include <cstdarg>

#define GRB_ERROR_SERIES 600

typedef short GRBALPHABET;

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 10,

		Rule(NS('S'), GRB_ERROR_SERIES, 4,
			Rule::Chain(8, TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), NS('K'), TS('}'), NS('S')),
			Rule::Chain(8, TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), NS('K'), TS('}')),
			Rule::Chain(5, TS('m'), TS('{'), NS('K'), TS('}'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('K'), TS('}'))
		),

		Rule(NS('P'), GRB_ERROR_SERIES + 1, 2,
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 2, 2,
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('E')),
			Rule::Chain(2, TS('t'), TS('i'))
		),

		Rule(NS('K'), GRB_ERROR_SERIES + 3, 16,
			Rule::Chain(7, TS('A'), TS('t'), TS('i'), TS('='), NS('W'), TS(';'), NS('K')),
			Rule::Chain(5, TS('A'), TS('t'), TS('i'), TS(';'), NS('K')),
			Rule::Chain(5, TS('i'), TS('='), NS('W'), TS(';'), NS('K')),
			Rule::Chain(4, TS('o'), NS('W'), TS(';'), NS('K')),
			Rule::Chain(4, TS('e'), NS('W'), TS(';'), NS('K')),
			Rule::Chain(8, TS('W'), TS('('), NS('W'), TS(')'), TS('{'), NS('K'), TS('}'), NS('K')),
			Rule::Chain(8, TS('U'), TS('('), NS('W'), TS(')'), TS('{'), NS('C'), TS('}'), NS('K')),
			Rule::Chain(4, TS('i'), NS('F'), TS(';'), NS('K')),

			Rule::Chain(6, TS('A'), TS('t'), TS('i'), TS('='), NS('W'), TS(';')),
			Rule::Chain(4, TS('A'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('W'), TS(';')),
			Rule::Chain(3, TS('o'), NS('W'), TS(';')),
			Rule::Chain(3, TS('e'), NS('W'), TS(';')),
			Rule::Chain(7, TS('W'), TS('('), NS('W'), TS(')'), TS('{'), NS('K'), TS('}')),
			Rule::Chain(7, TS('U'), TS('('), NS('W'), TS(')'), TS('{'), NS('C'), TS('}')),
			Rule::Chain(3, TS('i'), NS('F'), TS(';'))
		),

		Rule(NS('W'), GRB_ERROR_SERIES + 4, 25,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(3, TS('i'), NS('A'), NS('W')),
			Rule::Chain(3, TS('l'), NS('A'), NS('W')),
			Rule::Chain(4, TS('i'), TS('('), NS('N'), TS(')')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('A'), NS('W')),
			Rule::Chain(6, TS('i'), TS('('), NS('N'), TS(')'), NS('A'), NS('W')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), NS('A'), NS('W')),
			Rule::Chain(3, TS('i'), NS('L'), NS('W')),
			Rule::Chain(3, TS('l'), NS('L'), NS('W')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('L'), NS('W')),
			Rule::Chain(3, TS('i'), TS('^'), NS('W')),
			Rule::Chain(3, TS('l'), TS('^'), NS('W')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), TS('^'), NS('W')),
			Rule::Chain(2, TS('i'), TS('&')),
			Rule::Chain(2, TS('l'), TS('&')),
			Rule::Chain(4, TS('('), NS('W'), TS(')'), TS('&')),
			Rule::Chain(4, TS('i'), TS('&'), NS('A'), NS('W')),
			Rule::Chain(4, TS('l'), TS('&'), NS('A'), NS('W')),
			Rule::Chain(6, TS('('), NS('W'), TS(')'), TS('&'), NS('A'), NS('W')),
			Rule::Chain(4, TS('i'), TS('&'), NS('L'), NS('W')),
			Rule::Chain(4, TS('l'), TS('&'), NS('L'), NS('W')),
			Rule::Chain(6, TS('('), NS('W'), TS(')'), TS('&'), NS('L'), NS('W'))
		),

		Rule(NS('N'), GRB_ERROR_SERIES + 5, 4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('N')),
			Rule::Chain(3, TS('l'), TS(','), NS('N'))
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 6, 2,
			Rule::Chain(3, TS('('), NS('N'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

		Rule(NS('A'), GRB_ERROR_SERIES + 7, 5,
			Rule::Chain(1, TS('+')),
			Rule::Chain(1, TS('-')),
			Rule::Chain(1, TS('*')),
			Rule::Chain(1, TS('/')),
			Rule::Chain(1, TS('%'))
		),

		Rule(NS('L'), GRB_ERROR_SERIES + 8, 6,
			Rule::Chain(1, TS('>')),
			Rule::Chain(1, TS('<')),
			Rule::Chain(1, TS('#')),
			Rule::Chain(1, TS('!')),
			Rule::Chain(1, TS('~')),
			Rule::Chain(1, TS('@'))
		),

		Rule(NS('C'), GRB_ERROR_SERIES + 9, 4,
			Rule::Chain(5, TS('C'), TS('l'), TS(':'), NS('K'), NS('C')),
			Rule::Chain(4, TS('C'), TS('l'), TS(':'), NS('K')),
			Rule::Chain(4, TS('D'), TS(':'), NS('K'), NS('C')),
			Rule::Chain(3, TS('D'), TS(':'), NS('K'))
		)
	);

	Greibach getGreibach() { return greibach; }

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			k++;
		if (k < size)
			prule = rules[rc = k];
		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0;
		if (nchain >= 0 && nchain < size)
			chains[nchain].getCChain(b + 3);
		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)
			++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	}

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		size = psize;
		nt = new GRBALPHABET[size];
		va_list ap;
		va_start(ap, s);
		nt[0] = s;
		for (int i = 1; i < psize; ++i)
			nt[i] = va_arg(ap, GRBALPHABET);
		va_end(ap);
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; ++i)
			b[i] = alphabet_to_char(nt[i]);
		b[size] = 0;
		return b;
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbootomT, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbootomT;
		size = psize;
		rules = new Rule[size];
		va_list ap;
		va_start(ap, r);
		rules[0] = r;
		for (int i = 1; i < psize; ++i)
			rules[i] = va_arg(ap, Rule);
		va_end(ap);
	}

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		size = psize;
		chains = new Chain[size];
		va_list ap;
		va_start(ap, c);
		chains[0] = c;
		for (int i = 1; i < psize; ++i)
			chains[i] = va_arg(ap, Chain);
		va_end(ap);
	}
}
