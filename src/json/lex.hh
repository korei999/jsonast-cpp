#pragma once

#include "String.hh"
#include "Arena.hh"

namespace json
{

struct Token
{
    enum TYPE
    {
        LBRACE = '{',
        RBRACE = '}',
        LBRACKET = '[',
        RBRACKET = ']',
        QUOTE = '"',
        IDENT = 'I',
        NUMBER = 'N',
        TRUE = 'T',
        FALSE = 'F',
        NULL_ = 'n',
        ASSIGN = ':',
        COMMA = ',',
        DOT = '.',
        UNHANDLED = 'X',
        EOF_ = '\0',
    } type;
    adt::String svLiteral;
};

struct Lexer
{
    adt::Arena* pArena {};
    adt::String sFile;
    size_t pos = 0;

    Lexer(adt::Arena* p) : pArena(p) {}

    void loadFile(adt::String path);
    void skipWhiteSpace();
    Token number();
    Token stringNoQuotes();
    Token string();
    Token character(enum Token::TYPE type);
    Token next();
};

} /* namespace json */

