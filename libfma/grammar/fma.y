%define api.pure full
%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%locations

%code requires{
typedef void *yyscan_t;

#include <fma/parser/ParserHeading.hpp>
#include <fma/Parser.hpp>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>

using namespace FMA;
using namespace FMA::ast;

extern Node *parsedRootNode;
extern Parser *yyCurrentParser;

template<class Node> Node *_WITHLOC(unsigned int line, unsigned int col, Node *node) {
  node->setLocation(yyCurrentParser->reference(line, col));
  return node;
}

#define WL(type) \
  _WITHLOC(yylloc.first_line, yylloc.first_column, type)

}

%code {
  int yyerror(YYLTYPE *yylloc, yyscan_t scanner, const char *error);
  int yylex(YYSTYPE * yylval_param,YYLTYPE * yylloc_param, yyscan_t scanner);
}

%define parse.error verbose

%union{
  Statement *statement;
  Expression *expression;
  Parameter *parameter;
  Identifier *identifier;
  std::string *string;
  double number;
  ParameterList *parameters;
  MacroParameterList *macroParameters;
  MacroParameter *macroParameter;
  DecoratableStatement *decoratorStatement;
  CaseStatementCaseList *cases;
  CaseStatementCase *caseItem;
}

%token T_INT_CONST T_NEWLINE T_COLCOL T_MODULE T_END T_IDENTIFIER T_REQUIRE
%token T_BEGIN_STRING T_END_STRING T_STRINGLITERAL T_LRB T_RRB T_LSHIFT T_RSHIFT
%token T_AND T_REM T_TILDE T_NOT T_OR T_MULT T_DIV T_SUB T_ADD T_XOR T_EXPR_CONST
%token T_LSB T_RSB T_DOT T_IF T_UNLESS T_ELSE T_LOG_AND T_LOG_OR T_SYMBOL T_COMMA
%token T_BEGIN_INLINEEXPRESSION T_END_INLINEEXPRESSION T_BOOL_CONST T_EQUALITY_OP
%token T_ASSIGN T_ASSIGN_ADD T_ASSIGN_SUB T_ASSIGN_MUL T_ASSIGN_DIV T_ASSIGN_REM
%token T_ASSIGN_AND T_ASSIGN_OR T_ASSIGN_XOR T_ASSIGN_LSHIFT T_ASSIGN_RSHIFT T_ASSIGN_LOG_OR
%token T_ASSIGN_LOG_AND T_NOT_EQUALITY_OP T_LESSTHAN_EQUAL T_GREATERTHAL_EQUAL T_LESSTHAN
%token T_GREATERTHAN T_MACRO T_COL T_DO T_RETURN T_LRB_CALL T_UNARY T_KWARGS T_DECORATOR
%token T_DEF T_CASE T_WHEN T_NIL T_DOTDOT T_LCB T_RCB T_DATABLOCK T_CLASS T_WITHCONTEXT
%token T_COLCOLROOT T_SELF T_RAISE T_CLASS_ACCESSOR T_EXPORT T_IMPORT

%locations

%type <statement> RootStatementList RootStatement ModuleDeclarationStatement ExpressionStatement RequireStatement ImportStatement
%type <statement> ModuleBody PostfixStatement Statement PostfixStatementItem IfStatement ElseStatement UnlessStatement
%type <statement> MacroDeclarationStatement MacroBody ReturnStatement FunctionDeclarationStatement CaseStatement CaseItemStatements
%type <statement> DataBlockDeclarationStatement ClassDeclarationStatement LabelStatement RaiseStatement InlineMacroBody
%type <expression> Expression PrimaryExpression ConstantNumber String StringContent StringContentList BlockDeclaration 
%type <expression> AssignmentExpression ConditionalExpression LogicalOrExpression LogicalAndExpression InclusiveOrExpression
%type <expression> ExclusiveOrExpression AndExpression EqualityExpression RelationalExpression ShiftExpression T_NIL
%type <expression> AdditiveExpression MultiplicativeExpression UnaryExpression PostfixExpression NonConstantExpression
%type <expression> NoAssignExpression AssignExpressionChild InlineBlockDeclaration 
%type <decoratorStatement> Decorator DecoratorList DecoratorDeclarationStatement DecoratedContent

%type <number> T_INT_CONST T_BOOL_CONST
%type <string> T_IDENTIFIER T_STRINGLITERAL T_SYMBOL
%type <identifier> Identifier FullIdentifier ClassParent FullIdentifierList
%type <parameters> ParameterList ParameterListWithBlock ExpressionList NamedExpressionList InBraceParameterListWithBlock
%type <parameter> Parameter NamedExpression
%type <macroParameters> MacroParameterList MacroParameters
%type <macroParameter> MacroParameter MacroParameterWithPrefix
%type <cases> CaseItemList
%type <caseItem> CaseItem

%nonassoc T_ELSE

%start	Program

%%

Program:
    OptionalNL RootStatementList OptionalNL { parsedRootNode = $2; }
  ;

OptionalNL:
    NL
  |
  ;

T_LRB_ANY: T_LRB | T_LRB_CALL;

NL:
    SingleNL
  | NL SingleNL
  ;

_: NL
  | 
  ;

SingleNL:
    T_NEWLINE
  ;

Identifier:
    T_IDENTIFIER {$$=WL(new Identifier(*$1)); delete $1;}
  ;

FullIdentifier:
    FullIdentifierList { $$ = $1; }
  | T_COLCOLROOT FullIdentifierList { $$ = $2; $2->setRoot(true); }
  ;

FullIdentifierList:
    Identifier { $$=$1; }
  | FullIdentifierList T_COLCOL Identifier { $1->linkLastStatement($3); $$ = $1; }
  ;


RootStatementList:
    RootStatement { $$ = $1; }
  | RootStatementList NL RootStatement { $1->linkLastStatement($3); $$ = $1; }
  ;

RootStatement:
    ModuleDeclarationStatement { $$ = $1; }
  | RequireStatement { $$ = $1; }
  | ClassDeclarationStatement { $$ = $1; }
  | MacroDeclarationStatement { $$ = $1; }
  | FunctionDeclarationStatement { $$ = $1; }
  | DecoratorDeclarationStatement { $$ = $1; }
  | DataBlockDeclarationStatement { $$ = $1; }
  | LabelStatement { $$ = $1; }
  | ImportStatement { $$ = $1; }
  | Statement { $$ = $1; }
  ;

ImportStatement:
    T_IMPORT Identifier { $$ = WL(new ImportStatement($2)); }
  ;

LabelStatement:
    Identifier T_COL { $$ = WL(new LabelStatement($1)); }
  ;

CaseStatement:
    T_CASE Expression NL T_END { $$ = WL(new CaseStatement($2, CaseStatementCaseList())); }
  | T_CASE Expression NL CaseItemList T_END { $$ = WL(new CaseStatement($2, *$4)); delete $4; }
  ;

CaseItemList:
    CaseItem { $$ = new CaseStatementCaseList(); $$->push_back(*$1); delete $1; }
  | CaseItemList CaseItem  { $$ = $1; $$->push_back(*$2); delete $2; }
  ;

CaseItem:
    T_WHEN Expression CaseItemStatements { $$ = new CaseStatementCase($2, $3); }
  | T_ELSE CaseItemStatements { $$ = new CaseStatementCase($2); }
  ;

CaseItemStatements:
    NL { $$ = WL(new ExpressionStatement(WL(new NilExpression()))); }
  | NL RootStatementList NL { $$ = $2; }
  ;

IfStatement:
    T_IF Expression NL ElseStatement { IfStatement *stmt = new IfStatement($2); stmt->setElse($4); $$=WL(stmt); }
  | T_IF Expression NL RootStatementList NL ElseStatement { IfStatement *stmt = new IfStatement($2); stmt->setThen($4); stmt->setElse($6); $$=WL(stmt); }
  ;

UnlessStatement:
    T_UNLESS Expression NL ElseStatement { IfStatement *stmt = new IfStatement(new UnaryOperationExpression(UnaryOperationExpression::NOT, $2)); stmt->setElse($4); $$=WL(stmt); }
  | T_UNLESS Expression NL RootStatementList NL ElseStatement { IfStatement *stmt = new IfStatement(new UnaryOperationExpression(UnaryOperationExpression::NOT, $2)); stmt->setThen($4); stmt->setElse($6); $$=WL(stmt); }
  ;

ElseStatement:
    T_END { $$ = NULL; }
  | T_ELSE IfStatement { $$ = $2; }
  | T_ELSE NL T_END { $$ = NULL; }
  | T_ELSE NL RootStatementList NL T_END { $$ = $3; }
  ;

Statement:
    PostfixStatement { $$ = $1; }
  | IfStatement { $$ = $1; }
  | UnlessStatement { $$ = $1; }
  | CaseStatement { $$ = $1; }
  ;

PostfixStatement:
    PostfixStatementItem { $$ = $1; }
  | PostfixStatementItem T_IF NonConstantExpression { IfStatement *stmt=new IfStatement($3); $$ = WL(stmt); stmt->setThen($1); }
  | PostfixStatementItem T_UNLESS NonConstantExpression { IfStatement *stmt=new IfStatement(WL(new UnaryOperationExpression(UnaryOperationExpression::NOT, $3))); $$ = WL(stmt); stmt->setThen($1); }
  ;

PostfixStatementItem:
    ReturnStatement { $$ = $1; }
  | ExpressionStatement { $$ = $1; }
  | RaiseStatement { $$ = $1; }
  ;

RaiseStatement:
    T_RAISE Expression { $$ = WL(new RaiseStatement($2)); }
  ;

ReturnStatement:
    T_RETURN { $$ = WL(new ReturnStatement()); }
  | T_RETURN Expression { $$ = WL(new ReturnStatement($2)); }
  ;

RequireStatement:
    T_REQUIRE String { $$ = WL(new RequireStatement($2)); }
  ;

DecoratorDeclarationStatement:
    DecoratorList DecoratedContent { $$ = $2; $2->setDecoratorStatement(StatementPtr($1)); }
  ;

DecoratorList:
    Decorator { $$ = $1; }
  | DecoratorList Decorator { $$ = $2; $2->setDecoratorStatement(StatementPtr($1)); }
  ;

Decorator:
    T_DECORATOR FullIdentifier NL { $$ = WL(new DecoratorCallStatement($2, ParameterList())); }
  | T_DECORATOR FullIdentifier ParameterList NL { $$ = WL(new DecoratorCallStatement($2, *$3)); delete $3; }
  | T_DECORATOR FullIdentifier T_LRB_ANY ParameterList T_RRB NL { $$ = WL(new DecoratorCallStatement($2, *$4)); delete $4; }
  ;

DecoratedContent:
    MacroDeclarationStatement { $$ = dynamic_cast<MacroDeclarationStatement*>($1); }
  | FunctionDeclarationStatement { $$ = dynamic_cast<FunctionDeclarationStatement*>($1); }
  | DataBlockDeclarationStatement { $$ = dynamic_cast<DataBlockDeclarationStatement*>($1); }
  ;

FunctionDeclarationStatement:
    T_DEF Identifier NL MacroBody { $$ = WL(new FunctionDeclarationStatement($2, $4)); }
  | T_EXPORT T_DEF Identifier NL MacroBody { $$ = WL(new FunctionDeclarationStatement($3, $5, true)); }
  ;

DataBlockDeclarationStatement:
    T_DATABLOCK Identifier NL MacroBody { $$ = WL(new DataBlockDeclarationStatement($2, $4)); }
  ;

MacroDeclarationStatement:
    T_MACRO T_SELF T_DOT Identifier MacroParameters MacroBody { $$ = WL(new MacroDeclarationStatement($4, *$5, $6, true)); delete $5; }
  | T_MACRO T_DECORATOR T_SELF T_DOT Identifier MacroParameters MacroBody { MacroDeclarationStatement *stmt = new MacroDeclarationStatement($5, *$6, $7, true); $$ = WL(stmt); stmt->setIsDecorator(); delete $6; }
  | T_MACRO Identifier MacroParameters MacroBody { $$ = WL(new MacroDeclarationStatement($2, *$3, $4)); delete $3; }
  | T_MACRO T_DECORATOR Identifier MacroParameters MacroBody { MacroDeclarationStatement *stmt = new MacroDeclarationStatement($3, *$4, $5); $$ = WL(stmt); stmt->setIsDecorator(); delete $4; }
  ;

MacroParameters:
    NL { $$ = new MacroParameterList(); }
  | T_LRB_ANY T_RRB NL { $$ = new MacroParameterList(); }
  | T_LRB_ANY MacroParameterList T_RRB NL { $$ = $2; }
  ;

MacroBody:
    T_END { $$ = NULL; }
  | RootStatementList NL T_END { $$ = $1; }
  ;

InlineMacroBody:
    T_RCB { $$ = NULL; }
  | RootStatementList T_RCB { $$ = $1; }
  ;

ClassDeclarationStatement:
    T_CLASS Identifier ClassParent NL
    ModuleBody { $$ = WL(new ClassDeclarationStatement($2, $3, $5)); }
  ;

ClassParent:
    { $$ = NULL; }
  | T_LESSTHAN FullIdentifier { $$ = $2; }
  ;

ModuleDeclarationStatement:
    T_MODULE FullIdentifier NL
    ModuleBody { $$ = WL(new ModuleDeclarationStatement($2, $4)); }
  ;

ModuleBody:
    T_END { $$ = NULL; }
  | RootStatementList NL T_END { $$ = $1; }
  ;

ExpressionStatement:
    NoAssignExpression { $$ = WL(new ExpressionStatement(new CallExpression($1))); }
  | NoAssignExpression ParameterListWithBlock { CallExpression *expr=new CallExpression($1); $$ = WL(new ExpressionStatement(expr)); expr->setArguments(*$2); delete $2; }
  | Expression { $$ = WL(new ExpressionStatement($1)); }
  ;

MacroParameterList:
    MacroParameterWithPrefix { $$ = new MacroParameterList(); $$->push_back(MacroParameterPtr($1)); }
  | MacroParameterList T_COMMA MacroParameterWithPrefix { $$ = $1; $1->push_back(MacroParameterPtr($3)); }
  ;

InBraceParameterListWithBlock:
    ParameterList T_RRB { $$ = $1; }
  | ParameterList T_RRB InlineBlockDeclaration { $$ = $1; $1->push_back(ParameterPtr(new Parameter(Parameter::BLOCK, ExpressionPtr($3))));}
  ;

ParameterListWithBlock:
    ParameterList { $$ = $1; }
  | ParameterList BlockDeclaration { $$ = $1; $1->push_back(ParameterPtr(new Parameter(Parameter::BLOCK, ExpressionPtr($2)))); }
  | BlockDeclaration { $$ = new std::vector<ParameterPtr>(); $$->push_back(ParameterPtr(new Parameter(Parameter::BLOCK, ExpressionPtr($1))));}
  | ParameterList T_COMMA InlineBlockDeclaration { $$ = $1; $1->push_back(ParameterPtr(new Parameter(Parameter::BLOCK, ExpressionPtr($3)))); }
  | InlineBlockDeclaration { $$ = new std::vector<ParameterPtr>(); $$->push_back(ParameterPtr(new Parameter(Parameter::BLOCK, ExpressionPtr($1))));}
  ;

BlockDeclaration:
    T_DO NL MacroBody { $$ = WL(new InlineMacroExpression(MacroParameterList(), $3)); }
  | T_DO T_OR MacroParameterList T_OR NL MacroBody { $$ = WL(new InlineMacroExpression(*$3, $6)); delete $3; }
  ;

InlineBlockDeclaration:
    T_LCB InlineMacroBody { $$ = WL(new InlineMacroExpression(MacroParameterList(), $2)); }
  | T_LCB T_OR MacroParameterList T_OR InlineMacroBody { $$ = WL(new InlineMacroExpression(*$3, $5)); delete $3; }
  ;

ParameterList:
    Parameter { $$ = new std::vector<ParameterPtr>(); $$->push_back(ParameterPtr($1)); }
  | ParameterList T_COMMA Parameter { $$ = $1; $1->push_back(ParameterPtr($3)); }
  ;

MacroParameterWithPrefix:
    T_UNARY T_MULT MacroParameter { $$ = $3; $3->setType(MacroParameter::ARGS); }
  | T_UNARY T_AND MacroParameter { $$ = $3; $3->setType(MacroParameter::BLOCK); }
  | T_UNARY T_KWARGS MacroParameter { $$ = $3; $3->setType(MacroParameter::KWARGS); }
  | MacroParameter { $$ = $1; }
  ;

MacroParameter:
    Identifier { $$ = new MacroParameter(IdentifierPtr($1)); }
  | Identifier T_ASSIGN RelationalExpression { $$ = new MacroParameter(IdentifierPtr($1), ExpressionPtr($3)); }
  ;

Parameter:
    Expression { $$ = new Parameter(ExpressionPtr($1)); }
  | NamedExpression { $$ = $1; }
  | T_UNARY T_MULT Expression { $$ = new Parameter(Parameter::ARGS, ExpressionPtr($3)); }
  | T_UNARY T_KWARGS Expression { $$ = new Parameter(Parameter::KWARGS, ExpressionPtr($3)); }
  | T_WITHCONTEXT Expression { $$ = new Parameter(Parameter::CONTEXT, ExpressionPtr($2)); }
  ;

ExpressionList:
    Expression { $$ = new ParameterList(); $$->push_back(ParameterPtr(new Parameter(ExpressionPtr($1)))); }
  | ExpressionList T_COMMA Expression { $$ = $1; $$->push_back(ParameterPtr(new Parameter(ExpressionPtr($3)))); }
  ;

NamedExpressionList:
    NamedExpression { $$ = new ParameterList(); $$->push_back(ParameterPtr($1)); }
  | NamedExpressionList _ T_COMMA _ NamedExpression { $$ = $1; $$->push_back(ParameterPtr($5)); }
  ;

NamedExpression:
    T_IDENTIFIER T_COL _ Expression { $$ = new Parameter(*$1, ExpressionPtr($4)); delete $1; }
  ;

Expression:
    NonConstantExpression { $$ = $1; }
  | T_EXPR_CONST NonConstantExpression { $$ = WL(new ConstantExpression($2)); }
  ;

NonConstantExpression:
    AssignmentExpression { $$ = $1; }
  ;

AssignmentExpression:
    NoAssignExpression { $$ =$1; }
  | AssignmentExpression T_ASSIGN AssignExpressionChild { $$ = WL(new AssignExpression($1, $3)); }
  | AssignmentExpression T_ASSIGN_ADD AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_ADD, $3)); }
  | AssignmentExpression T_ASSIGN_SUB AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_SUB, $3)); }
  | AssignmentExpression T_ASSIGN_MUL AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_MUL, $3)); }
  | AssignmentExpression T_ASSIGN_DIV AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_DIV, $3)); }
  | AssignmentExpression T_ASSIGN_REM AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_REM, $3)); }
  | AssignmentExpression T_ASSIGN_AND AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_AND, $3)); }
  | AssignmentExpression T_ASSIGN_XOR AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_XOR, $3)); }
  | AssignmentExpression T_ASSIGN_OR AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_OR, $3)); }
  | AssignmentExpression T_ASSIGN_LSHIFT AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_LSHIFT, $3)); }
  | AssignmentExpression T_ASSIGN_RSHIFT AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_RSHIFT, $3)); }
  | AssignmentExpression T_ASSIGN_LOG_AND AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_LOG_AND, $3)); }
  | AssignmentExpression T_ASSIGN_LOG_OR AssignExpressionChild { $$ = WL(new OperationExpression($1, OperationExpression::ASSIGN_LOG_OR, $3)); }
  ;

AssignExpressionChild:
    NoAssignExpression { $$ = $1; }
  | NoAssignExpression ParameterListWithBlock { CallExpression *expr = WL(new CallExpression($1)); $$ = expr; expr->setArguments(*$2); delete $2; }
  ;

NoAssignExpression:
    ConditionalExpression { $$ = $1; }
  | T_UNARY T_AND ConditionalExpression { $$ = WL(new UnaryOperationExpression(UnaryOperationExpression::POINTER, $3)); }
  ;

ConditionalExpression:
		LogicalOrExpression { $$ = $1; }
  ;

LogicalOrExpression:
    LogicalAndExpression { $$ = $1; }
  | LogicalOrExpression T_LOG_OR LogicalAndExpression { $$ = WL(new OperationExpression($1, OperationExpression::LOG_OR, $3)); }
  ;

LogicalAndExpression:
		InclusiveOrExpression { $$ = $1; }
  | LogicalAndExpression T_LOG_AND InclusiveOrExpression { $$ = WL(new OperationExpression($1, OperationExpression::LOG_AND, $3)); }
  ;

InclusiveOrExpression:
		ExclusiveOrExpression { $$ = $1; }
  | InclusiveOrExpression T_OR ExclusiveOrExpression { $$ = WL(new OperationExpression($1, OperationExpression::OR, $3)); }
  ;

ExclusiveOrExpression:
		AndExpression { $$ = $1; }
  | ExclusiveOrExpression T_XOR AndExpression { $$ = WL(new OperationExpression($1, OperationExpression::XOR, $3)); }
  ;

AndExpression:
    EqualityExpression { $$ = $1; }
  | AndExpression T_AND EqualityExpression { $$ = WL(new OperationExpression($1, OperationExpression::AND, $3)); }
  ;

EqualityExpression: 
	  RelationalExpression { $$ = $1; }
  | EqualityExpression T_EQUALITY_OP RelationalExpression { $$ = WL(new OperationExpression($1, OperationExpression::EQUALITY, $3)); }
  | EqualityExpression T_NOT_EQUALITY_OP RelationalExpression { $$ = WL(new OperationExpression($1, OperationExpression::NOT_EQUALITY, $3)); }
  | EqualityExpression T_LESSTHAN_EQUAL RelationalExpression { $$ = WL(new OperationExpression($1, OperationExpression::LESSTHAN_EQUAL, $3)); }
  | EqualityExpression T_GREATERTHAL_EQUAL RelationalExpression { $$ = WL(new OperationExpression($1, OperationExpression::GREATERTHAN_EQUAL, $3)); }
  | EqualityExpression T_LESSTHAN RelationalExpression { $$ = WL(new OperationExpression($1, OperationExpression::LESSTHAN, $3)); }
  | EqualityExpression T_GREATERTHAN RelationalExpression { $$ = WL(new OperationExpression($1, OperationExpression::GREATERTHAN, $3)); }
  ;

RelationalExpression:
		ShiftExpression { $$ = $1; }
  ;

ShiftExpression:
		AdditiveExpression { $$ = $1; }
  | ShiftExpression T_LSHIFT AdditiveExpression { $$ = WL(new OperationExpression($1, OperationExpression::LSHIFT, $3)); }
  | ShiftExpression T_RSHIFT AdditiveExpression { $$ = WL(new OperationExpression($1, OperationExpression::RSHIFT, $3)); }
  ;

AdditiveExpression:
		MultiplicativeExpression { $$ = $1; }
  | AdditiveExpression T_ADD MultiplicativeExpression { $$ = WL(new OperationExpression($1, OperationExpression::ADD, $3)); }
  | AdditiveExpression T_SUB MultiplicativeExpression { $$ = WL(new OperationExpression($1, OperationExpression::SUB, $3)); }
  ;

MultiplicativeExpression:
    UnaryExpression { $$ = $1; }
  | MultiplicativeExpression T_MULT UnaryExpression { $$ = WL(new OperationExpression($1, OperationExpression::MUL, $3)); }
  | MultiplicativeExpression T_DIV UnaryExpression { $$ = WL(new OperationExpression($1, OperationExpression::DIV, $3)); }
  | MultiplicativeExpression T_REM UnaryExpression { $$ = WL(new OperationExpression($1, OperationExpression::REM, $3)); }
  ;

UnaryExpression:
    PostfixExpression { $$ = $1; }
  | T_UNARY T_TILDE UnaryExpression { $$ = WL(new UnaryOperationExpression(UnaryOperationExpression::TILDE, $3)); }
  | T_UNARY T_NOT UnaryExpression { $$ = WL(new UnaryOperationExpression(UnaryOperationExpression::NOT, $3)); }
  | T_UNARY T_SUB UnaryExpression { $$ = WL(new OperationExpression(new ConstantNumber(0), OperationExpression::SUB, $3)); }
  | T_UNARY T_ADD UnaryExpression { $$ = $3; }
  ;

PostfixExpression:
    PrimaryExpression { $$ = $1; }
  | PostfixExpression T_DOTDOT UnaryExpression { $$ = WL(new ConstantRange($1, $3));}
  | PostfixExpression T_LSB NonConstantExpression T_RSB { $$ = WL(new OperationExpression($1, OperationExpression::INDEX, $3));}
  | PostfixExpression T_DOT Identifier { $$ = WL(new OperationExpression($1, OperationExpression::MEMBER, $3->createStringExpression()));}
  | PostfixExpression T_DOT T_CLASS { $$ = WL(new OperationExpression($1, OperationExpression::MEMBER, new StringLiteral("class")));}
  | PostfixExpression T_COLCOL Identifier { $$ = WL(new OperationExpression($1, OperationExpression::MEMBER, $3->createStringExpression()));}
  | PostfixExpression InlineBlockDeclaration {
      CallExpression *call = new CallExpression($1);
      $$ = WL(call);
      ParameterList list;
      list.push_back(ParameterPtr(new Parameter(Parameter::BLOCK, ExpressionPtr($2))));
      call->setArguments(list);
    }
  | PostfixExpression T_LRB_CALL T_RRB { $$ = WL(new CallExpression($1)); }
  | PostfixExpression T_LRB_CALL InBraceParameterListWithBlock { CallExpression *call = new CallExpression($1); $$ = WL(call); call->setArguments(*$3); delete $3; }
  | T_DECORATOR Identifier { $$ = WL(new OperationExpression(new IdentifierExpression(new Identifier("self")), OperationExpression::MEMBER, $2->createStringExpression())); }
  | T_CLASS_ACCESSOR Identifier { $$ = WL(new OperationExpression(
      new OperationExpression(
        new IdentifierExpression(new Identifier("self")),
        OperationExpression::MEMBER,
        new StringLiteral("class")
      ),
      OperationExpression::MEMBER,
      $2->createStringExpression()
    )); }
  ;

PrimaryExpression:
    ConstantNumber { $$ = $1; }
  | T_LRB NonConstantExpression T_RRB { $$ = $2; }
  | T_NIL { $$ = WL(new NilExpression()); }
  | String { $$ = $1; }
  | FullIdentifier { $$ = WL(new IdentifierExpression($1)); }
  | T_LCB _ T_RCB { $$ = WL(new MapExpression(ParameterList())); }
  | T_LCB _ NamedExpressionList _ T_RCB { $$ = WL(new MapExpression(*$3)); delete $3; }
  | T_UNARY T_LSB _ T_RSB { $$ = WL(new ArrayExpression(ParameterList())); }
  | T_UNARY T_LSB ExpressionList T_RSB { $$ = WL(new ArrayExpression(*$3)); delete $3; }
  ;

ConstantNumber:
    T_INT_CONST { $$ = WL(new ConstantNumber($1)); }
  | T_BOOL_CONST { $$ = WL(new ConstantBoolean($1)); }
  ;

String:
    T_BEGIN_STRING T_END_STRING { $$ = WL(new StringLiteral("")); }
  | T_BEGIN_STRING StringContentList T_END_STRING { $$ = $2; }
  | T_SYMBOL { $$ = WL(new StringLiteral(*$1)); delete $1; }
  ;

StringContentList:
    StringContent { $$ = $1; }
  | StringContentList StringContent {
    Expression *toString = WL(new OperationExpression($2, OperationExpression::MEMBER, new StringLiteral("to_s")));
    $$ = WL(new OperationExpression($1, OperationExpression::ADD, new CallExpression(toString)));
  }
  ;

StringContent:
    T_STRINGLITERAL { $$ = WL(new StringLiteral(*$1)); delete $1; }
  | T_BEGIN_INLINEEXPRESSION NonConstantExpression T_END_INLINEEXPRESSION { $$ = $2; }
  ;

%%

int yyerror(YYLTYPE *yylloc, yyscan_t, const char *error) {
  yyCurrentParser->error(yylloc->first_line, yylloc->first_column, error);
  return 0;
}