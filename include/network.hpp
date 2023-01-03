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
#include <linux/if_packet.h>
#include <netinet/ip.h>
#include <ifaddrs.h>

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <exception>
#include <cstring>

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
                
                InterfaceNotFound(std::string iface)
                {
                    msg="interface not found:"+iface;
                }
                
                const char* what() const throw()
                {
                    return msg.c_str();
                }
            };

            class ParseError: public std::exception
            {
                public:

                std::string msg;

                ParseError(std::string value)
                {
                    msg="Failed to parse address:"+value;
                }

                const char* what() const throw()
                {
                    return msg.c_str();
                }
            };
        }

        /*! Initializes an in_addr from a string. Raises an exception if parsing fails */
        struct in_addr ip4(std::string addr);

        /*! Initializes an in6_addr from a string. Raises an exception if parsing fails */
        struct in6_addr ip6(std::string addr);

        /*! Returns a vector of bytes from a in_addr */
        std::vector<uint8_t> to_bytes(struct in_addr& addr);

        /*! Returns a vector of bytes from a in6_addr */
        std::vector<uint8_t> to_bytes(struct in6_addr& addr);

        /*! Returns a vector of words (uint16_t) from a in6_addr */
        std::vector<uint16_t> to_words(struct in6_addr& addr);

        /*! Returns number of mask bits or -1 if bad-formed */
        int maskbits(struct in_addr& addr);

        /*! Returns number of mask bits or -1 if bad-formed */
        int maskbits(struct in6_addr& addr);

        /*!
            Gets number of bits from a mask ipv4/6 address or -1 if bad-formed
        */
        int maskbits(struct sockaddr* addr);

        bool in_range(struct in_addr& addr,struct in_addr& subnet,struct in_addr& mask);

        bool in_range(struct in6_addr& addr,struct in6_addr& subnet,struct in6_addr& mask);

        struct in_addr mask4(int bits);

        struct in6_addr mask6(int bits);

        struct in_addr subnet(struct in_addr& addr, struct in_addr& mask);

        struct in6_addr subnet(struct in6_addr& addr, struct in6_addr& mask);

        /*!
            Rrepresents an interface address setup:
            - address
            - netmask
            - broadcast
        */
        class IFAddress
        {
            protected:

            struct sockaddr_storage _address;
            struct sockaddr_storage _netmask;
            struct sockaddr_storage _broadcast;

            public:

            IFAddress(struct sockaddr* address,
                    struct sockaddr* netmask,
                    struct sockaddr* broadcast)
            {
                if (address) {
                    std::memcpy(&_address,address,sizeof(_address));
                }
                if (netmask) {
                    std::memcpy(&_netmask,netmask,sizeof(_netmask));
                }
                if (broadcast) {
                    std::memcpy(&_broadcast,broadcast,sizeof(_broadcast));
                }
            }

            /*!
                Gets the address family, using socket.h constants, typically:
                AF_PACKET, AF_INET, AF_INET6...
            */
            uint32_t family() const
            {
                return _address.ss_family;
            }

            struct sockaddr* address() const
            {
                return (struct sockaddr*)(&_address);
            }

            struct sockaddr* netmask() const
            {
                return (struct sockaddr*)(&_netmask);
            }

            struct sockaddr* broadcast() const
            {
                return (struct sockaddr*)(&_broadcast);
            }
        };

        class CachedInterface
        {
            public:
            
            uint32_t update_id;
            std::string name;
            uint32_t flags;
            struct sockaddr_ll address;
            struct sockaddr_ll broadcast;
            std::vector<IFAddress> addresses;
            
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
            struct sockaddr_ll& hwaddress();
            
            /*! Hardware broadcast address */
            struct sockaddr_ll& hwbroadcast();
            
            /*!
                Gets a list of current interface addresses
            */
            std::vector<IFAddress>& addresses();
            
            /*!
                gets a list of all avialable interfaces
            */
            static std::vector<Interface> list();
            
            static void update();
        };
    }
}

std::ostream& operator<<(std::ostream& os,struct in6_addr& addr);
std::ostream& operator<<(std::ostream& os,struct in_addr& addr);
std::ostream& operator<<(std::ostream& os,struct sockaddr_ll& addr);
std::ostream& operator<<(std::ostream& os,struct sockaddr& addr);
std::ostream& operator<<(std::ostream& os,struct sockaddr* addr);

#endif
