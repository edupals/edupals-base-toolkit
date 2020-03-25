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
#include <exception>

namespace edupals
{
    namespace network
    {
        namespace exception
        {
            class InterfaceNotFound : public std::exception
            {
                public:
                
                std::string msg;
                
                InterfaceNotFound(std::string& iface)
                {
                    msg="interface not found:"+iface;
                }
                
                const char* what() const throw()
                {
                    return msg.c_str();
                }
            };
        }
        
        /*!
            MAC Address class
        */
        class MAC
        {
            public:
            /*! 6 byte address array */
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
    
        /*!
            IP 4 Address
        */
        class IP4
        {
            public:
            
            /*! 4 byte address */
            std::array<uint8_t,4> address;
            
            IP4(){};
            
            /*!
                Create address from uint32 representation
            */
            IP4(uint32_t address);
            
            /*!
                Create address from array representation
            */
            IP4(std::array<uint8_t,4> address);
            
            /*!
                Create a string representation
            */
            std::string to_string();
            
            /*!
                Acces to address byte n
            */
            uint8_t operator [] (int n);
        };
        
        class Interface
        {
            protected:
            
            std::string read_str(std::string prop);
            uint32_t read_u32(std::string prop);
            
            public:
            
            std::string name;
            std::string path;
            
            Interface(std::string name);
            
            bool carrier();
            
            uint32_t mtu();
            
            uint32_t type();
            
            MAC address();
            
            bool exists();
            
            static std::vector<Interface> list();
        };
        
        /*!
            This class represents a Linux network device (it may not be a 
            physical device, but a virtual one)
        */
        class Device
        {
            public:
            
            /*! Device path */
            std::string path;
            
            /*! Device name */
            std::string name;
            
            /*! Hardware MAC Address */
            MAC address;
            
            /*! Carrier status (connection ) */
            bool carrier;
            
            /*! Current mtu */
            uint32_t mtu;
            
            /*!
                Hardware type, common values are 1 for Ethernet and
                772 for loopback, see if_arp.h for details
            */
            uint32_t type;
            
            Device(){};
            
            /*!
                Create device from path
            */
            Device(std::string name);
            
            /*!
                Update properties
            */
            void update();
        };
        
        /*!
            Get a list of available network devices
        */
        std::vector<std::string> get_devices();
    }
}


#endif
