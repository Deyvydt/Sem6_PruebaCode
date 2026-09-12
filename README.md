

## Gramática inicial

```
Program  ::= StmtList

StmtList ::= Stmt { ';' Stmt }*

Stmt     ::= Id '=' CExp  |  'print' '(' CExp ')'

CExp     ::= Expr { ('+' | '-') Expr }*

Expr     ::= Term { ('*' | '/') Term }*

Term     ::= Factor [ '**' Factor ]

Factor   ::= Number | '(' CExp ')' | 'sqrt' '(' CExp ')' | Id
```
## Gramática con estructuras de control agregadas
```
Program  ::= StmtList

StmtList ::= Stmt { ';' Stmt }*

Body     ::= Stmt { ';' Stmt }*

Stmt     ::= Id '=' CExp
           | 'print' '(' CExp ')'
           | 'if' CExp 'then' Body { 'elif' CExp 'then' Body }* [ 'else' Body ] 'endif'
           | 'do' Body 'while' CExp

CExp     ::= Expr { ('+' | '-') Expr }*

Expr     ::= Term { ('*' | '/') Term }*

Term     ::= Factor [ '**' Factor ]

Factor   ::= Number
           | '(' CExp ')'
           | 'sqrt' '(' CExp ')'
           | Id
```
## Estructura del proyecto

```
scanner.h/.cpp   Analizador léxico 
token.h/.cpp     Definición de Token
parser.h/.cpp    Analizador sintáctico 
ast.h/.cpp       Nodos del AST
visitor.h/.cpp   Visitors de impresión y evaluación
main.cpp         Punto de entrada
inputs/          Programas de entrada de ejemplo
outputs/         Resultados generados 
run_all_inputs.py  Script para compilar y ejecutar todos los inputs
```

## Compilación y ejecución

```bash
g++ main.cpp scanner.cpp token.cpp parser.cpp ast.cpp visitor.cpp -o a.out
./a.out inputs/input1.txt
```

También se puede usar el script que compila y corre todos los archivos de
`inputs/`, guardando tokens, AST y salida en `outputs/`:

```bash
python run_all_inputs.py
```


