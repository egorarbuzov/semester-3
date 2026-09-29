// ============================================================
// Компилятор языка IVA-2026
// Группа 8, вариант 1
// Целевой язык: x64 NASM (Windows PE / Linux ELF64)
//
// Только генерация .asm. Ассемблирование и линковка — вручную.
//
// Сборка:  clang++ -std=c++17 -O2 -o iva_compiler iva_compiler.cpp
// Запуск:  ./iva_compiler test.iva [--target=win|linux] [-v]
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <algorithm>

// ============================================================
// 1. ТОКЕНЫ
// ============================================================

enum class TokKind {
    KW_UINT8, KW_BOOL, KW_STRING, KW_FUNC, KW_RETURN,
    KW_MAIN, KW_IF, KW_ELSE, KW_DO, KW_WHILE,
    KW_TRUE, KW_FALSE, KW_PRINT,
    NUMBER, STRING, IDENT,
    OP_OR, OP_AND, OP_BITOR, OP_BITAND, OP_BITNOT,
    OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE,
    OP_ASSIGN, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV,
    LPAREN, RPAREN, LBRACE, RBRACE, SEMI, COMMA,
    END
};

struct Token {
    TokKind kind;
    std::string value;
    long long num = 0;
    int line = 1;
    int col  = 1;
};

static std::map<std::string, TokKind> KEYWORDS = {
    {"uint8",  TokKind::KW_UINT8},
    {"bool",   TokKind::KW_BOOL},
    {"string", TokKind::KW_STRING},
    {"func",   TokKind::KW_FUNC},
    {"return", TokKind::KW_RETURN},
    {"main",   TokKind::KW_MAIN},
    {"if",     TokKind::KW_IF},
    {"else",   TokKind::KW_ELSE},
    {"do",     TokKind::KW_DO},
    {"while",  TokKind::KW_WHILE},
    {"true",   TokKind::KW_TRUE},
    {"false",  TokKind::KW_FALSE},
    {"print",  TokKind::KW_PRINT},
};

static std::string kindName(TokKind k) {
    switch (k) {
        case TokKind::KW_UINT8:  return "uint8";
        case TokKind::KW_BOOL:   return "bool";
        case TokKind::KW_STRING: return "string";
        case TokKind::KW_FUNC:   return "func";
        case TokKind::KW_RETURN: return "return";
        case TokKind::KW_MAIN:   return "main";
        case TokKind::KW_IF:     return "if";
        case TokKind::KW_ELSE:   return "else";
        case TokKind::KW_DO:     return "do";
        case TokKind::KW_WHILE:  return "while";
        case TokKind::KW_TRUE:   return "true";
        case TokKind::KW_FALSE:  return "false";
        case TokKind::KW_PRINT:  return "print";
        case TokKind::NUMBER:    return "NUMBER";
        case TokKind::STRING:    return "STRING";
        case TokKind::IDENT:     return "IDENT";
        case TokKind::OP_OR:     return "||";
        case TokKind::OP_AND:    return "&&";
        case TokKind::OP_BITOR:  return "|";
        case TokKind::OP_BITAND: return "&";
        case TokKind::OP_BITNOT: return "~";
        case TokKind::OP_EQ:     return "==";
        case TokKind::OP_NE:     return "!=";
        case TokKind::OP_LT:     return "<";
        case TokKind::OP_GT:     return ">";
        case TokKind::OP_LE:     return "<=";
        case TokKind::OP_GE:     return ">=";
        case TokKind::OP_ASSIGN: return "=";
        case TokKind::OP_PLUS:   return "+";
        case TokKind::OP_MINUS:  return "-";
        case TokKind::OP_MUL:    return "*";
        case TokKind::OP_DIV:    return "/";
        case TokKind::LPAREN:    return "(";
        case TokKind::RPAREN:    return ")";
        case TokKind::LBRACE:    return "{";
        case TokKind::RBRACE:    return "}";
        case TokKind::SEMI:      return ";";
        case TokKind::COMMA:     return ",";
        case TokKind::END:       return "EOF";
    }
    return "?";
}

// ============================================================
// 2. ЛЕКСИЧЕСКИЙ АНАЛИЗАТОР
// ============================================================

class Lexer {
public:
    Lexer(const std::string& src) : src_(src) {}

    std::vector<Token> tokenize(std::vector<std::string>& errors) {
        std::vector<Token> out;
        int line = 1, col = 1;
        size_t i = 0;
        while (i < src_.size()) {
            char c = src_[i];

            if (std::isspace((unsigned char)c)) {
                if (c == '\n') { ++line; col = 1; }
                else ++col;
                ++i;
                continue;
            }
            if (c == '/' && i + 1 < src_.size() && src_[i+1] == '/') {
                while (i < src_.size() && src_[i] != '\n') ++i;
                continue;
            }

            int startCol = col;

            if (std::isdigit((unsigned char)c)) {
                std::string num;
                while (i < src_.size() && std::isdigit((unsigned char)src_[i])) {
                    num += src_[i]; ++i; ++col;
                }
                Token t; t.kind = TokKind::NUMBER;
                t.value = num;
                t.num = std::stoll(num);
                t.line = line; t.col = startCol;
                out.push_back(t);
                continue;
            }

            if (std::isalpha((unsigned char)c) || c == '_') {
                std::string id;
                while (i < src_.size() &&
                       (std::isalnum((unsigned char)src_[i]) || src_[i] == '_')) {
                    id += src_[i]; ++i; ++col;
                }
                Token t; t.line = line; t.col = startCol; t.value = id;
                auto it = KEYWORDS.find(id);
                if (it != KEYWORDS.end()) t.kind = it->second;
                else t.kind = TokKind::IDENT;
                out.push_back(t);
                continue;
            }

            if (c == '"') {
                std::string s;
                ++i; ++col;
                bool closed = false;
                while (i < src_.size()) {
                    if (src_[i] == '"') { closed = true; ++i; ++col; break; }
                    if (src_[i] == '\n') break;
                    s += src_[i]; ++i; ++col;
                }
                if (!closed) {
                    errors.push_back("ЛЕКС_ОШИБКА: незакрытая строка в " +
                                     std::to_string(line) + ":" +
                                     std::to_string(startCol));
                    continue;
                }
                Token t; t.kind = TokKind::STRING; t.value = s;
                t.line = line; t.col = startCol;
                out.push_back(t);
                continue;
            }

            auto two = [&](char a, char b, TokKind k) -> bool {
                if (i + 1 < src_.size() && src_[i] == a && src_[i+1] == b) {
                    Token t; t.kind = k; t.value = std::string() + a + b;
                    t.line = line; t.col = startCol;
                    out.push_back(t);
                    i += 2; col += 2;
                    return true;
                }
                return false;
            };
            if (two('|','|',TokKind::OP_OR))   continue;
            if (two('&','&',TokKind::OP_AND))  continue;
            if (two('=','=',TokKind::OP_EQ))   continue;
            if (two('!','=',TokKind::OP_NE))   continue;
            if (two('<','=',TokKind::OP_LE))   continue;
            if (two('>','=',TokKind::OP_GE))   continue;

            TokKind k;
            switch (c) {
                case '|': k = TokKind::OP_BITOR;  break;
                case '&': k = TokKind::OP_BITAND; break;
                case '~': k = TokKind::OP_BITNOT; break;
                case '<': k = TokKind::OP_LT;     break;
                case '>': k = TokKind::OP_GT;     break;
                case '=': k = TokKind::OP_ASSIGN; break;
                case '+': k = TokKind::OP_PLUS;   break;
                case '-': k = TokKind::OP_MINUS;  break;
                case '*': k = TokKind::OP_MUL;    break;
                case '/': k = TokKind::OP_DIV;    break;
                case '(': k = TokKind::LPAREN;    break;
                case ')': k = TokKind::RPAREN;    break;
                case '{': k = TokKind::LBRACE;    break;
                case '}': k = TokKind::RBRACE;    break;
                case ';': k = TokKind::SEMI;      break;
                case ',': k = TokKind::COMMA;     break;
                default:
                    errors.push_back(std::string("ЛЕКС_ОШИБКА: неизвестный символ '") +
                                     c + "' в " + std::to_string(line) + ":" +
                                     std::to_string(startCol));
                    ++i; ++col;
                    continue;
            }
            Token t; t.kind = k; t.value = std::string(1, c);
            t.line = line; t.col = startCol;
            out.push_back(t);
            ++i; ++col;
        }
        Token eof; eof.kind = TokKind::END; eof.line = line; eof.col = col;
        out.push_back(eof);
        return out;
    }

private:
    std::string src_;
};

// ============================================================
// 3. AST
// ============================================================

struct Expr;
struct Stmt;
using ExprPtr = std::shared_ptr<Expr>;
using StmtPtr = std::shared_ptr<Stmt>;

struct Expr {
    enum Kind { NUM, STR, BOOL, VAR, CALL, BINOP, UNOP } kind;
    long long num = 0;
    std::string str;
    bool bval = false;
    ExprPtr left, right;
    std::vector<ExprPtr> args;
};

struct Stmt {
    enum Kind {
        VAR_DECL, ASSIGN, IF, DO_WHILE, WHILE,
        PRINT, RETURN, BLOCK
    } kind;
    std::string type;
    std::string name;
    ExprPtr expr;
    std::vector<StmtPtr> body;
    std::vector<StmtPtr> elseBody;
};

struct FuncDecl {
    std::string retType;
    std::string name;
    std::vector<std::pair<std::string,std::string>> params;
    std::vector<StmtPtr> body;
};

struct Program {
    std::vector<StmtPtr> globals;
    std::vector<FuncDecl> funcs;
    std::vector<StmtPtr> mainBody;
};

// ============================================================
// 4. СИНТАКСИЧЕСКИЙ АНАЛИЗАТОР
// ============================================================

class Parser {
public:
    Parser(std::vector<Token> toks) : toks_(std::move(toks)) {}

    Program parse(std::vector<std::string>& errors) {
        errors_ = &errors;
        Program p;
        while (check(TokKind::KW_UINT8) || check(TokKind::KW_BOOL) ||
               check(TokKind::KW_STRING)) {
            p.globals.push_back(parseVarDecl());
        }
        while (check(TokKind::KW_FUNC)) {
            p.funcs.push_back(parseFuncDecl());
        }
        expect(TokKind::KW_MAIN, "ожидалось 'main'");
        p.mainBody = parseBlock();
        expect(TokKind::END, "ожидался конец файла");
        return p;
    }

private:
    std::vector<Token> toks_;
    size_t pos_ = 0;
    std::vector<std::string>* errors_ = nullptr;

    const Token& peek() const { return toks_[pos_]; }
    bool check(TokKind k) const { return peek().kind == k; }
    const Token& advance() { return toks_[pos_++]; }

    void error(const std::string& msg) {
        const Token& t = peek();
        errors_->push_back("СИНТ_ОШИБКА: " + msg + " в " +
                           std::to_string(t.line) + ":" +
                           std::to_string(t.col) +
                           " (получено '" + kindName(t.kind) + "')");
        throw std::runtime_error("syntax");
    }

    Token expect(TokKind k, const std::string& msg) {
        if (!check(k)) error(msg);
        return advance();
    }

    StmtPtr parseVarDecl() {
        std::string type;
        if (check(TokKind::KW_UINT8))       { type = "uint8";  advance(); }
        else if (check(TokKind::KW_BOOL))   { type = "bool";   advance(); }
        else if (check(TokKind::KW_STRING)) { type = "string"; advance(); }
        else error("ожидался тип");

        Token nameTok = expect(TokKind::IDENT, "ожидалось имя переменной");
        ExprPtr init;
        if (check(TokKind::OP_ASSIGN)) {
            advance();
            init = parseExpr();
        }
        expect(TokKind::SEMI, "ожидался ';'");

        auto s = std::make_shared<Stmt>();
        s->kind = Stmt::VAR_DECL;
        s->type = type;
        s->name = nameTok.value;
        s->expr = init;
        return s;
    }

    FuncDecl parseFuncDecl() {
        expect(TokKind::KW_FUNC, "ожидалось 'func'");
        FuncDecl f;
        if (check(TokKind::KW_UINT8))       { f.retType = "uint8";  advance(); }
        else if (check(TokKind::KW_BOOL))   { f.retType = "bool";   advance(); }
        else if (check(TokKind::KW_STRING)) { f.retType = "string"; advance(); }
        else error("ожидался тип возвращаемого значения");

        f.name = expect(TokKind::IDENT, "ожидалось имя функции").value;
        expect(TokKind::LPAREN, "ожидалась '('");

        if (!check(TokKind::RPAREN)) {
            while (true) {
                std::string pt;
                if (check(TokKind::KW_UINT8))       { pt = "uint8";  advance(); }
                else if (check(TokKind::KW_BOOL))   { pt = "bool";   advance(); }
                else if (check(TokKind::KW_STRING)) { pt = "string"; advance(); }
                else error("ожидался тип параметра");
                std::string pn = expect(TokKind::IDENT, "ожидалось имя параметра").value;
                f.params.emplace_back(pt, pn);
                if (check(TokKind::COMMA)) { advance(); continue; }
                break;
            }
        }
        expect(TokKind::RPAREN, "ожидалась ')'");
        f.body = parseBlock();
        return f;
    }

    std::vector<StmtPtr> parseBlock() {
        expect(TokKind::LBRACE, "ожидалась '{'");
        std::vector<StmtPtr> body;
        while (!check(TokKind::RBRACE) && !check(TokKind::END)) {
            body.push_back(parseStmt());
        }
        expect(TokKind::RBRACE, "ожидалась '}'");
        return body;
    }

    StmtPtr parseStmt() {
        if (check(TokKind::KW_UINT8) || check(TokKind::KW_BOOL) ||
            check(TokKind::KW_STRING)) return parseVarDecl();
        if (check(TokKind::KW_IF))     return parseIf();
        if (check(TokKind::KW_DO))     return parseDoWhile();
        if (check(TokKind::KW_WHILE))  return parseWhile();
        if (check(TokKind::KW_PRINT))  return parsePrint();
        if (check(TokKind::KW_RETURN)) return parseReturn();
        if (check(TokKind::LBRACE)) {
            auto s = std::make_shared<Stmt>();
            s->kind = Stmt::BLOCK;
            s->body = parseBlock();
            return s;
        }
        if (check(TokKind::IDENT))     return parseAssign();
        error("неожиданный токен");
        return nullptr;
    }

    StmtPtr parseAssign() {
        Token nameTok = expect(TokKind::IDENT, "ожидалось имя");
        expect(TokKind::OP_ASSIGN, "ожидалось '='");
        ExprPtr e = parseExpr();
        expect(TokKind::SEMI, "ожидался ';'");
        auto s = std::make_shared<Stmt>();
        s->kind = Stmt::ASSIGN;
        s->name = nameTok.value;
        s->expr = e;
        return s;
    }

    StmtPtr parseIf() {
        expect(TokKind::KW_IF, "ожидалось 'if'");
        ExprPtr cond = parseExpr();
        auto body = parseBlock();
        std::vector<StmtPtr> elseBody;
        if (check(TokKind::KW_ELSE)) {
            advance();
            elseBody = parseBlock();
        }
        auto s = std::make_shared<Stmt>();
        s->kind = Stmt::IF;
        s->expr = cond;
        s->body = body;
        s->elseBody = elseBody;
        return s;
    }

    StmtPtr parseDoWhile() {
        expect(TokKind::KW_DO, "ожидалось 'do'");
        auto body = parseBlock();
        expect(TokKind::KW_WHILE, "ожидалось 'while'");
        ExprPtr cond = parseExpr();
        expect(TokKind::SEMI, "ожидался ';'");
        auto s = std::make_shared<Stmt>();
        s->kind = Stmt::DO_WHILE;
        s->body = body;
        s->expr = cond;
        return s;
    }

    StmtPtr parseWhile() {
        expect(TokKind::KW_WHILE, "ожидалось 'while'");
        ExprPtr cond = parseExpr();
        auto body = parseBlock();
        auto s = std::make_shared<Stmt>();
        s->kind = Stmt::WHILE;
        s->expr = cond;
        s->body = body;
        return s;
    }

    StmtPtr parsePrint() {
        expect(TokKind::KW_PRINT, "ожидалось 'print'");
        expect(TokKind::LPAREN, "ожидалась '('");
        ExprPtr e = parseExpr();
        expect(TokKind::RPAREN, "ожидалась ')'");
        expect(TokKind::SEMI, "ожидался ';'");
        auto s = std::make_shared<Stmt>();
        s->kind = Stmt::PRINT;
        s->expr = e;
        return s;
    }

    StmtPtr parseReturn() {
        expect(TokKind::KW_RETURN, "ожидалось 'return'");
        ExprPtr e = parseExpr();
        expect(TokKind::SEMI, "ожидался ';'");
        auto s = std::make_shared<Stmt>();
        s->kind = Stmt::RETURN;
        s->expr = e;
        return s;
    }

    ExprPtr parseExpr() { return parseOr(); }

    ExprPtr parseOr() {
        ExprPtr l = parseAnd();
        while (check(TokKind::OP_OR)) { advance(); l = mkBin("||", l, parseAnd()); }
        return l;
    }
    ExprPtr parseAnd() {
        ExprPtr l = parseBitOr();
        while (check(TokKind::OP_AND)) { advance(); l = mkBin("&&", l, parseBitOr()); }
        return l;
    }
    ExprPtr parseBitOr() {
        ExprPtr l = parseBitAnd();
        while (check(TokKind::OP_BITOR)) { advance(); l = mkBin("|", l, parseBitAnd()); }
        return l;
    }
    ExprPtr parseBitAnd() {
        ExprPtr l = parseCmp();
        while (check(TokKind::OP_BITAND)) { advance(); l = mkBin("&", l, parseCmp()); }
        return l;
    }
    ExprPtr parseCmp() {
        ExprPtr l = parseAdd();
        while (check(TokKind::OP_EQ) || check(TokKind::OP_NE) ||
               check(TokKind::OP_LT) || check(TokKind::OP_GT) ||
               check(TokKind::OP_LE) || check(TokKind::OP_GE)) {
            TokKind k = advance().kind;
            l = mkBin(kindName(k), l, parseAdd());
        }
        return l;
    }
    ExprPtr parseAdd() {
        ExprPtr l = parseMul();
        while (check(TokKind::OP_PLUS) || check(TokKind::OP_MINUS)) {
            TokKind k = advance().kind;
            l = mkBin(kindName(k), l, parseMul());
        }
        return l;
    }
    ExprPtr parseMul() {
        ExprPtr l = parseUnary();
        while (check(TokKind::OP_MUL) || check(TokKind::OP_DIV)) {
            TokKind k = advance().kind;
            l = mkBin(kindName(k), l, parseUnary());
        }
        return l;
    }
    ExprPtr parseUnary() {
        if (check(TokKind::OP_BITNOT) || check(TokKind::OP_MINUS)) {
            TokKind k = advance().kind;
            ExprPtr e = parseUnary();
            auto u = std::make_shared<Expr>();
            u->kind = Expr::UNOP;
            u->str = kindName(k);
            u->right = e;
            return u;
        }
        return parsePrimary();
    }
    ExprPtr parsePrimary() {
        if (check(TokKind::NUMBER)) {
            auto e = std::make_shared<Expr>();
            e->kind = Expr::NUM; e->num = advance().num;
            return e;
        }
        if (check(TokKind::STRING)) {
            auto e = std::make_shared<Expr>();
            e->kind = Expr::STR; e->str = advance().value;
            return e;
        }
        if (check(TokKind::KW_TRUE) || check(TokKind::KW_FALSE)) {
            auto e = std::make_shared<Expr>();
            e->kind = Expr::BOOL;
            e->bval = (peek().kind == TokKind::KW_TRUE);
            advance();
            return e;
        }
        if (check(TokKind::IDENT)) {
            std::string name = advance().value;
            if (check(TokKind::LPAREN)) {
                advance();
                auto e = std::make_shared<Expr>();
                e->kind = Expr::CALL;
                e->str = name;
                if (!check(TokKind::RPAREN)) {
                    while (true) {
                        e->args.push_back(parseExpr());
                        if (check(TokKind::COMMA)) { advance(); continue; }
                        break;
                    }
                }
                expect(TokKind::RPAREN, "ожидалась ')'");
                return e;
            }
            auto e = std::make_shared<Expr>();
            e->kind = Expr::VAR;
            e->str = name;
            return e;
        }
        if (check(TokKind::LPAREN)) {
            advance();
            ExprPtr e = parseExpr();
            expect(TokKind::RPAREN, "ожидалась ')'");
            return e;
        }
        error("ожидалось выражение");
        return nullptr;
    }

    ExprPtr mkBin(const std::string& op, ExprPtr l, ExprPtr r) {
        auto e = std::make_shared<Expr>();
        e->kind = Expr::BINOP;
        e->str = op;
        e->left = l; e->right = r;
        return e;
    }
};

// ============================================================
// 5. СЕМАНТИЧЕСКИЙ АНАЛИЗАТОР
// ============================================================

class SemanticAnalyzer {
public:
    std::vector<std::string> analyze(const Program& p) {
        for (auto& s : p.globals) {
            declare(s->name, s->type);
            if (s->expr) checkExpr(s->expr);
        }
        for (auto& f : p.funcs) {
            if (funcs_.count(f.name))
                errors_.push_back("СЕМ_ОШИБКА: функция '" + f.name + "' уже объявлена");
            std::vector<std::string> ptypes;
            for (auto& pr : f.params) ptypes.push_back(pr.first);
            funcs_[f.name] = {f.retType, ptypes};
        }
        for (auto& f : p.funcs) {
            auto saved = symbols_;
            for (auto& pr : f.params) declare(pr.second, pr.first);
            checkBlock(f.body);
            symbols_ = saved;
        }
        checkBlock(p.mainBody);
        return errors_;
    }

private:
    std::map<std::string, std::string> symbols_;
    std::map<std::string, std::pair<std::string, std::vector<std::string>>> funcs_;
    std::vector<std::string> errors_;

    void declare(const std::string& name, const std::string& type) {
        if (symbols_.count(name))
            errors_.push_back("СЕМ_ОШИБКА: переменная '" + name + "' уже объявлена");
        symbols_[name] = type;
    }

    void checkBlock(const std::vector<StmtPtr>& body) {
        for (auto& s : body) checkStmt(s);
    }

    void checkStmt(const StmtPtr& s) {
        switch (s->kind) {
            case Stmt::VAR_DECL:
                if (s->expr) checkExpr(s->expr);
                declare(s->name, s->type);
                break;
            case Stmt::ASSIGN:
                if (!symbols_.count(s->name))
                    errors_.push_back("СЕМ_ОШИБКА: переменная '" + s->name + "' не объявлена");
                checkExpr(s->expr);
                break;
            case Stmt::IF:
                checkExpr(s->expr);
                checkBlock(s->body);
                checkBlock(s->elseBody);
                break;
            case Stmt::DO_WHILE:
            case Stmt::WHILE:
                checkExpr(s->expr);
                checkBlock(s->body);
                break;
            case Stmt::PRINT:
            case Stmt::RETURN:
                checkExpr(s->expr);
                break;
            case Stmt::BLOCK:
                checkBlock(s->body);
                break;
        }
    }

    void checkExpr(const ExprPtr& e) {
        if (!e) return;
        switch (e->kind) {
            case Expr::VAR:
                if (!symbols_.count(e->str))
                    errors_.push_back("СЕМ_ОШИБКА: переменная '" + e->str + "' не объявлена");
                break;
            case Expr::CALL: {
                if (!funcs_.count(e->str) &&
                    e->str != "intToStr" && e->str != "strLen") {
                    errors_.push_back("СЕМ_ОШИБКА: функция '" + e->str + "' не объявлена");
                } else if (funcs_.count(e->str)) {
                    auto& sig = funcs_[e->str];
                    if (sig.second.size() != e->args.size())
                        errors_.push_back("СЕМ_ОШИБКА: неверное число аргументов у функции '" + e->str + "'");
                }
                for (auto& a : e->args) checkExpr(a);
                break;
            }
            case Expr::BINOP:
                checkExpr(e->left);
                checkExpr(e->right);
                break;
            case Expr::UNOP:
                checkExpr(e->right);
                break;
            default: break;
        }
    }
};

// ============================================================
// 6. ГЕНЕРАТОР АССЕМБЛЕРА
// ============================================================

enum class Target { WIN64, LINUX64 };

class AsmGen {
public:
    AsmGen(Target t) : target_(t) {}

    std::string generate(const Program& p) {
        prog_ = &p;
        out_.str("");

        // ---- 1. Регистрация всех имён как переменных ----
        collectGlobals(p);
        for (auto& f : p.funcs) {
            for (auto& pr : f.params) addVar(pr.second);
            collectLocals(f.body);
        }
        collectLocals(p.mainBody);

        // ---- 2. СНАЧАЛА собираем ВСЕ строковые литералы ----
        // (иначе .data окажется пустым к моменту вывода)
        for (auto& g : p.globals) collectStringsStmt(g);
        for (auto& f : p.funcs)   collectStringsBlock(f.body);
        collectStringsBlock(p.mainBody);

        // ---- 3. Шапка ----
        emit("; ============================================");
        emit("; Сгенерировано компилятором IVA-2026");
        emit("; Целевая платформа: " +
             std::string(target_ == Target::WIN64 ? "Windows x64 (PE)" : "Linux x64 (ELF64)"));
        emit("; ============================================");
        if (target_ == Target::WIN64) emit("default rel");
        emit("");

        emitSectionData();
        emit("");
        emitSectionText();

        return out_.str();
    }

private:
    std::ostringstream out_;
    const Program* prog_ = nullptr;
    Target target_;
    std::map<std::string, std::string> varLabels_;
    std::vector<std::pair<std::string,std::string>> strings_;
    int strCounter_ = 0;
    int lblCounter_ = 0;

    void emit(const std::string& s) { out_ << s << "\n"; }

    // ---------- регистрация переменных ----------
    void collectGlobals(const Program& p) {
        for (auto& s : p.globals) addVar(s->name);
    }
    void collectLocals(const std::vector<StmtPtr>& body) {
        for (auto& s : body) {
            if (s->kind == Stmt::VAR_DECL) addVar(s->name);
            if (s->kind == Stmt::IF) {
                collectLocals(s->body);
                collectLocals(s->elseBody);
            }
            if (s->kind == Stmt::DO_WHILE || s->kind == Stmt::WHILE ||
                s->kind == Stmt::BLOCK) {
                collectLocals(s->body);
            }
        }
    }
    void addVar(const std::string& name) {
        if (!varLabels_.count(name)) {
            varLabels_[name] = "v_" + name + "_" + std::to_string(lblCounter_++);
        }
    }

    // ---------- сбор строковых литералов ----------
    void collectStringsBlock(const std::vector<StmtPtr>& body) {
        for (auto& s : body) collectStringsStmt(s);
    }
    void collectStringsStmt(const StmtPtr& s) {
        if (!s) return;
        collectStringsExpr(s->expr);
        switch (s->kind) {
            case Stmt::IF:
                collectStringsBlock(s->body);
                collectStringsBlock(s->elseBody);
                break;
            case Stmt::DO_WHILE:
            case Stmt::WHILE:
            case Stmt::BLOCK:
                collectStringsBlock(s->body);
                break;
            default: break;
        }
    }
    void collectStringsExpr(const ExprPtr& e) {
        if (!e) return;
        if (e->kind == Expr::STR) addString(e->str);
        collectStringsExpr(e->left);
        collectStringsExpr(e->right);
        for (auto& a : e->args) collectStringsExpr(a);
    }

    // addString теперь идемпотентна: для одинаковых строк — одна метка
    std::string addString(const std::string& s) {
        for (auto& kv : strings_) {
            if (kv.second == s) return kv.first;
        }
        std::string lbl = "str_" + std::to_string(strCounter_++);
        strings_.emplace_back(lbl, s);
        return lbl;
    }

    std::string escapeAsmStr(const std::string& s) {
        std::string r;
        for (char c : s) {
            if (c == '"') r += "\\\"";
            else if (c == '\\') r += "\\\\";
            else if (c == '\n') r += "\\n";
            else if (c == '\t') r += "\\t";
            else if (c == '\r') r += "\\r";
            else r += c;
        }
        return "\"" + r + "\"";
    }

    // ---------- секция данных ----------
    void emitSectionData() {
        emit("section .data");
        for (auto& kv : varLabels_)
            emit("    " + kv.second + ": dq 0");
        for (auto& s : strings_)
            emit("    " + s.first + ": db " + escapeAsmStr(s.second) + ", 0");
        emit("    newline: db 10, 0");
        emit("    numbuf:  times 32 db 0");
        emit("    concat_buf: times 1024 db 0");
        if (target_ == Target::WIN64) {
            emit("    written: dq 0");
        }
    }

    // ---------- секция кода ----------
    void emitSectionText() {
        emit("section .text");
        if (target_ == Target::WIN64) {
            emit("extern ExitProcess");
            emit("extern GetStdHandle");
            emit("extern WriteConsoleA");
            emit("extern lstrlenA");
            emit("global mainCRTStartup");
        } else {
            emit("global _start");
        }
        emit("");
        emitRuntime();
        for (auto& f : prog_->funcs) genFunc(f);

        emit("; ---------- точка входа ----------");
        if (target_ == Target::WIN64) {
            emit("mainCRTStartup:");
            emit("    push rbp");
            emit("    mov rbp, rsp");
            emit("    sub rsp, 40");
            for (auto& g : prog_->globals) genStmt(g);
            for (auto& s : prog_->mainBody) genStmt(s);
            emit("    xor ecx, ecx");
            emit("    call ExitProcess");
        } else {
            emit("_start:");
            for (auto& g : prog_->globals) genStmt(g);
            for (auto& s : prog_->mainBody) genStmt(s);
            emit("    mov rax, 60");
            emit("    xor rdi, rdi");
            emit("    syscall");
        }
        emit("");
    }

    // ---------- runtime ----------
    void emitRuntime() {
        if (target_ == Target::WIN64) emitRuntimeWin();
        else                          emitRuntimeLinux();
    }

    void emitRuntimeWin() {
        emit("; --- print_str(rcx=ptr) ---");
        emit("print_str:");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    sub rsp, 48");
        emit("    mov rbx, rcx");
        emit("    mov ecx, -11");
        emit("    call GetStdHandle");
        emit("    mov rcx, rax");
        emit("    mov rdx, rbx");
        emit("    push rcx");
        emit("    push rdx");
        emit("    mov rcx, rbx");
        emit("    call lstrlenA");
        emit("    pop rdx");
        emit("    pop rcx");
        emit("    mov r8, rax");
        emit("    lea r9, [written]");
        emit("    mov qword [rsp+32], 0");
        emit("    call WriteConsoleA");
        emit("    mov rsp, rbp");
        emit("    pop rbp");
        emit("    ret");
        emit("");

        emit("; --- intToStr(rcx=num, rdx=buf, r8=size) -> rax ---");
        emit("intToStr:");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    push rbx");
        emit("    mov rax, rcx");
        emit("    mov rsi, rdx");
        emit("    mov rcx, r8");
        emit("    mov rbx, 10");
        emit("    mov rdi, rsi");
        emit("    add rdi, rcx");
        emit("    dec rdi");
        emit("    mov byte [rdi], 0");
        emit("    test rax, rax");
        emit("    jnz .digit");
        emit("    dec rdi");
        emit("    mov byte [rdi], '0'");
        emit("    jmp .done");
        emit(".digit:");
        emit("    xor rdx, rdx");
        emit("    div rbx");
        emit("    add dl, '0'");
        emit("    dec rdi");
        emit("    mov [rdi], dl");
        emit("    test rax, rax");
        emit("    jnz .digit");
        emit(".done:");
        emit("    mov rax, rdi");
        emit("    pop rbx");
        emit("    mov rsp, rbp");
        emit("    pop rbp");
        emit("    ret");
        emit("");

        emit("; --- strLen(rcx=ptr) -> rax ---");
        emit("strLen:");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    sub rsp, 32");
        emit("    call lstrlenA");
        emit("    mov rsp, rbp");
        emit("    pop rbp");
        emit("    ret");
        emit("");

        emit("; --- strcat2(rcx=dst, rdx=src) -> rax ---");
        emit("strcat2:");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    push rbx");
        emit("    push rsi");
        emit("    push rdi");
        emit("    mov rdi, rcx");
        emit("    mov rsi, rdx");
        emit(".seek_end:");
        emit("    cmp byte [rdi], 0");
        emit("    je .copy");
        emit("    inc rdi");
        emit("    jmp .seek_end");
        emit(".copy:");
        emit("    mov al, [rsi]");
        emit("    mov [rdi], al");
        emit("    test al, al");
        emit("    jz .done");
        emit("    inc rdi");
        emit("    inc rsi");
        emit("    jmp .copy");
        emit(".done:");
        emit("    mov rax, rcx");
        emit("    pop rdi");
        emit("    pop rsi");
        emit("    pop rbx");
        emit("    mov rsp, rbp");
        emit("    pop rbp");
        emit("    ret");
        emit("");

        emit("; --- strcopy(rcx=dst, rdx=src) -> rax ---");
        emit("strcopy:");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    mov rax, rcx");
        emit(".cp:");
        emit("    mov bl, [rdx]");
        emit("    mov [rcx], bl");
        emit("    test bl, bl");
        emit("    jz .done");
        emit("    inc rcx");
        emit("    inc rdx");
        emit("    jmp .cp");
        emit(".done:");
        emit("    mov rsp, rbp");
        emit("    pop rbp");
        emit("    ret");
        emit("");
    }

    void emitRuntimeLinux() {
        emit("; --- print_str(rdi=ptr) ---");
        emit("print_str:");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    push rdi");
        emit("    call strLen");
        emit("    mov rdx, rax");
        emit("    pop rsi");
        emit("    mov rax, 1");
        emit("    mov rdi, 1");
        emit("    syscall");
        emit("    mov rsp, rbp");
        emit("    pop rbp");
        emit("    ret");
        emit("");

        emit("; --- intToStr(rdi=num, rsi=buf, rdx=size) -> rax ---");
        emit("intToStr:");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    push rbx");
        emit("    mov rax, rdi");
        emit("    mov r8, rsi");
        emit("    mov rcx, rdx");
        emit("    mov rbx, 10");
        emit("    mov r9, r8");
        emit("    add r9, rcx");
        emit("    dec r9");
        emit("    mov byte [r9], 0");
        emit("    test rax, rax");
        emit("    jnz .digit");
        emit("    dec r9");
        emit("    mov byte [r9], '0'");
        emit("    jmp .done");
        emit(".digit:");
        emit("    xor rdx, rdx");
        emit("    div rbx");
        emit("    add dl, '0'");
        emit("    dec r9");
        emit("    mov [r9], dl");
        emit("    test rax, rax");
        emit("    jnz .digit");
        emit(".done:");
        emit("    mov rax, r9");
        emit("    pop rbx");
        emit("    mov rsp, rbp");
        emit("    pop rbp");
        emit("    ret");
        emit("");

        emit("; --- strLen(rdi=ptr) -> rax ---");
        emit("strLen:");
        emit("    xor rax, rax");
        emit(".lp:");
        emit("    cmp byte [rdi+rax], 0");
        emit("    je .done");
        emit("    inc rax");
        emit("    jmp .lp");
        emit(".done:");
        emit("    ret");
        emit("");

        emit("; --- strcat2(rdi=dst, rsi=src) -> rax ---");
        emit("strcat2:");
        emit("    mov r8, rdi");
        emit(".seek:");
        emit("    cmp byte [rdi], 0");
        emit("    je .copy");
        emit("    inc rdi");
        emit("    jmp .seek");
        emit(".copy:");
        emit("    mov al, [rsi]");
        emit("    mov [rdi], al");
        emit("    test al, al");
        emit("    jz .done");
        emit("    inc rdi");
        emit("    inc rsi");
        emit("    jmp .copy");
        emit(".done:");
        emit("    mov rax, r8");
        emit("    ret");
        emit("");

        emit("; --- strcopy(rdi=dst, rsi=src) -> rax ---");
        emit("strcopy:");
        emit("    mov rax, rdi");
        emit(".cp:");
        emit("    mov bl, [rsi]");
        emit("    mov [rdi], bl");
        emit("    test bl, bl");
        emit("    jz .done");
        emit("    inc rdi");
        emit("    inc rsi");
        emit("    jmp .cp");
        emit(".done:");
        emit("    ret");
        emit("");
    }

    // ---------- функции ----------
    void genFunc(const FuncDecl& f) {
        emit("; ---------- func " + f.name + " ----------");
        emit(f.name + ":");
        emit("    push rbp");
        emit("    mov rbp, rsp");
        emit("    sub rsp, 40");

        static const char* winRegs[]   = {"rcx", "rdx", "r8", "r9"};
        static const char* linuxRegs[] = {"rdi", "rsi", "rdx", "rcx"};
        const char** regs = (target_ == Target::WIN64) ? winRegs : linuxRegs;

        for (size_t i = 0; i < f.params.size() && i < 4; ++i) {
            std::string lbl = varLabels_[f.params[i].second];
            emit("    mov [" + lbl + "], " + regs[i]);
        }

        bool lastIsReturn = false;
        for (auto& s : f.body) {
            genStmt(s);
            lastIsReturn = (s->kind == Stmt::RETURN);
        }
        if (!lastIsReturn) {
            emit("    mov rsp, rbp");
            emit("    pop rbp");
            emit("    ret");
        }
        emit("");
    }

    // ---------- инструкции ----------
    void genStmt(const StmtPtr& s) {
        switch (s->kind) {
            case Stmt::VAR_DECL: {
                std::string lbl = varLabels_[s->name];
                if (s->expr) {
                    genExprTo(s->expr, "rax");
                    emit("    mov [" + lbl + "], rax");
                } else {
                    emit("    mov qword [" + lbl + "], 0");
                }
                break;
            }
            case Stmt::ASSIGN: {
                std::string lbl = varLabels_[s->name];
                genExprTo(s->expr, "rax");
                emit("    mov [" + lbl + "], rax");
                break;
            }
            case Stmt::IF: {
                std::string lElse = ".Lelse_" + std::to_string(lblCounter_++);
                std::string lEnd  = ".Lend_"  + std::to_string(lblCounter_++);
                genExprTo(s->expr, "rax");
                emit("    test rax, rax");
                emit("    jz " + (s->elseBody.empty() ? lEnd : lElse));
                for (auto& x : s->body) genStmt(x);
                if (!s->elseBody.empty()) {
                    emit("    jmp " + lEnd);
                    emit(lElse + ":");
                    for (auto& x : s->elseBody) genStmt(x);
                }
                emit(lEnd + ":");
                break;
            }
            case Stmt::DO_WHILE: {
                std::string lTop = ".Ldo_" + std::to_string(lblCounter_++);
                emit(lTop + ":");
                for (auto& x : s->body) genStmt(x);
                genExprTo(s->expr, "rax");
                emit("    test rax, rax");
                emit("    jnz " + lTop);
                break;
            }
            case Stmt::WHILE: {
                std::string lTop = ".Lwh_"  + std::to_string(lblCounter_++);
                std::string lEnd = ".Lend_" + std::to_string(lblCounter_++);
                emit(lTop + ":");
                genExprTo(s->expr, "rax");
                emit("    test rax, rax");
                emit("    jz " + lEnd);
                for (auto& x : s->body) genStmt(x);
                emit("    jmp " + lTop);
                emit(lEnd + ":");
                break;
            }
            case Stmt::PRINT: {
                genExprTo(s->expr, "rax");
                if (target_ == Target::WIN64) {
                    emit("    mov rcx, rax");
                    emit("    call print_str");
                    emit("    lea rcx, [newline]");
                    emit("    call print_str");
                } else {
                    emit("    mov rdi, rax");
                    emit("    call print_str");
                    emit("    lea rdi, [newline]");
                    emit("    call print_str");
                }
                break;
            }
            case Stmt::RETURN: {
                genExprTo(s->expr, "rax");
                emit("    mov rsp, rbp");
                emit("    pop rbp");
                emit("    ret");
                break;
            }
            case Stmt::BLOCK:
                for (auto& x : s->body) genStmt(x);
                break;
        }
    }

    // ---------- выражения ----------
    void genExprTo(const ExprPtr& e, const std::string& dstReg) {
        if (!e) { emit("    xor " + dstReg + ", " + dstReg); return; }
        switch (e->kind) {
            case Expr::NUM:
                emit("    mov " + dstReg + ", " + std::to_string(e->num));
                break;
            case Expr::BOOL:
                emit("    mov " + dstReg + ", " + (e->bval ? "1" : "0"));
                break;
            case Expr::STR: {
                std::string lbl = addString(e->str);
                emit("    lea " + dstReg + ", [" + lbl + "]");
                break;
            }
            case Expr::VAR: {
                std::string lbl = varLabels_[e->str];
                emit("    mov " + dstReg + ", [" + lbl + "]");
                break;
            }
            case Expr::CALL: {
                genCall(e);
                if (dstReg != "rax") emit("    mov " + dstReg + ", rax");
                break;
            }
            case Expr::UNOP: {
                genExprTo(e->right, "rax");
                if (e->str == "~") {
                    emit("    not rax");
                    emit("    and rax, 0xFF");
                } else if (e->str == "-") {
                    emit("    neg rax");
                }
                if (dstReg != "rax") emit("    mov " + dstReg + ", rax");
                break;
            }
            case Expr::BINOP: {
                // конкатенация строк
                if (e->str == "+" &&
                    (e->left->kind == Expr::STR || e->right->kind == Expr::STR ||
                     e->left->kind == Expr::CALL || e->right->kind == Expr::CALL)) {
                    genStringConcat(e);
                    if (dstReg != "rax") emit("    mov " + dstReg + ", rax");
                    break;
                }
                genExprTo(e->left, "rax");
                emit("    push rax");
                genExprTo(e->right, "rax");
                emit("    mov rbx, rax");
                emit("    pop rax");
                std::string op = e->str;
                if      (op == "+")  emit("    add rax, rbx");
                else if (op == "-")  emit("    sub rax, rbx");
                else if (op == "*")  emit("    imul rax, rbx");
                else if (op == "/") { emit("    cqo"); emit("    idiv rbx"); }
                else if (op == "|") { emit("    or  rax, rbx"); emit("    and rax, 0xFF"); }
                else if (op == "&") { emit("    and rax, rbx"); emit("    and rax, 0xFF"); }
                else if (op == "==") { emit("    cmp rax, rbx"); emit("    sete al"); emit("    movzx rax, al"); }
                else if (op == "!=") { emit("    cmp rax, rbx"); emit("    setne al"); emit("    movzx rax, al"); }
                else if (op == "<")  { emit("    cmp rax, rbx"); emit("    setl al");  emit("    movzx rax, al"); }
                else if (op == ">")  { emit("    cmp rax, rbx"); emit("    setg al");  emit("    movzx rax, al"); }
                else if (op == "<=") { emit("    cmp rax, rbx"); emit("    setle al"); emit("    movzx rax, al"); }
                else if (op == ">=") { emit("    cmp rax, rbx"); emit("    setge al"); emit("    movzx rax, al"); }
                else if (op == "&&") { emit("    and rax, rbx"); emit("    test rax, rax"); emit("    setne al"); emit("    movzx rax, al"); }
                else if (op == "||") { emit("    or  rax, rbx"); emit("    test rax, rax"); emit("    setne al"); emit("    movzx rax, al"); }
                if (dstReg != "rax") emit("    mov " + dstReg + ", rax");
                break;
            }
        }
    }

    void genStringConcat(const ExprPtr& e) {
        if (target_ == Target::WIN64) {
            genExprTo(e->left, "rax");
            emit("    mov rsi, rax");           // левый указатель
            genExprTo(e->right, "rax");
            emit("    mov rdx, rax");           // правый указатель
            emit("    lea rcx, [concat_buf]");
            emit("    call strcopy");           // dst=rcx, src=rdx
            emit("    lea rcx, [concat_buf]");
            emit("    mov rdx, rsi");           // src = левая часть
            emit("    call strcat2");
            emit("    lea rax, [concat_buf]");
        } else {
            genExprTo(e->left, "rax");
            emit("    mov rsi, rax");
            genExprTo(e->right, "rax");
            emit("    mov rdx, rax");
            emit("    lea rdi, [concat_buf]");
            emit("    mov rsi, rdx");
            emit("    call strcopy");
            emit("    lea rdi, [concat_buf]");
            emit("    call strcat2");
            emit("    lea rax, [concat_buf]");
        }
    }

    // ---------- вызовы ----------
    void genCall(const ExprPtr& e) {
        const std::string& name = e->str;

        // ---- intToStr ----
        if (name == "intToStr") {
            if (e->args.size() != 1) { emit("    xor rax, rax"); return; }
            if (target_ == Target::WIN64) {
                genExprTo(e->args[0], "rcx");
                emit("    lea rdx, [numbuf]");
                emit("    mov r8, 32");
                emit("    call intToStr");
            } else {
                genExprTo(e->args[0], "rdi");
                emit("    lea rsi, [numbuf]");
                emit("    mov rdx, 32");
                emit("    call intToStr");
            }
            return;
        }

        // ---- strLen ----
        if (name == "strLen") {
            if (target_ == Target::WIN64) genExprTo(e->args[0], "rcx");
            else                          genExprTo(e->args[0], "rdi");
            emit("    call strLen");
            return;
        }

        // ---- пользовательская функция ----
        static const char* winRegs[]   = {"rcx", "rdx", "r8", "r9"};
        static const char* linuxRegs[] = {"rdi", "rsi", "rdx", "rcx"};
        const char** regs = (target_ == Target::WIN64) ? winRegs : linuxRegs;

        for (size_t i = 0; i < e->args.size() && i < 4; ++i) {
            genExprTo(e->args[i], "rax");
            emit("    push rax");
        }
        for (size_t i = e->args.size(); i-- > 0 && i < 4; ) {
            emit(std::string("    pop ") + regs[i]);
        }

        if (target_ == Target::WIN64) emit("    sub rsp, 32");
        emit("    call " + name);
        if (target_ == Target::WIN64) emit("    add rsp, 32");
    }
};

// ============================================================
// 7. MAIN
// ============================================================

static void printUsage(const char* prog) {
    std::cerr <<
        "Использование: " << prog << " <source.iva> [--target=win|linux] [-v]\n"
        "  --target=win   Windows x64 (по умолчанию)\n"
        "  --target=linux Linux x64\n"
        "  -v             подробный вывод\n"
        "Генерируется только .asm. Ассемблирование/линковка — вручную.\n";
}

int main(int argc, char** argv) {
    if (argc < 2) { printUsage(argv[0]); return 1; }

    std::string srcPath;
    Target target = Target::WIN64;
    bool verbose = false;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if      (a == "-v")             verbose = true;
        else if (a == "--target=win")   target = Target::WIN64;
        else if (a == "--target=linux") target = Target::LINUX64;
        else if (a.rfind("--", 0) == 0) {
            std::cerr << "Неизвестный параметр: " << a << "\n";
            printUsage(argv[0]); return 1;
        }
        else srcPath = a;
    }
    if (srcPath.empty()) { printUsage(argv[0]); return 1; }

    std::ifstream in(srcPath);
    if (!in) { std::cerr << "Не удалось открыть " << srcPath << "\n"; return 1; }
    std::stringstream ss; ss << in.rdbuf();
    std::string src = ss.str();

    // ---- ЛЕКСИКА ----
    std::cout << "=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===\n";
    Lexer lex(src);
    std::vector<std::string> lexErrors;
    std::vector<Token> tokens = lex.tokenize(lexErrors);
    for (auto& e : lexErrors) std::cout << e << "\n";
    if (verbose) {
        for (size_t i = 0; i < tokens.size() && i < 40; ++i) {
            std::cout << "  " << kindName(tokens[i].kind);
            if (!tokens[i].value.empty()) std::cout << " (" << tokens[i].value << ")";
            std::cout << "  @" << tokens[i].line << ":" << tokens[i].col << "\n";
        }
    }
    std::cout << "  всего токенов: " << tokens.size() << "\n";
    if (!lexErrors.empty()) { std::cout << "Трансляция прервана.\n"; return 2; }

    // ---- СИНТАКСИС ----
    std::cout << "\n=== СИНТАКСИЧЕСКИЙ АНАЛИЗ ===\n";
    Parser parser(tokens);
    std::vector<std::string> synErrors;
    Program prog;
    try { prog = parser.parse(synErrors); }
    catch (std::exception&) {
        for (auto& s : synErrors) std::cout << s << "\n";
        std::cout << "Трансляция прервана.\n"; return 3;
    }
    for (auto& s : synErrors) std::cout << s << "\n";
    std::cout << "  AST построен.\n";

    // ---- СЕМАНТИКА ----
    std::cout << "\n=== СЕМАНТИЧЕСКИЙ АНАЛИЗ ===\n";
    SemanticAnalyzer sem;
    auto semErrors = sem.analyze(prog);
    if (!semErrors.empty()) {
        for (auto& s : semErrors) std::cout << s << "\n";
        std::cout << "Трансляция прервана.\n"; return 4;
    }
    std::cout << "  Семантических ошибок нет.\n";

    // ---- ГЕНЕРАЦИЯ ASM ----
    std::cout << "\n=== ГЕНЕРАЦИЯ АССЕМБЛЕРА (NASM) ===\n";
    AsmGen asmgen(target);
    std::string asmCode = asmgen.generate(prog);

    std::string base = srcPath;
    size_t dot = base.find_last_of('.');
    if (dot != std::string::npos) base = base.substr(0, dot);
    std::string asmPath = base + ".asm";

    { std::ofstream f(asmPath); f << asmCode; }
    std::cout << "Ассемблер: " << asmPath << "\n";
    std::cout << "Сборка (Windows): nasm -f win64 -o " << base << ".obj " << asmPath << "\n";
    std::cout << "Сборка (Linux):   nasm -f elf64 -o " << base << ".o " << asmPath << "\n";
    std::cout << "                  ld -o " << base << " " << base << ".o\n";

    return 0;
}