#include<bits/stdc++.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;
union Node{
  char* str;
  int value;
  Node(){
    value=0;
    str=NULL;
  }
  ~Node(){
    // printf("Node Detleted\n");
  }
  class Iterate *Iterate;
  class Expression *Expression;
  class Conditions *Conditions;
  class Conditional_Statement *Conditional_Statement;
  class Declaration_Variable *Declaration_Variable;
  class Variable_List *Variable_List;
  class Dec_Variable_List *Dec_Variable_List;
  class Variable *Variable;
  class Expression_Variable *Expression_Variable;
  class Declaration_List *Declaration_List;
  class Print_Statement *Print_Statement;
  class Read_Statement *Read_Statement;
  class Assignment_Statement *Assignment_Statement;
  class While_Loop *While_Loop;
  class For_Loop *For_Loop;
  class GoTo_Statement *GoTo_Statement;
  class Labeled_Statement *Labeled_Statement;
  class Code_List *Code_List;
  class Code *Code;
  class Program *Program;
};

typedef union Node YYSTYPE;
#define YYSTYPE_IS_DECLARED 1;
void CodeGenrate();

class Code{
public:
  virtual void accept(class Visitor *v)=0;
  virtual Value* Codegen() = 0;
};

class Program{
private:
  class Declaration_List* Declaration_List;
  class Code_List* Code_List  ;
public:
  Program(class Declaration_List* Declaration_List,class Code_List* Code_List){
    Program::Declaration_List=Declaration_List;
    Program::Code_List=Code_List;
  }
  void accept(Visitor *v);
  class Declaration_List* get_Declaration_List(){return Declaration_List;};
  class Code_List* get_Code_List(){return Code_List;};
  virtual Value* Codegen();
};
class Code_List:public Code{
private:
  std::vector<class Code*> Code_List;
public:
  void push_back(class Code* Code){Code_List::Code_List.push_back(Code);}
  int get_Code_ListSize(){return Code_List.size();}
  void accept(Visitor *v);
  std::vector<class Code*> get_Code_List(){return Code_List;}
  virtual Value* Codegen();
};
class Print_Statement:public Code{
private:
  std::string PrintType;
  std::string StringToPrint;
  class Variable_List* Variables;
public:
  Print_Statement(std::string type,std::string String,class Variable_List* Variables){
    Print_Statement::PrintType=type;
    Print_Statement::StringToPrint=String;
    Print_Statement::Variables=Variables;
  }
  void accept(Visitor *v);
  std::string get_PrintType(){return PrintType;}
  std::string get_StringToPrint(){return StringToPrint;}
  class Variable_List* get_Variables(){return Variables;}
  virtual Value* Codegen();
};

class Read_Statement:public Code{
private:
  class Variable_List* Variables;
public:
  Read_Statement(class Variable_List* Variables){
    Read_Statement::Variables=Variables;
  }
  void accept(Visitor *v);
  class Variable_List* get_Variables(){return Variables;}
  virtual Value* Codegen();
};
class Assignment_Statement:public Code {
private:
  class Expression_Variable* Assignment_Variable;
  class Expression* Expression;

public:
  Assignment_Statement(class Expression_Variable* Assignment_Variable,class Expression* Expression){
    Assignment_Statement::Assignment_Variable=Assignment_Variable;
    Assignment_Statement::Expression=Expression;
  }
  void accept(Visitor *v);
  class Expression_Variable* get_Assignment_Variable(){return Assignment_Variable;}
  class Expression* get_Expression(){return Expression;}
  virtual Value* Codegen();
};
class While_Loop:public Code{
private:
  class Conditions* Conditions;
  class Code_List *Code_List;

public:
  While_Loop(class Conditions* Conditions,class Code_List *Code_List){
    While_Loop::Conditions=Conditions;
    While_Loop::Code_List=Code_List;
  }
  void accept(Visitor *v);
  class Conditions* get_Conditions(){return Conditions;};
  class Code_List* get_Code_List(){return Code_List;};
  virtual Value* Codegen();
};
class For_Loop:public Code{
private:
  std::string Identifier;
  class Iterate *Iterate;
  class Code_List *Code_List;
public:
  For_Loop(std::string Identifier,class Iterate *Iterate,class Code_List* Code_List){
    For_Loop::Identifier=Identifier;
    For_Loop::Iterate=Iterate;
    For_Loop::Code_List=Code_List;
  }
  void accept(Visitor *v);
  std::string get_Identifier(){return Identifier;};
  class Iterate* get_Iterate(){return Iterate;};
  class Code_List* get_Code_List(){return Code_List;};
  virtual Value* Codegen();
};

class Iterate{
private:
  class Expression* Start;
  class Expression* End;
  class Expression* Offset;
public:
  Iterate(class Expression* start,class Expression* end){
    Iterate::Start=start;Iterate::End=end;Iterate::Offset=NULL;
  }
  Iterate(class Expression* start,class Expression* end,class Expression* offset){
    Iterate::Start=start;Iterate::End=end;Iterate::Offset=offset;
  }
  void accept(Visitor *v);
  class Expression* get_start(){return Start;}
  class Expression* get_end(){return End;}
  class Expression* get_offset(){return Offset;}
  // //virtual Value* Codegen();
};

class Dec_Variable_List{
private:
  std::vector<class Variable*> Variable_List;
public:
  void push_back(class Variable* Variable){Dec_Variable_List::Variable_List.push_back(Variable);}
  void accept(Visitor *v);
  std::vector<class Variable*> get_Variable_List(){return Variable_List;}
  // //virtual Value* Codegen();
};
class Variable_List{
private:
  std::vector<class Expression_Variable*> Variable_List;
public:
  void push_back(class Expression_Variable* Variable){Variable_List::Variable_List.push_back(Variable);}
  void accept(Visitor *v);
  std::vector<class Expression_Variable*> get_Variable_List(){return Variable_List;}
  // //virtual Value* Codegen();
};


class Declaration_List{
private:
  std::vector<class Declaration_Variable*> Declaration_List;
public:
  void push_back(class Declaration_Variable* Declaration_Variable){Declaration_List::Declaration_List.push_back(Declaration_Variable);}
  void accept(Visitor *v);
  std::vector<class Declaration_Variable*> get_Declaration_List(){return Declaration_List;}
  virtual Value* Codegen();
};

class Declaration_Variable{
private:
  std::string specifier;
  class Dec_Variable_List* Declaration;
public:
  Declaration_Variable(std::string specifier,class Dec_Variable_List* Declaration){
    Declaration_Variable::specifier=specifier;
    Declaration_Variable::Declaration=Declaration;
    // printf("SPECIFIER\t%s\n", specifier.c_str());
  }
  void accept(Visitor *v);
  std::string get_specifier(){return specifier;}
  class Dec_Variable_List* get_Declaration(){return Declaration;};
  virtual Value* Codegen();
};
class Variable{
private:
  std::string name;
  std::string type;
  int limit;
public:
  Variable(std::string name,std::string type){
    Variable::name=name;
    Variable::type=type;
    Variable::limit=1;
  }
  Variable(std::string name,std::string type,int limit){
    Variable::name=name;
    Variable::type=type;
    Variable::limit=limit;
  }
  void accept(Visitor *v);
  std::string get_VariableName(){return name;}
  bool is_Array(){return type==std::string("Array");}
  int get_limit(){return limit;}
  // virtual Value* Codegen();
};
class Expression{
protected:
  class Expression *left;
  class Expression *right;
public:
  Expression(class Expression* left,class Expression* right)
  {
    Expression::left=left;
    Expression::right=right;
  }
  Expression(){Expression::left=NULL;
      Expression::right=NULL;
}
  ~Expression(){}
  virtual int accept(class Visitor *v)=0;
  virtual Value* Codegen()=0;
};

class Add:public Expression{
private:
  std::string Operator;
public:
  Add(class Expression* left,class Expression* right):Expression(left,right){Operator="+";}
  int accept(Visitor *v);
  std::string get_Operator(){return Operator;}
  class Expression* get_left(){return left;}
  class Expression* get_right(){return right;}
  virtual Value* Codegen();
};

class Sub:public Expression{
private:
  std::string Operator;
public:
  Sub(class Expression* left,class Expression* right):Expression(left,right){Operator="-";}
  int accept(Visitor *v);
  std::string get_Operator(){return Operator;}
  class Expression* get_left(){return left;}
  class Expression* get_right(){return right;}
  virtual Value* Codegen();
};

class Mult:public Expression{
private:
  std::string Operator;
public:
  Mult(class Expression* left,class Expression* right):Expression(left,right){Operator="*";}
  int accept(Visitor *v);
  std::string get_Operator(){return Operator;}
  class Expression* get_left(){return left;}
  class Expression* get_right(){return right;}
  virtual Value* Codegen();
};

class Div:public Expression{
private:
  std::string Operator;
public:
  Div(class Expression* left,class Expression* right):Expression(left,right){Operator="/";}
  int accept(Visitor *v);
  std::string get_Operator(){return Operator;}
  class Expression* get_left(){return left;}
  class Expression* get_right(){return right;}
  virtual Value* Codegen();
};
class Conditions{
public:
  virtual int accept(class Visitor *v)=0;
  virtual Value* Codegen()=0;
};
class AND_Condition:public Conditions{
private:
  std::string Condition;
  class Conditions* left;
  class Conditions* right;
public:
  AND_Condition(class Conditions* left,class Conditions* right){
    AND_Condition::left=left;
    AND_Condition::right=right;
    AND_Condition::Condition="and";
    // printf("Conditon\t%s\n",Condition.c_str() );
  }
  // AND_Condition(){AND_Condition::left=NULL;AND_Condition::right=NULL;AND_Condition::Condition="";}
  int accept(Visitor *v);
  std::string get_Condition(){return Condition;}
  class Conditions* get_left(){return left;}
  class Conditions* get_right(){return right;}
  virtual Value* Codegen();
};
class OR_Condition:public Conditions{
private:
  std::string Condition;
  class Conditions* left;
  class Conditions* right;
public:
  OR_Condition(class Conditions* left,class Conditions* right){
    OR_Condition::left=left;
    OR_Condition::right=right;
    OR_Condition::Condition="or";
    // printf("Conditon\t%s\n",Condition.c_str() );
  }
  // OR_Condition(){OR_Condition::left=NULL;OR_Condition::right=NULL;OR_Condition::Condition="";}
  int accept(Visitor *v);
  std::string get_Condition(){return Condition;}
  class Conditions* get_left(){return left;}
  class Conditions* get_right(){return right;}
  virtual Value* Codegen();
};

class Comparator:public Conditions{
private:
  std::string Operator;
  class Expression* left;
  class Expression* right;

public:
  Comparator(class Expression* left,std::string Operator,class Expression* right){
    Comparator::left=left;
    Comparator::right=right;
    Comparator::Operator=Operator;
    // printf("Comparator\t%s\n",Operator.c_str() );
  }
  int accept(Visitor *v);
  std::string get_Operator(){return Operator;}
  class Expression* get_left(){return left;}
  class Expression* get_right(){return right;}
  virtual Value* Codegen();
};

class Conditional_Statement:public Code{
private:
  class Conditions* Conditions;
  class Code* If;
  class Code* Else;
public:
  Conditional_Statement(class Conditions* Conditions,class Code *If,class Code *Else){
    Conditional_Statement::Conditions=Conditions;
    Conditional_Statement::If=If;
    Conditional_Statement::Else=Else;
    // printf("IF STATEMENT IS CALLED\n");
  }
  Conditional_Statement(class Conditions* Conditions,class Code *If){
    Conditional_Statement::Conditions=Conditions;
    Conditional_Statement::If=If;
    Conditional_Statement::Else=NULL;
    // printf("IF STATEMENT IS CALLED\n");
  }
  void accept(Visitor *v);
  class Conditions * get_Conditions(){return Conditions;}
  class Code * get_If(){return If;}
  class Code * get_Else(){return Else;}
  virtual Value* Codegen();
};

class GoTo_Statement:public Code{
private:
  std::string Identifier;
  class Conditions* Conditions;
public:
  GoTo_Statement(std::string Identifier){
    GoTo_Statement::Identifier=Identifier;
    GoTo_Statement::Conditions=NULL;
  }
  GoTo_Statement(std::string Identifier,class Conditions*Conditions){
    GoTo_Statement::Identifier=Identifier;
    GoTo_Statement::Conditions=Conditions;
  }
  void accept(Visitor *v);
  std::string get_Label(){return Identifier;}
  class Conditions* get_Conditions(){return Conditions;}
  virtual Value* Codegen();
};

class Labeled_Statement:public Code{
private:
  std::string Label;
  int Code_List;
public:
  Labeled_Statement(std::string Label){
    Labeled_Statement::Label=Label;
  }
  void assign_code(int Code_List);
  void accept(Visitor *v);
  std::string get_Label(){return Label;}
  // class Code_List* get_Code_List(){return Code_List;}
  virtual Value* Codegen();
};


class Number:public Expression{
private:
  int value;
  // std::string str;
public:
  Number(int value):Expression(){
    Number::value=value;
  }
  ~Number(){}
  int accept(Visitor *v);
  int get_value(){return value;}
  virtual Value* Codegen();
};
class Expression_Variable:public Expression{
public:
  virtual int accept(Visitor *v)=0;
  virtual bool isArray()=0;
  virtual std::string get_name()=0;
  virtual class Expression* get_Expression()=0;
  virtual Value* Codegen()=0;
};
class Identifier:public Expression_Variable{
private:
  std::string name;
public:
  Identifier(std::string name){
    Identifier::name=name;
  }
  int accept(Visitor *v);
  bool isArray(){return 0;}
  std::string get_name() {return name;}
  class Expression* get_Expression(){return NULL;}
  virtual Value* Codegen();
};
class Array:public Expression_Variable{
private:
  std::string name;
  class Expression* Expression;
public:
  Array(std::string name,class Expression* Expression){
    Array::name=name;
    Array::Expression=Expression;
  }
  ~Array(){}
  int accept(Visitor *v);
  std::string get_name(){return name;}
  bool isArray(){return 1;}
  class Expression* get_Expression(){return Expression;}
  virtual Value* Codegen();
};

class Visitor{
  public:
    virtual void visit(class Iterate *)=0;
    virtual int visit(class OR_Condition *)=0;
    virtual int visit(class AND_Condition *)=0;
    virtual void visit(class Conditional_Statement *)=0;
    virtual void visit(class Declaration_Variable *)=0;
    virtual void visit(class Variable_List *)=0;
    virtual void visit(class Dec_Variable_List *)=0;
    virtual void visit(class Variable *)=0;
    virtual void visit(class Declaration_List *)=0;
    virtual void visit(class Print_Statement *)=0;
    virtual void visit(class Read_Statement *)=0;
    virtual void visit(class Assignment_Statement *)=0;
    virtual void visit(class While_Loop *)=0;
    virtual void visit(class For_Loop *)=0;
    virtual void visit(class GoTo_Statement *)=0;
    virtual void visit(class Labeled_Statement *)=0;
    virtual void visit(class Code_List *)=0;
    // virtual void visit(class Code *)=0;
    virtual void visit(class Program *)=0;
    virtual int visit(class Array *)=0;
    virtual int visit(class Number *)=0;
    virtual int visit(class Identifier *)=0;
    virtual int visit(class Comparator *)=0;
    virtual int visit(class Add *)=0;
    virtual int visit(class Sub *)=0;
    virtual int visit(class Mult *)=0;
    virtual int visit(class Div *)=0;
};

class TraverseVisitor:public Visitor{
public:
  void visit(Iterate*);
  int visit(AND_Condition *e);
  int visit(OR_Condition *e);
  void visit(Conditional_Statement *e);
  void visit(Declaration_Variable *e);
  void visit(Variable_List *e);
  void visit(Declaration_List *e);
  void visit(Dec_Variable_List *e);
  void visit(Variable *e);
  void visit(Print_Statement *e);
  void visit(Read_Statement *e);
  void visit(Assignment_Statement *e);
  void visit(While_Loop *e);
  void visit(For_Loop *e);
  void visit(GoTo_Statement *e);
  void visit(Labeled_Statement *e);
  void visit(Code_List *e);
  void visit(Program *e);
  int visit(Array*);
  int visit(Number*);
  int visit(Identifier*);
  int visit(Comparator *e);
  int visit(Add *e);
  int visit(Sub *e);
  int visit(Mult *e);
  int visit(Div *e);
};
