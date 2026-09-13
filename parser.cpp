#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}


// =============================
// Reglas gramaticales
// =============================

Program* Parser::parseProgram() {
    Program* programa = new Program();
    programa->cuerpo.push_back(parseStm());
    while(match(Token::SEMICOLON)){
        programa->cuerpo.push_back(parseStm());
    }
    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return programa;
}

Stm* Parser::parseStm(){
    if (match(Token::ID))
    {
        AssignStatement* stm = new AssignStatement();
        stm->variable = previous->text;
        match(Token::ASSIGN);
        stm->valor =  parseCE();
        return stm;
    }
    else if (match(Token::PRINT))
    {
        PrintStatement* stm = new PrintStatement();
        match(Token::LPAREN);
        stm->valor =  parseCE();
        match(Token::RPAREN);

        return stm;
    }

    else if (match(Token::IF))
    {
        IfStatement* stm = new IfStatement();

        // if CExp then Body
        stm->condiciones.push_back(parseCE());
        if (!match(Token::THEN))
            throw runtime_error("Error sintactico: se esperaba 'then'");
        stm->cuerpos.push_back(parseBody());

        // { elif CExp then Body }*
        while (match(Token::ELIF)) {
            stm->condiciones.push_back(parseCE());
            if (!match(Token::THEN))
                throw runtime_error("Error sintactico: se esperaba 'then' tras 'elif'");
            stm->cuerpos.push_back(parseBody());
        }

        // [ else Body ]
        if (match(Token::ELSE)) {
            stm->elseBody = parseBody();
        }

        // endif
        if (!match(Token::ENDIF))
            throw runtime_error("Error sintactico: se esperaba 'endif'");

        return stm;
    }
    else if (match(Token::DO))
    {
        DoWhileStatement* stm = new DoWhileStatement();

        // do Body while CExp
        stm->cuerpo = parseBody();
        if (!match(Token::WHILE))
            throw runtime_error("Error sintactico: se esperaba 'while'");
        stm->condicion = parseCE();

        return stm;
    }

    else {
        throw runtime_error("Error sintáctico");
    }
    
}

// Body ::= Stm { ';' Stm }*
Body* Parser::parseBody() {
    Body* cuerpo = new Body();
    cuerpo->slist.push_back(parseStm());
    while (match(Token::SEMICOLON)) {
        cuerpo->slist.push_back(parseStm());
    }
    return cuerpo;
}

Exp* Parser::parseCE() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}



Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}


Exp* Parser::parseT() {
    Exp* l = parseF();
    if (match(Token::POW)) {
        BinaryOp op = POW_OP;
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseF() {
    Exp* e; 
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    } 
    else if (match(Token::ID)) {
        string va  = previous->text;
        return new IdExp(va);
    } 
    else if (match(Token::LPAREN))
    {
        e = parseCE();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {   
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}