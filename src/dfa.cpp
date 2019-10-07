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
 

#include <dfa.hpp>

#include <stdexcept>
#include <cstring>

using namespace edupals::parser;
using namespace std;

DFA::DFA()
{
    capacity = 128;
    stack = new int8_t[capacity];
}

DFA::~DFA()
{
    delete [] stack;
}

void DFA::push(int8_t c)
{
    if (cursor==capacity) {
        size_t new_capacity=capacity*1.5f;
        int8_t* tmp = new int8_t[new_capacity];
        
        std::memcpy(tmp,stack,capacity);
        
        delete [] stack;
        stack=tmp;
        capacity=new_capacity;
    }
    
    cursor++;
    stack[cursor]=c;

    if (cursor==0) {
        start();
    }
    else {
        step();
    }
    
    if (_end) {
        last=cursor;
    }
}

void DFA::reset()
{
    last=0;
    cursor=-1;
    _accept=false;
    _end=false;
}

bool DFA::accept()
{
    return _accept;
}

bool DFA::end()
{
    return _end;
}

void DFA::start()
{
}

void DFA::step()
{
}

string DFA::value()
{
    return string((const char*)stack,last+1);
}

size_t DFA::size()
{
    return cursor+1;
}
