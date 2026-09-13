#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <unordered_map>

class BinaryExp;
class NumberExp;
class SqrtExp;
class Body;
class IfStatement;
class DoWhileStatement;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(SqrtExp* exp) = 0;
    virtual int visit(Program* p) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual int visit(PrintStatement* stm) = 0;
    virtual int visit(AssignStatement* stm) = 0;
    virtual int visit(Body* b) = 0;
    virtual int visit(IfStatement* stm) = 0;
    virtual int visit(DoWhileStatement* stm) = 0;
};

class PrintVisitor : public Visitor {
public:

    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(Program* p) override;
    int visit(PrintStatement* stm) override; 
    int visit(AssignStatement* stm) override;
    int visit(Body* b) override;
    int visit(IfStatement* stm) override;
    int visit(DoWhileStatement* stm) override;
    void imprimir(Program* program);
private:
    int nivel = 0;            // nivel de anidamiento, para la sangria
    void sangria();           // imprime 2 espacios por nivel
};

class EVALVisitor : public Visitor {
public:
    unordered_map<string,int> memoria;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(Program* p) override;
    int visit(PrintStatement* stm) override; 
    int visit(AssignStatement* stm) override;
    int visit(Body* b) override;
    int visit(IfStatement* stm) override;
    int visit(DoWhileStatement* stm) override;
    void interprete(Program* program);
};


#endif // VISITOR_H