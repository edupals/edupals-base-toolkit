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

#include <fstream>

using namespace edupals;
using namespace edupals::configfile;
using namespace std;

class Comment : public parser::DFA
{
    public:
    
    void step() override 
    {
    
        if (_end) {
            _accept=false;
        }
    
        if (cursor==0) {
            if (stack[0]=='#') {
                _accept=true;
            }
        }
        else {
            if (_accept and stack[cursor]=='\n') {
                _end=true;
                last=cursor;
            }
        }
    }
};

class Name : public parser::DFA
{
    public:
    
    void step() override
    {
        int8_t c = stack[cursor];
        
        if (cursor==0) {
            
            if ((c>='a' and c<='z') or (c>='A' and c<='Z')) {
                _accept=true;
                _end=true;
                last=cursor;
            }
        }
        else {
            if (_accept) {
                bool valid=false;
                
                if (c==' ' or c=='_' or c=='-') {
                    _end=false;
                    valid=true;
                }
                if ((c>='a' and c<='z') or (c>='A' and c<='Z')) {
                    _end=true;
                    last=cursor;
                    valid=true;
                }
                
                _accept=valid;
            }
        }
    }
};

class Value : public parser::DFA
{
    public:
    
    void step() override
    {
        if (cursor==0) {
            if (stack[0]=='=') {
                _accept=true;
            }
        }
        else {
            if (_end) {
                _end=false;
                _accept=false;
            }
            else {
                if (stack[cursor]=='\n') {
                    _end=true;
                    last=cursor;
                }
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
    ini_lexer.add_token("VALUE",&value);

    ifstream file;
    
    file.open(name);
    
    ini_lexer.parse(file);
    
    file.close();

}

Section& Config::operator [] (string name)
{
}
