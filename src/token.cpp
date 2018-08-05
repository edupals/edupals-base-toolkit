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

void Word::step()
{
    if (cursor==match.size()) {
        _accept=false;
    }
    else {
        if (cursor==0) {
            _accept = (match[cursor]==stack[cursor]);
        }
        else {
            _accept = _accept and (match[cursor]==stack[cursor]);
        }
    }
    
    if (_accept) {
        last=cursor;
        _end = cursor==match.size()-1;
    }
}

Char::Char(char match)
{
    this->match=match;
}

void Char::step()
{
    if (cursor==0) {
        if (stack[0]==match) {
            _accept=true;
            _end=true;
            last=0;
        }
    }
    else {
        _accept=false;
    }
}

void Bool::step()
{
    if (cursor==0) {
        if (stack[0]=='t') {
            path=true;
            _accept=true;
        }
        else {
            if (stack[0]=='f') {
                path=false;
                _accept=true;
            }
            else {
                _accept=false;
            }
        }
    }
    else {
    
        if (!_accept) {
            return;
        }
        
        if (path) {
            if (cursor>3) {
                _accept=false;
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
                    last=3;
                break;
                
            }
        }
        else {
            if (cursor>4) {
                _accept=false;
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
                    last=4;
                break;
                
            }

        }
    }
}