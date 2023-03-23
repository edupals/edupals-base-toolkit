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

#include <iostream>

using namespace edupals;
using namespace edupals::variant;
using namespace std;

static size_t dump_name(string& name,ostream& stream);
static size_t dump_element(string& key,Variant& value,ostream& stream);
static size_t dump_document(Variant& value,ostream& stream);

static string load_name(istream& stream);
static uint8_t load_element(string& name, Variant& value,istream& stream);
static Variant load_document(istream& stream);
static Variant load_array(istream& stream);

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
    int64_t i64;
    double f64;
    
    switch (value.type()) {
        
        case Type::Float:
            stream.put(0x01);
            size=1;
            size+=dump_name(key,stream);
            f64=(double)value.get_float();
            stream.write((char*)&f64,8);
            size+=8;
        break;

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

        case Type::Int64:
            stream.put(0x12);
            size=1;
            size+=dump_name(key,stream);
            i64=value.get_int64();
            stream.write((char*)&i64,8);
            size+=8;
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
    
    /*
        jump 4 bytes
        old: stream.seekp(4,ios_base::cur);

        As we may not jump forward on stringstream based ostream, it is better to just zeroed
        and come back later
    */
    stream.put(0x00);
    stream.put(0x00);
    stream.put(0x00);
    stream.put(0x00);
    
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

static int64_t read_i64(istream& stream)
{
    int64_t value;
    stream.read((char*)&value,8);

    if (!stream) {
        throw bson::exception::UnexpectedEOF();
    }

    return value;
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

static uint8_t load_element(string& name, Variant& value,istream& stream)
{
    uint8_t u8;
    double f64;
    int32_t i32;
    int64_t i64;
    string str;
    Variant tmp1;
    Variant tmp2;
    vector<string> keys;
    
    uint8_t btype = 0;

    btype = read_i8(stream);

    if (btype == 0) {
        return 0;
    }

    name = load_name(stream);

    switch (btype) {

        /* double */
        case 0x01:
            f64 = read_double(stream);
            value = f64;
        break;
        
        /* string */
        case 0x02:
            i32 = read_i32(stream);
            str="";
            
            if (i32>0) {
                for (int n=0;n<(i32-1);n++) {
                    str+= read_i8(stream);
                }
            }
            
            u8 = read_i8(stream);
            // check for 0x00
            
            value = str;
            
        break;
        
        /* document */
        case 0x03:
            value = load_document(stream);
        break;
        
        /* array */
        case 0x04:
            value = load_array(stream);
        break;
        
        /* bool */
        case 0x08:
            u8 = read_i8(stream);
            value = (bool)u8;
        break;
        
        /* int32 */
        case 0x10:
            i32 = read_i32(stream);
            value = (int32_t) i32;
        break;

        /* int64 */
        case 0x12:
            i64 = read_i64(stream);
            value = (int64_t) i64;
        break;
    }
    
    return btype;
}

static Variant load_document(istream& stream)
{
    int32_t i32;
    
    Variant document=Variant::create_struct();
    
    i32 = read_i32(stream);
    string name;
    Variant value;

    while (load_element(name,value,stream)>0) {

        document[name] = value;
    }

    return document;
}

static Variant load_array(istream& stream)
{
    int32_t i32;

    Variant array = Variant::create_array(0);

    i32 = read_i32(stream);
    string name;
    Variant value;

    while (load_element(name,value,stream)>0) {

        array.append(value);
    }

    return array;
}

Variant edupals::bson::load(istream& stream)
{
    Variant ret;
    
    ret = load_document(stream);

    return ret;
}
