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
using namespace std;

grammar::Parser::Parser()
{
    ws = new token::Group ({' ','\t','\n'});
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
    
}

bool grammar::Parser::is_value(parser::DFA* token)
{
    if (token==float_num or token==int_num or token==str or token==boolean) {
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
    
    bool value_ready=false;
    
    /* value */
    if (is_value(token)) {
        push(ProductionType::Value);
        Production& p=stack.back();
        p.value=get_value(token);
        pop();
        value_ready=true;
    }
    
    /* left curly */
    if (token==lc) {
        push(ProductionType::S0);
        return;
    }

    /* left bracket */
    if (token==lb) {
        push(ProductionType::A0);
        return;
    }
    
    /* object */
    if (top.type==ProductionType::S0) {
        
        top.value=Variant::create_struct();
        
        /* right curly */
        if (token==rc) {
            pop();
        }
        /* key */
        if (token==str) {
            top.key=static_cast<token::String*>(token)->get_string();;
            top.type=ProductionType::S1;
        }
        
        return;
    }
    
    if (top.type==ProductionType::S1) {
        if (token==colon) {
            top.type=ProductionType::S2;
        }
        
        return;
    }
    
    if (top.type==ProductionType::S2) {
        
        if (top.down) {
            top.value[top.key]=last.value;
            top.down=false;
        }
        
        if (token==comma) {
            top.type=ProductionType::S3;
        }
        
        if (token==rc) {
            pop();
        }
        
        return;
    }
    
    if (top.type==ProductionType::S3) {
        if (token==str) {
            top.key=static_cast<token::String*>(token)->get_string();;
            top.type=ProductionType::S1;
        }
        
        return;
    }
    
    /* array */
    if (top.type==ProductionType::A0) {
        top.value=Variant::create_array(0);
        top.type=ProductionType::A1;
    }
    
    if (top.type==ProductionType::A1) {
        
        if (top.down) {
            top.value.append(last.value);
            top.down=false;
        }
        
        if (token==rb) {
            pop();
            return;
        }
        
        if (token==comma) {
            return;
        }
        
    }
}

Variant grammar::Parser::parse(istream& stream)
{
    lexer.set_input(&stream);
    
    stack.clear();
    
    push(ProductionType::Value);
    
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
    
    Production& p=stack[1];
    
    return p.value;
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

/*
static void on_step(DFA* dfa,string token,Grammar* grammar)
{
    
    
    if (token=="WS") {
        return;
    }
    
    Production& top = grammar->top();
    
    //clog<<"production "<<top.name<<" token "<<token<<endl;
    
    bool value_ready=false;
    
    if (is_value(token)) {
        grammar->push("value");
        Production& p=grammar->top();
        p.value=get_value(dfa,token);
        grammar->pop();
        value_ready=true;
    }
    
    if (token=="LEFT_CURLY") {
        grammar->push("s0");
        return;
    }

    if (token=="LEFT_BRACKET") {
        grammar->push("a0");
        return;
    }
    
    if (top.name=="s0") {
        
        top.value=Variant::create_struct();
        
        if (token=="RIGHT_CURLY") {
            grammar->pop();
        }
        
        if (token=="STRING") {
            top.key=static_cast<token::String*>(dfa)->get_string();;
            top.name="s1";
        }
        
        return;
    }
    
    if (top.name=="s1") {
        if (token=="COLON") {
            top.name="s2";
        }
        
        return;
    }
    
    if (top.name=="s2") {
        
        if (top.down) {
            Variant tmp=grammar->last().value;
            top.value[top.key]=tmp;
            top.down=false;
        }
        
        if (token=="COMMA") {
            top.name="s3";
        }
        
        if (token=="RIGHT_CURLY") {
            grammar->pop();
        }
        
        return;
    }
    
    if (top.name=="s3") {
        if (token=="STRING") {
            top.key=static_cast<token::String*>(dfa)->get_string();;
            top.name="s1";
        }
        
        return;
    }
    
    if (top.name=="a0") {
        top.value=Variant::create_array(0);
        top.name="a1";
    }
    
    if (top.name=="a1") {
        
        if (top.down) {
            top.value.append();
            top.value[top.value.count()-1]=grammar->last().value;
            top.down=false;
        }
        
        if (token=="RIGHT_BRACKET") {
            grammar->pop();
            return;
        }
        
        if (token=="COMMA") {
            return;
        }
        
    }
    
}
*/
    
Variant edupals::json::load(istream& stream)
{
    /*
    token::Group ws({' ','\t','\n'});
    token::Char lb('[');
    token::Char rb(']');
    token::Char lc('{');
    token::Char rc('}');
    token::Char colon(':');
    token::Char comma(',');
    token::Float float_num;
    token::Integer int_num;
    token::Word null("null");
    token::String str;
    token::Boolean boolean;
    
    Lexer lexer;
    
    lexer.add_token("WS",&ws);
    lexer.add_token("LEFT_BRACKET",&lb);
    lexer.add_token("RIGHT_BRACKET",&rb);
    lexer.add_token("LEFT_CURLY",&lc);
    lexer.add_token("RIGHT_CURLY",&rc);
    lexer.add_token("COLON",&colon);
    lexer.add_token("COMMA",&comma);
    
    lexer.add_token("FLOAT",&float_num);
    lexer.add_token("INTEGER",&int_num);
    lexer.add_token("BOOLEAN",&boolean);
    
    lexer.add_token("NULL",&null);
    lexer.add_token("STRING",&str);
    
    lexer.set_input(&stream);
    
    Grammar grammar;
    grammar.push("value");
    
    while (lexer.step()) {
        DFA* dfa = lexer.get_dfa();
        string token = lexer.get_token();
        
        on_step(dfa,token,&grammar);
        
    }
    
    if (!lexer.eof()) {
        throw runtime_error("Unknown token");
    }
    else {
        if (lexer.pending()) {
            throw runtime_error("Unexpected EOF");
        }
    }
    
    Production& p=grammar.stack[1];
    
    return p.value;
    */
    
    grammar::Parser parser;
    
    return parser.parse(stream);
}
