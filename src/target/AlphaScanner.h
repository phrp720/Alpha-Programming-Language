#ifndef ALPHA_YYFLEXLEXER
#define ALPHA_YYFLEXLEXER
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct alpha_token_t {
  unsigned int numLine;
  unsigned int numToken;
  string content;
  string type;
  string name;
  string tokenteam;
};

/*
enum KEYWORDS { WHILE,IF,ELSE,FOR,FUNCTION,RETURN,BREAK,CONTINUE,LOCAL,NIL,FALSE,TRUE,AND,NOT,OR };
enum OPERATORS { DEF, PLUS, MINUS, PLUS_PLUS, MINUS_MINUS, TIMES, SLASH, EQL, NEQ, LSS, GTR, LEQ, GEQ, PERCENTAGE};
enum PUNCTUATION { LEFT_BRACE, RIGHT_BRACE, SEMICOLON, LEFT_PARENTHESES, RIGHT_PARENTHESES, COMMA, PERIOD, PERIOD_PERIOD, COLON, COLON_COLON,LEFT_BRACKET, RIGHT_BRACKET};
enum COMMENT{ LINE_COMMENT, BLOCK_COMMENT, MULITLINE_COMMENT, NESTED_COMMENT };*/

class AlphaScanner : public yyFlexLexer {
  public:
    unsigned int numLine;
    int numToken;
    vector<alpha_token_t> listTokens;
    void addToken(unsigned int line, string content, string token_team,string type, string name);
    void printTable(std::ostream& arg_yyout);
    AlphaScanner(std::istream * is = 0, std::ostream * os = 0);
    int alpha_yylex();
    virtual int yylex();
};

#endif
