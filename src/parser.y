%{
  #include<bits/stdc++.h>
  #include "node.h"
  extern FILE *yyin;
  int yylex (void);
  void yyerror (char const *s);
  extern union Node yylval;
  extern Visitor* v;
  extern std::map<std::string,std::pair<bool,std::vector<int> > > SymbolTable;
  Program* start=NULL;
  extern bool Error;
  /*extern std::map<std::string,class Code_List*> LabelCode;*/
%}

%token DECLBLOCK
%token CODEBLOCK
%token <value> NUMBER
%token <str> IDENTIFIER
%token <str> SPECIFIER
%token FOR
%token WHILE
%token GOTO
%token IF
%token ELSE
%token READ
%token <str> PRINT
%token <str> PRINTLN
%token <str> STRING

%token <str> OR AND

%token <str> EQ NTEQ LTEQ GTEQ LT GT

%token ETOK
%left <str> ADD SUB
%left <str> MULT DIV
/*%left '*'
%left '/'*/
%type <Expression> expression
/*%type <Expression> term*/
%type <Expression> factor
%type <Conditions> conditions
%type <Conditions> condition
%type <str> comparator
%type <Conditional_Statement> conditional_statement
%type <GoTo_Statement> goto_statement
%type <Labeled_Statement> labeled_statement
%type <Dec_Variable_List> dec_variables
%type <Variable> dec_variable
%type <Variable_List> variables
%type <Expression_Variable> variable

%type <Declaration_Variable> declare
%type <Declaration_List> declarations
%type <Declaration_List> declaration_list
%type <str> specifier

%type <Print_Statement> print_statement
%type <Read_Statement> read_statement
%type <Assignment_Statement> assignment_statement
%type <While_Loop> while_loop
%type <For_Loop> for_loop
%type <Iterate> iterate
%type <Code_List> code
%type <Code_List> statements_list

%type <Program> program

%%

program:  declaration_list statements_list{$$=new Program($1,$2);start=$$;}

declaration_list:  DECLBLOCK '{' declarations '}'{$$=$3;}

declarations: declare ';' declarations{$3->push_back($1);$$=$3;}
            |{$$ = new Declaration_List();}
            ;

declare:specifier dec_variable dec_variables{$3->push_back($2);$$ = new Declaration_Variable(std::string($1),$3);}

specifier: SPECIFIER{$$=$1;}
        ;

dec_variables:',' dec_variable dec_variables{$3->push_back($2);$$=$3;}
            |{$$ = new Dec_Variable_List();}
            ;


dec_variable:IDENTIFIER{$$=new Variable(std::string($1),std::string(""));}
            |IDENTIFIER '[' NUMBER ']'{$$=new Variable(std::string($1),std::string("Array"),$3);}
            ;

statements_list:  CODEBLOCK '{' code '}'{$$=$3;}

variables: ',' variable variables{$3->push_back($2);$$=$3;}
         |{$$ = new Variable_List();}
         ;

variable: '(' variable ')'{$$=$2;}
        |IDENTIFIER{$$=new Identifier(std::string($1));}
        |IDENTIFIER '[' expression ']'{$$=new Array(std::string($1),$3);}
        ;


code:labeled_statement code{$1->assign_code($2->get_Code_ListSize());$2->push_back($1);$$=$2;}
    |assignment_statement ';' code{$3->push_back($1);$$=$3;}
    |for_loop code{$2->push_back($1);$$=$2;}
    |while_loop code{$2->push_back($1);$$=$2;}
    |conditional_statement code{$2->push_back($1);$$=$2;}
    |goto_statement ';' code{$3->push_back($1);$$=$3;}
    |print_statement ';' code{$3->push_back($1);$$=$3;}
    |read_statement ';' code{$3->push_back($1);$$=$3;}
    |'{' code '}' code{$4->push_back($2);$$=$4;}
    |{$$=new Code_List();}
    ;

labeled_statement:IDENTIFIER ':'{$$=new Labeled_Statement(std::string($1));}
                  ;

conditional_statement: IF conditions '{' code '}' ELSE '{' code '}'{$$=new Conditional_Statement($2,$4,$8);}
                      |IF conditions '{' code '}'{$$=new Conditional_Statement($2,$4);}
                      ;
goto_statement:GOTO IDENTIFIER{$$=new GoTo_Statement(std::string($2));}
              |GOTO IDENTIFIER IF conditions{$$=new GoTo_Statement(std::string($2),$4);}
              ;
print_statement:PRINT STRING variables{$$=new Print_Statement(std::string($1),std::string($2),$3);}
                |PRINTLN STRING variables{$$=new Print_Statement(std::string($1),std::string($2),$3);}
                ;
read_statement:READ variable variables{$3->push_back($2);$$=new Read_Statement($3);}
              ;

conditions: condition AND conditions{$$=new AND_Condition($1,$3);}
          | condition OR conditions{$$=new OR_Condition($1,$3);}
          | condition{$$=$1;}
          ;
condition:'(' conditions ')'{$$=$2;}
          |expression comparator expression{$$=new Comparator($1,std::string($2),$3);}
          ;
comparator:LT{$$=$1;}
          |GT{$$=$1;}
          |LTEQ{$$=$1;}
          |GTEQ{$$=$1;}
          |EQ{$$=$1;}
          |NTEQ{$$=$1;}
          ;

assignment_statement:variable '=' expression{$$=new Assignment_Statement($1,$3);}

for_loop: FOR IDENTIFIER '=' iterate '{' code '}'{$$=new For_Loop(std::string($2),$4,$6);}

while_loop: WHILE conditions '{' code '}'{$$=new While_Loop($2,$4);}

iterate : expression ',' expression{$$=new Iterate($1,$3);}
        | expression ',' expression ',' expression{$$=new Iterate($1,$3,$5);}
        ;
expression	:	expression ADD expression{$$=new Add($1,$3);}
	          |	expression SUB expression{$$=new Sub($1,$3);}
            | expression MULT expression{$$=new Mult($1,$3);}
            | expression DIV expression{$$=new Div($1,$3);}
            | factor{$$=$1;}
            ;
factor      : '(' expression ')'{$$=$2;}
            | IDENTIFIER{$$=new Identifier(std::string($1));}
            | NUMBER{$$=new Number($1);}
            | IDENTIFIER '[' expression ']'{$$=new Array(std::string($1),$3);}
            ;

%%

void yyerror (char const *s)
{
       fprintf (stderr, "%s\n", s);
}

int main(int argc, char *argv[])
{
	if (argc == 1 ) {
		fprintf(stderr, "Correct usage: bcc filename\n");
		exit(1);
	}

	if (argc > 2) {
		fprintf(stderr, "Passing more arguments than necessary.\n");
		fprintf(stderr, "Correct usage: bcc filename\n");
	}

	yyin = fopen(argv[1], "r");
	yyparse();
  if(start)
  {
    start->accept(v);
    if(!Error)
    {
      start->Codegen();
      CodeGenrate();
  }else{std::cout<<"Error occured"<<std::endl;}
  }
      /*LabelCode["L1"]->accept(v);*/
  /*std::cout<<SymbolTable["name1"].second.size()<<std::endl;*/
}
