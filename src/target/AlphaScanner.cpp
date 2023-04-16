#include <vector>
#include "AlphaScanner.h"
#include"OffsetCounter.h"
#include"expression.h"
#include"Symbol_table.h"
#include"quads.h"
#include"expression_methods.h"
#include"tempfunctions.h"
#include "parser.hpp"

AlphaScanner::AlphaScanner(std::istream * is, std::ostream * os): yyFlexLexer(is, os) {
	listTokens = vector<alpha_token_t>();
	numLine = 0;
    numToken = 0;
}

int AlphaScanner::alpha_yylex() {
	int result;
	while ((result = yylex()) == COMMENT);
	return result;
}

void AlphaScanner::addToken(unsigned int line, string content, string token_team,string type, string name) {
    struct alpha_token_t alphatoken;
    alphatoken.numLine = line;
    alphatoken.content = content;
    alphatoken.type = type;
    alphatoken.name = name;
    alphatoken.numToken = ++numToken;
    alphatoken.tokenteam=token_team;
    listTokens.push_back(alphatoken);
    return;
}

void AlphaScanner::printTable(std::ostream& arg_yyout) {
    for(vector<struct alpha_token_t>::iterator token = listTokens.begin(); token != listTokens.end(); ++token) {
        arg_yyout << token->numLine << ": " << "#" << token->numToken << " \"" << token->content << "\" " << token->tokenteam << " " << token->type << " <- " << token->name << "\n";
    }
}
