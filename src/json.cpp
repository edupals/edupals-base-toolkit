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

using namespace edupals::variant;
using namespace edupals::parser;
using namespace std;
using namespace std::placeholders;

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

struct Production{
    string name;
    int step;
    Variant value;
    
    Production(string name,int step=0)
    {
        this->name=name;
        this->step=step;
    }
};

struct Grammar
{
    vector<Production> stack;
    Variant value;
};

static void on_accepted(DFA* dfa,string token,void* data)
{
    
    if (token=="WS") {
        //nothing to do
        return;
    }
    
    Grammar* grammar = static_cast<Grammar*>(data);
    Production& top = grammar->stack.back();
    
    clog<<"production: "<<top.name<<" step "<<top.step<<" token "<<token<<endl;
    
    if (top.name=="value") {
        
        if (token=="INTEGER") {
            clog<<"integer: "<<dfa->value()<<endl;
            grammar->value=std::stoi(dfa->value());
            grammar->stack.pop_back();
        }
        
        if (token=="FLOAT") {
            clog<<"float: "<<dfa->value()<<endl;
            grammar->value=std::stof(dfa->value());
            grammar->stack.pop_back();
        }
        
        if (token=="STRING") {
            clog<<"string: "<<dfa->value()<<endl;
            grammar->value=dfa->value();
           grammar->stack.pop_back();
        }
        
        if (token=="BOOLEAN") {
            clog<<"value: "<<dfa->value()<<endl;
            if (dfa->value()=="true") {
                grammar->value=true;
            }
            else {
                grammar->value=false;
            }
            grammar->stack.pop_back();
        }
        
        if (token=="LEFT_CURLY") {
            grammar->stack.push_back(Production("struct"));
        }
        
        if (token=="LEFT_BRACKET") {
            grammar->stack.push_back(Production("array"));
            grammar->stack.back()->value=Grammar::create_array();
            clog<<"array"<<endl;
            grammar->stack.push_back(Production("value"));
            
        }

    }
    
    if (top.name=="array") {
        
        if (top.step==0) {
            if (token=="COMMA") {
                grammar->stack.push_back(Production("value"));
            }
            if (token=="RIGHT_BRACKET") {
                clog<<"end"<<endl;
                grammar->stack.pop_back();
                grammar->stack.pop_back();
            }
        }
    }
    
    if (top.name=="struct") {
        
        if (top.step==0 ) {
            clog<<"struct"<<endl;
            top.step=1;
        }
        
        if (top.step==1 and token=="STRING") {
            clog<<"key: "<<dfa->value()<<endl;
            top.step++;
            return;
        }
        
        if (top.step==2 and token=="COLON") {
            top.step++;
            grammar->stack.push_back(Production("value"));
            return;
        }
        
        if (top.step==3) {
            if (token=="COMMA") {
                top.step=1;
                return;
            }
            if (token=="RIGHT_CURLY") {
                clog<<"end"<<endl;
                grammar->stack.pop_back();
                grammar->stack.pop_back();
            }
        }
        
    }

}

static void on_rejected(string expression,void* data)
{
    clog<<"-- syntax error: "<<expression<<endl;
}

Variant edupals::json::load(istream& stream)
{
    
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
    
    Grammar grammar;
    grammar.stack.push_back(Production("value"));
    
    std::function<void(parser::DFA*,string,void*)> cb_accepted = on_accepted;
    std::function<void(string,void*)> cb_rejected = on_rejected;
    lexer.signal_accepted(cb_accepted);
    lexer.signal_rejected(cb_rejected);

    lexer.parse(stream,&grammar);
    
    clog<<"result "<<grammar.value.get_int32()<<endl;
    
    return grammar.value;
}