#pragma once
#include "In.h"
#include "LT.h"
#include "IT.h"

namespace Lex
{
    void LexicalAnalysis(In::IN& in, LT::LexTable& lextable, IT::IdTable& idtable);
}