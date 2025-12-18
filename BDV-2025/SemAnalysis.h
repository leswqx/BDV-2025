#pragma once
#include "LexAnalysis.h"
#include "Log.h"

namespace Semantic
{
	bool semanticsCheck(Lexer::LEX& tables, Log::LOG& log);
}