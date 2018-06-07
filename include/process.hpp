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
 

#ifndef EDUPALS_PROCESS
#define EDUPALS_PROCESS

#include <cstdint>
#include <string>

namespace edupals
{
    namespace system
    {
        class Process
        {
            public:
            
            int32_t pid;
            
            Process();
            Process(int32_t pid);
            
            std::string get_proc();
            std::string get_name();
            std::string get_cmdline();
        };
    }
}

#endif
