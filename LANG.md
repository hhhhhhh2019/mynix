# Терминалы

Имя|Рег. выражение
|-|-|
NUMBER|`[+-]?[0-9]+`
FLOAT_NUMBER|`[+-]?([1-9][0-9]*[eE][1-9][0-9]*\|(([1-9][0-9]*\.)\|(\.[0-9]+))([0-9]*)?([eE][\-\+]?[1-9][0-9]*)?)`
STRING|`"([^\\"]\|\\\\\|\\")*"`
LBR|`\(`
RBR|`\)`
LCBR|`\{`
RCBR|`\}`
LSBR|`\[`
RSBR|`\]`
COMMA|`,`
DOT|`\.`
SEMICOLON|`;`
DOLLAR|`\$`
PLUS|`\+`
MINUS|`\-`
STAR|`\*`
SLASH|`\/`
AMPERSAND|`\&`
PIPE|`\|`
CARET|`\^`
EXCLAMATION|`\!`
EPSILON|`\.\.\.`
ASSIGN|`=`
EQUALS|`==`
NOT_EQUALS|`!=`
LESS|`<`
LESS_EQUALS|`<=`
MORE|`>`
MORE_EQUALS|`>=`
RETURN|`return`
INHERIT|`inherit`
WITH|`with`
IMPORT|`import`
IF|`if`
ELSE|`else`
THEN|`then`
FUNCTION|`function`
FOR|`for`
IN|`in`
DONE|`done`


# Нетерминалы

Имя|Состав
-|-
Name|`Name . UNDEFINED`
Name|`UNDEFINED`
Array|`[ ]`
Array|`[ Aargs ]`
Array|`[ Aargs , ]`
Aargs|`Aarg`
Aargs|`Aarg , Aargs`
Aarg|`Value`
Aarg|`Value`
Set|`{ }`
Set|`{ Sargs }`
Sargs|`Sarg`
Sargs|`Sarg Sargs`
Sarg|`Name = Value ;`
Sarg|`inherit Name ;`
Sarg|`import STRING ;`
Call|`Name ( )`
Call|`Name ( Cargs )`
Cargs|`Cargs , Value`
Cargs|`Value`
Func_decl|`function ( Fargs ) Body done`
Func_decl|`function ( ) Body done`
Fargs|`Farg`
Fargs|`Farg , Fargs`
Farg|`UNDEFINED`
Value|`NUMBER`
Value|`FLOAT_NUMBER`
Value|`STRING`
Value|`Array`
Value|`Set`
Value|`LBR Expr RBR`
Value|`Func_decl`
Value|`Call`
Value|`! Value`
Value|`Import`
Import|`import STRING`
Body|`Expr`
Body|`Expr Body`
Expr|`Name = E1`
Expr|`for UNDEFINED in Value Body done`
Expr|`if E1 then Body done`
Expr|`if E1 then Body else Body done`
Expr|`E1`
E1|`if E1 then E1 else E1`
E1|`E2`
E2|`E3`
E2|`E2 == E2`
E2|`E2 != E2`
E2|`E2 > E2`
E2|`E2 >= E2`
E2|`E2 < E2`
E2|`E2 <= E2`
E3|`E4`
E3|`E3 & E3`
E3|`E3 \| E3`
E3|`E3 ^ E3`
E4|`E5`
E4|`E4 + E4`
E4|`E4 - E4`
E5|`Value`
E5|`E5 * E5`
E5|`E5 / E5`
