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
#include <vector>
#include <string>

using namespace edupals;
using namespace edupals::variant;
using namespace std;

static size_t dump_name(string& name,ostream& stream);
static size_t dump_element(string& key,Variant& value,ostream& stream);
static size_t dump_document(Variant& value,ostream& stream);

static size_t dump_name(string& name,ostream& stream)
{
    for (char c:name) {
        stream.put(c);
    }
    stream.put(0x00);
    
    return (1+name.size());
}

static size_t dump_element(string& key,Variant& value,ostream& stream)
{
    size_t size=0;
    int32_t i32;
    double f64;
    
    switch (value.type()) {
        
        case Type::Double:
            stream.put(0x01);
            size=1;
            size+=dump_name(key,stream);
            f64=value.get_double();
            stream.write((char*)&f64,8);
            size+=8;
        break;
            
        case Type::String:
            stream.put(0x02);
            size=1;
            size+=dump_name(key,stream);
            i32=1+value.get_string().size();
            stream.write((char*)&i32,4);
            size+=4;
            size+=i32;
            for (char c:value.get_string()) {
                stream.put(c);
            }
            stream.put(0x00);
            
        break;
        
        case Type::Struct:
            stream.put(0x03);
            size=1;
            size+=dump_name(key,stream);
            size+=dump_document(value,stream);
        break;
        
        case Type::Array:
            stream.put(0x04);
            size=1;
            size+=dump_name(key,stream);
            size+=dump_document(value,stream);
        break;
        
        case Type::Boolean:
            stream.put(0x08);
            size=1;
            size+=dump_name(key,stream);
            if (value.get_boolean()) {
                stream.put(0x01);
            }
            else {
                stream.put(0x00);
            }
            size+=1;
        break;
        
        case Type::Int32:
            stream.put(0x10);
            size=1;
            size+=dump_name(key,stream);
            i32=value.get_int32();
            stream.write((char*)&i32,4);
            size+=4;
        break;
        
        default:
            throw bson::exception::UnsupportedExport(value.type());
        break;
    }
    
    return size;
}

static size_t dump_document(Variant& value,ostream& stream)
{
    bool is_array=false;
    
    if (value.type()==Type::Array) {
        is_array=true;
    }
    else {
        if (value.type()!=Type::Struct) {
            //throw a more specific exception
            throw bson::exception::UnsupportedExport(value.type());
        }
    }
    
    //we don't know size yet
    streampos header=stream.tellp();
    
    size_t size=4;
    
    //jump 4 bytes
    stream.seekp(4,ios_base::cur);
    
    if (is_array) {
        for (int n=0;n<value.count();n++) {
            string key=std::to_string(n);
            size+=dump_element(key,value[n],stream);
        }
    }
    else {
        for (string& key:value.keys()) {
            size+=dump_element(key,value[key],stream);
        }
    }
    
    //end character
    stream.put(0x00);
    size+=1;
    
    streampos end=stream.tellp();
    stream.seekp(header);
    
    stream.write((char*)&size,4);
    stream.seekp(end);
    
    return size;
}

void edupals::bson::dump(Variant& value,ostream& stream)
{
    dump_document(value,stream);
}

Variant edupals::bson::load(istream& stream)
{
    Variant ret;
    
    return ret;
}
