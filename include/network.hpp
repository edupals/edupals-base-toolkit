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

#include <sys/types.h>
#include <ifaddrs.h>

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
        
        class RawAddress
        {
            public:
                
            uint16_t family;
            std::vector<uint8_t> value;
            
            RawAddress(){}
            RawAddress(struct sockaddr* addr);
            virtual ~RawAddress(){}
            
            virtual uint8_t data (int n);
            
            virtual std::string to_string();
        };
        
        /*!
            MAC Address class
        */
        class MAC: public RawAddress
        {
            public:
            
            MAC(){};
            
            /*!
                Create MAC from sockaddr struct
            */
            explicit MAC(RawAddress& address);
            
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
            std::string to_string() override;
            
            virtual uint8_t operator [] (int n)
            {
                return value[n];
            }
            
        };
    
        /*!
            IP 4 Address
        */
        class IP4: public RawAddress
        {
            public:
            
            IP4()
            {
                value[0]=0;
                value[1]=0;
                value[2]=0;
                value[3]=0;
            };
            
            explicit IP4(RawAddress& address);
            
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
            std::string to_string() override;
            
            /*!
                Gets uint32 address representation
            */
            uint32_t get_uint32();
            
            virtual uint8_t operator [] (int n)
            {
                return value[n];
            }
        };
        
        /*!
            IP 6 Address
        */
        class IP6: public RawAddress
        {
            public:
            
            IP6()
            {
                for (size_t n=0;n<16;n++) {
                    value[n] = 0;
                }
            };
            
            explicit IP6(RawAddress& address);
            
            /*!
                Create address from array representation
            */
            IP6(std::array<uint16_t,8> address);
            
            /*!
                Create a string representation
            */
            std::string to_string() override;
            
            virtual uint16_t operator [] (int n);
            
        };
        
        class Mask4 : public IP4
        {
            public:
            
            explicit Mask4(RawAddress& address);
            
            Mask4(std::array<uint8_t,4> address);
            
            explicit Mask4(IP4& address);
            
            Mask4 (uint32_t bits);
            
            /*!
                Get number of bits mask representation
                return -1 if mask is not valid 1 sequence
            */
            int32_t bits();
            
            /*!
                Whenever the mask is valid or not
            */
            bool valid();
            
            /*!
                Checks if ip is in range for the subnet/mask
            */
            bool in_range(IP4& subnet,IP4& ip);
            
        };
        
        class Mask6 : public IP6
        {
            public:
            
            explicit Mask6 (RawAddress& address);
            
            Mask6 (std::array<uint16_t,8> address);
            
            explicit Mask6 (IP6& address);
            
            Mask6 (uint32_t bits);
            
            /*!
                Get number of bits mask representation
                return -1 if mask is not valid 1 sequence
            */
            int32_t bits();
            
            /*!
                Whenever the mask is valid or not
            */
            bool valid();
            
            /*!
                Checks if ip is in range for the subnet/mask
            */
            bool in_range(IP6& subnet,IP6& ip);
            
        };

        
        class AddressSetup
        {
            protected:
            
            RawAddress _address;
            RawAddress _netmask;
            RawAddress _broadcast;
            
            public:
            
            AddressSetup(struct sockaddr* address,
                    struct sockaddr* netmask,
                    struct sockaddr* broadcast) :
                    _address(address),
                    _netmask(netmask),
                    _broadcast(broadcast)
            {
            }
            
            uint16_t family()
            {
                return _address.family;
            }
            
            RawAddress& address()
            {
                return _address;
            }
            
            RawAddress& netmask()
            {
                return _netmask;
            }
            
            RawAddress& broadcast()
            {
                return _broadcast;
            }
            
            RawAddress& peer()
            {
                return _broadcast;
            }
        };
        
        class CachedInterface
        {
            public:
            
            uint32_t update_id;
            std::string name;
            uint32_t flags;
            MAC address;
            MAC broadcast;
            std::vector<AddressSetup> addresses;
            
            void push_address(struct ifaddrs* addr);
        };
        
        /*!
            This class represents a Linux network device (it may not be a 
            physical device, but a virtual one)
        */
        class Interface
        {
            protected:
            
            std::string read_str(std::string prop);
            uint32_t read_u32(std::string prop);
            
            CachedInterface* cache;
            
            public:
            
            /*! interface name */
            std::string name;
            
            /*! sysfs path */
            std::string path;
            
            Interface(){};
            
            Interface(std::string name);
            
            /*! Carrier status (connection ) */
            bool carrier();
            
            /*! Current mtu */
            uint32_t mtu();
            
            /*!
                Hardware type, common values are 1 for Ethernet and
                772 for loopback, see if_arp.h for details
            */
            uint32_t type();
            
            bool up();
            
            bool loopback();
            
            bool p2p();
            
            bool broadcast();
            
            bool multicast();
            
            /*! whenever interface exists or not */
            bool exists();

            /*! Hardware MAC Address */
            MAC& hwaddress();
            
            /*! Hardware broadcast address */
            MAC& hwbroadcast();
            
            
            std::vector<AddressSetup>& addresses();
            
            /*!
                gets a list of all avialable interfaces
            */
            static std::vector<Interface> list();
            
            static void update();
        };
    }
}

std::ostream& operator<<(std::ostream& os,edupals::network::MAC& addr);
std::ostream& operator<<(std::ostream& os,edupals::network::IP4& addr);
std::ostream& operator<<(std::ostream& os,edupals::network::IP6& addr);

#endif
