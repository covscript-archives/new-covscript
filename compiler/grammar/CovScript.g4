grammar CovScript;

@lexer::header {
#include <covscript/compiler/utils/unicode.hpp>
}

// Parser
compilationUnit
    :   statementList EOF
    ;

// Statements
statement
    :   packageDeclStatement SEMI?
    |   variableDeclStatement SEMI?
    |   functionDeclStatement
    |   importStatement SEMI?
    |   expressionStatement SEMI?
    |   returnStatement SEMI?
    |   throwStatement SEMI?
    |   loopControlStatement SEMI?
    |   blockDeclStatement SEMI?
    |   ifStatement
    |   whileStatement
    |   loopStatement
    |   forStatement
    |   forEachStatement
    |   tryStatement
    |   withStatement
    |   switchStatement
    |   classDeclStatement SEMI?
    ;

packageDeclStatement
    :   KEYWORD_PACKAGE IDENTIFIER
    ;

variableDeclStatement
    :   (KEYWORD_VAR | KEYWORD_CONST) IDENTIFIER ASSIGN expression
    |   (KEYWORD_VAR | KEYWORD_CONST) LPAREN? variableBindingList RPAREN? ASSIGN expression
    ;

variableBindingList
    :   IDENTIFIER (COMMA IDENTIFIER)*
    ;

functionDeclStatement
    :   KEYWORD_FUNCTION IDENTIFIER LPAREN parameterList RPAREN KEYWORD_OVERRIDE? functionBody
    ;

functionBody
    :   LBRACE statementList RBRACE
    ;

importStatement
    :   KEYWORD_IMPORT IDENTIFIER
    ;

expressionStatement
    :   preIncrementExpression
    |   preDecrementExpression
    |   primaryExpressionAsStatement (INC | DEC | assignmentOperator expression)?
    |   structuredBindingPrefix ASSIGN expression
    ;

structuredBindingPrefix
    :   LPAREN variableBindingList RPAREN
    ;

returnStatement
    :   KEYWORD_RETURN expression?
    ;

throwStatement
    :   KEYWORD_THROW expression
    ;

blockDeclStatement
    :   LBRACE statementList RBRACE
    ;

ifStatement
    :   KEYWORD_IF LPAREN? expression RPAREN?
        statement
        (KEYWORD_ELSE statement)?
    ;

whileStatement
    :   KEYWORD_WHILE LPAREN? expression RPAREN? statement
    ;

loopStatement
    :   KEYWORD_LOOP statement (KEYWORD_UNTIL expression)?
    ;

forStatement
    :   KEYWORD_FOR LPAREN? forInit COMMA forCondition COMMA forUpdate RPAREN?
        KEYWORD_DO? statement
    ;

forInit
    :   IDENTIFIER ASSIGN expression
    ;

forCondition
    :   expression
    ;

forUpdate
    :   expressionStatement
    ;

forEachStatement
    :   KEYWORD_FOREACH IDENTIFIER KEYWORD_IN expression KEYWORD_DO? statement
    ;

loopControlStatement
    :   KEYWORD_CONTINUE
    |   KEYWORD_BREAK
    ;

tryStatement
    :   KEYWORD_TRY statement catchBody?
    ;

catchBody
    :   KEYWORD_CATCH LPAREN? IDENTIFIER RPAREN? statement
    ;

withStatement
    :   KEYWORD_WITH LPAREN? withInit RPAREN? statement
    ;

withInit
    :   KEYWORD_VAR IDENTIFIER ASSIGN expression
    ;

switchStatement
    :   KEYWORD_SWITCH LPAREN? expression RPAREN?
        LBRACE
            caseEntry*
            defaultEntry?
        RBRACE
    ;

caseEntry
    :   KEYWORD_CASE LPAREN? expression RPAREN? statement
    ;

defaultEntry
    :   KEYWORD_DEFAULT statement
    ;

classDeclStatement
    :   (KEYWORD_CLASS | KEYWORD_STRUCT) IDENTIFIER (KEYWORD_EXTENDS IDENTIFIER)?
        LBRACE
            classBody
        RBRACE
    ;

classBody
    :   (variableDeclStatement | functionDeclStatement)*
    ;

// Statement Helpers
statementList
    :   statement*
    ;

// Expressions
expression
    :   conditionalExpression
    ;

conditionalExpression
    :   conditionalLogicExpression (QUESTION expression COLON expression)?
    ;

conditionalLogicExpression
    :   bitOperationExpression conditionalLogicExpressionRest*
    ;

conditionalLogicExpressionRest
    :   (AND | OR) bitOperationExpression
    ;

bitOperationExpression
    :   relationalExpression bitOperationExpressionRest*
    ;

bitOperationExpressionRest
    :   (BITAND | BITOR | BITXOR) relationalExpression
    ;

relationalExpression
    :   additiveExpression ((LT | GT | LE | GE | EQUAL | NOTEQUAL) additiveExpression)?
    ;

additiveExpression
    :   multiplicativeExpression additiveExpressionRest*
    ;

additiveExpressionRest
    :   (ADD | SUB | KEYWORD_AND | KEYWORD_OR) multiplicativeExpression
    ;

multiplicativeExpression
    :   unaryExpression multiplicativeExpressionRest*
    ;

multiplicativeExpressionRest
    :   (MUL | DIV | MOD) unaryExpression
    ;

unaryExpression
    :   (ADD | SUB | BANG | KEYWORD_NOT | BITNOT) unaryExpression
    |   preIncrementExpression
    |   preDecrementExpression
    |   castExpression
    |   primaryExpression (INC | DEC)?
    ;

preIncrementExpression
    :   INC primaryExpression
    ;

preDecrementExpression
    :   DEC primaryExpression
    ;

castExpression
    :   LPAREN IDENTIFIER RPAREN unaryExpression
    ;

primaryExpression
    :   primaryPrefix primarySuffix*
    ;

primaryExpressionAsStatement
    :   primaryPrefixAsStatement primarySuffix*
    ;

primaryPrefix
    :   primaryPrefixAsStatement
    |   literalExpression
    |   typeidExpression
    ;

primaryPrefixAsStatement
    :   quotedExpression
    |   IDENTIFIER
    |   allocationExpression
    |   lambdaExpression
    ;

primarySuffix
    :   invocationExpression
    |   arrayVisitExpression
    |   arrayFlatExpression
    |   memberVisitExpression
    ;

invocationExpression
    :   LPAREN argumentList RPAREN
    ;

arrayVisitExpression
    :   LBRACK additiveExpression RBRACK
    ;

arrayFlatExpression
    :   EXPAND
    ;

memberVisitExpression
    :   (DOT | ARROW) IDENTIFIER
    ;

literalExpression
    :   NumberLiteral
    |   StringLiteral
    |   CharacterLiteral
    |   BooleanLiteral
    |   NullLiteral
    |   arrayLiteral
    |   mapLiteral
    ;

quotedExpression
    :   LPAREN conditionalLogicExpression (COMMA conditionalLogicExpression)* RPAREN
    ;

arrayLiteral
    :   LBRACE (expression (COMMA expression)*)? COMMA? RBRACE
    ;

mapLiteral
    :   LPAREN (mapEntry (COMMA mapEntry)*)? RPAREN
    ;

mapEntry
    :   expression COLON expression
    ;

typeidExpression
    :   KEYWORD_TYPEID expression
    ;

allocationExpression
    :   (KEYWORD_NEW | KEYWORD_GCNEW) expression (LPAREN argumentList RPAREN)?
    ;

lambdaExpression
    :   LBRACK RBRACK LPAREN parameterList RPAREN ARROW lambdaBody
    ;

lambdaBody
    :   functionBody
    |   quotedExpression
    ;

// Helpers
argumentList
    :   (expression (COMMA expression)*)?
    ;

parameterList
    :   (IDENTIFIER (EXPAND | ((COMMA IDENTIFIER)* | COMMA IDENTIFIER EXPAND)))?
    ;

assignmentOperator
    :   ASSIGN
    |   MUL_ASSIGN
    |   DIV_ASSIGN
    |   MOD_ASSIGN
    |   ADD_ASSIGN
    |   SUB_ASSIGN
    |   LSHIFT_ASSIGN
    |   RSHIFT_ASSIGN
    |   AND_ASSIGN
    |   XOR_ASSIGN
    |   OR_ASSIGN
    ;

// Lexer
KEYWORD_FUNCTION: 'function';
KEYWORD_VAR: 'var';
KEYWORD_CONST: 'const';
KEYWORD_NEW: 'new';
KEYWORD_GCNEW: 'gcnew';
KEYWORD_TYPEID: 'typeid';
KEYWORD_IMPORT: 'import';
KEYWORD_PACKAGE: 'package';
KEYWORD_FOR: 'for';
KEYWORD_WHILE: 'while';
KEYWORD_LOOP: 'loop';
KEYWORD_UNTIL: 'until';
KEYWORD_FOREACH: 'foreach';
KEYWORD_IN: 'in';
KEYWORD_DO: 'do';
KEYWORD_BREAK: 'break';
KEYWORD_CONTINUE: 'continue';
KEYWORD_RETURN: 'return';
KEYWORD_THROW: 'throw';
KEYWORD_TRY: 'try';
KEYWORD_CATCH: 'catch';
KEYWORD_STRUCT: 'struct';
KEYWORD_CLASS: 'class';
KEYWORD_EXTENDS: 'extends';
KEYWORD_OVERRIDE: 'override';
KEYWORD_USING: 'using';
KEYWORD_NAMESPACE: 'namespace';
KEYWORD_BLOCK: 'block';
KEYWORD_GLOBAL: 'global';
KEYWORD_AND: 'and';
KEYWORD_OR: 'or';
KEYWORD_NOT: 'not';
KEYWORD_IF: 'if';
KEYWORD_ELSE: 'else';
KEYWORD_SWITCH: 'switch';
KEYWORD_CASE: 'case';
KEYWORD_DEFAULT: 'default';
KEYWORD_WITH: 'with';

// Literal
NumberLiteral
    :   IntegerLiteral
    |   FloatingPointLiteral
    ;

IntegerLiteral
	:	DecimalIntegerLiteral
	|	HexIntegerLiteral
	|	OctalIntegerLiteral
	|	BinaryIntegerLiteral
	;

fragment
DecimalIntegerLiteral
	:	DecimalNumeral IntegerTypeSuffix?
	;

fragment
HexIntegerLiteral
	:	HexNumeral IntegerTypeSuffix?
	;

fragment
OctalIntegerLiteral
	:	OctalNumeral IntegerTypeSuffix?
	;

fragment
BinaryIntegerLiteral
	:	BinaryNumeral IntegerTypeSuffix?
	;

fragment
IntegerTypeSuffix
	:	[lL]
	;

fragment
DecimalNumeral
	:	'0'
	|	NonZeroDigit (Digits? | Underscores Digits)
	;

fragment
Digits
	:	Digit (DigitsAndUnderscores? Digit)?
	;

fragment
Digit
	:	'0'
	|	NonZeroDigit
	;

fragment
NonZeroDigit
	:	[1-9]
	;

fragment
DigitsAndUnderscores
	:	DigitOrUnderscore+
	;

fragment
DigitOrUnderscore
	:	Digit
	|	'_'
	;

fragment
Underscores
	:	'_'+
	;

fragment
HexNumeral
	:	'0' [xX] HexDigits
	;

fragment
HexDigits
	:	HexDigit (HexDigitsAndUnderscores? HexDigit)?
	;

fragment
HexDigit
	:	[0-9a-fA-F]
	;

fragment
HexDigitsAndUnderscores
	:	HexDigitOrUnderscore+
	;

fragment
HexDigitOrUnderscore
	:	HexDigit
	|	'_'
	;

fragment
OctalNumeral
	:	'0' Underscores? OctalDigits
	;

fragment
OctalDigits
	:	OctalDigit (OctalDigitsAndUnderscores? OctalDigit)?
	;

fragment
OctalDigit
	:	[0-7]
	;

fragment
OctalDigitsAndUnderscores
	:	OctalDigitOrUnderscore+
	;

fragment
OctalDigitOrUnderscore
	:	OctalDigit
	|	'_'
	;

fragment
BinaryNumeral
	:	'0' [bB] BinaryDigits
	;

fragment
BinaryDigits
	:	BinaryDigit (BinaryDigitsAndUnderscores? BinaryDigit)?
	;

fragment
BinaryDigit
	:	[01]
	;

fragment
BinaryDigitsAndUnderscores
	:	BinaryDigitOrUnderscore+
	;

fragment
BinaryDigitOrUnderscore
	:	BinaryDigit
	|	'_'
	;

// Floating-Point Literals

FloatingPointLiteral
	:	DecimalFloatingPointLiteral
	|	HexadecimalFloatingPointLiteral
	;

fragment
DecimalFloatingPointLiteral
	:	Digits '.' Digits? ExponentPart? FloatTypeSuffix?
	|	'.' Digits ExponentPart? FloatTypeSuffix?
	|	Digits ExponentPart FloatTypeSuffix?
	|	Digits FloatTypeSuffix
	;

fragment
ExponentPart
	:	ExponentIndicator SignedInteger
	;

fragment
ExponentIndicator
	:	[eE]
	;

fragment
SignedInteger
	:	Sign? Digits
	;

fragment
Sign
	:	[+-]
	;

fragment
FloatTypeSuffix
	:	[fFdD]
	;

fragment
HexadecimalFloatingPointLiteral
	:	HexSignificand BinaryExponent FloatTypeSuffix?
	;

fragment
HexSignificand
	:	HexNumeral '.'?
	|	'0' [xX] HexDigits? '.' HexDigits
	;

fragment
BinaryExponent
	:	BinaryExponentIndicator SignedInteger
	;

fragment
BinaryExponentIndicator
	:	[pP]
	;

// Boolean Literals

BooleanLiteral
	:	'true'
	|	'false'
	;

// Null Literal
NullLiteral
    :   'null'
    ;

// Character Literals

CharacterLiteral
	:	'\'' SingleCharacter '\''
	|	'\'' EscapeSequence '\''
	;

fragment
SingleCharacter
	:	~['\\\r\n]
	;

// String Literals
StringLiteral
	:	'"' StringCharacters? '"'
	;
fragment
StringCharacters
	:	StringCharacter+
	;
fragment
StringCharacter
	:	~["\\\r\n]
	|	EscapeSequence
	;
// §3.10.6 Escape Sequences for Character and String Literals
fragment
EscapeSequence
	:	'\\' [btnfr"'\\]
	|	OctalEscape
    |   UnicodeEscape // This is not in the spec but prevents having to preprocess the input
	;

fragment
OctalEscape
	:	'\\' OctalDigit
	|	'\\' OctalDigit OctalDigit
	|	'\\' ZeroToThree OctalDigit OctalDigit
	;

fragment
ZeroToThree
	:	[0-3]
	;

// This is not in the spec but prevents having to preprocess the input
fragment
UnicodeEscape
    :   '\\' 'u'+ HexDigit HexDigit HexDigit HexDigit
    ;

// Separators

LPAREN : '(';
RPAREN : ')';
LBRACE : '{';
RBRACE : '}';
LBRACK : '[';
RBRACK : ']';
SEMI : ';';
COMMA : ',';
DOT : '.';
AT : '@';
COLONCOLON : '::';


// Operators

ASSIGN : '=';
GT : '>';
LT : '<';
BANG : '!';
QUESTION : '?';
COLON : ':';
EQUAL : '==';
LE : '<=';
GE : '>=';
NOTEQUAL : '!=';
AND : '&&';
OR : '||';
ADD : '+';
SUB : '-';
MUL : '*';
DIV : '/';
INC : '++';
DEC : '--';
ARROW : '->';

BITAND : '&';
BITOR : '|';
BITXOR : '^';
BITNOT : '~';
MOD : '%';

EXPAND: '...';

ADD_ASSIGN : '+=';
SUB_ASSIGN : '-=';
MUL_ASSIGN : '*=';
DIV_ASSIGN : '/=';
AND_ASSIGN : '&=';
OR_ASSIGN : '|=';
XOR_ASSIGN : '^=';
MOD_ASSIGN : '%=';
LSHIFT_ASSIGN : '<<=';
RSHIFT_ASSIGN : '>>=';

// §3.8 Identifiers (must appear after all keywords in the grammar)

IDENTIFIER
	:	ScriptLetter ScriptLetterOrDigit*
	;

fragment
ScriptLetter
	:	[a-zA-Z$_] // these are the "java letters" below 0x7F
	|	// covers all characters above 0x7F which are not a surrogate
		~[\u0000-\u007F\uD800-\uDBFF]
		{cs::compiler::Character::isJavaIdentifierStart(_input->LA(-1))}?
	|	// covers UTF-16 surrogate pairs encodings for U+10000 to U+10FFFF
		[\uD800-\uDBFF] [\uDC00-\uDFFF]
		{cs::compiler::Character::isJavaIdentifierStart(cs::compiler::Character::toCodePoint((wchar_t)_input->LA(-2), (wchar_t)_input->LA(-1)))}?
	;

fragment
ScriptLetterOrDigit
	:	[a-zA-Z0-9$_] // these are the "java letters or digits" below 0x7F
	|	// covers all characters above 0x7F which are not a surrogate
		~[\u0000-\u007F\uD800-\uDBFF]
		{cs::compiler::Character::isJavaIdentifierPart(_input->LA(-1))}?
	|	// covers UTF-16 surrogate pairs encodings for U+10000 to U+10FFFF
		[\uD800-\uDBFF] [\uDC00-\uDFFF]
		{cs::compiler::Character::isJavaIdentifierPart(cs::compiler::Character::toCodePoint((wchar_t)_input->LA(-2), (wchar_t)_input->LA(-1)))}?
	;

//
// Whitespace and comments
//

WS  :  [ \t\r\n\u000C]+ -> skip
    ;

COMMENT
    :   '/*' .*? '*/' -> channel(HIDDEN)
    ;

LINE_COMMENT
    :   '#' ~[\r\n]* -> channel(HIDDEN)
    ;

BEGIN_MARK
    :   '@begin' -> channel(HIDDEN)
    ;

END_MARK
    :   '@end' -> channel(HIDDEN)
    ;
