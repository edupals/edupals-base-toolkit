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
 
#include <grammar.hpp>
#include <unistd.h>
#include <iostream>
#include <list>

using namespace edupals::parser;
using namespace std;

void Grammar::add(string name,vector<string> value)
{
    Production prod;
    
    prod.name=name;
    prod.value=value;
    
    productions.push_back(prod);
}

bool is_terminal(string& token)
{
    return (token[0]=='$');
}

bool is_epsilon(string& token)
{
    return (is_terminal(token) and token.size()==1);
}

vector<Production> Grammar::search(string name)
{
    vector<Production> tmp;
    
    for (Production p:productions) {
        if (p.name==name) {
            tmp.push_back(p);
        }
    }
    
    return tmp;
}

int Grammar::R(Production& p,list<string>& tokens)
{
    string token = tokens.front();
    sleep(1);
    clog<<"-token "<<token<<endl;
    
    for (string& t:p.value) {
        if (is_terminal(t)) {
            if (t==token) {
                clog<<"match "<<t<<endl;
                tokens.pop_front();
                
                if (tokens.size()==0) {
                    clog<<"end"<<endl;
                    return 1;
                }
                
                token = tokens.front();
                clog<<"next token "<<token<<endl;
            }
            else {
                if (is_epsilon(t)) {
                    clog<<"epsilon"<<endl;
                    return 1;
                }
                else {
                    cerr<<"unexpected "<<token<<endl;
                    return -1;
                }
            }
        }
        else {
            clog<<"non terminal "<<t<<endl;
            vector<Production> prods = search(t);
            int ret;
            
            for (Production& q:prods) {
                clog<<"exploring "<<q.name<<endl;
                ret=R(q,tokens);
                
                if (ret>0) {
                    break;
                }
            }
            
            if (ret<0) {
                return -1;
            }
        }
    }
    
    return 1;
}

void Grammar::test(vector<string> tokens)
{
    list<string> buffer;
    for (string s:tokens) {
        buffer.push_back(s);
    }
    R(productions[0],buffer);
    
}
