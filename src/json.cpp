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

using namespace std;
using namespace edupals::variant;
using namespace edupals::parser;

void edupals::json::dump(Variant& value,ostream& stream)
{
    vector<string> keys;
    size_t count;
    
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
            stream<<std::to_string(value.get_int32());
        break;
        
        case Type::Float:
            stream<<std::to_string(value.get_float());
        break;
        
        case Type::Double:
            stream<<std::to_string(value.get_double());
        break;
        
        case Type::String:
            stream<<value.get_string();
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
}

static void on_accepted(DFA* dfa,string name)
{
    
    if (name=="WS") {
        //nothing to do
        return;
    }
    
    clog<<"-- token: "<<name<<"="<<dfa->value()<<endl;
}

static void on_rejected(string expression)
{
    clog<<"-- syntax error: "<<expression<<endl;
}

Variant edupals::json::load(istream& stream)
{
    Variant result;
    
    token::Char ws(' ');
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
    token::Bool boolean;
    
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
    
    std::function<void(parser::DFA* dfa,string name)> cb_accepted = on_accepted;
    std::function<void(string expression)> cb_rejected = on_rejected;
    lexer.signal_accepted(cb_accepted);
    lexer.signal_rejected(cb_rejected);
    
    
    /*
    lexer.signal_accepted(
        [](parser::DFA* dfa,string name) {
            clog<<"-- token: "<<name<<"="<<dfa->value()<<endl;
        }
    );
    
    lexer.signal_rejected(
        [](string expression) {
            clog<<"-- syntax error: "<<expression<<endl;
        }
    );
    */
    lexer.parse(stream);
        
    return result;
}