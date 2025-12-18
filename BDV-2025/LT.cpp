#include "pch.h"
#include "LT.h"
#include "Error.h"
#include <iomanip>
#include <iostream>

namespace LT
{
	Entry::Entry() { lexema = 0; sn = 0; idxTI = NULLDX_TI; }

	Entry::Entry(char lexema, int snn, int idxti)
	{
		this->lexema = lexema;
		this->sn = snn;
		this->idxTI = idxti;
	}

	LexTable Create(int size)
	{
		if (size > MAXSIZE_LT) throw ERROR_THROW(202);
		LexTable lextable;
		lextable.maxsize = size;
		lextable.size = 0;
		lextable.table = new Entry[size];
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size + 1 > lextable.maxsize) throw ERROR_THROW(202);
		lextable.table[lextable.size++] = entry;
	}

	void writeLexTable(std::ostream* stream, LT::LexTable& lextable)
	{
		*stream << "------------------------------ ÒÀÁËÈÖÀ ËÅÊÑÅÌ  ------------------------\n" << std::endl;
		*stream << "|  N | ËÅÊÑÅÌÀ | ÑÒÐÎÊÀ | ÈÍÄÅÊÑ Â ÒÈ |" << std::endl;
		for (int i = 0; i < lextable.size; i++)
		{
			*stream << "|" << std::setw(3) << i << " | " << std::setw(4) << lextable.table[i].lexema << "    |  " << std::setw(3)
				<< lextable.table[i].sn << "   |";
			if (lextable.table[i].idxTI == NULLDX_TI)
				*stream << "             |" << std::endl;
			else
				*stream << std::setw(8) << lextable.table[i].idxTI << "     |" << std::endl;
		}
		*stream << "\n";
	}

	void writeLexemsOnLines(std::ostream* stream, LT::LexTable& lextable)
	{
		*stream << "\n-----------------  ËÅÊÑÅÌÛ ÐÀÑÏÎËÎÆÅÍÍÛÅ ÏÎ ÑÒÐÎÊÀÌ ÈÑÕÎÄÍÎÃÎ ÒÅÊÑÒÀ ---------------------\n" << std::endl;
		for (int i = 0; i < lextable.size; )
		{
			int line = lextable.table[i].sn;
			*stream << std::setw(3) << line << " | ";
			while (i < lextable.size && lextable.table[i].sn == line)
			{
				*stream << lextable.table[i].lexema;
				if (lextable.table[i].idxTI != NULLDX_TI)
					*stream << "[" << lextable.table[i].idxTI << "]";
				i++;
			}
			*stream << std::endl;
		}
		*stream << "-------------------------------------------------------------------------\n\n";
	}
}