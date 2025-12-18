#pragma once
#include <iostream>
#include <climits>
#include <cstring>

#define MAXSIZE_ID	32
#define SCOPED_ID_MAXSIZE   MAXSIZE_ID*2
#define MAXSIZE_TI	4096
#define UINT_DEFAULT	0x00000000
#define STR_DEFAULT	0x00
#define NULLIDX_TI	0xffffffff
#define STR_MAXSIZE	255
#define UINT_MAXSIZE UINT_MAX
#define UINT_MINSIZE 0
#define MAX_PARAMS_COUNT 3
#define STR_TO_INT_PARAMS_CNT   1
#define INT_TO_STR_PARAMS_CNT   1
#define STR_LEN_PARAMS_CNT      1
#define STR_CAT_PARAMS_CNT      2
#define STR_TO_INT_TYPE         IT::IDDATATYPE::UINT
#define INT_TO_STR_TYPE         IT::IDDATATYPE::STR
#define STR_LEN_TYPE            IT::IDDATATYPE::UINT
#define STR_CAT_TYPE            IT::IDDATATYPE::STR

namespace IT 
{
	enum IDDATATYPE
	{
		UINT = 1,
		STR = 2,
		UNDEF = 3
	};

	enum IDTYPE
	{
		V = 1,
		F = 2,
		P = 3,
		L = 4,
		S = 5
	};

	enum STDFNC 
	{
		F_STR_TO_INT,
		F_INT_TO_STR,
		F_STR_LEN,
		F_STR_CAT,
		F_NOT_STD
	};

	static const IDDATATYPE STR_TO_INT_PARAMS[] = { IT::IDDATATYPE::STR };
	static const IDDATATYPE INT_TO_STR_PARAMS[] = { IT::IDDATATYPE::UINT };
	static const IDDATATYPE STR_LEN_PARAMS[] = { IT::IDDATATYPE::STR };
	static const IDDATATYPE STR_CAT_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };

	struct Entry
	{

		union
		{
			unsigned int vint;
			
			struct
			{
				int len;
				char str[STR_MAXSIZE - 1];
			} vstr;

			struct
			{
				int count;
				IDDATATYPE* types;
			} params;


		} value;

		int			idxfirstLE;		
		char		id[SCOPED_ID_MAXSIZE];
		IDDATATYPE	iddatatype;
		IDTYPE		idtype;

	Entry()
	{
		this->idxfirstLE = 0;
		this->iddatatype = UNDEF;
		this->idtype = V;
		this->id[0] = '\0';
		this->value.vint = UINT_DEFAULT;
	};

	Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype)
	{
		strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
		this->id[SCOPED_ID_MAXSIZE - 1] = '\0';
		this->idxfirstLE = idxLT;
		this->iddatatype = datatype;
		this->idtype = idtype;
		this->value.vint = UINT_DEFAULT;
	};
	};

	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create(int size = NULL);
	void Add(
		IdTable& idtable,
		Entry entry); 
	int isId(
		IdTable& idtable,
		char id[SCOPED_ID_MAXSIZE]);
	bool SetValue(IT::Entry* entry, char* value);
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable);
}