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

#include <iostream>

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

Production& Grammar::search(string name)
{

}

void Grammar::R(Production& p,int n,vector<string>& tokens);
{
    int m=n;
    string token = tokens[m];
    
    for (string& t:p.value) {
        if (is_terminal(t)) {
            if (t==token) {
                m++;
                if (m==tokens.size()) {
                    clog<<"end"<<endl;
                    return;
                }
                
                token = tokens[m];
            }
            else {
                if (is_epsilon(t)) {
                }
                else {
                    cerr<<"unexpected "<<token<<endl;
                    return;
                }
            }
        }
        else {
        }
    }
}

void Grammar::test(vector<string> tokens)
{
    R(productions[0],0);
    
}
