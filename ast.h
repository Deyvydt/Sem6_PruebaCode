#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

using namespace std;

class Visitor; 

// Operadores binarios soportados
enum BinaryOp { 
    PLUS_OP, 
    MINUS_OP, 
    MUL_OP, 
    DIV_OP,
    POW_OP
};

// Clase abstracta Exp
class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();

};

// Expresión numérica
class NumberExp : public Exp {
public:
    int value;
    int accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};

// Raiz cuadrada
class SqrtExp : public Exp {
public:
    Exp* value;
    int accept(Visitor* visitor);
    SqrtExp(Exp* v);
    ~SqrtExp();
};

class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

class Stm {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;    
};

class AssignStatement : public Stm{
public:
    string variable;
    Exp* valor;
    int  accept(Visitor* visitor);
    AssignStatement(){};
    ~AssignStatement(){};
}
;

class PrintStatement : public Stm{
public:
    Exp* valor;
    int  accept(Visitor* visitor);
    PrintStatement(){};
    ~PrintStatement(){};
}
;


class Program {
public:
    list<Stm*> cuerpo;
    Program(){};
    ~Program(){};
    int accept(Visitor* visitor);   
}
;

#endif // AST_H
