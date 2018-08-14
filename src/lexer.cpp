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

void Lexer::add_token(string name,DFA* dfa)
{
    tokens.push_back(dfa);
    names[dfa]=name;
}

void Lexer::parse(istream& input)
{
    list<char> chars;
    list<char> lookahead;
    
    parser::DFA* last=nullptr;
    int count=0;
    
    reset_tokens();
    
    while (true) {
        char c;
        
        if (!input.eof()) {
            
            input.get(c);
            chars.push_back(c);
        }
        
        if (chars.size()==0) {
            break;
        }
        
        c = chars.front();
        clog<<"["<<c<<"]"<<endl;
        count=0;
        
        lookahead.push_back(c);
        
        for (DFA* t:tokens) {
            t->push(c);
            if (t->accept()) {
                //clog<<c<<" accepted by "<<names[t]<<endl;
                count++;
                if (t->end()) {
                    last=t;
                    lookahead.clear();
                }
            }
        }
        //clog<<"count " <<count<<endl;
        
        if (count==0) {
            if (last==nullptr) {
                clog<<"Failed to parse expression"<<endl;
                break;
            }
            else {
                if (last->end()) {
                    clog<<"* "<<names[last]<<":"<<last->value()<<endl;
                    last=nullptr;
                    count=0;
                    reset_tokens();
                }
                else {
                
                    if (lookahead.size()==0) {
                        clog<<"syntax error: "<<last->value()<<endl;
                        break;
                    }
                    else {
                    
                        chars.pop_front();
                        while (lookahead.size()>0) {
                            chars.push_front(lookahead.back());
                            clog<<"repushing ["<<lookahead.back()<<"]"<<endl;
                            lookahead.pop_back();
                        }
                        
                        clog<<"* "<<names[last]<<":"<<last->value()<<endl;
                        last=nullptr;
                        count=0;
                        reset_tokens();
                        
                        
                    }
                }
            }
        }
        else {
            chars.pop_front();
        }
        
        
    }
}
