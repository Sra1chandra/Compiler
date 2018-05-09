#include <bits/stdc++.h>
std::map<std::string,int> LabelCode;
#include "node.h"
static Module *TheModule = new Module("Sravan Chandra",llvm::getGlobalContext());
static LLVMContext &Context = getGlobalContext();
static IRBuilder<> Builder(Context);
FunctionType* funcType =llvm::FunctionType::get(Builder.getVoidTy(),false);
Function *fooFun =llvm::Function::Create(funcType,Function::ExternalLinkage,"main",TheModule);
std::map <std::string,BasicBlock*> LabelMap;
void CodeGenrate(){
  // std::cout << "Generating LLVM IR Code"<<std::endl;
  TheModule->dump();
}
Visitor* v=new TraverseVisitor();
std::map<std::string,std::pair<bool,std::vector<int> > > SymbolTable;
bool Error=0;
int Goto=-1;
void Labeled_Statement::assign_code(int Code_List){
  Labeled_Statement::Code_List=Code_List;
  if ( LabelCode.find(Label) == LabelCode.end() ) {
    LabelCode[Label]=  Code_List;
    // printf("Completed\n");
  }
  else{Error=1;printf("\nLabel %s was declared twice\n", Label.c_str());}
}

void Add_to_SymbolTable(std::string name,bool isArray,int limit){
  if ( SymbolTable.find(name) == SymbolTable.end() ) {
    // printf("Variable_Added to SymbolTable %s\n",name.c_str());
    std::vector<int> v(limit);
    SymbolTable[name]=std::make_pair(isArray,v);
  }
  else {
    printf("\n%s already declared\n",name.c_str() );
    Error=1;
  }
}
bool isVariableDeclared(std::string name){
  if ( SymbolTable.find(name) == SymbolTable.end() )
      return 0;
    else return 1;
}
void setValue(std::string name,int limit,int value)
{
  if((int)SymbolTable[name].second.size()>limit)
    SymbolTable[name].second[limit]=value;
  else
  {
    // std::cout <<isVariableDeclared(name)<<name<<"  "<<SymbolTable[name].second.size()<<"  "<<limit << '\n';
    printf("\nSEGMENTATION FAULT\n");
    Error=1;
  }
}
int getValue(std::string name,int limit)
{
  // printf("%d\n",SymbolTable[name].second[limit]);
  return SymbolTable[name].second[limit];
}
Value *Program::Codegen(){
  Value *V = ConstantInt::get(getGlobalContext(), APInt(32,0));
  Declaration_List->Codegen();
  BasicBlock *start = BasicBlock::Create(Context,"start",fooFun);
  Builder.SetInsertPoint(start);
  for (std::map<std::string,int>::iterator it=LabelCode.begin(); it!=LabelCode.end(); ++it)
    LabelMap[it->first]=BasicBlock::Create(Context,std::string(it->first),fooFun);
  Code_List->Codegen();
  Value* v=Builder.CreateRetVoid();
  return V;
}

Value *Declaration_List::Codegen(){
  // std::cout << "DECLARATION LIST TRAVERSING" << '\n';
  for (int i = Declaration_List.size()-1; i >= 0 ; i--) {
    Declaration_List[i]->Codegen();
    if(Error)
      break;
  }
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,1));
  return v;
}
Value *Declaration_Variable::Codegen(){
  llvm::Type *type;
  if(specifier == "int"){
    type = Type::getInt32Ty(Context);
  }
  for (int i = 0; i < Declaration->get_Variable_List().size(); i++) {
    class Variable* Variable=Declaration->get_Variable_List()[i];
    if(Variable->is_Array()){
       ArrayType* arrType = ArrayType::get(type,Variable->get_limit());
       TheModule->getOrInsertGlobal(Variable->get_VariableName(),arrType);
       GlobalVariable* gVar = TheModule->getNamedGlobal(Variable->get_VariableName());
       gVar->setLinkage(GlobalValue::ExternalLinkage);
      //  gVar->setAlignment(4)
       gVar->setInitializer(ConstantAggregateZero::get(arrType));
     }
     else{
       ConstantInt* init=ConstantInt::get(Context,APInt(32,0));
       TheModule->getOrInsertGlobal(Variable->get_VariableName(),type);
       GlobalVariable* gVar = TheModule->getNamedGlobal(Variable->get_VariableName());
       gVar->setLinkage(GlobalValue::ExternalLinkage);
       gVar->setAlignment(4);
       gVar->setInitializer(init);
    }
  }
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,1));
  return v;
}
Value *Code_List::Codegen(){
  // std::cout << "Code_List TRAVERSING" << '\n';
  // BasicBlock *start = BasicBlock::Create(Context,"start",fooFun);
  // Builder.SetInsertPoint(start);
  for (int i = Code_List.size()-1; i >= 0; i--) {
    // BasicBlock *BasicBlock=BasicBlock::Create(Context,"hello_google",fooFun);
    // LabelMap["hello_google"]=BasicBlock;
    Code_List[i]->Codegen();
  }
  // Value* v=Builder.CreateRetVoid();
}

Value *Number::Codegen() {
  return ConstantInt::get(Type::getInt32Ty(getGlobalContext()),get_value());
}
Value *Identifier::Codegen(){
  Value *Variable=TheModule->getNamedGlobal(name);
  return Builder.CreateLoad(Variable);
}
Value *Array::Codegen(){
  Value *Variable=TheModule->getNamedGlobal(name);
  Value* index = Expression->Codegen();
  std::vector<Value*> value;
  value.push_back(ConstantInt::get(Context,APInt(32,0)));
  value.push_back(index);
  Value* arr=Builder.CreateGEP(Variable,value,"ptr");
  return  Builder.CreateLoad(arr);
}

Value *Assignment_Statement::Codegen(){
  Value* Assign;
  if(Assignment_Variable->isArray()){
    Value *Variable=TheModule->getNamedGlobal(Assignment_Variable->get_name());
    Value* index = Assignment_Variable->get_Expression()->Codegen();
    std::vector<Value*> value;
    value.push_back(ConstantInt::get(Context,APInt(32,0)));
    value.push_back(index);
    Assign=Builder.CreateGEP(Variable,value,"ptr");
  }
  else  Assign=TheModule->getNamedGlobal(Assignment_Variable->get_name());

  Value* value=Expression->Codegen();
  return Builder.CreateStore(value,Assign);
}
Value *For_Loop::Codegen(){
  Value* Assign=TheModule->getNamedGlobal(Identifier);
  Value* value=Iterate->get_start()->Codegen();
  Value* left=Builder.CreateStore(value,Assign);
  Value* right=Iterate->get_end()->Codegen();
  Value*C=Builder.CreateICmpULT(value,right,"ltcomparetmp");
  BasicBlock *ForBB=BasicBlock::Create(Context,"for",fooFun);
  BasicBlock *DoneBB=BasicBlock::Create(Context,"done",fooFun);
  Builder.CreateCondBr(C,ForBB,DoneBB);
  Builder.SetInsertPoint(ForBB);
  Value* v=Code_List->Codegen();
  value=Builder.CreateLoad(Assign);
  if(Iterate->get_offset())
    value=Builder.CreateAdd(value, Iterate->get_offset()->Codegen(), "addtmp");
    else value=Builder.CreateAdd(value, ConstantInt::get(Type::getInt32Ty(getGlobalContext()),1), "addtmp");

  Builder.CreateStore(value,Assign);
  left=Builder.CreateLoad(Assign);
  right=Iterate->get_end()->Codegen();
  C=Builder.CreateICmpULT(left,right,"ltcomparetmp");
  Builder.CreateCondBr(C,ForBB,DoneBB);
  Builder.SetInsertPoint(DoneBB);
}

Value *While_Loop::Codegen(){
  Value *C=Conditions->Codegen();
  BasicBlock *WhileBB=BasicBlock::Create(Context,"while",fooFun);
  BasicBlock *DoneBB=BasicBlock::Create(Context,"done",fooFun);
  Builder.CreateCondBr(C,WhileBB,DoneBB);
  Builder.SetInsertPoint(WhileBB);
  Value* v=Code_List->Codegen();
  C=Conditions->Codegen();
  Builder.CreateCondBr(C,WhileBB,DoneBB);
  Builder.SetInsertPoint(DoneBB);
  return ConstantInt::get(Context,APInt(32,1));
}
Value *OR_Condition::Codegen(){
  Value* L=left->Codegen();
  Value* R=right->Codegen();
  return Builder.CreateOr(L, R, "or");
}
Value *AND_Condition::Codegen(){
  Value* L=left->Codegen();
  Value* R=right->Codegen();
  return Builder.CreateAnd(L, R, "and");
}
Value *Conditional_Statement::Codegen(){
  Value *C=Conditions->Codegen();
  BasicBlock *ifBB=BasicBlock::Create(Context,"then",fooFun);
  BasicBlock *elseBB=BasicBlock::Create(Context,"else",fooFun);
  BasicBlock *MergeBB=BasicBlock::Create(Context,"ifcont",fooFun);
  Builder.CreateCondBr(C,ifBB,elseBB);
  Builder.SetInsertPoint(ifBB);
  Value* v=If->Codegen();
  v=Builder.CreateBr(MergeBB);
  Builder.SetInsertPoint(elseBB);
  if(Else!=NULL){
    v=Else->Codegen();
  }
  v=Builder.CreateBr(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  return ConstantInt::get(Context,APInt(32,1));
}

Value *GoTo_Statement::Codegen(){
  // printf("GOTO STATEMENT LABEL:\t");

  //  printf("%s\n", e->get_Label().c_str());
   if ( LabelMap.find(Identifier) == LabelMap.end() ) {
     printf("\nLabel %s not found\n",Identifier.c_str());
     exit(0);
   }
   BasicBlock *Goto=LabelMap[Identifier];
   if(Conditions){
     Value* C=Conditions->Codegen();
     BasicBlock *NextBB=BasicBlock::Create(Context,"next",fooFun);
     Builder.CreateCondBr(C,Goto,NextBB);
     Builder.SetInsertPoint(NextBB);
   }
   else{
    //  BasicBlock *NextBB=BasicBlock::Create(Context,"next",fooFun);
     Builder.CreateBr(Goto);
     BasicBlock *NextBB=BasicBlock::Create(Context,"next",fooFun);
     Builder.SetInsertPoint(NextBB);

    //  Builder.SetInsertPoint(NextBB);
   }
   return ConstantInt::get(Context,APInt(32,1));
}

Value *Print_Statement::Codegen(){

  Value* v=ConstantInt::get(Context,APInt(32,1));
  int N=Variables->get_Variable_List().size();
  for(int i=0; i<N ;i++)
    StringToPrint+=" %d";
  if(PrintType=="println")StringToPrint+="\n";

  Value* Print=Builder.CreateGlobalStringPtr(StringToPrint);
  std::vector<Value *>Matter;
  std::vector<Type *> type;
  Matter.push_back(Print);
  type.push_back(Print->getType());
  for (int i = N-1; i >=0; i--) {
    Value *v=Variables->get_Variable_List()[i]->Codegen();
    Matter.push_back(v);
    type.push_back(v->getType());
  }
  ArrayRef<Type*> TypeArgs(type);
  ArrayRef<Value*> RefArgs(Matter);
  FunctionType *FunType =FunctionType::get(Type::getInt32Ty(Context),TypeArgs,false);
  Constant* PrintFunc=TheModule->getOrInsertFunction("printf",FunType);
  return Builder.CreateCall(PrintFunc,RefArgs);
}
Value *Read_Statement::Codegen(){
  int N=Variables->get_Variable_List().size();
  std::string StringToPrint="";
  for(int i=0; i<N ;i++)
    StringToPrint+="%d";
  Value* Print=Builder.CreateGlobalStringPtr(StringToPrint);
  std::vector<Value *>Matter;
  std::vector<Type *> type;
  Matter.push_back(Print);
  type.push_back(Print->getType());
  for (int i = N-1; i >=0; i--) {
    // std::cout << i << '\n';
    class Expression_Variable* Assignment_Variable=Variables->get_Variable_List()[i];
    Value* v;
    if(Assignment_Variable->isArray()){
      Value *Variable=TheModule->getNamedGlobal(Assignment_Variable->get_name());
      Value* index = Assignment_Variable->get_Expression()->Codegen();
      std::vector<Value*> value;
      value.push_back(ConstantInt::get(Context,APInt(32,0)));
      value.push_back(index);
      v=Builder.CreateGEP(Variable,value,"ptr");
    }
    else  v=TheModule->getNamedGlobal(Assignment_Variable->get_name());
    Matter.push_back(v);
    type.push_back(v->getType());
  }
  ArrayRef<Type*> TypeArgs(type);
  ArrayRef<Value*> RefArgs(Matter);
  FunctionType *FunType =FunctionType::get(Type::getInt32Ty(Context),TypeArgs,false);
  Constant* ScanFunc=TheModule->getOrInsertFunction("scanf",FunType);
  return Builder.CreateCall(ScanFunc,RefArgs);

}
Value *Labeled_Statement::Codegen(){

  Value*v=Builder.CreateBr(LabelMap[Label]);
  Builder.SetInsertPoint(LabelMap[Label]);
}

Value *Add::Codegen() {
  Value *L = get_left()->Codegen();
  Value *R = get_right()->Codegen();
  if(L == 0 || R == 0) return 0;
  return Builder.CreateAdd(L, R, "addtmp");
}
Value *Sub::Codegen() {
  Value *L = get_left()->Codegen();
  Value *R = get_right()->Codegen();
  if(L == 0 || R == 0) return 0;
  return Builder.CreateSub(L, R, "subtmp");
}
Value *Mult::Codegen() {
  Value *L = get_left()->Codegen();
  Value *R = get_right()->Codegen();
  if(L == 0 || R == 0) return 0;
  return Builder.CreateMul(L, R, "multtmp");
}
Value *Div::Codegen() {
  Value *L = get_left()->Codegen();
  Value *R = get_right()->Codegen();
  if(L == 0 || R == 0) return 0;
  return Builder.CreateUDiv(L, R, "divtmp");
}

Value *Comparator::Codegen(){
    Value* left=get_left()->Codegen();
    std::string Operator=get_Operator();
    Value* right=get_right()->Codegen();
    if (Operator==std::string("=="))return Builder.CreateICmpEQ(left,right,"equalcomparetmp");
    if (Operator==std::string(">="))return Builder.CreateICmpUGE(left,right,"gecomparetmp");
    if (Operator==std::string("<="))return Builder.CreateICmpULE(left,right,"lecomparetmp");
    if (Operator==std::string(">"))return Builder.CreateICmpUGT(left,right,"gtcomparetmp");
    if (Operator==std::string("<"))return Builder.CreateICmpULT(left,right,"ltcomparetmp");
    if (Operator==std::string("!="))return Builder.CreateICmpNE(left,right,"notequalcomparetmp");
}



void TraverseVisitor::visit(Program *e) {
  // printf("PROGRAM TRAVERSING\n");
  e->get_Declaration_List()->accept(v);
  if(!Error)
    e->get_Code_List()->accept(v);
}
void TraverseVisitor::visit(Declaration_List *e) {
  // std::cout << "DECLARATION LIST TRAVERSING" << '\n';
  std::vector<class Declaration_Variable*> Declaration_List=e->get_Declaration_List();
  for (int i = Declaration_List.size()-1; i >= 0 ; i--) {
    Declaration_List[i]->accept(v);
    if(Error)
      break;
  }
}
void TraverseVisitor::visit(Declaration_Variable *e) {
  // std::cout <<e->get_specifier()<<"   "<<"Declaration_Variable TRAVERSING" << '\n';
  e->get_Declaration()->accept(v);
}
void TraverseVisitor::visit(Code_List *e) {
  // std::cout << "Code_List TRAVERSING" << '\n';
  std::vector<class Code*> Code_List=e->get_Code_List();
  for (int i = Code_List.size()-1; i >= 0; i--) {
    Code_List[i]->accept(v);
    // std::cout <<"CODE" <<i << '\n';
    if(Error)break;
    if(Goto!=-1){
      // std::cout << "Goto"<<Goto << '\n';
      i=Goto;
      Goto=-1;
    }
  }
}
void TraverseVisitor::visit(Labeled_Statement *e) {
  // printf("Labeled_Statement\t");
  // std::string Label=e->get_Label();
  // printf("%s\n", e->get_Label().c_str());
  // e->get_Code_List()->accept(v);
}
void TraverseVisitor::visit(Assignment_Statement *e) {
  std::string name=e->get_Assignment_Variable()->get_name();
  int value=e->get_Expression()->accept(v);
  if(Error)return;
  if(!isVariableDeclared(name)){
    printf("\n%s was not declared\n" ,name.c_str());Error=1;return;
  }
  if(e->get_Assignment_Variable()->isArray())
    setValue(name,e->get_Assignment_Variable()->get_Expression()->accept(v),value);
  else
    setValue(name,0,value);
  // printf("%s Assigned to %d\n",name.c_str(),value);
}
void TraverseVisitor::visit(For_Loop *e) {
  std::string name =e->get_Identifier();
  int start=e->get_Iterate()->get_start()->accept(v);
  int end=e->get_Iterate()->get_end()->accept(v);
  int offset;
  if(e->get_Iterate()->get_offset())
    offset=e->get_Iterate()->get_offset()->accept(v);
    else{offset=1;
  // Add_to_SymbolTable(name,1,1);
  // std::cout << "offset"<<offset << '\n';}
}
  if(!isVariableDeclared(name))Error=1;
  if(Error) return;
  setValue(name,0,start);
  int i;
  for(i=start;i<end;i=i+offset)
  {
    // printf("%d %d %d\n",getValue(name,0), end,offset );
    // printf("{\n");
    e->get_Code_List()->accept(v);
    // std::cout << Error << '\n';
    if(Error)break;
    // printf("}\n");
    setValue(name,0,getValue(name,0)+offset);
  }
  // printf("FOR\n");
}
void TraverseVisitor::visit(Iterate *e) {
  // printf("%d\t%d\t%d\n",e->get_start(),e->get_end(),e->get_offset());
}
void TraverseVisitor::visit(While_Loop* e) {
  /* code */
  // printf("While_Loop\n");
  while(e->get_Conditions()->accept(v))
  {
    // printf("{\n");
    e->get_Code_List()->accept(v);
    if(Error)break;
    // printf("}\n");
  }
}
int TraverseVisitor::visit(AND_Condition* e) {
  // printf("Conditons\ns");
  int left=e->get_left()->accept(v);
  // printf("%s\n",e->get_Condition().c_str());
  int right=e->get_right()->accept(v);
  return left&right;
}
int TraverseVisitor::visit(OR_Condition* e) {
  // printf("Conditons\n");
  int left=e->get_left()->accept(v);
  // printf("%s\n",e->get_Condition().c_str());
  int right=e->get_right()->accept(v);
  return left|right;
}
int TraverseVisitor::visit(Comparator* e) {
  // printf("Comparator\n");
    int left=e->get_left()->accept(v);
    // printf("%s\n", e->get_Operator().c_str());
    std::string Operator=e->get_Operator();
    int right=e->get_right()->accept(v);
    if(Error)return 0;
    if (Operator==std::string("=="))return left==right;
    if (Operator==std::string(">="))return left>=right;
    if (Operator==std::string("<="))return left<=right;
    if (Operator==std::string(">"))return left>right;
    if (Operator==std::string("<"))return left<right;
    if (Operator==std::string("!="))return left!=right;
    return 0;
}
void TraverseVisitor::visit(Conditional_Statement* e){
  // printf("IF STATEMENT\n");
  if(e->get_Conditions()->accept(v))
  {
    if(Error)return;
    e->get_If()->accept(v);
    if(Error)return;
  }
  else if(e->get_Else()!=NULL){
    if(Error)return;
    e->get_Else()->accept(v);
  }
  // printf("IF CODE\n");
  // {printf("ELSE CODE\n");e->get_Else()->accept(v);}
}
void TraverseVisitor::visit(GoTo_Statement* e) {
  // printf("GOTO STATEMENT LABEL:\t");
  std::string Label=e->get_Label();
  //  printf("%s\n", e->get_Label().c_str());
   if ( LabelCode.find(Label) == LabelCode.end() ) {
     printf("\nLabel %s not found\n",Label.c_str());
     Error=1;
     return ;
   }
   Goto=LabelCode[Label];
   Conditions* Condition=e->get_Conditions();
   if(Condition!=NULL){
    //  printf("IF \t");s
     if(!Condition->accept(v)){
       Goto=-1;
   }
  }
}

void TraverseVisitor::visit(Dec_Variable_List* e) {
  // std::cout << "Dec_Variable_List TRAVERSING" << '\n';
  std::vector<class Variable*> Variable_List =e->get_Variable_List();
  for (int i = Variable_List.size()-1; i >= 0; i--) {
    Variable_List[i]->accept(v);
    if(Error)
      break;
  }
}

void TraverseVisitor::visit(Variable_List* e) {
  // std::cout << "Variable_List TRAVERSING" << '\n';
  // std::vector<class Expression_Variable*> Variable_List =e->get_Variable_List();
  // for (int i = Variable_List.size()-1; i >= 0; i--) {
  //   Variable_List[i]->accept(v);
  // }
}

void TraverseVisitor::visit(Variable* e) {
  // std::cout << "Variable TRAVERSING" << '\n';
  // std::cout << "s" <<e->get_limit()<< '\n';
  Add_to_SymbolTable(e->get_VariableName(),e->is_Array(),e->get_limit());
}
void TraverseVisitor::visit(Print_Statement* e) {
  printf("%s",e->get_StringToPrint().c_str());
  std::vector<class Expression_Variable*> Variable_List=e->get_Variables()->get_Variable_List();
  for (int i = Variable_List.size()-1; i >= 0; i--) {
    Variable_List[i]->accept(v);
    std::string name=Variable_List[i]->get_name();
    if(Error)break;
    if(Variable_List[i]->isArray()){
      int limit=Variable_List[i]->get_Expression()->accept(v);
      if(Error)break;
      printf(" %d", getValue(name,limit));
    }
    else
      printf(" %d",getValue(name,0));
  }
  if(e->get_PrintType()==std::string("println"))
    printf("\n");

}
void TraverseVisitor::visit(Read_Statement* e) {
  std::vector<class Expression_Variable*> Variable_List=e->get_Variables()->get_Variable_List();
  int value;
  for (int i = Variable_List.size()-1; i >= 0; i--) {
    Variable_List[i]->accept(v);
    std::string name=Variable_List[i]->get_name();
    if(!isVariableDeclared(name)){Error=1;break;}
    int limit=0;
    if(Variable_List[i]->isArray())
      limit=Variable_List[i]->get_Expression()->accept(v);
    if(Error)return;
    if(scanf("%d", &value))
      setValue(name,limit,value);
  }
}
int TraverseVisitor::visit(Number* e) {
  // printf("Number\t%d\n",e->get_value());
  return e->get_value();
}
int TraverseVisitor::visit(Identifier* e) {
  std::string name=e->get_name();
  // printf("Identifier\t%s\n",name.c_str());

  if(isVariableDeclared(name))
    return getValue(name,0);
  else{printf("\n%s was not declared\n", name.c_str());Error=1;return 0;}
}
int TraverseVisitor::visit(Array* e){
  std::string name=e->get_name();
  // printf("hello google\n");
  int limit=e->get_Expression()->accept(v);
  // printf("%d\n", limit);
  // printf("%s[%d]=%d\n", name.c_str() ,limit,getValue(name,limit));
  if(isVariableDeclared(name))
    return getValue(name,limit);
  else{printf("\n%s[] was not declared\n", name.c_str());Error=1;return 0;}
}
int TraverseVisitor::visit(Add *e) {
  int left=e->get_left()->accept(v);
  // printf("%s\n",e->get_Operator().c_str() );
  int right=e->get_right()->accept(v);
  // printf("%d + %d = %d",left,right,left+right);
  return left+right;

}
int TraverseVisitor::visit(Sub *e) {
  int left=e->get_left()->accept(v);
  // printf("%s\n",e->get_Operator().c_str() );
  int right=e->get_right()->accept(v);
  // printf("%d - %d = %d",left,right,left-right);
  return left-right;

}
int TraverseVisitor::visit(Mult *e) {
  int left=e->get_left()->accept(v);
  // printf("%s\n",e->get_Operator().c_str() );
  int right=e->get_right()->accept(v);
  // printf("%d * %d = %d",left,right,left*right);
  return left*right;

}
int TraverseVisitor::visit(Div *e) {
  int left=e->get_left()->accept(v);
  // printf("%s\n",e->get_Operator().c_str() );
  int right=e->get_right()->accept(v);
  // printf("%d / %d = %d",left,right,left/right);
  return left/right;
}
void Iterate::accept(Visitor *v){v->visit(this);}
void Conditional_Statement::accept(Visitor *v){v->visit(this);}
void Declaration_Variable::accept(Visitor *v){v->visit(this);}
void Variable_List::accept(Visitor *v){v->visit(this);}
void Dec_Variable_List::accept(Visitor *v){v->visit(this);}
void Variable::accept(Visitor *v){v->visit(this);}
void Declaration_List::accept(Visitor *v){v->visit(this);}
void Print_Statement::accept(Visitor *v){v->visit(this);}
void Read_Statement::accept(Visitor *v){v->visit(this);}
void Assignment_Statement::accept(Visitor *v){v->visit(this);}
void While_Loop::accept(Visitor *v){v->visit(this);}
void For_Loop::accept(Visitor *v){v->visit(this);}
void GoTo_Statement::accept(Visitor *v){v->visit(this);}
void Labeled_Statement::accept(Visitor *v){v->visit(this);}
void Code_List::accept(Visitor *v){v->visit(this);}
void Program::accept(Visitor *v){v->visit(this);}
int Array::accept(Visitor *v){return v->visit(this);}
int Number::accept(Visitor *v){return v->visit(this);}
int Identifier::accept(Visitor *v){return v->visit(this);}
int Comparator::accept(Visitor *v){return v->visit(this);}
int Add::accept(Visitor *v){return v->visit(this);}
int Sub::accept(Visitor *v){return v->visit(this);}
int Mult::accept(Visitor *v){return v->visit(this);}
int Div::accept(Visitor *v){return v->visit(this);}
int AND_Condition::accept(Visitor *v){return v->visit(this);}
int OR_Condition::accept(Visitor *v){return v->visit(this);}
