#include "pch.h"
#include "Header.h"

int In::InWord::size = 0;

namespace In
{
    IN getin(wchar_t infile[], std::ostream* stream)
    {
        unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
        std::ifstream instream(infile);
        if (!instream.is_open())
            throw ERROR_THROW(102);

        IN in{};
        in.lines = 1;
        in.words = nullptr;

        int pos = 0;
        bool isLiteral = false;

        while (true)
        {
            unsigned char ch = instream.get();
            if (instream.eof())
                break;

            if (in.code[ch] == IN::Q)
                isLiteral = !isLiteral;

            switch (in.code[ch])
            {
            case IN::N:
                text[in.size++] = ch;
                in.lines++;
                pos = -1;
                break;
            case IN::T:
            case IN::P:
            case IN::S:
            case IN::Q:
                text[in.size++] = ch;
                break;
            case IN::F:
                Log::writeError(stream, Error::GetError(200, in.lines, pos));
                in.ignor++;
                break;
            case IN::I:
                in.ignor++;
                break;
            default:
                text[in.size++] = ch;
                break;
            }
            pos++;
        }

        text[in.size] = IN_CODE_NULL;
        in.text = text;
        instream.close();
        return in;
    }

    void addWord(InWord* words, char* word, int line)
    {
        if (*word == IN_CODE_NULL)
            return;

        if (InWord::size >= MAXSIZE_LT - 1)
            throw ERROR_THROW(202);

        words[InWord::size].line = line;
        strcpy_s(words[InWord::size].word, MAX_LEN_BUFFER, word);
        InWord::size++;
    }

    InWord* getWordsTable(std::ostream* stream, unsigned char* text, int* code, int textSize)
    {
        InWord* words = new InWord[MAXSIZE_LT];
        int bufpos = 0;
        int line = 1;
        char buffer[MAX_LEN_BUFFER] = "";

        for (int i = 0; i < textSize; i++)
        {
            if (text[i] == ':')
            {
                if (bufpos > 0)
                {
                    buffer[bufpos] = IN_CODE_NULL;
                    addWord(words, buffer, line);
                    bufpos = 0;
                }
                char colon[] = { ':', IN_CODE_NULL };
                addWord(words, colon, line);
                continue;
            }

            switch (code[text[i]])
            {
            case IN::S:
            {
                if (bufpos > 0) {
                    buffer[bufpos] = IN_CODE_NULL;
                    addWord(words, buffer, line);
                    bufpos = 0;
                }

			auto tryAddComposite = [&](unsigned char first, unsigned char second, char a, char b) -> bool
			{
				if (text[i] == first && i + 1 < textSize && text[i + 1] == second)
				{
					char composite[3] = { a, b, IN_CODE_NULL };
					addWord(words, composite, line);
					i++;
					return true;
				}
				return false;
			};

			if (tryAddComposite('=', '=', '=', '=')) break;
			if (tryAddComposite('!', '=', '!', '=')) break;
			if (tryAddComposite('>', '=', '>', '=')) break;
			if (tryAddComposite('<', '=', '<', '=')) break;

                char letter[] = { (char)text[i], IN_CODE_NULL };
                addWord(words, letter, line);
                break;
            }
            case IN::N:
            case IN::P:
                if (bufpos > 0) {
                    buffer[bufpos] = IN_CODE_NULL;
                    addWord(words, buffer, line);
                    bufpos = 0;
                }
                if (code[text[i]] == IN::N)
                    line++;
                break;
            case IN::Q:
            {
                if (bufpos > 0) {
                    buffer[bufpos] = IN_CODE_NULL;
                    addWord(words, buffer, line);
                    bufpos = 0;
                }

                bool isltrlgood = true;
                for (int j = i + 1; text[j] != IN_CODE_QUOTE; j++)
                {
                    if (code[text[j]] == IN::N)
                    {
                        Log::writeError(stream, Error::GetError(311, line, 0));
                        isltrlgood = false;
                        break;
                    }
                }
                if (isltrlgood)
                {
                    buffer[bufpos++] = IN_CODE_QUOTE;
                    for (int j = 1;; j++)
                    {
                        if (i + j == textSize) break;
                        
                        if (bufpos >= MAX_LEN_BUFFER - 1)
                        {
                            buffer[bufpos] = IN_CODE_NULL;
                            addWord(words, buffer, line);
                            bufpos = 0;

                            while (i + j < textSize && text[i + j] != IN_CODE_QUOTE && text[i + j] != IN_CODE_ENDL)
                            {
                                j++;
                            }
                            
                            if (i + j < textSize && text[i + j] == IN_CODE_QUOTE)
                            {

                            }
                            
                            i = i + j; 
                            break; 
                        }

                        buffer[bufpos++] = text[i + j];
                        if (text[i + j] == IN_CODE_QUOTE)
                        {
                            buffer[bufpos] = IN_CODE_NULL;
                            addWord(words, buffer, line);
                            i = i + j;
                            bufpos = 0; 
                            break;
                        }
                    }
                }
                break;
            }
            default:
                if (bufpos < MAX_LEN_BUFFER - 1) {
                    buffer[bufpos++] = text[i];
                }
                break;
            }
        }

        if (bufpos > 0) {
            buffer[bufpos] = IN_CODE_NULL;
            addWord(words, buffer, line);
        }

        return words;
    }

    void printTable(InWord* table)
    {
        std::cout << "--------------------- ÒÀÁËÈÖÀ ÑËÎÂ ---------------------" << std::endl;
        for (int i = 0; i < InWord::size; i++)
            std::cout << std::setw(2) << i << std::setw(3) << table[i].line << " | " << table[i].word << std::endl;
        std::cout << "Âñåãî ñëîâ: " << InWord::size << std::endl;
    }
}