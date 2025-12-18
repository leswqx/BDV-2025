#include "pch.h"
#include "Header.h"
#include "Generator.h"
#include <iostream>
#include <fstream>

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "Russian");
	std::cout << "--- ЗАПУСК КОМПИЛЯТОРА BDV-2025 ---" << std::endl;

	Log::LOG log;

	try 
	{
		Parm::PARM parm;
		wcscpy_s(parm.in, L"../BDV-2025/in.txt");
		wcscpy_s(parm.out, L"../Assembler/out.asm");
		wcscpy_s(parm.log, L"../BDV-2025/in.txt.log");

		//Parm::PARM parm = Parm::getparm(argc, argv);

		log = Log::getstream(parm.log);
		Log::writeLog(log);
		Log::writeParm(log, parm);

		std::cout << "1. Чтение исходного файла..." << std::endl;
		In::IN in = In::getin(parm.in, log.stream);
		Log::writeIn(log.stream, in);

		std::cout << "2. Первичный разбор на слова..." << std::endl;
		in.words = In::getWordsTable(log.stream, in.text, in.code, in.size);
		In::printTable(in.words);

		std::cout << "3. Лексический анализ (построение таблиц)..." << std::endl;
		Lexer::LEX tables;
		bool lex_ok = Lexer::analyze(tables, in, log, parm);

		if (lex_ok)
		{
			LT::writeLexTable(log.stream, tables.lextable);
			IT::writeIdTable(log.stream, tables.idtable);
			LT::writeLexemsOnLines(log.stream, tables.lextable);
			std::cout << "   [OK] Лексический анализ прошел успешно!" << std::endl;

			std::cout << "4. Синтаксический анализ (проверка грамматики)..." << std::endl;
			FST_TRACE_n = -1;
			MFST_TRACE_START(log);
			MFST::Mfst mfst(tables, GRB::getGreibach()); 
			bool synt_ok = mfst.start(log);

			mfst.savededucation();
			mfst.printrules(log);

			if (synt_ok)
			{
				std::cout << "   [OK] Синтаксических ошибок нет!" << std::endl;

				std::cout << "5. Семантический анализ (проверка типов)..." << std::endl;
				bool sem_ok = Semantic::semanticsCheck(tables, log);

				if (sem_ok)
				{
					std::cout << "   [OK] Семантических ошибок нет!" << std::endl;

					std::cout << "6. Построение Польской нотации..." << std::endl;
					bool polish_ok = Polish::PolishNotation(tables, log);

					if (polish_ok)
					{
						std::cout << "7. Генерация ассемблерного кода..." << std::endl;
						Gener::CodeGeneration(tables, parm, log);
						std::cout << "   [OK] Ассемблерный код сгенерирован!" << std::endl;

					std::cout << "-----------------------------------" << std::endl;
					std::cout << "КОМПИЛЯЦИЯ ЗАВЕРШЕНА УСПЕШНО!" << std::endl;
					std::cout << "Проверьте файл: test.log" << std::endl;
					}
					else
					{
						std::cout << "   [ERROR] Ошибка при построении Польской записи." << std::endl;
					}
				}
				else
				{
					std::cout << "   [ERROR] Найдены семантические ошибки." << std::endl;
				}
			}
			else
			{
				std::cout << "   [ERROR] Найдены синтаксические ошибки." << std::endl;
			}
		}
		else
		{
			std::cout << "   [ERROR] Лексический анализ завершился с ошибками." << std::endl;
		}

		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		std::cout << "\n!!! КРИТИЧЕСКАЯ ОШИБКА !!!" << std::endl;
		std::cout << "ID: " << e.id << ", Сообщение: " << e.message << std::endl;
		if (e.position.line != -1)
			std::cout << "Строка: " << e.position.line << ", Позиция: " << e.position.col << std::endl;

		if (log.stream != nullptr)
			Log::writeError(log.stream, e);
	}

	system("pause");
	return 0;
}