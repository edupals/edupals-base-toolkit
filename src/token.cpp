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
 
#include <token.hpp>

using namespace edupals::parser::token;
using namespace std;

Word::Word(string match)
{
    this->match=match;
}

void Word::start()
{
    _accept = (match[0]==stack[0]);
    
    if (match.size()==1) {
        _end=_accept;
    }
}

void Word::step()
{
    if (cursor>=match.size()) {
        _accept=false;
        _end=false;
    }
    else {
        _accept = _accept and (match[cursor]==stack[cursor]);
        
        if (cursor==match.size()-1) {
            _end=_accept;
        }
    }
}

Char::Char(char match)
{
    this->match=match;
}

void Char::start()
{
    _accept = (stack[0]==match);
    _end=_accept;
}

void Char::step()
{
    _accept=false;
    _end=false;
}

Group::Group(vector<char> chars)
{
    this->chars=chars;
}

void Group::start()
{
    char c=stack[0];
    
    _accept=false;
    _end=false;
    
    for (char match:chars) {
        if (match==c) {
            _accept=true;
            _end=true;
            break;
        }
    }
}

void Group::step()
{
    _accept=false;
    _end=false;
}

void Boolean::start()
{
    if (stack[0]=='t') {
        path=true;
        _accept=true;
    }
    else {
        if (stack[0]=='f') {
            path=false;
            _accept=true;
        }
    }
}

void Boolean::step()
{
    
    if (!_accept) {
        return;
    }
        
    if (path) {
        if (cursor>3) {
            _accept=false;
            _end=false;
            return;
        }
        
        switch (cursor) {
            case 1:
                _accept=stack[1]=='r';
            break;
            
            case 2:
                _accept=stack[2]=='u';
            break;
            
            case 3:
                _accept=stack[3]=='e';
                _end=true;
            break;
            
        }
    }
    else {
        if (cursor>4) {
            _accept=false;
            _end=false;
            return;
        }
        
        switch (cursor) {
            case 1:
                _accept=stack[1]=='a';
            break;
            
            case 2:
                _accept=stack[2]=='l';
            break;
            
            case 3:
                _accept=stack[3]=='s';
            break;
            
            case 4:
                _accept=stack[4]=='e';
                _end=true;
            break;
            
        }
    }
}

bool Boolean::get_bool()
{
    if (value()=="true") {
        return true;
    }
    else {
        return false;
    }
}

void Integer::start()
{
    char c = stack[0];
    
    if (c=='+' or c=='-') {
        _accept=true;
    }
    
    if (c>='0' and c<='9') {
        _accept=true;
        _end=true;
    }
}

void Integer::step()
{
    if (!_accept) {
        return;
    }
    
    if (stack[cursor]>='0' and stack[cursor]<='9') {
        _accept=true;
    }
    else {
        _accept=false;
    }
    
    _end=_accept;
}

int Integer::get_int()
{
    return std::stoi(value());
}

Float::Float()
{
    is.imbue (std::locale("C"));
}

void Float::start()
{
    char c = stack[0];
    dot=false;
    
    if (c=='+' or c=='-') {
        _accept=true;
    }
    
    if (c>='0' and c<='9') {
        _accept=true;
    }
    
    if (c=='.') {
        _accept=true;
        dot=true;
    }

}

void Float::step()
{
    if (!_accept) {
        return;
    }
    
    char c = stack[cursor];
    
    if (dot) {
        if (c>='0' and c<='9') {
            _accept=true;
            _end=true;
        }
        else {
            _accept=false;
            _end=false;
        }
    }
    else {
        if (c=='.') {
            dot=true;
        }
        else {
            if (c>='0' and c<='9') {
                _accept=true;
            }
            else {
                _accept=false;
            }
        }
    }
    
}

float Float::get_float()
{
    float f;
    is.str(value());
    is>>f;
    
    return f;
}

void String::start()
{
    char c = stack[0];
    
    if (c=='"') {
        _accept=true;
    }
}

void String::step()
{
    if (!_accept) {
        return;
    }
    
    // we are no longer accepting  chars
    if (_end) {
        _accept=false;
        _end=false;
        return;
    }
    
    char c = stack[cursor];
    
    if (c=='"') {
        if (stack[cursor-1]!='\\') {
            _end=true;
        }
    }
}

string String::get_string()
{
    string tmp;
    
    //TODO: check for size
    tmp=value();
    
    return tmp.substr(1,tmp.size()-2);
}
