/*
 * Copyright (C) 2018 Edupals project
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
 
#ifndef EDUPALS_LEXER
#define EDUPALS_LEXER

#include <dfa.hpp>

#include <vector>
#include <map>
#include <string>
#include <istream>

namespace edupals
{
    namespace parser
    {
        class Lexer
        {
            private:
            
            std::vector<DFA*> tokens;
            std::map<DFA*,std::string> names;
            
            public:
            
            void add_token(std::string name,DFA* dfa);
            
            void parse(std::istream& input);
        };
    }
}

#endif
