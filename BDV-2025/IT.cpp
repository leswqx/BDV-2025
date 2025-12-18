#include "pch.h"
#include "Header.h"
#include <iomanip>

#pragma warning(disable : 4996)
#include <cstdlib>

#define W(x, y)\
		<< std::setw(x) << (y) <<
#define STR(n, line, type, id)\
		"|" W(4,n) " |  " W(5,line) "    |" W(17,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"

namespace IT
{
    IdTable Create(int size)
    {
        if (size > MAXSIZE_TI)
        {
            throw ERROR_THROW(203);
        }

        IdTable idtable;
        idtable.table = new Entry[idtable.maxsize = size];
        idtable.size = NULL;
        return idtable;
    }

    void Add(IdTable& idtable, Entry entry)
    {
        if (idtable.size >= idtable.maxsize)
        {
            throw ERROR_THROW(203);
        }

        idtable.table[idtable.size++] = entry;
    }

    int isId(IdTable& idtable, char id[SCOPED_ID_MAXSIZE])
    {
        for (int i = 0; i < idtable.size; i++)
        {
            if (strcmp(idtable.table[i].id, id) == 0)
                return i;
        }

        return NULLIDX_TI;
    }

    bool SetValue(IT::IdTable& idtable, int index, char* value)
    {
        return SetValue(&(idtable.table[index]), value);
    }

    bool SetValue(IT::Entry* entry, char* value)
    {
        bool rc = true;
        if (entry->iddatatype == UINT)
        {
            errno = 0;
            unsigned long temp = std::strtoul(value, NULL, 10);

            if (errno == ERANGE || temp > UINT_MAXSIZE)
            {
                temp = UINT_MAXSIZE;
                rc = false;
            }
            entry->value.vint = (unsigned int)temp;
        }
	else if (entry->iddatatype == STR)
	{
		int len = (int)strlen(value);
		if (len >= 2)
		{
			int str_len = len - 2;
			if (str_len >= STR_MAXSIZE - 1)
				str_len = STR_MAXSIZE - 2;
			
			for (int i = 0; i < str_len; i++)
				entry->value.vstr.str[i] = value[i + 1];
			entry->value.vstr.str[str_len] = '\0';
			entry->value.vstr.len = str_len;
		}
		else
		{
			entry->value.vstr.str[0] = '\0';
			entry->value.vstr.len = 0;
		}
	}

        return rc;
    }

    void writeIdTable(std::ostream* stream, IT::IdTable& idtable)
    {
        *stream << "-----------------------------  ÒÀÁËÈÖÀ ÈÄÅÍÒÈÔÈÊÀÒÎÐÎÂ  ----------------------------\n" << std::endl;
        *stream << "|  N  |Èíäåêñ â ËÒ| Òèï èäåíòèôèêàòîðà |        Òèï        | Çíà÷åíèå (ïàðàìåòðû)" << std::endl;
        for (int i = 0; i < idtable.size; i++)
        {
            IT::Entry* e = &idtable.table[i];
            char type[50] = "";

        switch (e->iddatatype)
        {
        case IT::IDDATATYPE::UINT:
            strcat_s(type, 50, "  uint   ");
            break;
        case IT::IDDATATYPE::STR:
            strcat_s(type, 50, " string  ");
            break;
        case IT::IDDATATYPE::UNDEF:
            strcat_s(type, 50, "UNDEFINED");
            break;
        }

        switch (e->idtype)
        {
        case IT::IDTYPE::V:
            strcat_s(type, 50, "  variable");
            break;
        case IT::IDTYPE::F:
            strcat_s(type, 50, "  function");
            break;
        case IT::IDTYPE::P:
            strcat_s(type, 50, " parameter");
            break;
        case IT::IDTYPE::L:
            strcat_s(type, 50, "   literal");
            break;
        case IT::IDTYPE::S:
            strcat_s(type, 50, "  LIB FUNC");
            break;
        default:
            strcat_s(type, 50, "UNDEFINED ");
            break;
        }

        *stream << STR(i, e->idxfirstLE, type, e->id);
        if (e->idtype == IT::IDTYPE::L || (e->idtype == IT::IDTYPE::V && e->iddatatype != IT::IDDATATYPE::UNDEF))
            {
                if (e->iddatatype == IT::IDDATATYPE::UINT)
                    *stream << e->value.vint;
                else
                    *stream << "[" << (int)e->value.vstr.len << "]" << e->value.vstr.str;
            }
            if (e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S)
            {
                for (int i = 0; i < e->value.params.count; i++)
                {
                    *stream << " P" << i << ":";
                    switch (e->value.params.types[i])
                    {
                    case IT::IDDATATYPE::UINT:
                        *stream << "UINT |";
                        break;
                    case IT::IDDATATYPE::STR:
                        *stream << "STRING |";
                        break;
                    case IT::IDDATATYPE::UNDEF:
                        *stream << "UNDEFINED";
                        break;
                    }
                }
            }
            *stream << std::endl;
        }
        *stream << "\n===================================================================================\n\n";
    }
};