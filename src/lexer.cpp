/*
 * Copyright (C) 2018 Edupals project
 *
 * Author:
 *  Enrique Medina Gremaldos <quiqueiii@gmail.com>
 *
 * Source:
 *  https://github.com/edupals/edupals-base-toolkit
 *
 * This file is a part of edupals-base-toolkit.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */
 
#include <lexer.hpp>

#include <list>
#include <iostream>

using namespace edupals::parser;
using namespace std;

void Lexer::reset_tokens()
{
    for (DFA* t:tokens) {
        t->reset();
    }
}

bool Lexer::get_char(char& c)
{
    
    if (buffer==0) {
        if (!input->get(c)) {
            _eof=true;
            return false;
        }
    }
    else {
        c=buffer;
        buffer=0;
    }
    
    return true;
}

Lexer::Lexer()
{
    input=nullptr;
    buffer=0;
}

Lexer::~Lexer()
{
}

void Lexer::set_input(istream* input)
{
    this->input=input;
    buffer=0;
    _eof=false;
}

void Lexer::add_token(string name,DFA* dfa)
{
    tokens.push_back(dfa);
    names[dfa]=name;
}

bool Lexer::step()
{
    accepted=nullptr;
    last=nullptr;
    
    char c;
    int count;

    reset_tokens();
    
    eat:
    
    if (!get_char(c)) {
        if (!accepted) {
            return false;
        }
        else {
            return true;
        }
    }
    
    count=0;
    
    for (DFA* t:tokens) {
        if (!t->valid()) {
            continue;
        }
        
        t->push(c);
        
        if (t->accept()) {
            count++;
            last=t;
            
            if (t->end()) {
                accepted=t;
            }
        }
    }
    
    if (count==0) {
        if (accepted) {
            buffer=c;
            return true;
        }
        else {
            return false;
        }
    }
    
    goto eat;
    
    return true;
}

string Lexer::get_token()
{
    return names[accepted];
}

DFA* Lexer::get_dfa()
{
    return accepted;
}

bool Lexer::eof()
{
    return _eof;
}

bool Lexer::pending()
{
    return (last!=nullptr);
}
