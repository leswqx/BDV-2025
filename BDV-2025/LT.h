#pragma once
#include <fstream>

#define LEX_SEPARATORS	 'S'
#define	LEX_ID_TYPE    	 't'		
#define	LEX_ID			 'i'		
#define	LEX_LITERAL		 'l'		
#define	LEX_FUNCTION     'f'		
#define	LEX_MAIN		 'm'		
#define	LEX_SEPARATOR	 ';'
#define	LEX_COMMA		 ','
#define	LEX_LEFTBRACE	 '{'
#define	LEX_BRACELET	 '}'		
#define	LEX_LEFTHESIS	 '('		
#define	LEX_RIGHTTHESIS	 ')'
#define LEX_SWITCH       'U'
#define LEX_CASE         'C'
#define LEX_DEFAULT      'D'
#define LEX_WHILE        'W'

#define	LEX_EQUAL		 '='
#define LEX_POWER2       '^'
#define LEX_SQUARE       '&'
#define LEX_PLUS         '+'
#define LEX_MINUS        '-'
#define LEX_STAR         '*'
#define LEX_DIRSLASH     '/'
#define LEX_PERSENT      '%'
#define LEX_MORE         '>'
#define LEX_LESS         '<'
#define LEX_GREATEREQ    '~'
#define LEX_LESSEQ       '@'
#define LEX_EQUALS       '#'
#define LEX_NOTEQUALS    '!'

#define	LEX_LITERAL_HEX  'H'
#define LEX_WRITE		 'o'
#define LEX_NEWLINE		 '|'
#define LEX_RETURN		 'e'
#define LEX_SCRIPT		 'p'
#define LEX_NEW			 'A'

#define	LEXEMA_FIXSIZE    1
#define	MAXSIZE_LT		 4096
#define	NULLDX_TI	 0xffffffff


namespace LT
{
	struct Entry
	{
		char lexema;
		int sn;
		int idxTI;

		Entry();
		Entry(char lexema, int snn, int idxti = NULLDX_TI);
	};

	struct LexTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	void writeLexTable(std::ostream* stream, LT::LexTable& lextable);
	void writeLexemsOnLines(std::ostream* stream, LT::LexTable& lextable);
};

