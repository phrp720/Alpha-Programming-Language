%{
    #include<iostream>
    #include <string>
    #include <vector>
    #include <algorithm>
    #include<assert.h>
    #include<iomanip>
    #include "AlphaScanner.h"
    #include "Instructions.h"
    #include "Symbol_table.h"
    #include "quads.h"
    #include "expression.h"
    #include "OffsetCounter.h"
    #include "expression_methods.h"
    #include "tempfunctions.h"
    extern AlphaScanner * lexer;
    Symbol_table *symbol_table = new Symbol_table();
    Temp_fun *tempFun = new Temp_fun();
    CurrentsScopeOffset * offsetCount = new CurrentsScopeOffset();
    Quads *quads = new Quads();
    InstructionTable* instruction_table = new InstructionTable();
    #define yylex() lexer->alpha_yylex();
    int yyerror (std::string yaccProvidedMessage);
    int infuncCounter=0;/*gia ta returns*/
    int loopcounter = 0;
    unsigned int Func_Jump = 0;
    int funcNameLocal = 0;
    int scope = 0;
    int ifflag = 0;
    extern int mynumLine;
    vector <int> loopcounterstack;
    vector <int> offsetstack;
    extern int mynumLine;
    void initiallizer();
    /*check*/

%}

%defines
%output "./parser.cpp"

%union {
    char* stringValue;
    int intValue;
    double realValue;
    float floatValue;
    unsigned unsigned_val;
    Socaller* caller;
    expr* exprValue;
    Node* nodetype;
    StmtInfo* stmtInfo;
    for_things* forprefix;
    std::list<expr*>* elisttype;
}

%start program

%token <stringValue> ID
%token <stringValue> STRING
%token <intValue> CONSTINT
%token <floatValue> REALCONST
%type <stmtInfo> stmt statement_loop loopstmt  forstmt whilestmt block
%type <exprValue> assignexpr term primary call expr member lvalue const indexedelem
%type <stringValue>  func_choose_name
%type <unsigned_val> whilecond whilestart ifprefix elseprefix funcbody N M
%type <caller> methodcall callsuffix normcall
%type <nodetype>  funcdef funcprefix
%type <elisttype>  elist  objectdef  indexed
%type <forprefix> forprefix


/*diafanies lene funcdef kai funcprefix oti einai node* */
/*oti einai to elist kai to index einai kai to objectapps*/


%token WHILE IF ELSE FOR FUNCTION RETURN BREAK CONTINUE LOCAL NIL FALSE TRUE
%token DEF PLUS MINUS PLUS_PLUS MINUS_MINUS TIMES SLASH EQL NEQ LSS GTR LEQ GEQ PERCENTAGE
%token SEMICOLON LEFT_BRACE RIGHT_BRACE LEFT_PARENTHESES RIGHT_PARENTHESES COMMA PERIOD PERIOD_PERIOD COLON COLON_COLON LEFT_BRACKET RIGHT_BRACKET
%token NEWLINE COMMENT

%right DEF
%right NOT PLUS_PLUS MINUS_MINUS UMINUS
%left PLUS MINUS
%left TIMES SLASH PERCENTAGE
%left PERIOD PERIOD_PERIOD
%left LEFT_PARENTHESES RIGHT_PARENTHESES
%left LEFT_BRACE RIGHT_BRACE
%left OR
%left AND
%nonassoc EQL NEQ
%nonassoc GTR LEQ GEQ LSS


%destructor {free($$);} ID

%%
program: {initiallizer();} statement_loop {}

statement_loop:     statement_loop stmt {
                       	$$ = new StmtInfo();
                        /*$2->breakList = std::vector<int>($1->breakList.begin() + 1, $1->breakList.end());
                        $2->contList = std::vector<int>($1->contList.begin() + 1, $1->contList.end());*/

                        vector<int> vectBreak($1->breakList.size() + $2->breakList.size());
                        merge($1->breakList.begin(),
                                           $1->breakList.end(),
                                           $2->breakList.begin(),
                                           $2->breakList.end(),
                                           vectBreak.begin());

                        vector<int> vectCont($1->contList.size() + $2->contList.size());
                        merge($1->contList.begin(),
                                           $1->contList.end(),
                                           $2->contList.begin(),
                                           $2->contList.end(),
                                           vectCont.begin());
                        $$->breakList.assign(vectBreak.begin(), vectBreak.end());
                        $$->contList.assign(vectCont.begin(), vectCont.end());
                    }
                    | {
                       	StmtInfo* stmt = new StmtInfo();
                        $$ = stmt;
                    }
                    ;

stmt:               expr SEMICOLON {
                       	StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = false;
                        $$ = stmt;

                    }
                    | NEWLINE {
                       	StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = true;
                        $$ = stmt;
                    }
                    | ifstmt {
                        StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = true;
                        $$ = stmt;
                    };
                    | whilestmt {
                       	StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = true;
                        $$ = stmt;
                    }
                    | block {
                        StmtInfo* stmt1 = new StmtInfo();
                        stmt1->isLoopStmt = true;
                        $$ = stmt1;
                    }
                    | forstmt {
                        StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = true;
                        $$ = stmt;
                    }
                    | returnstmt {
                        StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = false;
                        $$ = stmt;
                    }
                    | BREAK SEMICOLON {
                        StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = true;
                        $$ = stmt;

                        if (loopcounter > 0) {
                        	stmt->breakList.push_back(1);
                        	quads->emit(iopcode::jump, NULL, NULL, NULL, 0, mynumLine);
                        } else {
                        	yyerror("Break must be inside a loop");
                        }

                    }
                    | CONTINUE SEMICOLON {


                        StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = true;
                        $$ = stmt;
                        if (loopcounter > 0) {
                           quads->emit(iopcode::jump, NULL, NULL, NULL, 0, mynumLine);
                        }  else {
                        	yyerror("Continue must be inside a loop");
                        }
                    }
                    | SEMICOLON {
                        StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = false;
                        $$ = stmt;
                    }
                    | funcdef {

                        StmtInfo* stmt = new StmtInfo();
                        stmt->isLoopStmt = true;
                        $$ = stmt;
                    }
                    ;

expr:               assignexpr { $$ = $1; }
                    | expr PLUS expr{

                        $$ = arithmetic_op_Expr(iopcode::add, $1 , $3);
                    }
                    | expr MINUS expr {

                        $$ = arithmetic_op_Expr(iopcode::sub, $1 , $3);
                    }
                    | expr TIMES expr{

                        $$ = arithmetic_op_Expr(iopcode::mul, $1 , $3);
                    }
                    | expr SLASH expr {

                        $$ = arithmetic_op_Expr(iopcode::div_i, $1 , $3);
                    }
                    | expr PERCENTAGE expr {

                        $$ = arithmetic_op_Expr(iopcode::mod, $1 , $3);
                    }
                    | expr GEQ expr {

                        $$ = relop_op_Expr(iopcode::if_greatereq, $1, $3);
                     }
                    | expr GTR expr {
                    	;
                        $$ = relop_op_Expr(iopcode::if_greater, $1, $3);
                     }
                    | expr LEQ expr {
                        $$ = relop_op_Expr(iopcode::if_lesseq, $1, $3);
                     }
                    | expr LSS expr {
                        $$ = relop_op_Expr(iopcode::if_less, $1, $3);
                     }
                    | expr EQL expr {
                        $$ = relop_op_Expr(iopcode::if_eq, $1, $3);
                     }
                    | expr NEQ  expr {
                        $$ = relop_op_Expr(iopcode::if_noteq, $1, $3);
                     }
                    | expr AND expr {
                        expr* exp=new expr();
                        exp->type = boolexpr_e;
                        if($1->isTmp())
                        {
							exp->sym = $1->sym;
							exp->name=$1->name;
						}
						else{
							exp->sym=tempFun->newtemp();
							exp->name=tempFun->getCurrentTempNum();
						}

                        quads->emit(iopcode::and_i, $1, $3, exp, 0, mynumLine);
                        $$ = exp;
                    }
                    | expr OR expr {


                        expr* exp=new expr();
                        exp->type =boolexpr_e;
                        if($1->isTmp()){
							exp->sym= $1->sym;
							exp->name=$1->name;
						}
						else {
							exp->sym = tempFun->newtemp();
							exp->name=tempFun->getCurrentTempNum();
						}
                        quads->emit(iopcode::or_i, $1, $3, exp, 0, mynumLine);
                        $$ = exp;
                     }
                    | term { $$ = $1; }
                    ;

term:               LEFT_PARENTHESES expr RIGHT_PARENTHESES { $$ = $2; }
                    | MINUS expr %prec MINUS{
                        check_arith($2);
                        $$ = new expr();
                        $$->type = arithexpr_e;
                        if($2->isTmp()){
							$$->sym = $2->sym;
							$$->name=$2->name;
						}
                        else{
							$$->sym=tempFun->newtemp();
							$$->name=tempFun->getCurrentTempNum();
                        }
                        quads->emit(iopcode::uminus, $2, NULL, $$, 0, mynumLine);
                    }
                    | NOT expr {/*exoyme merikh apotimish edw*/
                        $$ = new expr();
                        $$->type = arithexpr_e;
                        if($2->isTmp()){
							$$->sym=$2->sym;
							$$->name=$2->name;
						}
						else {
							$$->sym=tempFun->newtemp();
							$$->name=tempFun->getCurrentTempNum();
						}
                        quads->emit(iopcode::not_i, $2, NULL, $$, 0, mynumLine);
                       /* $$->sym=NULL;*/
                    }
                    | PLUS_PLUS lvalue {
                        check_arith($2);
                        if($2->type == tableitem_e){
                            $$ = emit_iftableitem($2);
                            quads->emit(iopcode::add, $$, newexpr_constint(1),$$, 0, mynumLine);
                            quads->emit(iopcode::tablesetelem, $2, $2->index, $$, 0, mynumLine);
                        }
                        else{
                            quads->emit(add, $2,newexpr_constint(1), $2, 0, mynumLine);
                            $$ = new expr();
                            $$->type = arithexpr_e;
                            $$->sym = tempFun->newtemp();
                            $$->name=tempFun->getCurrentTempNum();
                            quads->emit(iopcode::assign, $2, NULL, $$, 0, mynumLine);
                        }
                    }
                    | lvalue PLUS_PLUS  {
                        check_arith($1);
                        $$ = new expr();
                        $$->type = var_e;
                        $$->sym = tempFun->newtemp();
                        $$->name=tempFun->getCurrentTempNum();
                        if ($1->type == tableitem_e) {
                            expr* value = emit_iftableitem($1);
                            quads->emit(iopcode::assign, value, NULL, $$, 0, mynumLine);
                            quads->emit(iopcode::add, value, newexpr_constint(1), value, 0, mynumLine);
                            quads->emit(iopcode::tablesetelem, $1, $1->index, value, 0, mynumLine);
                        }
                        else {
                            quads->emit(iopcode::assign, $1, NULL, $$, 0, mynumLine);
                            quads->emit(iopcode::add, $1, newexpr_constint(1), $1, 0, mynumLine);
                        }
                    }
                    | MINUS_MINUS lvalue {
                        check_arith($2);
                        if($2->type == tableitem_e){
                            $$ = emit_iftableitem($2);
                            quads->emit(iopcode::sub, $$, newexpr_constint(1),$$, 0, mynumLine);
                            quads->emit(iopcode::tablesetelem, $2, $2->index, $$, 0, mynumLine);
                        }
                        else{
                            quads->emit(iopcode::sub, $2,newexpr_constint(1), $2, 0, mynumLine);
                            $$ = new expr();
                            $$->type = arithexpr_e;
                            $$->sym = tempFun->newtemp();
                             $$->name=tempFun->getCurrentTempNum();
                            quads->emit(iopcode::assign, $2, NULL, $$, 0, mynumLine);
                       }
                    }
                    | lvalue MINUS_MINUS {
                        check_arith($1);
                        $$ = new expr();
                        $$->type = var_e;
                        $$->sym = tempFun->newtemp();
                         $$->name=tempFun->getCurrentTempNum();
                        if ($1->type == tableitem_e) {
                            expr* value = emit_iftableitem($1);
                            quads->emit(iopcode::assign, value, NULL, $$, 0, mynumLine);
                            quads->emit(iopcode::sub, value, newexpr_constint(1), value, 0, mynumLine);
                            quads->emit(iopcode::tablesetelem, $1, $1->index, value, 0, mynumLine);
                        }
                        else {
                            quads->emit(iopcode::assign, $1, NULL, $$, 0, mynumLine);
                            quads->emit(iopcode::sub, $1, newexpr_constint(1), $1, 0, mynumLine);
                        }
                     }
                    | primary { $$ = $1; }

                    ;

assignexpr:             lvalue DEF expr {
							if ($1->type == tableitem_e){
								/*$1->index->name = "PIPES";*/
								quads->emit(iopcode::tablesetelem,$1->index,$3,$1,0, mynumLine);
								$$= emit_iftableitem($1);
								$$->type=assignexpr_e;
							} else {
								quads->emit(iopcode::assign,$3,NULL, $1, 0, mynumLine);
								$$=new expr();
								$$->type=assignexpr_e;
								$$->sym = tempFun->newtemp();
								$$->name=tempFun->getCurrentTempNum();

								quads->emit(iopcode::assign,$1,NULL,$$, 0, mynumLine);
							}

                        }/*etoimo*/

primary:                lvalue{ $$ = emit_iftableitem($1); }
                        | call { $$ = $1; }
                        | objectdef{
                        	$$->name=tempFun->getCurrentTempNum();
                        	$$->type=tableitem_e;
                        }
                        |  LEFT_PARENTHESES funcdef RIGHT_PARENTHESES {
                            $$ = new expr();
                            $$->type = programfunc_e;
                            $$->sym = $2;
                            $$->name = $2->id;
                        }
                        | const {$$=$1;}
                        /*etoimo*/
                        ;

lvalue:      ID {
                Node* sym = symbol_table->lookup($1, scope);
                if(!sym) {

                    if (scope) {
                        sym = symbol_table->insert($1, mynumLine, local_variable, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                    } else {
                        sym = symbol_table->insert($1, mynumLine, global_variable, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                    }

                    offsetCount->inccurrScopeOffset();
                }
                $$ = create_lvalue_expr(sym->id, sym);
            }
            | LOCAL ID {
                Node* sym = symbol_table->lookup($2, scope);

                if(symbol_table->lookup($2, 0) != NULL && symbol_table->lookup($2,0)->type == library_function){
                    yyerror("ERROR  Shadow of library function");
                } else if(!symbol_table->lookup($2,scope)){
                    sym = symbol_table->insert($2, mynumLine, local_variable, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                    offsetCount->inccurrScopeOffset();
                } else if(symbol_table->lookup($2,scope) && symbol_table->lookup($2,scope)->type == user_function){
                    yyerror("ERROR: a user function with this name already defined.");
                }
                $$ = create_lvalue_expr(sym->id, sym);
                free($2);
             }
            | COLON_COLON ID {
                Node* sym = symbol_table->lookup($2, 0);
                if(!sym){
                     yyerror("ERROR  no global variable declared");
                }

                $$ = create_lvalue_expr(sym->id, sym);
                free($2);
            }
            | member { $$ = $1; }
            /*etoimo*/
            ;

member:                     lvalue PERIOD ID { $$ = member_item($1, $3); }
                                | lvalue LEFT_BRACKET expr RIGHT_BRACKET
                                {
                                    $1 = emit_iftableitem($1);
                                    $$ = new expr();
                                    $$->type=tableitem_e;
                                    $$->sym=$1->sym;
                                    $$->name = $1->name;
                                    $$->index=$3;
                                }
                                | call PERIOD ID { $$ = member_item($1,$3); free($3);}
                                | call LEFT_BRACKET expr RIGHT_BRACKET{

                                }
                                ;

call:                       call LEFT_PARENTHESES elist RIGHT_PARENTHESES {
                                $$ =  make_call($1,$3);
                            }
                            | lvalue callsuffix {
                                $1 = emit_iftableitem($1);
                                if ($2->method ){
                                    expr* t = $1;
                                    $1 = emit_iftableitem(member_item(t, $2->name));
                                   /* $2.elist->next = t;prosthetei sto expr* vector ena stoixeio*/
                                }
                                $$ = make_call($1, $2->elist);
                            }
                            | LEFT_PARENTHESES funcdef RIGHT_PARENTHESES LEFT_PARENTHESES elist RIGHT_PARENTHESES {
                                expr* func = new expr();
                                func->type = programfunc_e;
                                func->sym = $2;
                                $$ = make_call(func, $5);
                            }
                            ;

callsuffix: /*etoimo*/                normcall{ $$=$1; }
                                | methodcall{ $$=$1; }
                                ;

normcall:                   LEFT_PARENTHESES elist RIGHT_PARENTHESES{
                                            $$ = new Socaller();
                                            $$->name = "";
                                            $$->method = 0;
                                            $$->elist = $2;
                            }/*etoimo*/
                            ;

methodcall:                 PERIOD_PERIOD ID LEFT_PARENTHESES elist RIGHT_PARENTHESES {
                                $$=new Socaller();
                                $$->elist = $4;
                                $$->method = 1;
                                $$->name = $2;
                                // TODO: FIX IT pithanon ortho
                            }


 elist:						expr{
 								std::list<expr*>* objList = new std::list<expr*>();
 								objList->push_back($1);
 								$$ = objList;
 							}
							| expr COMMA elist {
								std::list<expr*>* objList = new std::list<expr*>();
								objList = $3;
								objList->push_back($1);
								$$ = $3;
							}
							| {
								std::list<expr*> *objList = new std::list<expr*>();
								$$ = objList;
							}
							;

/*
elist:                      elist_fill {
								$$ = $1;
                            }
                            | {
								$$ = new std::list<expr*>();
                            }
                            ;

elist_fill:                 elist_loop expr {
								$1->push_back($2);
                            }
                            ;

elist_loop:                 elist_loop expr COMMA {
								$1->push_back($2);
                            }
                            | {
								$$ = new std::list<expr*>();
                            }
                            ;
*/



 objectdef:                 LEFT_BRACKET elist RIGHT_BRACKET {
								expr * toRet = new expr();
								toRet->type=newtable_e;
								int i=0;
								$2->reverse();
								toRet->sym = tempFun->newtemp();
                                toRet->name = tempFun->getCurrentTempNum();

                                quads->emit(iopcode::tablecreate,toRet,NULL,NULL,0,mynumLine);
                                for (expr* exp : *($2)) {
                                    quads->emit(iopcode::tablesetelem, newexpr_constint(i++), exp, toRet, 0, mynumLine);
                                }

								$$ = $2;
							}
							|  LEFT_BRACKET indexed RIGHT_BRACKET{
								expr * toRet = new expr();
								toRet->type = newtable_e;
								$2->reverse();
								toRet->sym = tempFun->newtemp();
                                toRet->name = tempFun->getCurrentTempNum();

                                quads->emit(iopcode::tablecreate,toRet,NULL,NULL,0,mynumLine);

								for (expr* exp : *($2)) {
                                	quads->emit(iopcode::tablesetelem, exp, NULL, toRet, 0,mynumLine);
                                }

								$$ =$2;
							}

                            ;

indexed: indexedelem {
			$$->push_back($1);
		}
        | indexedelem COMMA indexed {
			$3->push_back($1);
			$$ = $3;
        }
        ;

indexedelem: LEFT_BRACE expr COLON expr RIGHT_BRACE {
				$2->index =  $4;
				$$ = $2;
			}
			;


block:    LEFT_BRACE {if(ifflag==1){scope--;ifflag=0;}scope++; } statement_loop RIGHT_BRACE { symbol_table->hide(scope); scope--;$$=$3; }
        ;

funcdef:        funcprefix funcblockstart LEFT_PARENTHESES { scope++;infuncCounter++; } idlist RIGHT_PARENTHESES{ scope--;offsetCount->enterScopeSpace();
                offsetCount->resetfunctionlocalsoffset(); } funcbody funcblockend {
                        offsetCount->exitScopeSpace();
                        $1->locals=$funcbody;
                        int tmp;
                        if(offsetstack.size()!=0){
                         tmp=offsetstack.back();
                         offsetstack.pop_back();
                        }
                        offsetCount->restorecurrscopeoffset(tmp);
                        $$=$1;
                        quads->emit(funcend,NULL,NULL, create_lvalue_expr($$->id, $$), 0,mynumLine); /* TODO: na to 3anadoume auto */
                        quads->patchLabel(Func_Jump,quads->nextQuadLabel());
                    }

funcprefix: FUNCTION func_choose_name{
                $$=new Node($2, mynumLine, user_function, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                $$->functionAddress=quads->nextQuadLabel();
                Func_Jump = quads->nextQuadLabel();
                quads->emit(jump, NULL, NULL, NULL, quads->nextQuadLabel()+3,mynumLine); /*na doyme to jump */
                quads->emit(funcstart,NULL,NULL, create_lvalue_expr($$->id, $$),0,mynumLine);
                offsetstack.push_back(offsetCount->currScopeOffset());
                offsetCount-> enterScopeSpace();
                offsetCount->resetformalargsoffset();
            }


funcbody:   block {
                $$ = offsetCount->currScopeOffset();
                offsetCount->exitScopeSpace();
                infuncCounter--;
            }


func_choose_name:     ID
                {
                if(symbol_table->lookup($1,0) != NULL && symbol_table->lookup($1,0)->type == library_function){
                               yyerror("ERROR : shadow");
                } else{
                    if(!symbol_table->lookup($1,scope))
                    {
                        symbol_table->insert($1, mynumLine, user_function, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                    }
                    else if(symbol_table->lookup($1, scope)->id == $1)
                    {
                        yyerror("ERROR : user function already defined");
                    }
                  /*  else if(symbol_table->lookup($2,scope)->type == library_function)
                    {
                        yyerror("ERROR : library function already defined");
                    }*/
                    else if(symbol_table->lookup($1, scope)->id == $1
                        && (symbol_table->lookup($1,scope)->type == global_variable
                        || symbol_table->lookup($1,scope)->type == local_variable) )
                    {
                        yyerror("ERROR  : variable already defined");
                    }
                  }
                  $$=$1;
                }
                |
                {
                    symbol_table->insert("_f"+std::to_string(funcNameLocal), mynumLine, user_function, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                    std::string name = "_f" + std::to_string(funcNameLocal);
                    funcNameLocal++;
                    $$ = (char*) name.c_str();
                }
                ;
funcblockstart:         { loopcounterstack.push_back(loopcounter);loopcounter=0; }
funcblockend:           {
                            if(loopcounterstack.size()!=0){
                                loopcounter= loopcounterstack.back();
                                loopcounterstack.pop_back();
                            }
                        }

const:                  CONSTINT { $$ = newexpr_constint($1);}
                        | REALCONST { $$ = newexpr_constdouble($1);}
                        | STRING { $$=newexpr_conststring($1); }
                        | NIL { $$=newexpr_constnil(); }
                        | TRUE { $$=newexpr_constbool(true); }
                        | FALSE { $$=newexpr_constbool(false); }
                        /*etoimo*/
                        ;

idlist:                 ID {
                            if(symbol_table->lookup($1,0) != NULL && symbol_table->lookup($1,0)->type == library_function){
                                yyerror("ERROR : shadow");
                            } else {
                                symbol_table->insert($1, mynumLine, formal_argument, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                                offsetCount->inccurrScopeOffset();
                            }
                        }
                        | idlist COMMA ID {
                            if(symbol_table->lookup($3,0) != NULL && symbol_table->lookup($3,0)->type == library_function){
                                    yyerror("ERROR : shadow");
                            } else {
                                symbol_table->insert($3, mynumLine, formal_argument, scope, offsetCount->currentScopeSpace(), offsetCount->currScopeOffset());
                                 offsetCount->inccurrScopeOffset();
                            }
                        }         /*etoimo*/
                        | {}
                        ;

ifstmt:                 ifprefix stmt  {
				if(ifflag==1){
				symbol_table->hide(scope);
				scope--;
				ifflag=0;
				}
                            quads->patchLabel($1,quads->nextQuadLabel());
                        }
                        | ifprefix stmt elseprefix stmt {
                            quads->patchLabel($1, $3 + 1);
                            quads->patchLabel($3, quads->nextQuadLabel());
                        }
                        ;

ifprefix:                IF LEFT_PARENTHESES expr RIGHT_PARENTHESES {
			    ifflag=1;
			    scope++;
                            quads->emit(if_eq, $3, newexpr_constbool(1), NULL, quads->nextQuadLabel() + 2, mynumLine);
                            $$ = quads->nextQuadLabel();
                            quads->emit(jump, NULL, NULL, NULL, 0, mynumLine);
                        }

elseprefix:             ELSE {
                            $$ = quads->nextQuadLabel();
                            quads->emit(jump, NULL, NULL, NULL, 0, mynumLine);
                        }
                        ;
whilestart:             WHILE{
                            $$=quads->nextQuadLabel();
                        }
whilecond:              LEFT_PARENTHESES expr RIGHT_PARENTHESES{
                            quads->emit(if_eq,$2,newexpr_constbool(1),NULL,quads->nextQuadLabel()+2,mynumLine);
                            $$=quads->nextQuadLabel();
                            quads->emit(jump, NULL, NULL, NULL, quads->nextQuadLabel()+2, mynumLine);
                        }
whilestmt:              whilestart whilecond loopstmt {
                            quads->emit(jump, NULL, NULL, NULL, $1, mynumLine);
                           	quads->patchLabel($2, quads->nextQuadLabel());
                            quads->patchlist($3->breakList, quads->nextQuadLabel());
                            quads->patchlist($3->contList,$1);
                         }
                        ;

N:                      { $$ = quads->nextQuadLabel(); quads->emit(jump,NULL, NULL, NULL, 0, mynumLine);}

M:                      { $$ = quads->nextQuadLabel(); }

forprefix:              FOR LEFT_PARENTHESES elist SEMICOLON M expr SEMICOLON {
                             $$=new for_things();
                             $$->test =$5;
                             $$->enter = quads->nextQuadLabel();
                             quads->emit(if_eq, $6, newexpr_constbool(1),NULL, 0,mynumLine);
                         }

forstmt:                forprefix N elist RIGHT_PARENTHESES N loopstmt N
                         {
                            quads->patchLabel($1->enter, $5+1);
                            quads->patchLabel($2,quads->nextQuadLabel());
                            quads->patchLabel($5, $1->test);
                            quads->patchLabel($7, $2+1);
                            quads->patchlist($6->breakList, quads->nextQuadLabel());
                            quads->patchlist($6->contList, $2+1);
                            $$ = $6;
                         }
                         ;

returnstmt:             RETURN expr SEMICOLON{
                             if(!infuncCounter){
                                yyerror("Return out of a function");
                            }else{
                                quads->emit(iopcode::ret,NULL,NULL,$2,0,mynumLine);
                            }
                        }
                        | RETURN SEMICOLON{
                            if(!infuncCounter){
                                yyerror("Return out of a function");
                            }else{
                                quads->emit(iopcode::ret,NULL,NULL,NULL,0,mynumLine);
                            }
                        }
loopstart:              {++loopcounter;}
loopend:                {--loopcounter;}
loopstmt:               loopstart stmt loopend {
                            $$ = $2;
                        };





%%


int yyerror (std::string yaccProvidedMessage) {
    cout << yaccProvidedMessage << " at line ==> " << mynumLine << endl;
    exit(0);
}

void initiallizer() {
    scope = 0;
    symbol_table->insert("print", 0, library_function, 0, 0, 0);
    symbol_table->insert("input", 0, library_function, 0, 0, 0);
    symbol_table->insert("objectmemberkeys", 0, library_function, 0, 0, 0);
    symbol_table->insert("objecttotalmembers", 0, library_function, 0, 0, 0);
    symbol_table->insert("objectcopy", 0, library_function, 0, 0, 0);
    symbol_table->insert("totalarguments", 0, library_function, 0, 0, 0);
    symbol_table->insert("argument", 0, library_function, 0, 0, 0);
    symbol_table->insert("typeof", 0, library_function, 0, 0, 0);
    symbol_table->insert("strtonum", 0, library_function, 0, 0, 0);
    symbol_table->insert("sqrt", 0, library_function, 0, 0, 0);
    symbol_table->insert("cos", 0, library_function, 0, 0, 0);
    symbol_table->insert("sin", 0, library_function, 0, 0, 0);
}