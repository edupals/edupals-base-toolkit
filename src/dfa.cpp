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

#include <limits>

using namespace edupals::parser;
using namespace std;

DFA::DFA()
{
}

DFA::~DFA()
{
}

void DFA::reset()
{
    last=0;
    cursor= std::numeric_limits<size_t>::max();
    _accept=false;
    _end=false;
    _valid=true;
    stack.clear();
}

string DFA::value()
{
    return stack.substr(0,last+1);
}

size_t DFA::size()
{
    return stack.size();
}
