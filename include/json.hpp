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
 
#ifndef EDUPALS_JSON
#define EDUPALS_JSON

#include "variant.hpp"
#include "token.hpp"
#include "lexer.hpp"

#include <ostream>
#include <vector>
#include <exception>

namespace edupals
{
    namespace json
    {
        namespace exception
        {
            class SyntaxError : public std::exception
            {
                public:
                
                std::string message;
                
                SyntaxError(std::string message)
                {
                    this->message=message;
                }
                
                const char* what () const throw ()
                {
                    return message.c_str();
                }
            };
        }
        
        namespace grammar
        {
            enum class ProductionType
            {
                Object0,
                Object1,
                Object2,
                Object3,
                Object4,
                Array0,
                Array1,
                Json0
            };
            
            class Production
            {
                public:
                
                ProductionType type;
                variant::Variant value;
                std::string key;
                bool down;
            };
            
            class Parser
            {
                
                protected:
                std::vector<Production> stack;
                Production last;
                
                parser::DFA* ws;
                parser::DFA* lb;
                parser::DFA* rb;
                parser::DFA* lc;
                parser::DFA* rc;
                parser::DFA* colon;
                parser::DFA* comma;
                parser::DFA* float_num;
                parser::DFA* int_num;
                parser::DFA* null;
                parser::DFA* str;
                parser::DFA* boolean;
                
                parser::Lexer lexer;
                
                bool is_value(parser::DFA* token);
                variant::Variant get_value(parser::DFA* token);
                
                void push(ProductionType type);
                
                void pop();
                
                void step(parser::DFA* token);
                
                public:
                
                Parser();
                virtual ~Parser();
                
                variant::Variant parse(std::istream& stream);
            };
        }
        
        /*!
            Serialize Variant container into a json format
            \param value Variant container
            \param stream valid output stream
        */
        void dump(variant::Variant& value,std::ostream& stream);
        
        /*!
            Load a json into a Variant
            \param stream valid input stream
            \return a Variant container matching parsed json
        */
        variant::Variant load(std::istream& stream);
    }
}

#endif
