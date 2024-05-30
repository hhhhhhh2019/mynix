# Терминалы

Имя|Рег. выражение
|-|-|
NUMBER|`[+-]?[0-9]+`
FLOAT_NUMBER|`[+-]?([1-9][0-9]*[eE][1-9][0-9]*\|(([1-9][0-9]*\.)\|(\.[0-9]+))([0-9]*)?([eE][\-\+]?[1-9][0-9]*)?)`
STRING|`"([^\\"\n]\|\\\\\|\\")*"`
STRING|`''([^\\"]\|\\\\\|\\")*''`
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
FOR|`for`
IN|`in`
DONE|`done`
RETURN|`return`
SPACE|`\s+`
COMMENT|`#.*\n`


# Нетерминалы

Имя|Состав
-|-
Name|`Name . UNDEFINED`
Name|`UNDEFINED`
Array|`[ ]`
Array|`[ Aargs ]`
Array|`[ Aargs , ]`
Array|`with Name [ ]`
Array|`with Name [ Aargs ]`
Array|`with Name [ Aargs , ]`
Aargs|`Aarg`
Aargs|`Aarg , Aargs`
Aarg|`Value`
Set|`{ }`
Set|`{ Sargs }`
Set|`with Name { }`
Set|`with Name { Sargs }`
Sargs|`Sarg`
Sargs|`Sarg Sargs`
Sarg|`Name = Value ;`
Sarg|`inherit Name ;`
Sarg|`import STRING ;`
Call|`Name ( )`
Call|`Name ( Cargs )`
Cargs|`Cargs , Value`
Cargs|`Value`
Func_decl|`{ Fargs } : Body done`
Func_decl|`{ } : Body done`
Fargs|`Farg`
Fargs|`Farg , Fargs`
Farg|`UNDEFINED`
Farg|`...`
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
Import|`import STRING ( Cargs )`
Import|`import STRING`
Iarg|`Value`
Body|`Expr`
Body|`Expr Body`
Expr|`Name = E1`
Expr|`for UNDEFINED in Value Body done`
Expr|`if E1 then Body done`
Expr|`if E1 then Body else Body done`
Expr|`return E1`
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

# Некоторые особенности


## Модули

Каждый файл представляет из себя функцию без аргументов.
Если результатом выполнения такого файла является функция, то используется результат её выполнения. Причём, если импорту указан только путь, то передаются аргументы функции, выполняемой в данный момент.


### Импорты


#### Файл a.nix

```
return {
    a = 1;
    b.c = 2;
}
```

#### Файл b.nix

```
return {
    foo = 1;
    bar = 2;
    import "./a.nix";
}
```

Вернёт

```
{
    foo = 1;
    bar = 2;
    a = 1;
    b = {
        c = 2;
    };
}
```

## Циклы for

Проходится можно только по элементам массива или аттрибутам именованного множества.
