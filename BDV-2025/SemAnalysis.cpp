#include "SemAnalysis.h"
#include "Error.h"
#include "Log.h"
#include <stack>
#include <unordered_set>
#include <vector>

namespace
{
	using namespace Lexer;

	struct SwitchContext
	{
		std::unordered_set<unsigned int> values;
		bool hasCase = false;
		bool hasDefault = false;
		int depth = 0;
		int line = 0;
	};

	struct ExprFlags
	{
		bool hasString = false;
		bool hasUint = false;
		bool hasArithmetic = false;
		bool hasComparison = false;
	};

	bool isOperand(char lex) { return lex == LEX_ID || lex == LEX_LITERAL; }
	bool isArithmetic(char lex)
	{
		return lex == LEX_PLUS || lex == LEX_MINUS || lex == LEX_STAR ||
			lex == LEX_DIRSLASH || lex == LEX_PERSENT || lex == LEX_POWER2 || lex == LEX_SQUARE;
	}
	bool isComparison(char lex)
	{
		return lex == LEX_MORE || lex == LEX_LESS || lex == LEX_EQUALS ||
			lex == LEX_NOTEQUALS || lex == LEX_GREATEREQ || lex == LEX_LESSEQ;
	}

	IT::IDDATATYPE getOperandType(LEX& tables, int lexIndex)
	{
		if (lexIndex < 0 || lexIndex >= tables.lextable.size) return IT::UNDEF;
		int idx = tables.lextable.table[lexIndex].idxTI;
		if (idx == NULLIDX_TI) return IT::UNDEF;
		return tables.idtable.table[idx].iddatatype;
	}

	ExprFlags analyzeExpression(LEX& tables, int start, char terminator)
	{
		ExprFlags flags;
		for (int k = start; k < tables.lextable.size; k++)
		{
			char lex = tables.lextable.table[k].lexema;
			if (lex == terminator) break;

			if (isOperand(lex))
			{
				int idx = tables.lextable.table[k].idxTI;
				IT::Entry& entry = tables.idtable.table[idx];

				bool isCall = (tables.lextable.table[k].lexema == LEX_ID) &&
					(entry.idtype == IT::IDTYPE::F || entry.idtype == IT::IDTYPE::S) &&
					(k + 1 < tables.lextable.size && tables.lextable.table[k + 1].lexema == LEX_LEFTHESIS);

				IT::IDDATATYPE type = entry.iddatatype;
				if (!isCall)
					type = getOperandType(tables, k);

				if (type == IT::IDDATATYPE::STR) flags.hasString = true;
				if (type == IT::IDDATATYPE::UINT) flags.hasUint = true;

				if (isCall)
				{
					int balance = 0;
					do
					{
						k++;
						char innerLex = tables.lextable.table[k].lexema;
						if (innerLex == LEX_LEFTHESIS) balance++;
						else if (innerLex == LEX_RIGHTTHESIS) balance--;
					} while (k < tables.lextable.size && balance > 0);
				}
			}
			else if (isArithmetic(lex)) flags.hasArithmetic = true;
			else if (isComparison(lex)) flags.hasComparison = true;
		}
		return flags;
	}

	bool ensureUintExpression(LEX& tables, int start, char terminator, Log::LOG& log, int line, int errId)
	{
		for (int k = start; k < tables.lextable.size; k++)
		{
			char lex = tables.lextable.table[k].lexema;
			if (lex == terminator) break;
			if (isOperand(lex))
			{
				if (getOperandType(tables, k) != IT::IDDATATYPE::UINT)
				{
					Log::writeError(log.stream, Error::GetError(errId, line, 0));
					return false;
				}
			}
		}
		return true;
	}

	bool isVisible(const std::vector<std::unordered_set<int>>& scopes, int idxTI)
	{
		for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
		{
			if (it->find(idxTI) != it->end()) return true;
		}
		return false;
	}

	IT::IDDATATYPE findNeighborOperand(LEX& tables, int from, int step)
	{
		for (int k = from + step; k >= 0 && k < tables.lextable.size; k += step)
		{
			char lex = tables.lextable.table[k].lexema;
			if (isOperand(lex))
				return getOperandType(tables, k);
		}
		return IT::UNDEF;
	}

	void validateComparisonOperands(LEX& tables, int pos, Log::LOG& log, bool& ok)
	{
		IT::IDDATATYPE left = findNeighborOperand(tables, pos, -1);
		IT::IDDATATYPE right = findNeighborOperand(tables, pos, 1);
		if (left == IT::UNDEF || right == IT::UNDEF || left != right)
		{
			ok = false;
			Log::writeError(log.stream, Error::GetError(326, tables.lextable.table[pos].sn, 0));
		}
	}

	void validatePowerOperands(LEX& tables, int pos, Log::LOG& log, bool& ok)
	{
		IT::IDDATATYPE left = findNeighborOperand(tables, pos, -1);
		IT::IDDATATYPE right = findNeighborOperand(tables, pos, 1);
		if (left != IT::IDDATATYPE::UINT || right != IT::IDDATATYPE::UINT)
		{
			ok = false;
			Log::writeError(log.stream, Error::GetError(319, tables.lextable.table[pos].sn, 0));
		}
	}

	void validateSquareOperands(LEX& tables, int pos, Log::LOG& log, bool& ok)
	{
		IT::IDDATATYPE left = findNeighborOperand(tables, pos, -1);
		if (left != IT::IDDATATYPE::UINT)
		{
			ok = false;
			Log::writeError(log.stream, Error::GetError(319, tables.lextable.table[pos].sn, 0));
		}
	}

	bool validateExpressionForType(LEX& tables, int start, char terminator, IT::IDDATATYPE expected,
		Log::LOG& log, int line, bool& ok)
	{
		ExprFlags flags = analyzeExpression(tables, start, terminator);

		if (expected == IT::IDDATATYPE::STR)
		{
			if (flags.hasUint)
			{
				ok = false;
				Log::writeError(log.stream, Error::GetError(314, line, 0));
				return false;
			}
			if (flags.hasArithmetic || flags.hasComparison)
			{
				ok = false;
				Log::writeError(log.stream, Error::GetError(316, line, 0));
				return false;
			}
		}
		else if (expected == IT::IDDATATYPE::UINT)
		{
			if (flags.hasString)
			{
				ok = false;
				Log::writeError(log.stream, Error::GetError(314, line, 0));
				return false;
			}
		}
		return true;
	}

	bool validateArguments(LEX& tables, int idLexIndex, Log::LOG& log, bool& ok)
	{
		if (idLexIndex + 1 >= tables.lextable.size) return true;
		if (tables.lextable.table[idLexIndex + 1].lexema != LEX_LEFTHESIS) return true;
		if (idLexIndex > 0 && tables.lextable.table[idLexIndex - 1].lexema == LEX_FUNCTION) return true;

		int idIdx = tables.lextable.table[idLexIndex].idxTI;
		if (idIdx == NULLIDX_TI) return true;
		IT::Entry& entry = tables.idtable.table[idIdx];
		if (entry.idtype != IT::IDTYPE::F && entry.idtype != IT::IDTYPE::S) return true;

		std::vector<IT::IDDATATYPE> actual;
		int pos = idLexIndex + 2;
		int actualCount = 0;
		for (; pos < tables.lextable.size; pos++)
		{
			char lex = tables.lextable.table[pos].lexema;
			if (lex == LEX_RIGHTTHESIS) break;
			if (isOperand(lex))
			{
				actual.push_back(getOperandType(tables, pos));
				actualCount++;
				if (actualCount > MAX_PARAMS_COUNT)
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(307, tables.lextable.table[idLexIndex].sn, 0));
				}
			}
		}

		size_t expectedCount = static_cast<size_t>(entry.value.params.count);
		if (actual.size() != expectedCount)
		{
			ok = false;
			Log::writeError(log.stream, Error::GetError(308, tables.lextable.table[idLexIndex].sn, 0));
			return false;
		}

		for (size_t a = 0; a < actual.size(); ++a)
		{
			if (actual[a] != entry.value.params.types[a])
			{
				ok = false;
				Log::writeError(log.stream, Error::GetError(309, tables.lextable.table[idLexIndex].sn, 0));
				break;
			}
		}
		return true;
	}
}

namespace Semantic
{
	bool semanticsCheck(Lexer::LEX& tables, Log::LOG& log)
	{
		bool ok = true;
		bool mainFound = false;

		std::unordered_set<int> definedFunctions;
		std::stack<std::pair<int, int>> functionScopes;
		std::stack<IT::IDDATATYPE> functionReturns;
		int braceDepth = 0;
		int pendingFunctionIdx = -1;
		std::vector<int> pendingParams;

		std::stack<SwitchContext> switches;
		bool switchAwaitingBrace = false;
		SwitchContext pendingSwitch;

		std::vector<std::unordered_set<int>> varScopes;
		varScopes.push_back({});

		for (int i = 0; i < tables.lextable.size; i++)
		{
			char lex = tables.lextable.table[i].lexema;

			switch (lex)
			{
			case LEX_MAIN:
				if (mainFound)
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(302, tables.lextable.table[i].sn, 0));
				}
				else mainFound = true;
				break;

			case LEX_FUNCTION:
			{
				if (i + 1 >= tables.lextable.size || tables.lextable.table[i + 1].lexema != LEX_ID) break;
				int fnIdx = tables.lextable.table[i + 1].idxTI;
				if (fnIdx == NULLIDX_TI) break;
				if (!definedFunctions.insert(fnIdx).second)
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(305, tables.lextable.table[i].sn, 0));
				break;
			}

				IT::Entry& fn = tables.idtable.table[fnIdx];
				fn.idtype = IT::IDTYPE::F;

				// параметры
				std::vector<IT::IDDATATYPE> paramTypes;
				pendingParams.clear();
				int pos = i + 2;
				while (pos < tables.lextable.size && tables.lextable.table[pos].lexema != LEX_RIGHTTHESIS)
				{
					if (tables.lextable.table[pos].lexema == LEX_ID)
			{
						int paramIdx = tables.lextable.table[pos].idxTI;
						if (paramIdx != NULLIDX_TI)
						{
							IT::Entry& param = tables.idtable.table[paramIdx];
							param.idtype = IT::IDTYPE::P;
							paramTypes.push_back(param.iddatatype);
							pendingParams.push_back(paramIdx);
						}
					}
					pos++;
				}

				fn.value.params.count = static_cast<short>(paramTypes.size());
				if (fn.value.params.count > MAX_PARAMS_COUNT)
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(306, tables.lextable.table[i].sn, 0));
				}

				if (fn.value.params.count > 0)
				{
					fn.value.params.types = new IT::IDDATATYPE[paramTypes.size()];
					for (size_t t = 0; t < paramTypes.size(); ++t)
						fn.value.params.types[t] = paramTypes[t];
				}
				else fn.value.params.types = nullptr;

				pendingFunctionIdx = fnIdx;
				break;
			}

			case LEX_LEFTBRACE:
				braceDepth++;
				if (pendingFunctionIdx != -1)
				{
					functionScopes.push({ pendingFunctionIdx, braceDepth });
					functionReturns.push(tables.idtable.table[pendingFunctionIdx].iddatatype);
					pendingFunctionIdx = -1;
				}
				if (switchAwaitingBrace)
				{
					pendingSwitch.depth = braceDepth;
					switches.push(pendingSwitch);
					switchAwaitingBrace = false;
				}
				varScopes.push_back({});
				if (!pendingParams.empty())
				{
					for (int pid : pendingParams) varScopes.back().insert(pid);
					pendingParams.clear();
				}
				break;

			case LEX_BRACELET:
				if (!switches.empty() && switches.top().depth == braceDepth)
					{
					if (!switches.top().hasCase)
					{
						ok = false;
						Log::writeError(log.stream, Error::GetError(323, switches.top().line, 0));
					}
					switches.pop();
				}
				if (!functionScopes.empty() && functionScopes.top().second == braceDepth)
				{
					functionScopes.pop();
					if (!functionReturns.empty()) functionReturns.pop();
				}
				if (braceDepth > 0) braceDepth--;
				pendingFunctionIdx = -1;
				switchAwaitingBrace = false;
				if (!varScopes.empty()) varScopes.pop_back();
				break;

			case LEX_NEW:
			{
				// adv <type> <id>
				if (i + 2 < tables.lextable.size && tables.lextable.table[i + 2].lexema == LEX_ID)
				{
					int idIdx = tables.lextable.table[i + 2].idxTI;
					if (!varScopes.empty())
					{
						auto& scope = varScopes.back();
						if (scope.find(idIdx) != scope.end())
							{
							ok = false;
							Log::writeError(log.stream, Error::GetError(305, tables.lextable.table[i + 2].sn, 0));
							}
						else
						{
							scope.insert(idIdx);
						}
					}
				}
				break;
			}

			case LEX_ID:
			{
				int idx = tables.lextable.table[i].idxTI;
				if (idx != NULLIDX_TI)
				{
					IT::Entry& entry = tables.idtable.table[idx];
					if (entry.iddatatype == IT::UNDEF && entry.idtype != IT::IDTYPE::S && entry.idtype != IT::IDTYPE::F)
				{
						ok = false;
						Log::writeError(log.stream, Error::GetError(300, tables.lextable.table[i].sn, 0));
					}

					if (entry.idtype == IT::IDTYPE::F)
					{
						bool isDefinition = (i > 0 && tables.lextable.table[i - 1].lexema == LEX_FUNCTION);
						bool hasCallParens = (i + 1 < tables.lextable.size && tables.lextable.table[i + 1].lexema == LEX_LEFTHESIS);

						if (!isDefinition && !hasCallParens)
						{
							ok = false;
							Log::writeError(log.stream, Error::GetError(329, tables.lextable.table[i].sn, 0));
						}
					}

					if (!functionScopes.empty() && entry.idtype == IT::IDTYPE::F)
					{
						int currentFnIdx = functionScopes.top().first;
						if (currentFnIdx == idx && i + 1 < tables.lextable.size && tables.lextable.table[i + 1].lexema == LEX_LEFTHESIS)
					{
							ok = false;
							Log::writeError(log.stream, Error::GetError(328, tables.lextable.table[i].sn, 0));
					}
				}

					bool inParamList = (pendingFunctionIdx != -1);
					if (!inParamList &&
						(entry.idtype == IT::IDTYPE::V || entry.idtype == IT::IDTYPE::P) &&
						!isVisible(varScopes, idx))
					{
						ok = false;
						Log::writeError(log.stream, Error::GetError(300, tables.lextable.table[i].sn, 0));
					}
				}
				validateArguments(tables, i, log, ok);
				break;
			}

			case LEX_EQUAL:
				if (i > 0)
				{
					int idx = tables.lextable.table[i - 1].idxTI;
					if (idx != NULLIDX_TI)
			{
						IT::Entry& left = tables.idtable.table[idx];
						validateExpressionForType(tables, i + 1, LEX_SEPARATOR, left.iddatatype, log, tables.lextable.table[i].sn, ok);
					}
				}
				break;

			case LEX_RETURN:
				if (functionReturns.empty())
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(315, tables.lextable.table[i].sn, 0));
				}
				else
				{
					IT::IDDATATYPE expected = functionReturns.top();
					validateExpressionForType(tables, i + 1, LEX_SEPARATOR, expected, log, tables.lextable.table[i].sn, ok);
				}
				break;

			case LEX_WRITE:
			{
				ExprFlags flags = analyzeExpression(tables, i + 1, LEX_SEPARATOR);
				if (flags.hasString && flags.hasUint)
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(314, tables.lextable.table[i].sn, 0));
			}
			break;
		}

			case LEX_WHILE:
				if (i + 2 < tables.lextable.size)
			{
					if (!ensureUintExpression(tables, i + 2, LEX_RIGHTTHESIS, log, tables.lextable.table[i].sn, 324))
						ok = false;
			}
			break;

			case LEX_SWITCH:
				if (i + 2 < tables.lextable.size)
				{
					if (!ensureUintExpression(tables, i + 2, LEX_RIGHTTHESIS, log, tables.lextable.table[i].sn, 320))
						ok = false;
				}
				switchAwaitingBrace = true;
				pendingSwitch = {};
				pendingSwitch.line = tables.lextable.table[i].sn;
				break;

		case LEX_CASE:
				if (switches.empty())
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(619, tables.lextable.table[i].sn, 0));
				}
				else
				{
					if (i + 1 >= tables.lextable.size || tables.lextable.table[i + 1].lexema != LEX_LITERAL)
				{
						ok = false;
						Log::writeError(log.stream, Error::GetError(321, tables.lextable.table[i].sn, 0));
				}
					else
					{
						int litIdx = tables.lextable.table[i + 1].idxTI;
						if (litIdx == NULLIDX_TI || tables.idtable.table[litIdx].iddatatype != IT::IDDATATYPE::UINT)
						{
							ok = false;
							Log::writeError(log.stream, Error::GetError(321, tables.lextable.table[i].sn, 0));
						}
						else
						{
							unsigned value = tables.idtable.table[litIdx].value.vint;
							if (!switches.top().values.insert(value).second)
					{
								ok = false;
								Log::writeError(log.stream, Error::GetError(322, tables.lextable.table[i].sn, 0));
							}
							switches.top().hasCase = true;
					}
				}
			}
			break;

			case LEX_DEFAULT:
				if (switches.empty())
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(619, tables.lextable.table[i].sn, 0));
				}
				else if (switches.top().hasDefault)
				{
					ok = false;
					Log::writeError(log.stream, Error::GetError(619, tables.lextable.table[i].sn, 0));
				}
				else switches.top().hasDefault = true;
				break;

			case LEX_POWER2:
				validatePowerOperands(tables, i, log, ok);
				break;

			case LEX_SQUARE:
				validateSquareOperands(tables, i, log, ok);
				break;

			case LEX_DIRSLASH:
			case LEX_PERSENT:
			{
				int right = i + 1;
				if (right < tables.lextable.size && tables.lextable.table[right].lexema == LEX_LITERAL)
			{
					int idx = tables.lextable.table[right].idxTI;
					if (idx != NULLIDX_TI && tables.idtable.table[idx].iddatatype == IT::IDDATATYPE::UINT &&
						tables.idtable.table[idx].value.vint == 0)
				{
						ok = false;
						Log::writeError(log.stream, Error::GetError(318, tables.lextable.table[i].sn, 0));
				}
			}
			break;
		}

		case LEX_MORE:
		case LEX_LESS:
		case LEX_EQUALS:
		case LEX_NOTEQUALS:
			case LEX_GREATEREQ:
			case LEX_LESSEQ:
				validateComparisonOperands(tables, i, log, ok);
				break;
			}
		}

		if (!mainFound)
				{
			ok = false;
			Log::writeError(log.stream, Error::GetError(301, 0, 0));
				}

		return ok;
}
}

