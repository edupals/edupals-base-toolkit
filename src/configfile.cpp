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
 

#include <configfile.hpp>
#include <lexer.hpp>
#include <token.hpp>
#include <grammar.hpp>

#include <fstream>
#include <iostream>

using namespace edupals;
using namespace edupals::configfile;
using namespace std;

class Comment : public parser::DFA
{
    public:
    
    void start() override
    {
        if (stack[0]=='#') {
            _accept=true;
        }
    }
    
    void step() override 
    {
        if (!_accept) {
            return;
        }
        
        if (_end) {
            _accept=false;
            _end=false;
        }
        else {
            if (stack[cursor]=='\n') {
                _end=true;
            }
        }
    }
};

class Name : public parser::DFA
{
    public:
    
    void start() override
    {
        char c = stack[cursor];
        
        if ((c>='a' and c<='z') or (c>='A' and c<='Z')) {
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
        
        bool valid=false;
                
        if (c==' ' or c=='_' or c=='-') {
            _end=false;
            valid=true;
        }
        if ((c>='a' and c<='z') or (c>='A' and c<='Z') or (c>='0' and c<='9')) {
            _end=true;
            valid=true;
        }
        
        _accept=valid;
        _end = _end and valid;
    }
};

class Value : public parser::DFA
{
    public:
    
    void start() override
    {
        if (stack[0]=='=') {
            _accept=true;
        }
    }
    
    void step() override
    {
        if (!_accept) {
            return;
        }
        
        
        if (_end) {
            _end=false;
            _accept=false;
        }
        else {
            if (stack[cursor]=='\n') {
                _end=true;
            }
        }
    }
};

Section::Section()
{
}

Section::Section(string name)
{
}

void Section::clear()
{
}

string& Section::operator [] (string name)
{
}

Config::Config()
{
}

Config::Config(string name)
{
    parser::token::Char ws(' ');
    parser::token::Char cr('\n');
    parser::token::Char left('[');
    parser::token::Char right(']');
    parser::token::Char equal('=');
    Comment comment;
    Name sname;
    Value value;
    
    parser::Lexer ini_lexer;
    
    ini_lexer.add_token("WS",&ws);
    ini_lexer.add_token("CR",&cr);
    ini_lexer.add_token("LEFT",&left);
    ini_lexer.add_token("RIGHT",&right);
    ini_lexer.add_token("EQUAL",&equal);
    ini_lexer.add_token("COMMENT",&comment);
    ini_lexer.add_token("NAME",&sname);
    //ini_lexer.add_token("VALUE",&value);
    
    ini_lexer.signal_accepted(
        [](parser::DFA* dfa,string name,void* data) {
            clog<<"-- token: "<<name<<"="<<dfa->value()<<endl;
        }
    );
    
    ini_lexer.signal_rejected(
        [](string expression,void* data) {
            clog<<"-- syntax error: "<<expression<<endl;
        }
    );
        
    ifstream file;
    
    file.open(name);
    
    ini_lexer.parse(file);
    
    file.close();
    
}

Section& Config::operator [] (string name)
{
}
