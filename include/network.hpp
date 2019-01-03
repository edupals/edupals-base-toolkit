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
        /*!
            MAC Address class
        */
        class MAC
        {
            public:
            /*!
                6 byte address array
            */
            std::array<uint8_t,6> address;
            
            MAC(){};
            
            /*!
                Create MAC from byte array
            */
            MAC(std::array<uint8_t,6> address);
            
            /*!
                Create MAC from string representation
            */
            MAC(std::string address);
            
            /*!
                Returns a string representation of address
            */
            std::string to_string();
            
            /*!
                Access to address byte n
            */
            uint8_t operator [] (int n);
        };
    
        class IP4Address
        {
            public:
            
            std::array<uint8_t,4> address;
            
            IP4Address(){};
            IP4Address(uint32_t address);
            IP4Address(std::array<uint8_t,4> address);
            
            std::string to_string();
            
            uint8_t operator [] (int n);
        };
        
        /*!
            This class represents a Linux network device (it may not be a 
            physical device, but a virtual one)
        */
        class Device
        {
            public:
            /*!
                Device name
            */
            std::string name;
            
            /*!
                Hardware MAC Address
            */
            MAC address;
            
            /*!
                Carrier status (connection )
            */
            bool carrier;
            
            /*!
                Current mtu
            */
            uint32_t mtu;
        };
        
        /*!
            Get a list of available network devices
        */
        std::vector<Device> get_devices();
    }
}


#endif
