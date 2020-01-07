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


#include "json.hpp"
#include "lexer.hpp"
#include "token.hpp"

#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace edupals::variant;
using namespace edupals::parser;
using namespace edupals::json;
using namespace edupals::json::exception;
using namespace std;

grammar::Parser::Parser()
{
    ws = new token::Group ({' ','\t','\n','\r'});
    lb = new token::Char('[');
    rb = new token::Char(']');
    lc = new token::Char('{');
    rc = new token::Char('}');
    colon = new token::Char(':');
    comma = new token::Char(',');
    float_num = new token::Float();
    int_num = new token::Integer();
    null = new token::Word("null");
    str = new token::String();
    boolean = new token::Boolean();
    
    lexer.add_token("WS",ws);
    lexer.add_token("LEFT_BRACKET",lb);
    lexer.add_token("RIGHT_BRACKET",rb);
    lexer.add_token("LEFT_CURLY",lc);
    lexer.add_token("RIGHT_CURLY",rc);
    lexer.add_token("COLON",colon);
    lexer.add_token("COMMA",comma);
    
    lexer.add_token("FLOAT",float_num);
    lexer.add_token("INTEGER",int_num);
    lexer.add_token("BOOLEAN",boolean);
    
    lexer.add_token("NULL",null);
    lexer.add_token("STRING",str);
}

grammar::Parser::~Parser()
{
    delete ws;
    delete lb;
    delete rb;
    delete lc;
    delete rc;
    delete colon;
    delete comma;
    delete float_num;
    delete int_num;
    delete null;
    delete str;
    delete boolean;
}

bool grammar::Parser::is_value(parser::DFA* token)
{
    if (token==float_num or token==int_num or token==str or token==boolean or token==null) {
        return true;
    }
    
    return false;
}

Variant grammar::Parser::get_value(parser::DFA* token)
{
    Variant tmp;
    
    if (token==float_num) {
        tmp=static_cast<token::Float*>(token)->get_float();
    }
    
    if (token==int_num) {
        tmp=static_cast<token::Integer*>(token)->get_int();
    }
    
    if (token==str) {
        tmp=static_cast<token::String*>(token)->get_string();
    }
    
    if (token==boolean) {
        tmp=static_cast<token::Boolean*>(token)->get_bool();
    }
    
    if (token==null) {
        //nothing to do
    }
    
    return tmp;
}

void grammar::Parser::push(ProductionType type)
{
    Production prod;
    
    prod.type=type;
    prod.down=false;
    
    stack.push_back(prod);
}

void grammar::Parser::pop()
{
    last=stack.back();
    stack.pop_back();
    stack.back().down=true;
}

void grammar::Parser::step(parser::DFA* token)
{
    
    /* ignore whitespaces */
    if (token==ws) {
        return;
    }
    
    Production& top = stack.back();
    /*
    clog<<"production: "<<static_cast<int>(top.type)<<endl;
    clog<<"token: "<<lexer.get_token()<<endl;
    clog<<"value: "<<token->value()<<endl<<endl;
    */
    if (top.type==ProductionType::Object0) {
        
        if (token==lc) {
            top.value=Variant::create_struct();
            top.type=ProductionType::Object1;
            
            return;
        }
        
        throw SyntaxError("Expected { but got "+token->value());
    }
    
    if (top.type==ProductionType::Object1) {
        
        if (token==rc) {
            pop();
            return;
        }
        
        if (token==str) {
            top.key=static_cast<token::String*>(token)->get_string();
            top.type=ProductionType::Object2;
            return;
        }
        
        throw SyntaxError("Expected } or string but got "+token->value());
    }
    
    if (top.type==ProductionType::Object2) {
        
        if (token==colon) {
            top.type=ProductionType::Object3;
            return;
        }
        
        throw SyntaxError("Expected : but got "+token->value());
    }
    
    if (top.type==ProductionType::Object3) {
        if (is_value(token)) {
            last.value=get_value(token);
            top.type=ProductionType::Object4;
            return;
        }
        
        if (token==lc) {
            top.type=ProductionType::Object4;
            push(ProductionType::Object1);
            stack.back().value=Variant::create_struct();
            
            return;
        }
        
        if (token==lb) {
            top.type=ProductionType::Object4;
            push(ProductionType::Array0);
            stack.back().value=Variant::create_array(0);
            
            return;
        }
        
        throw SyntaxError("Expected value but got "+token->value());
    }
    
    if (top.type==ProductionType::Object4) {
        
        if (token==comma) {
            top.value[top.key]=last.value;
            top.type=ProductionType::Object1;
            return;
        }
        
        if (token==rc) {
            top.value[top.key]=last.value;
            pop();
            return;
        }
        
        throw SyntaxError("Expected } or , but got "+token->value());
    }
    
    if (top.type==ProductionType::Array0) {
        
        if (is_value(token)) {
            last.value=get_value(token);
            top.type=ProductionType::Array1;
            return;
        }
        
        if (token==rb) {
            pop();
            
            return;
        }
        
        if (token==lc) {
            top.type=ProductionType::Array1;
            push(ProductionType::Object1);
            stack.back().value=Variant::create_struct();
            
            return;
        }
        
        if (token==lb) {
            top.type=ProductionType::Array1;
            push(ProductionType::Array0);
            stack.back().value=Variant::create_array(0);
            
            return;
        }
        
        throw SyntaxError("Expected ] or value but got "+token->value());
    }
    
    if (top.type==ProductionType::Array1) {
        if (token==comma) {
            top.value.append(last.value);
            top.type=ProductionType::Array0;
            
            return;
        }
        
        if (token==rb) {
            top.value.append(last.value);
            pop();
            
            return;
        }
        
        throw SyntaxError("Expected , or ] but got "+token->value());
    }
    
}

Variant grammar::Parser::parse(istream& stream)
{
    lexer.set_input(&stream);
    
    stack.clear();
    
    push(ProductionType::Object0);
    
    while (lexer.step()) {
        DFA* dfa = lexer.get_dfa();
        step(dfa);
    }
    
    if (!lexer.eof()) {
        throw runtime_error("Unknown token");
    }
    else {
        if (lexer.pending()) {
            throw runtime_error("Unexpected EOF");
        }
    }
    
    return last.value;
}

void edupals::json::dump(Variant& value,ostream& stream)
{
    vector<string> keys;
    size_t count;
    
    std::ios_base::fmtflags flags(stream.flags());
    stream.imbue(std::locale("C"));
    stream<<std::setprecision(10)<<std::fixed;
    
    switch(value.type()) {
        case Type::Struct:
            stream<<"{";
            
            keys=value.keys();
        
            for (int n=0;n<keys.size();n++) {
                stream<<"\""<<keys[n]<<"\""<<":";
                
                Variant& child=value[keys[n]];
                edupals::json::dump(child,stream);
                
                if (n<keys.size()-1) {
                    stream<<",";
                }
            }
            stream<<"}";
        break;
        
        case Type::Array:
            stream<<"[";
        
            count = value.count();
            for (int n=0;n<count;n++) {
                
                Variant& child=value[n];
                edupals::json::dump(child,stream);
                
                if (n<count-1) {
                    stream<<",";
                }
            }
            stream<<"]";
        break;
        
        case Type::Int32:
            stream<<value.get_int32();
        break;
        
        case Type::Float:
            stream<<value.get_float();
        break;
        
        case Type::Double:
            stream<<value.get_double();
        break;
        
        case Type::String:
            stream<<"\""<<value.get_string()<<"\"";
        break;
        
        case Type::Boolean:
            if (value.get_boolean()) {
                stream<<"true";
            }
            else {
                stream<<"false";
            }
        break;
    }
    
    stream.flags(flags);
}
    
Variant edupals::json::load(istream& stream)
{
    grammar::Parser parser;
    
    return parser.parse(stream);
}
