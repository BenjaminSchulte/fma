{
  const n = require('./nodes/index');
  const Location = require('./Location').default;

  function c(node) {
    const l = location();
    node.setLocation(new Location(l.start.line, l.start.column));
    return node;
  }
}

Program
  = stmts:Stmts? { return c(new n.Program()).setChildren(stmts || []); }

Stmts
  = terms:Terms* list:StmtsList+ { return list.reduce(function(a, b) { return a.concat(b); }, [].concat(terms)); }

StmtsList
  = stmt:Stmt terms:Terms+ { return [stmt].concat(terms); }

Stmt
  = LabelDeclaration    // OK
  / stmt:ExprOrCommand post:PostStmt? {
    if (post) {
      console.log('TODO: POST');
    }

    return c(new n.ExpressionStatement(stmt));
  }
  / ModuleDeclaration   // OK
  / MacroDeclaration    // OK
  / FunctionDeclaration // OK
  / ClassDeclaration    // OK
  / IfStmt              // OK
  / CaseStmt            // OK

LabelDeclaration
  = ident:IDENTIFIER ":" { return c(new n.LabelDeclaration(ident)); }

PostStmt
  = __ keyword_if __ Expr
  / __ keyword_unless __ Expr
  / __ DoBlock

IfStmt
  = keyword_if __ condition:ExprOrCommand block:Block elseifs:ElseIfStmt* otherwise:ElseStmt? keyword_end {
    var cur = c(new n.IfStatement()).setChildren(block).setCondition(condition);

    for (let elseif of elseifs) {
      cur.setElse(elseif);
      cur = elseif;
    }

    if (otherwise) {
      cur.setElse(otherwise);
    }

    return cur;
  }

ElseIfStmt
  = keyword_elsif __ condition:ExprOrCommand block:Block {
    return c(new n.IfStatement()).setChildren(block).setCondition(condition);
  }

ElseStmt
  = keyword_else block:Block {
    return c(new n.ElseStatement()).setChildren(block);
  }

CaseStmt
  = keyword_case __ condition:Expr comment_left:Terms+ cases:CaseStmtWhens* comment_right:Terms* keyword_end {
    return c(new n.SwitchStatement()).setChildren(
      comment_left.concat(cases).concat(comment_right)
    ).setCondition(condition);
  }

CaseStmtWhens
  = keyword_when __ value:Expr block:Block {
    return c(new n.SwitchCase()).setValue(value).setChildren(block);
  }
  / keyword_else block:Block {
    return c(new n.ElseStatement()).setChildren(block);
  }

ExprOrCommand
  = expr:Expr

ExprAfter
  = args:CallArgs { return args; }

Expr
  = item:CalcExpr cond:InlineCondition? {
    if (cond) {
      console.log('TODO: Inline condition');
    }

    return item;
  }

InlineCondition
 = _ "?" _ Expr _ ":" _ Expr

CalcExpr
  = item:ExprItem list:ExprList* {
    if (!list.length) {
      return item;
    }

    const expr = c(new n.CalculateExpression(item));
    for (let calc of list) {
      expr.addCalculation(calc[0], calc[1]);
    }
    return expr;
  }

ExprItem
  = constant:"#"? _ not:"!"? _ root:"::"? primary:Primary {
    if (root) {
      console.log('Todo: root');
    }

    if (not) {
      primary = c(new n.NotExpression(primary));
    }

    if (constant) {
      primary = c(new n.ConstantExpression(primary));
    }

    return primary;
  }

ExprList
  = _ "<<" _ item:ExprItem { return ["<<", item] }
  / _ ">>" _ item:ExprItem { return [">>", item] }
  / _ "==" _ item:ExprItem { return ["==", item] }
  / _ "!=" _ item:ExprItem { return ["!=", item] }
  / _ "+=" _ item:ExprItem { return ["+=", item] }
  / _ "-=" _ item:ExprItem { return ["-=", item] }
  / _ "/=" _ item:ExprItem { return ["/=", item] }
  / _ "*=" _ item:ExprItem { return ["*=", item] }
  / _ "|=" _ item:ExprItem { return ["|=", item] }
  / _ "&=" _ item:ExprItem { return ["&=", item] }
  / _ "??" _ item:ExprItem { return ["??", item] }
  / _ "||" _ item:ExprItem { return ["||", item] }
  / _ "&&" _ item:ExprItem { return ["&&", item] }
  / _ "+" _ item:ExprItem { return ["+", item] }
  / _ "-" _ item:ExprItem { return ["-", item] }
  / _ "*" _ item:ExprItem { return ["*", item] }
  / _ "/" _ item:ExprItem { return ["/", item] }
  / _ "<" _ item:ExprItem { return ["<", item] }
  / _ ">" _ item:ExprItem { return [">", item] }
  / _ "=" _ item:ExprItem { return ["=", item] }
  / _ "%" _ item:ExprItem { return ["%", item] }
  / _ "&" _ item:ExprItem { return ["&", item] }
  / _ "|" _ item:ExprItem { return ["|", item] }

Decorator
  = "@" expr:ExprOrCommand Terms+ { return expr; }

Decorators
  = Decorator+

Block
  = comment_left:Terms stmts:Stmts? comment_right:Terms* {
    return (comment_left ? [comment_left] : []).concat(stmts ? stmts : []).concat(comment_right)
  }

FunctionDeclaration
  = dec:Decorators? keyword_def __ name:IDENTIFIER block:Block keyword_end  {
    return c(new n.FunctionDeclaration(name)).setChildren(block).setDecorators(dec ? dec : []);
  }

MacroDeclaration
  = keyword_macro __ name:IDENTIFIER args:AnyMacroArgs? block:Block keyword_end {
    return c(new n.MacroDeclaration(name)).setChildren(block).setArguments(args ? args : []);
  }

AnyMacroArgs
  =  _ "(" _ args:MacroArgs _ ")" _ { return args; }
  / __ args:MacroArgs { return args; }

MacroArgs
  = arg:MacroArg args:MacroArgList* { return [arg].concat(args); }

MacroArgList
  = _ "," _ arg:MacroArg { return arg; }

MacroArg
  = arg:MacroArgName defaultValue:MacroArgPost? {
    if (defaultValue) {
      arg.setDefaultValue(defaultValue);
    }

    return arg;
  }

MacroArgPost
  = _ "=" _ value:Expr { return value; }

MacroArgName
  = ident:IDENTIFIER { return c(new n.Argument(ident)) }
  / "&" ident:IDENTIFIER { return c(new n.Argument(ident)).setType(n.ParameterTypes.BLOCK) }
  / "**" ident:IDENTIFIER { return c(new n.Argument(ident)).setType(n.ParameterTypes.KEYWORD_ARGUMENTS) }
  / "*" ident:IDENTIFIER { return c(new n.Argument(ident)).setType(n.ParameterTypes.ARGUMENTS) }

ModuleDeclaration
  = keyword_module __ name:ModuleName block:Block keyword_end {
    return c(new n.ModuleDeclaration(name)).setChildren(block)
  }

ClassDeclaration
  = keyword_class __ name:ModuleName extendList:ClassExtends* block:Block keyword_end {
    return c(new n.ClassDeclaration(name)).setChildren(block).setExtends(extendList);
  }

ClassExtends
  = _ "<" _ expr:Expr { return expr; }

ModuleName
  = root:"::"? identifier:IDENTIFIER identifiers:ModuleNameList* { return c(new n.IdentifierList()).setChildren([identifier].concat(identifiers)).setIsRoot(!!root);}

ModuleNameList
  = "::" identifier:IDENTIFIER { return identifier; }

Command
  = Primary

PrimaryAfter
  = "." value:IDENTIFIER { return c(new n.ChildExpression(null)).setChild(value); }
  / "::" value:IDENTIFIER { return c(new n.ChildExpression(null)).setChild(value); }
  / "[" _ expr:Expr _ "]" { return c(new n.ChildExpression(null)).setChild(expr); }
  / _ "(" _ args:CallArgs? _ ")" { return c(new n.CallExpression(null)).setParameters(args || []); }
  / __ param:CallArgs { return c(new n.CallExpression(null)).setParameters(param ? param : []); }

Primary
  = value:PrimaryValue after:PrimaryAfter* {
    for (let item of after) {
      item.setParent(value);
      value = item;
    }
    return value;
  }

PrimaryValue
  = Literal
  / String
  / IDENTIFIER
  / Array
  / "(" _ expr:Expr _ ")" { return expr; }

CallArg
  = name:IDENTIFIER ":" _ value:ExprItem { return c(new n.Parameter(value)).setName(name); }
  / "&" !_ value:ExprItem { return c(new n.Parameter(value)).setType(n.ParameterTypes.BLOCK); }
  / "*" !_ value:ExprItem { return c(new n.Parameter(value)).setType(n.ParameterTypes.ARGUMENTS); }
  / "**" !_ value:ExprItem { return c(new n.Parameter(value)).setType(n.ParameterTypes.KEYWORD_ARGUMENTS); }
  / value:InlineBlock { return c(new n.Parameter(value)).setType(n.ParameterTypes.BLOCK); }
  / value:ExprItem { return c(new n.Parameter(value)); }

CallArgs
  = arg:CallArg args:CallArgsList* { return [arg].concat(args); }

CallArgsList
  = _ "," _ arg:CallArg { return arg; }

Array
  = "[" _ items:ArrayListItems? _ "]" { return c(new n.ArrayDeclaration()).setChildren(items ? items : []); }

ArrayListItems
  = item:Expr items:ArrayListItem* { return [item].concat(items); }

ArrayListItem
  = _ "," _ item:Expr { return item }

//Expr_Value

DoBlock
  = keyword_do _ args:BlockArgs? block:Block keyword_end {
    return c(
      new f.Parameter(c(new f.MacroDeclaration(null))
        .setChildren(block)
        .setArguments(args)
      )
    ).setType(n.ParameterTypes.BLOCK);
  }

InlineBlock
  = "{" _ args:BlockArgs? _ expr:Expr _ "}" {
    return c(new f.MacroDeclaration(null)).setChildren(block).setArguments(args);
  }

BlockArgs
  = "|" _ args:MacroArgs _ "|" { return args; }


// ------------------------------------------------------------------
//   Literals
// ------------------------------------------------------------------
Literal
  = Numeric

Numeric
  = f:[0-9]+ { return c(new n.NumericLiteral(parseInt(f.join(''), 10))); }
  / "$" f:[a-fA-F0-9]+ { return c(new n.NumericLiteral(parseInt(f.join(''), 16))); }

String
  = "\"" content:DoubleStringContent* "\"" { return c(new n.StringLiteral(c(new n.Identifier()).setChildren(content))); }
  / ":" ident:IDENTIFIER { return c(new n.StringLiteral(ident)); }

DoubleStringContent
  = ch:[^\"\\#]+ { return c(new n.IdentifierName(ch.join(''))) }
  / "\\" ch:. { return c(new n.IdentifierName(ch)) }
  / ch:"#" !"{" { return c(new n.IdentifierName(ch)) }
  / InlineExpression

InlineExpression
  = "#{" _ expr:Expr _ "}" { return expr; }

// ------------------------------------------------------------------
//   Token
// ------------------------------------------------------------------
InlineIdentifier
  = "\\{" _ Expr _ "}"

IdentifierStart
  = parts:[a-zA-Z_]+ { return c(new n.IdentifierName(parts.join(''))); }
  / InlineIdentifier

IdentifierPart
  = parts:[a-zA-Z0-9_]+ { return c(new n.IdentifierName(parts.join(''))); }
  / InlineIdentifier

IdentifierEnd
  = "?" { return c(new n.IdentifierName('?'))}
  / "!" { return c(new n.IdentifierName('!'))}

IdentifierString
  = !ReservedWord start:IdentifierStart center:IdentifierPart* end:IdentifierEnd? {
    return c(new n.Identifier()).setChildren([start].concat(center).concat(end ? [end] : []))
  }

IDENTIFIER
  = IdentifierString


// ------------------------------------------------------------------
//   Keywords
// ------------------------------------------------------------------
keyword_if = "if"
keyword_elsif = "elsif"
keyword_unless = "unless"
keyword_end = "end"
keyword_module = "module"
keyword_do = "do"
keyword_case = "case"
keyword_macro = "macro"
keyword_def = "def"
keyword_when = "when"
keyword_class = "class"
keyword_else = "else"

ReservedWord
  = "if"
  / "elsif"
  / "end"
  / "case"
  / "module"
  / "macro"
  / "do"
  / "unless"
  / "class"
  / "def"
  / "when"
  / "else"

// ------------------------------------------------------------------
//   Core content generation
// ------------------------------------------------------------------

EOL "newline"
  = _ comment:Comment? [\n]+ _ { return comment ? c(new n.Comment(comment)) : null; }

CommentContent
  = text:[^\n]* { return text.join(''); }

Comment
  = ";" text:CommentContent { return text; }

Terms
  = EOL

_ "whitespace"
  = ([ \t\r] / "\\\n")*

__ "whitespace"
  = ([ \t\r] / "\\\n")+
