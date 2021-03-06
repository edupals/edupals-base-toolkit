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

void container::Bytes::serialize(ostream& stream)
{
    stream<<"bytes (size "<<size()<<")";
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

void container::Array::serialize(ostream& stream)
{
    stream<<'[';
    for (size_t n=0;n<value.size();n++) {
        value[n].serialize(stream);
        if (n!=value.size()-1) {
            stream<<',';
        }
    }
    stream<<']';
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

void container::Struct::serialize(ostream& stream)
{
    stream<<'{';
    
    map<string,Variant>::iterator it;
    
    it=value.begin();
    
    while (it!=value.end()) {
        stream<<it->first<<":";
        it->second.serialize(stream);
        
        it++;
        
        if (it!=value.end()) {
            stream<<",";
        }
    }
    stream<<'}';
}

Variant::Variant()
{
    
}

Variant::Variant(bool value)
{
    data.reset(new container::Boolean(value));
}

Variant::Variant(int32_t value)
{
    data.reset(new container::Int32(value));
}

Variant::Variant(int64_t value)
{
    data.reset(new container::Int64(value));
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

bool Variant::none()
{
    if (data) {
        return (data.get()->type==Type::None);
    }
    
    return true;
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

Variant Variant::find(string key)
{
    if (data) {
        if ((*data).type==variant::Type::Struct) {
            container::Struct* cast = static_cast<container::Struct*>(data.get());
            map<string ,Variant>::iterator it = cast->value.find(key);
            if (it!=cast->value.end()) {
                return cast->value[string(key)];
            }
        }
    }
    
    return Variant();
}

Variant Variant::find(int index)
{
    if (data) {
        if ((*data).type==variant::Type::Array) {
            container::Array* cast = static_cast<container::Array*>(data.get());
            
            if (index<cast->value.size()) {
                return cast->value[index];
            }
        }
    }
    
    return Variant();
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

void Variant::serialize(ostream& stream)
{
    if (data) {
        data->serialize(stream);
    }
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

int64_t Variant::get_int64()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Int64) {
        throw variant::exception::InvalidType();
    }
    
    container::Int64* cast=static_cast<container::Int64*>(data.get());

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

vector<Variant>& Variant::get_array()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Array) {
        throw variant::exception::InvalidType();
    }
    
    container::Array* cast=static_cast<container::Array*>(data.get());
    
    return cast->value;
}

map<string,Variant>& Variant::get_struct()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    if ((*data).type!=variant::Type::Struct) {
        throw variant::exception::InvalidType();
    }
    
    container::Struct* cast=static_cast<container::Struct*>(data.get());
    
    return cast->value;
}

Variant& Variant::operator=(bool value)
{
    data.reset(new container::Boolean(value));
    return *this;
}

Variant& Variant::operator=(int32_t value)
{
    data.reset(new container::Int32(value));
    return *this;
}

Variant& Variant::operator=(int64_t value)
{
    data.reset(new container::Int64(value));
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

Variant& Variant::operator[](char* key)
{
    return get_value_from_key(string(key));
}

Variant& Variant::operator[](string key)
{
    return get_value_from_key(key);
}

Variant& Variant::operator/(string key)
{
    
    if (!data) {
        throw variant::exception::NotFound();
    }
    
    if ((*data).type!=variant::Type::Struct) {
        throw variant::exception::NotFound();
    }
    
    container::Struct* cast = static_cast<container::Struct*>(data.get());
    
    map<string ,Variant>::iterator it = cast->value.find(key);
    
    if (it==cast->value.end()) {
        throw exception::NotFound();
    }
    
    return cast->value[key];
}

Variant& Variant::operator/(int index)
{
    
    if (!data) {
        throw variant::exception::NotFound();
    }
    
    if ((*data).type!=variant::Type::Array) {
        throw variant::exception::NotFound();
    }
    
    container::Array* cast = static_cast<container::Array*>(data.get());
    
    if (index>=cast->value.size()) {
        throw exception::NotFound();
    }
    
    return cast->value[index];
}

Variant& Variant::operator/(variant::Type type)
{
    if (this->type()!=type) {
        throw exception::NotFound();
    }
    
    return *this;
}

bool Variant::to_boolean()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    Type type=(*data).type;
    
    switch(type) {
        
        case variant::Type::Boolean:
            return get_boolean();
        break;
        
        case variant::Type::Int32:
            return ((bool)get_int32());
        break;
        
        case variant::Type::Int64:
            return ((bool)get_int64());
        break;
        
        default:
            throw variant::exception::InvalidType();
    }
    
}

int32_t Variant::to_int32()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    Type type=(*data).type;
    
    switch(type) {
        
        case variant::Type::Boolean:
            return ((int32_t)get_boolean());
        break;
        
        case variant::Type::Int32:
            return get_int32();
        break;
        
        case variant::Type::Int64:
            return ((int32_t)get_int64());
        break;
        
        case variant::Type::Float:
            return ((int32_t)get_float());
        break;
        
        case variant::Type::Double:
            return ((int32_t)get_double());
        break;
        
        default:
            throw variant::exception::InvalidType();
    }
    
}

float Variant::to_float()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    Type type=(*data).type;
    
    switch (type) {
        
        case variant::Type::Boolean:
            return ((float)get_boolean());
        break;
        
        case variant::Type::Int32:
            return ((float)get_int32());
        break;
        
        case variant::Type::Int64:
            return ((float)get_int64());
        break;
        
        case variant::Type::Float:
            return get_float();
        break;
        
        case variant::Type::Double:
            return ((float)get_double());
        break;
        
        default:
            throw variant::exception::InvalidType();
    }
}

double Variant::to_double()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    Type type=(*data).type;
    
    switch (type) {
        
        case variant::Type::Boolean:
            return ((double)get_boolean());
        break;
        
        case variant::Type::Int32:
            return ((double)get_int32());
        break;
        
        case variant::Type::Int64:
            return ((double)get_int64());
        break;
        
        case variant::Type::Float:
            return ((double)get_float());
        break;
        
        case variant::Type::Double:
            return get_double();
        break;
        
        default:
            throw variant::exception::InvalidType();
    }
}

int64_t Variant::to_int64()
{
    if (!data) {
        throw variant::exception::Unitialized();
    }
    
    Type type=(*data).type;
    
    switch(type) {
        
        case variant::Type::Boolean:
            return ((int64_t)get_boolean());
        break;
        
        case variant::Type::Int32:
            return ((int64_t)get_int32());
        break;
        
        case variant::Type::Int64:
            return get_int64();
        break;
        
        case variant::Type::Float:
            return ((int64_t)get_float());
        break;
        
        case variant::Type::Double:
            return ((int64_t)get_double());
        break;
        
        default:
            throw variant::exception::InvalidType();
    }
}

std::ostream& edupals::variant::operator<<(std::ostream& os, Variant& v)
{
    v.serialize(os);
    
    return os;
}
