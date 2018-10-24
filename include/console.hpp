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
 
#ifndef EDUPALS_CONSOLE
#define EDUPALS_CONSOLE

#include <iostream>

namespace edupals
{
    namespace console
    {
        namespace format
        {
            const char* bold ="\e[1m";
            const char* dim = "\e[2m";
            const char* underlined = "\e[4m";
            const char* blink="\e[5m";
            const char* reverse="\e[7m";
            const char* hidden="\e[8m";
        }
        
        namespace reset
        {
            const char* all="\e[0m\e[39m";
            const char* bold ="\e[21m";
            const char* dim = "\e[22m";
            const char* underlined = "\e[24m";
            const char* blink="\e[25m";
            const char* reverse="\e[27m";
            const char* hidden="\e[28m";
        }
        
        namespace fg
        {
            const char* black="\e[30m";
            const char* red="\e[31m";
        }
        
        namespace bg
        {
        }
    }
}

#endif
