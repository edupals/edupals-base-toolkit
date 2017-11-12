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
 
#ifndef EDUPALS_SYSTEM
#define EDUPALS_SYSTEM

#include <string>
#include <vector>

namespace edupals 
{
    namespace system
    {
        /*!
            Gets current kernel version from /proc/version
            \return raw string
        */
        std::string version();
        
        /*!
            Gets system uptime
            \return uptime in seconds
        */
        double uptime();
        
        /*!
            Gets current boot cmdline
            \return raw string
        */
        std::string cmdline();
        
         /*!
            Gets installed RAM
            \return ram size in bytes
        */
        uint64_t get_total_memory();
        
        /*!
            Gets current free ram
            \return free ram in bytes
        */
        uint64_t get_free_memory();
        
        /*!
            Gets loaded kernel modules
            \return vector of module names
        */
        std::vector<std::string> get_modules();
    }
}

#endif
