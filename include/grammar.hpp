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


#ifndef EDUPALS_GRAMMAR
#define EDUPALS_GRAMMAR

#include <string>
#include <vector>
#include <list>

namespace edupals
{
    namespace parser
    {
        class Production
        {
            public:
            
            std::string name;
            std::vector<std::string> value;
        };
        
        class Grammar
        {
            private:
            std::vector<Production> productions;
            
            public:
            
            void add(std::string name,std::vector<std::string> value);
            
            std::vector<Production> search(std::string name);
            int R(Production& p,std::list<std::string>& tokens);
            void test(std::vector<std::string> tokens);
        };
    }
}

#endif