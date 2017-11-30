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

#ifndef EDUPALS_NETWORK
#define EDUPALS_NETWORK

#include <cstdint>
#include <string>
#include <vector>
#include <array>

namespace edupals
{
    namespace network
    {
    
        class MACAddress
        {
            public:
            
            std::array<uint8_t,6> address;
            
            MACAddress(std::array<uint8_t,6> address);
            
            std::string to_string();
            
            uint8_t operator [] (int n);
        };
    
        class IP4Address
        {
            public:
            
            std::array<uint8_t,4> address;
            
            IP4Address(uint32_t address);
            IP4Address(std::array<uint8_t,4> address);
            
            std::string to_string();
            
            uint8_t operator [] (int n);
        };
        
        
    }
}


#endif
