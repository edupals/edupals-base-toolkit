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
#include <sstream>

namespace edupals
{
    namespace parser
    {
        class Lexer
        {
            private:
            
            std::istream* input;
            char buffer;
            
            std::vector<DFA*> tokens;
            std::map<DFA*,std::string> names;
            
            void reset_tokens();
            bool get_char(char& c);
            
            bool _eof;
            DFA* accepted;
            DFA* last;
            
            public:
            
            /*!
                Constructor
            */
            Lexer();
            
            /*!
                Destructor
            */
            virtual ~Lexer();
            
            /*!
                Sets input stream
                \param input std stream
            */
            void set_input(std::istream* input);
            
            /*!
                Adds a token DFA to the list
                \param name token name
                \param dfa pointer to DFA matching the token
            */
            void add_token(std::string name,DFA* dfa);
            
            /*!
                Tries to parse a token
                It returns false either it reaches eof or can match any token
            */
            bool step();
            
            /*!
                gets matched token
            */
            std::string get_token();
            
            /*!
                gets matched DFA
            */
            DFA* get_dfa();
            
            /*!
                Return whenever eof has been reached
            */
            bool eof();
            
            /*!
                Whenever eof reached but some tokens were still being matched
            */
            bool pending();
            
        };
    }
}

#endif
