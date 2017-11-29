/*
 * Copyright (C) 2017 Edupals project
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

#ifndef EDUPALS_CMD
#define EDUPALS_CMD

#include <string>
#include <vector>

namespace edupals
{
    namespace cmd
    {
    
        enum class ArgumentType {
            None=0,
            Required=1,
            Optional=2
        };
        
        class Option
        {
            public:
            
            char short_name;
            std::string long_name;
            ArgumentType argument_type;
            
            Option(char short_name,std::string long_name,ArgumentType argument_type);
            Option(char short_name,ArgumentType argument_type);
            Option(std::string long_name,ArgumentType argument_type);
            
            std::string value;
        };
        
        class ParseResult
        {
            public:
            
            std::vector<Option> options;
            std::vector<std::string> args;
            
        };
    
        class ArgumentParser
        {
            private:
            
            std::vector<Option> options;
            
            public:
            
            void add_option(Option option);
            
            ParseResult parse(int argc,char* argv[]);
        };
    }
}

#endif
