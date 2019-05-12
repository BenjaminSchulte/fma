%option nounput
%option stack

%{
#include <fma/parser/ParserHeading.hpp>
#include "fma.y.hpp"
%}

%option yylineno

%x STRING
%x CALL_OR_IDENT
%x UNARYTEST
%x MACRO
%s INLINE_EXPRESSION

IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*[?!]?
SYMBOL :([_a-zA-Z0-9]+[?!]?)

COMMENT ;[^\r\n]*

ANYUNARY ([+\-\&~!\[]|\*{1,2})

DECIMALCONSTANT (([1-9][0-9]*)|0)
HEXCONSTANT [$]([a-fA-F0-9]+)

WHITESPACE [ \t]+
NEWLINE (\r\n?|\n)
ALL .

%%

<STRING>[^\\\"#]+ { yylval.string = new std::string(yytext); return T_STRINGLITERAL; }
<STRING>[\"] { yy_pop_state(); return T_END_STRING; }
<STRING>[#][^{] { yylval.string = new std::string(yytext); return T_STRINGLITERAL; }
<STRING>[#][{] { yy_push_state(INLINE_EXPRESSION); yy_push_state(UNARYTEST); return T_BEGIN_INLINEEXPRESSION; }
<INLINE_EXPRESSION>[}] { yy_pop_state(); return T_END_INLINEEXPRESSION; }

<CALL_OR_IDENT>[\(] { yy_pop_state(); yy_push_state(UNARYTEST); return T_LRB_CALL; }
<CALL_OR_IDENT>{WHITESPACE} { yy_pop_state(); yy_push_state(UNARYTEST); }
<CALL_OR_IDENT>.|\n { yyless(0); yy_pop_state(); }

<UNARYTEST>{ANYUNARY}/{WHITESPACE} { yyless(0); yy_pop_state(); }
<UNARYTEST>{ANYUNARY}/[\$\"a-zA-Z0-9_\(\[\{] { yyless(0); yy_pop_state(); return T_UNARY; }
<UNARYTEST>[\[]/{WHITESPACE}*[\]] { yyless(0); yy_pop_state(); return T_UNARY; }
<UNARYTEST>{WHITESPACE} { ; }
<UNARYTEST>.|\n { yyless(0); yy_pop_state(); }

<MACRO>{WHITESPACE} { ; }
<MACRO>(self) { yy_pop_state(); return T_SELF; }

(macro) { yy_push_state(MACRO); return T_MACRO; }
(raise) { return T_RAISE; }
(class) { return T_CLASS; }
(def) { return T_DEF; }
(datablock) { return T_DATABLOCK; }
(export) { return T_EXPORT; }
(module) { return T_MODULE; }
(require) { return T_REQUIRE; }
(import) { return T_IMPORT; }
(end) { return T_END; }
(if) { yy_push_state(UNARYTEST); return T_IF; }
(else) { return T_ELSE; }
(els)/(if) { return T_ELSE; }
(unless) { yy_push_state(UNARYTEST); return T_UNLESS; }
(true) { yylval.number=1; return T_BOOL_CONST;}
(false) { yylval.number=0; return T_BOOL_CONST;}
(do) { return T_DO; }
(return) { yy_push_state(UNARYTEST); return T_RETURN; }
(case) { return T_CASE; }
(when) { return T_WHEN; }
(nil) { return T_NIL; }
(with_context) { return T_WITHCONTEXT; }

<CALL_OR_IDENT>[:][:]/[a-zA-Z_] { return T_COLCOL; }
[:][:] { return T_COLCOLROOT; }
[:] {  yy_push_state(UNARYTEST); return T_COL; }

[\"] { yy_push_state(STRING); return T_BEGIN_STRING; }

{SYMBOL} { yylval.string = new std::string(yytext + 1); return T_SYMBOL; }

{DECIMALCONSTANT} {
  yylval.number = strtol(yytext, NULL, 10);
  return T_INT_CONST;
}

{HEXCONSTANT} {
  yylval.number = strtol(yytext+1, NULL, 16);
  return T_INT_CONST;
}

[+][=] { yy_push_state(UNARYTEST); return T_ASSIGN_ADD; }
[-][=] { yy_push_state(UNARYTEST); return T_ASSIGN_SUB; }
[*][=] { yy_push_state(UNARYTEST); return T_ASSIGN_MUL; }
[\/][=] { yy_push_state(UNARYTEST); return T_ASSIGN_DIV; }
[%][=] { yy_push_state(UNARYTEST); return T_ASSIGN_REM; }
[&][=] { yy_push_state(UNARYTEST); return T_ASSIGN_AND; }
[\|][=] { yy_push_state(UNARYTEST); return T_ASSIGN_OR; }
[\^][=] { yy_push_state(UNARYTEST); return T_ASSIGN_XOR; }
[<][<][=] { yy_push_state(UNARYTEST); return T_ASSIGN_LSHIFT; }
[>][>][=] { yy_push_state(UNARYTEST); return T_ASSIGN_RSHIFT; }
[\|][\|][=] { yy_push_state(UNARYTEST); return T_ASSIGN_LOG_OR; }
[&][&][=] { yy_push_state(UNARYTEST); return T_ASSIGN_LOG_AND; }

[&][&] { yy_push_state(UNARYTEST); return T_LOG_AND; }
[\|][\|] { yy_push_state(UNARYTEST); return T_LOG_OR; }
[#] { yy_push_state(UNARYTEST); return T_EXPR_CONST; }
[&] { yy_push_state(UNARYTEST); return T_AND; }
[%] { yy_push_state(UNARYTEST); return T_REM; }
[~] { yy_push_state(UNARYTEST); return T_TILDE; }
[!] { yy_push_state(UNARYTEST); return T_NOT; }
[\|] { yy_push_state(UNARYTEST); return T_OR; }
[\^] { yy_push_state(UNARYTEST); return T_XOR; }
[*][*] { yy_push_state(UNARYTEST); return T_KWARGS; }
[*] { yy_push_state(UNARYTEST); return T_MULT; }
[\/] { yy_push_state(UNARYTEST); return T_DIV; }
[-] { yy_push_state(UNARYTEST); return T_SUB; }
[+] { yy_push_state(UNARYTEST); return T_ADD; }
[{] { yy_push_state(UNARYTEST); return T_LCB; }
[}] { yy_push_state(UNARYTEST); return T_RCB; }
[\(] { yy_push_state(UNARYTEST); return T_LRB; }
[\)] { yy_push_state(UNARYTEST); return T_RRB; }
[\[] { yy_push_state(UNARYTEST); return T_LSB; }
[\]] { return T_RSB; }
[<][<] { yy_push_state(UNARYTEST); return T_LSHIFT; }
[>][>] { yy_push_state(UNARYTEST); return T_RSHIFT; }
[=][=] { yy_push_state(UNARYTEST); return T_EQUALITY_OP; }
[!][=] { yy_push_state(UNARYTEST); return T_NOT_EQUALITY_OP; }
[<][=] { yy_push_state(UNARYTEST); return T_LESSTHAN_EQUAL; }
[>][=] { yy_push_state(UNARYTEST); return T_GREATERTHAL_EQUAL; }
[<] { yy_push_state(UNARYTEST); return T_LESSTHAN; }
[>] { yy_push_state(UNARYTEST); return T_GREATERTHAN; }
[\.][\.] { return T_DOTDOT; }
[\.] { return T_DOT; }
[,] { yy_push_state(UNARYTEST); return T_COMMA; }
[=] { yy_push_state(UNARYTEST); return T_ASSIGN; }

<MACRO>[@] { return T_DECORATOR; }
[@][@] { return T_CLASS_ACCESSOR; }
[@] { return T_DECORATOR; }
<MACRO>.|\n { yyless(0); yy_pop_state(); }

{COMMENT} { ; }

{NEWLINE} {
  yy_push_state(UNARYTEST); 
  return T_NEWLINE;
}

{WHITESPACE} { ; }

{IDENTIFIER}	{ yylval.string = new std::string(yytext); yy_push_state(CALL_OR_IDENT);  return T_IDENTIFIER; }

{ALL} {
  yy_top_state(); // supress warning of unused function!
  fprintf(stderr, "Invalid token\n");
  exit(1);
}
