#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"


using namespace std;
unordered_map<std::string, int> memoria;
///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Body::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IfStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int DoWhileStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}


///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout <<  ")";
    return 0;
}

int PrintVisitor::visit(Program* p) {
    cout << "PROGRAMA" << endl;
    for (auto i: p->cuerpo)
    {
        i->accept(this);
    }
    
    return 0;
}

void PrintVisitor::sangria() {
    for (int i = 0; i < nivel; i++) cout << "  ";
}

int PrintVisitor::visit(PrintStatement* p) {
    sangria();
    cout << "print(" ;
    p->valor->accept(this);
    cout << ")" << endl;
    return 0;
}

int PrintVisitor::visit(AssignStatement* p) {
    sangria();
    cout << p->variable << "=";
    p->valor->accept(this);
    cout << endl; 
    return 0;
}


int PrintVisitor::visit(IdExp* p) {
    cout << p->value ;
    return 0;
}



int PrintVisitor::visit(Body* b) {
    nivel++;
    for (Stm* s : b->slist) {
        s->accept(this);
    }
    nivel--;
    return 0;
}

int PrintVisitor::visit(IfStatement* stm) {
    auto itCond = stm->condiciones.begin();
    auto itBody = stm->cuerpos.begin();
    bool primero = true;

    while (itCond != stm->condiciones.end() && itBody != stm->cuerpos.end()) {
        sangria();
        cout << (primero ? "if " : "elif ");
        (*itCond)->accept(this);
        cout << " then" << endl;
        (*itBody)->accept(this);
        primero = false;
        ++itCond;
        ++itBody;
    }

    if (stm->elseBody) {
        sangria();
        cout << "else" << endl;
        stm->elseBody->accept(this);
    }

    sangria();
    cout << "endif" << endl;
    return 0;
}

int PrintVisitor::visit(DoWhileStatement* stm) {
    sangria();
    cout << "do" << endl;
    stm->cuerpo->accept(this);
    sangria();
    cout << "while ";
    stm->condicion->accept(this);
    cout << endl;
    return 0;
}

void PrintVisitor::imprimir(Program* programa){
    if (programa)
    {
        cout << "Codigo:" << endl; 
        programa->accept(this);
        cout << endl;
    }
    return ;
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP:
            result = v1 + v2;
            break;
        case MINUS_OP:
            result = v1 - v2;
            break;
        case MUL_OP:
            result = v1 * v2;
            break;
        case DIV_OP:
            if (v2 != 0)
                result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = pow(v1,v2);
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(SqrtExp* exp) {
    return floor(sqrt( exp->value->accept(this)));
}

void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:" << endl;
        programa->accept(this);
    }
    return;

}

int EVALVisitor::visit(Program* p) {
    for (auto i:p->cuerpo)
    {
       i->accept(this);
    }
    
    return 0;
}

int EVALVisitor::visit(PrintStatement* p) {
    cout << p->valor->accept(this) << endl;
    return 0;
}

int EVALVisitor::visit(AssignStatement* p) {
    memoria[p->variable] = p->valor->accept(this);
    return 0;
}



int EVALVisitor::visit(IdExp* p) {
    return memoria[p->value];
}

int EVALVisitor::visit(Body* b) {
    for (Stm* s : b->slist) {
        s->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(IfStatement* stm) {
    auto itCond = stm->condiciones.begin();
    auto itBody = stm->cuerpos.begin();

    // Se evalua cada condicion en orden; se ejecuta el primer cuerpo cuya
    // condicion sea distinta de cero y se termina.
    while (itCond != stm->condiciones.end() && itBody != stm->cuerpos.end()) {
        if ((*itCond)->accept(this) != 0) {
            (*itBody)->accept(this);
            return 0;
        }
        ++itCond;
        ++itBody;
    }

    // Si ninguna condicion se cumplio, se ejecuta el 'else' (si existe).
    if (stm->elseBody) {
        stm->elseBody->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(DoWhileStatement* stm) {
    // do-while: el cuerpo se ejecuta al menos una vez.
    do {
        stm->cuerpo->accept(this);
    } while (stm->condicion->accept(this) != 0);
    return 0;
}