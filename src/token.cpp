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

Word::Word(string match)
{
    this->match=match;
}

void Word::step()
{
    if (cursor>match.size()) {
        _accept=false;
    }
    else {
        if (cursor==1) {
            _accept = (match[]==stack[cursor]);
        }
        else {
            _accept = _accept and (match[]==stack[cursor]);
        }
    }
    
    if (accept) {
        last=cursor;
    }
}
