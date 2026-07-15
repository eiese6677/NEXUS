# NEXUS Grammar Specification

Version: 0.1

This document defines the concrete and abstract grammar of the NEXUS programming language in EBNF form. The grammar is intended to guide parser implementation and to define the syntactic structure of the language.

---

## 1. Lexical Conventions

### 1.1 Whitespace

Whitespace is insignificant between tokens, except where it separates adjacent tokens that would otherwise merge.

```ebnf
Whitespace ::= " " | "\t" | "\n" | "\r"
```

### 1.2 Comments

```ebnf
Comment ::= SingleLineComment | MultiLineComment
SingleLineComment ::= "//" { AnyCharExceptNewline }
MultiLineComment ::= "/*" { AnyChar } "*/"
```

### 1.3 Identifiers

```ebnf
Identifier ::= Letter { Letter | Digit | "_" }
Letter ::= "A".."Z" | "a".."z"
Digit ::= "0".."9"
```

### 1.4 Literals

```ebnf
IntegerLiteral ::= Digit { Digit }
FloatLiteral ::= Digit { Digit } "." Digit { Digit }
StringLiteral ::= '"' { StringChar } '"'
CharLiteral ::= "'" CharChar "'"
BoolLiteral ::= "true" | "false"
```

### 1.5 Keywords

```ebnf
Keyword ::= "let"
          | "const"
          | "if"
          | "else"
          | "while"
          | "for"
          | "break"
          | "continue"
          | "return"
          | "fn"
          | "import"
          | "module"
          | "namespace"
          | "as"
          | "try"
          | "catch"
          | "throw"
          | "true"
          | "false"
          | "Int"
          | "Float"
          | "Bool"
          | "Char"
          | "String"
          | "Void"
          | "Any"
          | "Array"
          | "List"
          | "Map"
          | "print"
          | "input"
          | "len"
          | "type"
```

---

## 2. Program Structure

```ebnf
Program ::= { Statement | FunctionDeclaration | ImportDeclaration | ModuleDeclaration | NamespaceDeclaration }
```

### 2.1 Module and Namespace

```ebnf
ModuleDeclaration ::= "module" QualifiedName
NamespaceDeclaration ::= "namespace" Identifier
ImportDeclaration ::= "import" QualifiedName ["as" Identifier]
QualifiedName ::= Identifier { "." Identifier }
```

---

## 3. Statements

```ebnf
Statement ::= VariableDeclaration
            | ConstantDeclaration
            | AssignmentStatement
            | ExpressionStatement
            | IfStatement
            | WhileStatement
            | ForStatement
            | BreakStatement
            | ContinueStatement
            | ReturnStatement
            | TryStatement
            | Block
            | FunctionDeclaration
```

### 3.1 Block

```ebnf
Block ::= "{" { Statement } "}"
```

### 3.2 Variable Declaration

```ebnf
VariableDeclaration ::= "let" Identifier [":" TypeAnnotation] "=" Expression
```

### 3.3 Constant Declaration

```ebnf
ConstantDeclaration ::= "const" Identifier [":" TypeAnnotation] "=" Expression
```

### 3.4 Assignment

```ebnf
AssignmentStatement ::= Identifier "=" Expression
```

### 3.5 Expression Statement

```ebnf
ExpressionStatement ::= Expression
```

---

## 4. Expressions

```ebnf
Expression ::= AssignmentExpression
AssignmentExpression ::= ConditionalExpression ["=" AssignmentExpression]
ConditionalExpression ::= LogicalOrExpression
LogicalOrExpression ::= LogicalAndExpression { "||" LogicalAndExpression }
LogicalAndExpression ::= BitwiseOrExpression { "&&" BitwiseOrExpression }
BitwiseOrExpression ::= BitwiseXorExpression { "|" BitwiseXorExpression }
BitwiseXorExpression ::= BitwiseAndExpression { "^" BitwiseAndExpression }
BitwiseAndExpression ::= EqualityExpression { "&" EqualityExpression }
EqualityExpression ::= ComparisonExpression { ("==" | "!=") ComparisonExpression }
ComparisonExpression ::= ShiftExpression { ("<" | "<=" | ">" | ">=") ShiftExpression }
ShiftExpression ::= AdditiveExpression { ("<<" | ">>") AdditiveExpression }
AdditiveExpression ::= MultiplicativeExpression { ("+" | "-") MultiplicativeExpression }
MultiplicativeExpression ::= UnaryExpression { ("*" | "/" | "%") UnaryExpression }
UnaryExpression ::= ("!" | "~" | "-") UnaryExpression | PrimaryExpression
PrimaryExpression ::= Literal
                   | Identifier
                   | FunctionCall
                   | IndexExpression
                   | ParenthesizedExpression
                   | ArrayLiteral
                   | ListLiteral
                   | MapLiteral
                   | CastExpression
                   | InterpolatedString
```

### 4.1 Function Call

```ebnf
FunctionCall ::= Identifier "(" [ArgumentList] ")"
ArgumentList ::= Expression { "," Expression }
```

### 4.2 Index Expression

```ebnf
IndexExpression ::= PrimaryExpression "[" Expression "]"
```

### 4.3 Parenthesized Expression

```ebnf
ParenthesizedExpression ::= "(" Expression ")"
```

### 4.4 Array and List Literal

```ebnf
ArrayLiteral ::= "[" [Expression { "," Expression }] "]"
ListLiteral ::= "[" [Expression { "," Expression }] "]"
```

### 4.5 Map Literal

```ebnf
MapLiteral ::= "{" [MapEntry { "," MapEntry }] "}"
MapEntry ::= Expression ":" Expression
```

### 4.6 Cast Expression

```ebnf
CastExpression ::= PrimaryExpression "as" TypeName
```

### 4.7 Interpolated String

```ebnf
InterpolatedString ::= StringLiteral { InterpolationPart }
InterpolationPart ::= "${" Expression "}"
```

---

## 5. Control Flow

### 5.1 If Statement

```ebnf
IfStatement ::= "if" "(" Expression ")" Block ["else" Block]
```

### 5.2 While Statement

```ebnf
WhileStatement ::= "while" "(" Expression ")" Block
```

### 5.3 For Statement

```ebnf
ForStatement ::= "for" "(" Identifier "in" Expression ")" Block
```

### 5.4 Break and Continue

```ebnf
BreakStatement ::= "break"
ContinueStatement ::= "continue"
```

### 5.5 Return

```ebnf
ReturnStatement ::= "return" [Expression]
```

### 5.6 Try and Catch

```ebnf
TryStatement ::= "try" Block "catch" "(" Identifier ")" Block
```

---

## 6. Functions

```ebnf
FunctionDeclaration ::= "fn" Identifier "(" [ParameterList] ")" ":" TypeAnnotation Block
ParameterList ::= Parameter { "," Parameter }
Parameter ::= Identifier ":" TypeAnnotation
TypeAnnotation ::= TypeName
TypeName ::= PrimitiveType | CollectionType | Identifier
PrimitiveType ::= "Int" | "Float" | "Bool" | "Char" | "String" | "Void" | "Any"
CollectionType ::= "Array" "<" TypeName ">"
                | "List" "<" TypeName ">"
                | "Map" "<" TypeName "," TypeName ">"
```

---

## 7. Literals

```ebnf
Literal ::= IntegerLiteral
          | FloatLiteral
          | StringLiteral
          | CharLiteral
          | BoolLiteral
```

---

## 8. Grammar Notes

1. The grammar is written in EBNF form and is intended for parser construction.
2. `let` and `const` declarations require an initializer expression.
3. Function parameter types are mandatory.
4. Function return types are mandatory.
5. The parser must reject ambiguous or invalid constructs at parse time when possible.
6. Semicolons are not part of the grammar.
