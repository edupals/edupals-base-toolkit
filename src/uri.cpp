/*
 * Copyright (C) 2020 Edupals project
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

#include "uri.hpp"
#include "lexer.hpp"
#include "token.hpp"

#include <sstream>
#include <iostream>

using namespace edupals::uri;
using namespace edupals::parser;

using namespace std;

class Name: public DFA
{
    private:
    
    std::vector<int> dots;
    bool first;
    bool some_alpha;
                
    public:
    
    void start() override
    {
        char c = stack[0];
        
        first=false;
        some_alpha=false;
        
        if (token::is_num(c)) {
            _accept=true;
        }
        
        if (token::is_alpha_lower(c)) {
            _accept=true;
            some_alpha=true;
        }
    }
    
    void step() override
    {
        if (!_accept) {
        return;
        }
        
        char c = stack[cursor];
        
        if (c=='.') {
            _end=false;
            
            if (first) {
                _accept=false;
            }
            else {
                first=true;
                dots.push_back(cursor);
            }
        }
        else {
            
            if (token::is_num(c)) {
                _accept=true;
                _end = some_alpha;
            }
            else {
                if (token::is_alpha_lower(c)) {
                    some_alpha=true;
                    _accept=true;
                    _end = true;
                }
                else {
                    if (c=='-' and !first) {
                        some_alpha=true;
                        _accept=true;
                        _end=false;
                    }
                    else {
                        _accept=false;
                        _end=false;
                    }
                }
            }
            
        }
    }
};

Uri::Uri(string uri)
{
    this->port=0;
    
    DFA* colon = new token::Char(':');
    DFA* slash = new token::Char('/');
    DFA* hash = new token::Char('#');
    DFA* at = new token::Char('@');
    DFA* question = new token::Char('?');
    DFA* port = new token::Integer();
    DFA* ip4 = new token::IP4();
    DFA* name = new Name();
    
    Lexer lexer;
    
    lexer.add_token("COLON",colon);
    lexer.add_token("SLASH",slash);
    lexer.add_token("HASH",hash);
    lexer.add_token("AT",at);
    lexer.add_token("QUESTION",question);
    lexer.add_token("PORT",port);
    lexer.add_token("IP4",ip4);
    lexer.add_token("NAME",name);
    
    stringstream stream;
    
    stream<<uri;
    
    lexer.set_input(&stream);
    
    int rule = 0;
    string temp;
    
    while (lexer.step()) {
        //clog<<"rule: "<<rule<<" token: "<<lexer.get_token()<<endl;
        DFA* dfa = lexer.get_dfa();
        
        if (rule==0) {
            if (dfa!=name) {
                throw exception::SyntaxError("expected name");
            }
            this->scheme = dfa->value();
            rule++;
            continue;
        }
        
        if (rule==1) {
            if (dfa!=colon) {
                throw exception::SyntaxError("expected colon");
            }
            rule++;
            continue;
        }
        
        if (rule==2) {
            if (dfa!=slash) {
                throw exception::SyntaxError("expected slash");
            }
            rule++;
            continue;
        }
        
        if (rule==3) {
            if (dfa==slash) {
                // we have a host
                rule++;
                continue;
            }
            else {
                // we are already parsing path
                if (dfa==name) {
                    this->path="/";
                    rule=10;
                    continue;
                }
                else {
                    throw exception::SyntaxError("expected slash or name");
                }
            }
        }
        
        if (rule==4) {
            if (dfa==name) {
                temp=dfa->value();
                rule++;
                continue;
            }
            if (dfa==ip4) {
                this->host=dfa->value();
                this->ip=network::IP4(static_cast<token::IP4*>(dfa)->ip);
                
                rule=7;
                continue;
            }
            
            throw exception::SyntaxError("expected name or ip4");
        }
        
        if (rule==5) {
            if (dfa==at) {
                this->user=temp;
                rule++;
                continue;
            }
            
            this->host=temp;
            
            if (dfa==slash) {
                this->path+="/";
                rule=10;
                continue;
            }
            
            if (dfa==colon) {
                rule=8;
                continue;
            }
        }
        
        if (rule==6) {
            if (dfa==name) {
                this->host=dfa->value();
                rule++;
                continue;
            }
            
            if (dfa==ip4) {
                this->host=dfa->value();
                this->ip=network::IP4(static_cast<token::IP4*>(dfa)->ip);
                rule++;
                continue;
            }
            
            throw exception::SyntaxError("expected name or ip4");
        }
        
        if (rule==7) {
            if (dfa==colon) {
                rule++;
                continue;
            }
            
            if (dfa==slash) {
                this->path+="/";
                rule=10;
                continue;
            }
        }
        
        if (rule==8) {
            if (dfa!=port) {
                throw exception::SyntaxError("expected port");
            }
            this->port=static_cast<token::Integer*>(dfa)->get_int();
            rule++;
            continue;
        }
        
        if (rule==9) {
            if (dfa!=slash) {
                throw exception::SyntaxError("expected slash");
            }
            this->path+="/";
            rule++;
            continue;
        }
        
        if (rule==10) {
            if (dfa!=name) {
                throw exception::SyntaxError("expected name");
            }
            this->path+=dfa->value();
            
            rule=9;
            continue;
        }
        
    }
    if (!lexer.eof()) {
        throw exception::SyntaxError("Unknown token");
    }
    else {
        if (lexer.pending()) {
            throw exception::SyntaxError("Unexpected EOF");
        }
    }
    
    delete colon;
    delete slash;
    delete hash;
    delete at;
    delete question;
    delete port;
    delete ip4;
    delete name;
}
