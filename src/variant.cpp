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


#include "variant.hpp"

using namespace edupals;
using namespace std;

VariantContainerArray::VariantContainerArray(vector<Variant> value)
{
    this->type=VariantType::Array;
    this->count=value.size();
    this->value = new Variant[count];
    
    for (int n=0;n<value.size();n++) {
        this->value[n]=value[n];
    }
}

VariantContainerArray::~VariantContainerArray()
{
    delete [] value;
}

size_t VariantContainerArray::size()
{
    size_t total=0;
    for (int n=0;n<count;n++) {
        total+=value[n].size();
    }
    return total;
}

Variant::Variant()
{
    
}

Variant::Variant(int value)
{
    data.reset(new VariantContainerInt32(value));
}

Variant::Variant(float value)
{
    data.reset(new VariantContainerFloat(value));
}

Variant::Variant(double value)
{
    data.reset(new VariantContainerDouble(value));
}

Variant::Variant(string value)
{
    data.reset(new VariantContainerString(value));
}

Variant::Variant(const char* value)
{
    data.reset(new VariantContainerString(string(value)));
}

Variant::Variant(vector<Variant> value)
{
    data.reset(new VariantContainerArray(value));
}

Variant::~Variant()
{
}

size_t Variant::size()
{
    if (data) {
        return (*data).size();
    }
    else {
        return 0;
    }
}

int32_t Variant::get_int32()
{
    if (!data) {
        throw VariantException::Unitialized;
    }
    
    if ((*data).type!=VariantType::Int32) {
        throw VariantException::InvalidType;
    }
    
    VariantContainerInt32* cast=static_cast<VariantContainerInt32*>(data.get());
    
    return cast->value;
}

float Variant::get_float()
{
    if (!data) {
        throw VariantException::Unitialized;
    }
    
    if ((*data).type!=VariantType::Float) {
        throw VariantException::InvalidType;
    }
    
    VariantContainerFloat* cast=static_cast<VariantContainerFloat*>(data.get());
    
    return cast->value;
}

double Variant::get_double()
{
    if (!data) {
        throw VariantException::Unitialized;
    }
    
    if ((*data).type!=VariantType::Double) {
        throw VariantException::InvalidType;
    }
    
    VariantContainerDouble* cast=static_cast<VariantContainerDouble*>(data.get());
    
    return cast->value;
}

string Variant::get_string()
{
    if (!data) {
        throw VariantException::Unitialized;
    }
    
    if ((*data).type!=VariantType::String) {
        throw VariantException::InvalidType;
    }
    
    VariantContainerString* cast=static_cast<VariantContainerString*>(data.get());
    
    return cast->value;
}

Variant& Variant::operator=(int value)
{
    data.reset(new VariantContainerInt32(value));
    return *this;
}

Variant& Variant::operator=(float value)
{
    data.reset(new VariantContainerFloat(value));
    return *this;
}

Variant& Variant::operator=(double value)
{
    data.reset(new VariantContainerDouble(value));
    return *this;
}

Variant& Variant::operator=(string value)
{
    data.reset(new VariantContainerString(value));
    return *this;
}

Variant& Variant::operator=(const char* value)
{
    data.reset(new VariantContainerString(string(value)));
    return *this;
}

Variant& Variant::operator[](const int index)
{
    //ToDo: checkings
    
    VariantContainerArray* cast = static_cast<VariantContainerArray*>(data.get());
    
    return cast->value[index];
}
