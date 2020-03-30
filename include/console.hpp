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

namespace edupals
{
    namespace console
    {
        /*! beep */
        const char beep='\a';
        
        /*! white space */
        const char ws=' ';
        
        /*! back space */
        const char bs='\b';
        
        /*!  horitzontal tab */
        const char htab='\t';
        
        /*! vertical tab */
        const char vtab='\v';
        
        /*! line feed */
        const char lf='\n';
        
        /*! carriage return */
        const char cr='\r';
        
        namespace style
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
            const char* all="\e[0m\e[39m\e[49m";
            const char* style="\e[0m";
            const char* fg="\e[39m";
            const char* bg="\e[49m";
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
            const char* green="\e[32m";
            const char* yellow="\e[33m";
            const char* blue="\e[34m";
            const char* magenta="\e[35m";
            const char* cyan="\e[36m";
            const char* light_grey="\e[37m";
            const char* dark_grey="\e[90m";
            
            const char* light_red="\e[91m";
            const char* light_green="\e[92m";
            const char* light_yellow="\e[93m";
            const char* light_blue="\e[94m";
            const char* light_magenta="\e[95m";
            const char* light_cyan="\e[96m";
            const char* white="\e[97m";
        }
        
        namespace bg
        {
            const char* black="\e[40m";
            const char* red="\e[41m";
            const char* green="\e[42m";
            const char* yellow="\e[43m";
            const char* blue="\e[44m";
            const char* magenta="\e[45m";
            const char* cyan="\e[46m";
            const char* light_grey="\e[47m";
            const char* dark_grey="\e[100m";
            
            const char* light_red="\e[101m";
            const char* light_green="\e[102m";
            const char* light_yellow="\e[103m";
            const char* light_blue="\e[104m";
            const char* light_magenta="\e[105m";
            const char* light_cyan="\e[106m";
            const char* white="\e[107m";

        }
    }
}

#endif
