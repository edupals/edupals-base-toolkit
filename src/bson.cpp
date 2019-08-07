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

static string load_name(istream& stream);
static uint8_t load_element(Variant& parent,istream& stream);
static Variant load_document(istream& stream);

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

void edupals::bson::dump(ostream& stream,Variant& value)
{
    dump_document(value,stream);
}

static int32_t read_i32(istream& stream)
{
    int32_t value;
    stream.read((char*)&value,4);
    
    if (!stream) {
        throw bson::exception::UnexpectedEOF();
    }
    
    return value;
}

static int8_t read_i8(istream& stream)
{
    int8_t value;
    stream.read((char*)&value,1);
    
    if (!stream) {
        throw bson::exception::UnexpectedEOF();
    }
    
    return value;
}

static double read_double(istream& stream)
{
    double value;
    stream.read((char*)&value,8);
    
    if (!stream) {
        throw bson::exception::UnexpectedEOF();
    }
    
    return value;
}

static string load_name(istream& stream)
{
    string ret;
    char c;
    
    c = read_i8(stream);
    
    while(c!=0x00) {
        ret+=c;
        c = read_i8(stream);
    }
    
    return ret;
}

static uint8_t load_element(Variant& parent,istream& stream)
{
    uint8_t u8=0;
    string name;
    double f64;
    int32_t i32;
    string str;
    Variant tmp1;
    Variant tmp2;
    vector<string> keys;
    
    u8 = read_i8(stream);
    
    switch (u8) {
        
        /* double */
        case 0x01:
            name=load_name(stream);
            f64 = read_double(stream);
            parent[name]=f64;
        break;
        
        /* string */
        case 0x02:
            name=load_name(stream);
            i32 = read_i32(stream);
            str="";
            
            if (i32>0) {
                for (int n=0;n<(i32-1);n++) {
                    str+= read_i8(stream);
                }
            }
            
            u8 = read_i8(stream);
            // check for 0x00
            
            parent[name]=str;
            
        break;
        
        /* document */
        case 0x03:
            name=load_name(stream);
            parent[name]=load_document(stream);
        break;
        
        /* array */
        case 0x04:
            name=load_name(stream);
            //load array as a document
            tmp1=load_document(stream);
            
            //convert it to array
            keys=tmp1.keys();
            
            tmp2=Variant::create_array(keys.size());
            for (string& key:keys) {
                int index = std::stoi(key);
                tmp2[index]=tmp1[key];
            }
            
            parent[name]=tmp2;
            
        break;
        
        /* bool */
        case 0x08:
            name=load_name(stream);
            u8 = read_i8(stream);
            parent[name]=(bool)u8;
        break;
        
        /* int32 */
        case 0x10:
            name=load_name(stream);
            i32 = read_i32(stream);
            parent[name]=i32;
        break;
    }
    
    return u8;
}

static Variant load_document(istream& stream)
{
    int32_t i32;
    
    Variant document=Variant::create_struct();
    
    i32 = read_i32(stream);
    
    while (load_element(document,stream)>0) {
    }
    
    return document;
}

Variant edupals::bson::load(istream& stream)
{
    Variant ret;
    
    ret = load_document(stream);
    
    return ret;
}
