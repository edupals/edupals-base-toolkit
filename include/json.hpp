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

namespace edupals
{
    namespace json
    {
        namespace grammar
        {
            enum class ProductionType
            {
                S0,
                S1,
                S2,
                S3,
                A0,
                A1
            };
            
            class Production
            {
                ProductionType type;
                variant::Variant value;
                std::string key;
                bool down;
            };
            
            class Parser
            {
                
                protected:
                std::vector<Production> stack;
                
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
                
                Lexer lexer;
                
                public:
                
                Parser();
                virtual ~Parser();
                
                bool is_value(parser::DFA* token);
                variant::Variant get_value(parser::DFA* token);
                
                void push(ProductionType type);
                
                void step(parser::DFA* token);
            };
        }
        
        void dump(variant::Variant& value,std::ostream& stream);
        variant::Variant load(std::istream& stream);
    }
}

#endif
