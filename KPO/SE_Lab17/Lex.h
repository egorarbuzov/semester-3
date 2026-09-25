#pragma once
#include "In.h"
#include "LT.h"
#include "IT.h"

namespace Lex
{
    // Главная функция: выполнить лексический анализ
    // Вход: подготовленный текст (с сепараторами |)
    // Выход: заполненные lextable и idtable
    void LexicalAnalysis(In::IN& in, LT::LexTable& lextable, IT::IdTable& idtable);
}