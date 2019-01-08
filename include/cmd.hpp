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

            /*! single character option or 0 if none */
            char short_name;
            
            /*! long name option or empty string for none */
            std::string long_name;
            
            /*! type of option argument */
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
            
            std::vector<std::string> unknowns;
            std::vector<std::string> missings;
            
            ParseResult();
            
            /*!
                Return whenever the parse was successful or not
            */
            bool success();
        };
    
        class ArgumentParser
        {
            private:
            
            std::vector<Option> options;
            
            public:
            
            /*!
                Adds a new parse option
                Options must be added before calling parse method
            */
            void add_option(Option option);
            
            /*!
                Parse arguments
                \return A ParseResult object with options and arguments
            */
            ParseResult parse(int argc,char* argv[]);
        };
    }
}

#endif
