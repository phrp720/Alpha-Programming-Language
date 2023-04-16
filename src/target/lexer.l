%{
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include "AlphaScanner.h"
#include"OffsetCounter.h"
#include"expression.h"
#include"Symbol_table.h"
#include"quads.h"
#include"expression_methods.h"
#include"tempfunctions.h"
#include "parser.hpp"

using namespace std;

#define ENUM_TO_STR(ENUM) std::string(#ENUM)

int comment_nesting = 0;
int mynumLine = 1;
string token_teams[] = {"KEYWORD","OPERATOR","PUNCTUATION","CONSTINT","CONSTFLOAT","COMMENT", "ID", "STRING"};

void errorTerminate(string message);

bool numline_diff(const alpha_token_t &a,const alpha_token_t &b){
    return a.numLine<b.numLine&& a.numToken<b.numToken;
}
%}

%option noyywrap
%option yylineno
%option outfile = "./lexer.cpp"
%option yyclass="AlphaScanner"
%option c++

%x SINGLE_STRING

comment "//".*
letter [a-zA-Z]
digit [0-9]
underscore [_]
number {digit}+
id {letter}({letter}|{digit}|{underscore})*
float {number}+"."{digit}*

%%
"if" {
    addToken(numLine, YYText(),token_teams[0],ENUM_TO_STR(IF), "enumerated");
    return IF;
}
"while" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(WHILE), "enumerated");
    return WHILE;
}
"else" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(ELSE),"enumerated");
    return ELSE;
}
"for" {
    addToken(numLine, YYText(), token_teams[0],ENUM_TO_STR(FOR), "enumerated");
    return FOR;
}
"function" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(FUNCTION), "enumerated");
    return FUNCTION;
}
"return" {
    addToken(numLine, YYText(), token_teams[0],ENUM_TO_STR(RETURN), "enumerated");
    return RETURN;
}
"break" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(BREAK), "enumerated");
    return BREAK;
}
"continue" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(CONTINUE), "enumerated");
    return CONTINUE;
}
"and" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(AND), "enumerated");
    return AND;
}
"local" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(LOCAL), "enumerated");
    return LOCAL;
}
"nil" {
    addToken(numLine, YYText(), token_teams[0],ENUM_TO_STR(NIL), "enumerated");
    return NIL;
}
"false" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(FALSE), "enumerated");
    return FALSE;
}
"true" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(TRUE), "enumerated");
    return TRUE;
}
"or" {
    addToken(numLine, YYText(), token_teams[0],ENUM_TO_STR(OR), "enumerated");
    return OR;
}
"not" {
    addToken(numLine, YYText(),token_teams[0], ENUM_TO_STR(NOT), "enumerated");
    return NOT;
}
"=" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(DEF), "enumerated");
    return DEF;
}
"+" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(PLUS), "enumerated");
    return PLUS;
}
"-" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(MINUS), "enumerated");
    return MINUS;
}
"++" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(PLUS_PLUS), "enumerated");
    return PLUS_PLUS;
}
"--" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(MINUS_MINUS), "enumerated");
    return MINUS_MINUS;
}
"*" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(TIMES), "enumerated");
    return TIMES;
}
"==" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(EQL), "enumerated");
    return EQL;
}
"!=" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(NEQ), "enumerated");
    return NEQ;
}
"<" {
    addToken(numLine, YYText(), token_teams[1],ENUM_TO_STR(LSS), "enumerated");
    return LSS;
}
">" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(GTR), "enumerated");
    return GTR;
}
"<=" {
    addToken(numLine, YYText(), token_teams[1], ENUM_TO_STR(LEQ), "enumerated");
    return LEQ;
}
">=" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(GEQ), "enumerated");
    return GEQ;
}
"\/" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(SLASH), "enumerated");
    return SLASH;
}
"{" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(LEFT_BRACE),"enumerated");
    return LEFT_BRACE;
}
"}" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(RIGHT_BRACE), "enumerated");
    return RIGHT_BRACE;
}
";" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(SEMICOLON), "enumerated");
    return SEMICOLON;
}
"," {
    addToken(numLine, YYText(), token_teams[2], ENUM_TO_STR(COMMA), "enumerated");
    return COMMA;
}
"(" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(LEFT_PARENTHESES), "enumerated");
    return LEFT_PARENTHESES;
}
")" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(RIGHT_PARENTHESES), "enumerated");
    return RIGHT_PARENTHESES;
}
"\." {
    addToken(numLine, YYText(), token_teams[2],ENUM_TO_STR(PERIOD), "enumerated");
    return PERIOD;
}
".." {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(PERIOD_PERIOD), "enumerated");
    return PERIOD_PERIOD;
}
":" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(COLON), "enumerated");
    return COLON;
}
"::" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(COLON_COLON), "enumerated");
    return COLON_COLON;
}
"%" {
    addToken(numLine, YYText(),token_teams[1], ENUM_TO_STR(PERCENTAGE), "enumerated");
    return PERCENTAGE;
}
"[" {
    addToken(numLine, YYText(),token_teams[2], ENUM_TO_STR(LEFT_BRACKET), "enumerated");
    return LEFT_BRACKET;
}
"]" {
    addToken(mynumLine, YYText(),token_teams[2], ENUM_TO_STR(RIGHT_BRACKET), "enumerated");
    return RIGHT_BRACKET;
}
{id} {
    addToken(mynumLine, YYText(), token_teams[6], YYText(), "char*");
    yylval.stringValue = strdup(YYText());
    return ID;
}
{number} {
    addToken(mynumLine, YYText(),token_teams[3], YYText(), "integer");
    yylval.intValue = atoi(yytext);
    return CONSTINT;
 }
{float} {
    addToken(mynumLine, YYText(), token_teams[4], YYText(), "float");
    yylval.realValue = atof(YYText());
    return REALCONST;
}
{comment} {
    addToken(mynumLine, "", token_teams[5], ENUM_TO_STR(LINE_COMMENT), "enumerated");
}
"\n" {
    mynumLine++;
    return NEWLINE;
}
\"\" {
    addToken(mynumLine, "", token_teams[7], "", "string");
    yylval.stringValue = strdup("");
    return STRING;
 }
\"                          BEGIN(SINGLE_STRING);
<SINGLE_STRING>{
  \n                        { errorTerminate("The string misses \" to termiate before newline"); }
  <<EOF>>                   { errorTerminate("The string misses \" to termiate before EOF"); }
  ([^\\\"]|\\.)*            {
      string oldString = YYText();
      string newString = "";
      for (long unsigned int i = 0; i < oldString.size(); i++) {
          if (oldString[i] == '\\') {
            if (oldString[i+1] == '\\') {
                newString.push_back('\\');
                i++;
            } else if (oldString[i+1] == 'n') {
                newString.push_back('\n');
                i++;
            } else if (oldString[i+1] == 't') {
                newString.push_back('\t');
                i++;
            } else if (oldString[i+1] == '"') {
                newString.push_back('"');
                i++;
            } else {
                newString.push_back(oldString[i]);
            }
          } else {
            newString.push_back(oldString[i]);
          }
      }
      addToken(mynumLine, newString, token_teams[7], "", "string");
      yylval.stringValue = strdup(yytext);
      return STRING;
  }
  \"                        BEGIN(INITIAL);
  .                         ;
}
"/*" {                      int flag_for_nestedComments = 1;
    int c;
    int open_comment_counter = 0;
    int close_comment_counter = 0;
    int comm_nest_counter = 1; 		/*starts 1 because we have already read one*/
    while((c = yyinput()) != EOF){
        if(c == '/'){
            if((c = yyinput()) == '*'){
                if(comm_nest_counter==1 && flag_for_nestedComments==1){ /*first nested*/
                        addToken(mynumLine,"COMMENT",token_teams[5],ENUM_TO_STR(NESTED_COMMENT),"enumerated");
                    flag_for_nestedComments=0;
                }
                open_comment_counter++;
                ++comm_nest_counter;
                if(open_comment_counter>=2){
                        addToken(mynumLine,"",token_teams[5],ENUM_TO_STR(NESTED_COMMENT),"enumerated");
                }

            }else{
                unput(c);
            }
        }else if(c == '*'){
            if((c = yyinput()) == '/'){
                --comm_nest_counter;
                if(comm_nest_counter == 0){
                    break;
                }
                if(close_comment_counter==0){ 	/*first multiline comment*/
                    addToken(mynumLine,"",token_teams[5],ENUM_TO_STR(MULTILINE_COMMENT),"enumerated");
                    close_comment_counter++;
                }else if(open_comment_counter>close_comment_counter){
                    close_comment_counter++;
                }
                if(close_comment_counter==open_comment_counter){ /*starts counting comments from the start*/
                    close_comment_counter=0;
                    open_comment_counter=0;
                }
            }else{
                unput(c);
            }
        }
    }
    if(comm_nest_counter > 0){
        fprintf(stderr, "ERROR, nested comment doesnt close, line: %d\n", mynumLine);
        exit(0);
    }
    if(flag_for_nestedComments==1){ /*single multiline comment*/
            addToken(mynumLine,"",token_teams[5],ENUM_TO_STR(MULTILINE_COMMENT),"enumerated");
    }
    }
[\r  \t\v]  { /* ignore whitespace */ }
. { errorTerminate("Unkown character: " + (string) YYText()); }
<<EOF>> { return 0; }
%%

void errorTerminate(string message) {
    cerr << message << " in line " << endl;
    exit(0);
}
