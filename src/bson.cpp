/*
 * Copyright (C) 2019 Edupals project
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

#include "bson.hpp"

#include <iostream>

using namespace edupals::variant;
using namespace std;

static void dump_document(Variant& value,ostream& stream)
{
    if (value.type()!=Type::Struct) {
        //throw exception here
        cerr<<"Not a struct value!"<<endl;
        return;
    }
    
    //we don't know size yet
    streampos future_size=stream.tellg();
    
}

void edupals::bson::dump(Variant& value,ostream& stream)
{
    dump_document(value,stream);
}

Variant edupals::bson::load(istream& stream)
{
}
