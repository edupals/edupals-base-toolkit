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

#ifndef EDUPALS_CONFIGFILE
#define EDUPALS_CONFIGFILE

#include <string>
#include <map>

namespace edupals
{
    namespace configfile
    {
    
        class Section
        {
            public:
            
            std::string name;
            
            std::map<std::string key,std::string value> values;
            
            std::string& operator [] (std::string key);
        };
    
        class Config
        {
            public:
            
            std::map<std::string,Section> sections;
            
            ConfigFile();
            ConfigFile(std::string filename);
            
            Section& operator [] (std::string name);
        };
    }
}

#endif
