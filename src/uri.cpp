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

class Scheme: public DFA
{
    
    public:
    
    void start() override
    {
        char c = stack[0];
        
        if ( (c>='A' and c<='Z') or (c>='a' and c<='z') ) {
            _accept=true;
            _end=true;
        }
    }
    void step() override
    {
        if (!_accept) {
            return;
        }
        
        char c = stack[cursor];
        
        if (!( (c>='A' and c<='Z') or (c>='a' and c<='z') or (c>='0' and c<='9') or (c=='+') or (c=='.') or (c=='-'))) {
            _accept=false;
            _end=false;
        }
    }
};

Uri::Uri(string uri)
{
    DFA* colon = new token::Char(':');
    DFA* doubleslash = new token::Word("//");
    Scheme* scheme = new Scheme();
    DFA* hash = new token::Char('#');
    DFA* at = new token::Char('@');
    DFA* question = new token::Char('?');
    DFA* port = new token::Integer();
    DFA* ip4 = new token::IP4();
    DFA* hostname = new token::Hostname();
    
    Lexer lexer;
    
    lexer.add_token("COLON",colon);
    lexer.add_token("DS",doubleslash);
    lexer.add_token("SCHEME",scheme);
    lexer.add_token("HASH",hash);
    lexer.add_token("AT",at);
    lexer.add_token("QUESTION",question);
    lexer.add_token("PORT",port);
    lexer.add_token("IP4",ip4);
    lexer.add_token("HOSTNAME",hostname);
    
    stringstream stream;
    
    stream<<uri;
    
    lexer.set_input(&stream);
    
    while (lexer.step()) {
        clog<<lexer.get_token()<<endl;
        DFA* dfa = lexer.get_dfa();
       
        if (dfa==scheme) {
            clog<<"scheme: "<<dfa->value()<<endl;
        }
        
        if (dfa==ip4) {
            clog<<"ipv4:"<<endl;
            for (int n=0;n<4;n++) {
                clog<<(int)static_cast<token::IP4*>(dfa)->ip[n]<<endl;
            }
        }
        
        if (dfa==hostname) {
            clog<<"hostname:"<<dfa->value()<<endl;
        }
    }
    if (!lexer.eof()) {
        cerr<<"Unknown token";
    }
    else {
        if (lexer.pending()) {
            cerr<<"Unexpected EOF";
        }
    }
    delete colon;
    delete doubleslash;
    delete scheme;
    delete hash;
    delete at;
    delete question;
}
