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


using namespace edupals::variant;
using namespace std;

container::Bytes::Bytes(vector<uint8_t>& value)
{
    this->type=Type::Bytes;
    
    this->value=value;
}

container::Bytes::Bytes(uint8_t* values,size_t size)
{
    this->type=Type::Bytes;
    
    for (int n=0;n<size;n++) {
        this->value.push_back(value[n]);
    }
}

container::Array::Array(vector<Variant>& value)
{
    this->type=Type::Array;
    this->value=value;
}

container::Array::Array(size_t count)
{
    this->type=Type::Array;
    this->value.reserve(count);
    
    for (int n=0;n<count;n++) {
        this->value.push_back(Variant());
    }
}

container::Array::~Array()
{
    
}

size_t container::Array::size()
{
    size_t total=0;
    for (int n=0;n<value.size();n++) {
        total+=value[n].size();
    }
    return total;
}

size_t container::Array::count()
{
    return value.size();
}

container::Struct::Struct()
{
     this->type=Type::Struct;
}

size_t container::Struct::size()
{
    size_t total=0;
    
    for (std::pair<string,Variant>q:value) {
        total+=q.second.size();
    }
    
    return total;
}

Variant::Variant()
{
    
}

Variant::Variant(bool value)
{
    data.reset(new container::Boolean(value));
}

Variant::Variant(int value)
{
    data.reset(new container::Int32(value));
}

Variant::Variant(float value)
{
    data.reset(new container::Float(value));
}

Variant::Variant(double value)
{
    data.reset(new container::Double(value));
}

Variant::Variant(string value)
{
    data.reset(new container::String(value));
}

Variant::Variant(const char* value)
{
    data.reset(new container::String(string(value)));
}

Variant::Variant(vector<Variant>& value)
{
    data.reset(new container::Array(value));
}

Variant::Variant(vector<uint8_t>& value)
{
    data.reset(new container::Bytes(value));
}

Variant::Variant(uint8_t* value,size_t size)
{
    data.reset(new container::Bytes(value,size));
}

Variant::Variant(std::initializer_list<Variant> list)
{
    data.reset(new container::Array(0));
    
    for (Variant v:list) {
        append(v);
    }
}

Variant::~Variant()
{
}

Variant Variant::create_array(size_t count)
{
    Variant value;
    
    value.data.reset(new container::Array(count));
    
    return value;
}

size_t Variant::count()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Array) {
        throw variant::exception::InvalidType();
    }
    
    container::Array* cast = static_cast<container::Array*>(data.get());
    
    return cast->value.size();
}

Variant Variant::create_struct()
{
    Variant value;
    
    value.data.reset(new container::Struct());
    
    return value;
}

vector<string> Variant::keys()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Struct) {
        throw variant::exception::InvalidType();
    }
    
    container::Struct* cast = static_cast<container::Struct*>(data.get());
    
    vector<string> ks;
    
    for (auto k:cast->value) {
        ks.push_back(k.first);
    }
    
    return ks;
}

void Variant::append()
{
    append(Variant());
}

void Variant::append(Variant value)
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Array) {
        throw variant::exception::InvalidType();
    }
    
    container::Array* cast = static_cast<container::Array*>(data.get());
    
    cast->value.push_back(value);
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

Type Variant::type()
{
    if (data) {
        return data.get()->type;
    }
    
    return Type::None;
}

bool Variant::get_boolean()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Boolean) {
        throw variant::exception::InvalidType();
    }
    
    container::Boolean* cast=static_cast<container::Boolean*>(data.get());

    return cast->value;
}

int32_t Variant::get_int32()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Int32) {
        throw variant::exception::InvalidType();
    }
    
    container::Int32* cast=static_cast<container::Int32*>(data.get());

    return cast->value;
}

float Variant::get_float()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Float) {
        throw variant::exception::InvalidType();
    }
    
    container::Float* cast=static_cast<container::Float*>(data.get());

    return cast->value;
}

double Variant::get_double()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Double) {
        throw variant::exception::InvalidType();
    }
    
    container::Double* cast=static_cast<container::Double*>(data.get());

    return cast->value;
}

string Variant::get_string()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::String) {
        throw variant::exception::InvalidType();
    }
    
    container::String* cast=static_cast<container::String*>(data.get());

    return cast->value;
}

vector<uint8_t>& Variant::get_bytes()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Bytes) {
        throw variant::exception::InvalidType();
    }
    
    container::Bytes* cast=static_cast<container::Bytes*>(data.get());

    return cast->value;
}

Variant& Variant::operator=(bool value)
{
    data.reset(new container::Boolean(value));
    return *this;
}

Variant& Variant::operator=(int value)
{
    data.reset(new container::Int32(value));
    return *this;
}

Variant& Variant::operator=(float value)
{
    data.reset(new container::Float(value));
    return *this;
}

Variant& Variant::operator=(double value)
{
    data.reset(new container::Double(value));
    return *this;
}

Variant& Variant::operator=(string value)
{
    data.reset(new container::String(value));
    return *this;
}

Variant& Variant::operator=(const char* value)
{
    data.reset(new container::String(string(value)));
    return *this;
}

Variant& Variant::operator=(vector<uint8_t>& value)
{
    data.reset(new container::Bytes(value));
    return *this;
}

Variant& Variant::operator[](const int index)
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Array) {
        throw variant::exception::InvalidType();
    }
    
    container::Array* cast = static_cast<container::Array*>(data.get());

    if (index<0 or index>=cast->count()) {
        throw variant::exception::OutOfBounds();
    }
    
    return cast->value[index];
}

Variant& Variant::get_value_from_key(string key)
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Struct) {
        throw variant::exception::InvalidType();
    }
    
    container::Struct* cast = static_cast<container::Struct*>(data.get());
    
    map<string ,Variant>::iterator it = cast->value.find(string(key));
    if (it==cast->value.end()) {
        cast->value[string(key)]=Variant();
    }
    
    return cast->value[key];
}

Variant& Variant::operator[](const char* key)
{
    return get_value_from_key(string(key));
}

Variant& Variant::operator[](string key)
{
    return get_value_from_key(key);
}

std::ostream& edupals::variant::operator<<(std::ostream& os, Variant& v)
{
    switch(v.type()) {
        case variant::Type::Boolean:
            if (v.get_boolean()) {
                os<<"true";
            }
            else {
                os<<"false";
            }
        break;
        
        case variant::Type::Int32:
            os<<v.get_int32();
        break;
        
        case variant::Type::Float:
            os<<v.get_float();
        break;
        
        case variant::Type::Double:
            os<<v.get_double();
        break;
        
        case variant::Type::Bytes:
            os<<"bytes (size "<<v.get_bytes().size()<<")";
        break;
        
        case variant::Type::String:
            os<<"\'"<<v.get_string()<<"\'";
        break;
        
        case variant::Type::Array:
            os<<'[';
            for (size_t n=0;n<v.count();n++) {
                os<<v[n];
                if (n!=v.count()-1) {
                    os<<',';
                }
            }
            os<<']';
        break;
        
        case variant::Type::Struct:
            os<<'{';
            const vector<string> keys = v.keys();
            int last=keys.size()-1;
            for (size_t n=0;n<keys.size();n++) {
                os<<keys[n];
                os<<':';
                os<<v[keys[n]];
                if (n!=last) {
                    os<<',';
                }
            }
            os<<'}';
        break;
        
    }
    
    return os;
}
