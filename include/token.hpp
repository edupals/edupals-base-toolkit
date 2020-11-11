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
 
#ifndef EDUPALS_TOKEN
#define EDUPALS_TOKEN

#include <dfa.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <array>

namespace edupals
{
    namespace parser
    {
        namespace token
        {
            bool is_num(char c);
            bool is_alpha_lower(char c);
            bool is_alpha_upper(char c);
            
            class Word: public DFA
            {
                private:
                
                std::string match;
                
                public:
                
                Word(std::string match);
                
                void start() override;
                void step() override;
            };
            
            class Char: public DFA
            {
                private:
                
                char match;
                
                public:
                
                Char(char match);
                
                void start() override;
                void step() override;
            };
            
            class Group: public DFA
            {
                private:
                
                std::vector<char> chars;
                
                public:
                
                Group(std::vector<char> chars);
                
                void start() override;
                void step() override;
            };
            
            class Boolean: public DFA
            {
                private:
                
                bool path;
                
                public:
                
                void start() override;
                void step() override;
                
                bool get_bool();
            };
            
            class Integer: public DFA
            {
                public:
                
                void start() override;
                void step() override;
                
                int get_int();
            };
            
            class Float: public DFA
            {
                private:
                
                bool dot;
                
                std::istringstream is;
                
                public:
                
                Float();
                
                void start() override;
                void step() override;
                
                float get_float();
            };
            
            class String: public DFA
            {
                public:
                    
                void start() override;
                void step() override;
                
                std::string get_string();
            };
            
            class IP4: public DFA
            {
                private:
                
                int dots;
                int digits;
                
                int in;
                
                public:
                
                std::array<uint8_t,4> ip;
                    
                void start() override;
                void step() override;
            };
            
        }
    }
}

#endif
