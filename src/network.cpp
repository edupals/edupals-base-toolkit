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
 
 
#include <network.hpp>
#include <lexer.hpp>
#include <token.hpp>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <mutex>
#include <map>
#include <string>
#include <experimental/filesystem>

using namespace edupals::network;
using namespace edupals::network::exception;
using namespace edupals::parser;

using namespace std;
namespace fs=std::experimental::filesystem;

thread_local std::vector<CachedInterface> thread_cache;
thread_local uint32_t update_count;

/*
bool Mask4::in_range(IP4& subnet,IP4& ip)
{
    for (int n=0;n<4;n++) {
        uint8_t v = ~(subnet[n] ^ ip[n]);
        v = value[n] & v;
        if (v!=0xff) {
            return false;
        }
    }
    
    return true;
}

bool Mask6::in_range(IP6& subnet,IP6& ip)
{
    for (int n=0;n<16;n++) {
        uint8_t v = ~(subnet[n] ^ ip[n]);
        v = value[n] & v;
        if (v!=0xff) {
            return false;
        }
    }
    
    return true;
}
*/

struct in_addr edupals::network::ip4(string addr)
{
    struct in_addr ret;

    int status = inet_pton(AF_INET,addr.c_str(),&ret);

    if (status !=1) {
        throw exception::ParseError("Can not parse IPv4 address");
    }

    return ret;
}


struct in6_addr edupals::network::ip6(string addr)
{
    struct in6_addr ret;

    int status = inet_pton(AF_INET6,addr.c_str(),&ret);

    if (status !=1) {
        throw exception::ParseError("Can not parse IPv6 address");
    }

    return ret;
}

vector<uint8_t> edupals::network::to_bytes(struct in_addr& addr)
{
    vector<uint8_t> ret;

    uint8_t* value = (uint8_t*)&addr.s_addr;

    for (int n=0;n<4;n++) {
        ret.push_back(value[n]);
    }

    return ret;
}

vector<uint8_t> edupals::network::to_bytes(struct in6_addr& addr)
{
    vector<uint8_t> ret;

    uint8_t* value = (uint8_t*)&addr.s6_addr;

    for (int n=0;n<16;n++) {
        ret.push_back(value[n]);
    }

    return ret;
}

vector<uint16_t> edupals::network::to_words(struct in6_addr& addr)
{
    vector<uint16_t> ret;

    uint16_t* value = (uint16_t*)&addr.s6_addr;

    for (int n=0;n<8;n++) {
        ret.push_back(value[n]);
    }

    return ret;
}

int edupals::network::maskbits(in_addr& addr)
{
    int32_t num=0;
    bool knee=false;
    uint8_t* value = (uint8_t*)&addr.s_addr;

    for (int n=3;n>=0;n--) {
        for (int b=0;b<8;b++) {
            uint8_t m = 1<<b;
            uint8_t v = value[n] & m;

            if (knee) {
                if (v==0) {
                    return -1;
                }
            }
            else {
                if (v==0) {
                    num++;
                }
                else {
                    knee=true;
                }
            }
        }
    }

    return 32-num;
}

int edupals::network::maskbits(in6_addr& addr)
{
    int32_t num=0;
    bool knee=false;
    uint8_t* value = (uint8_t*)&addr.s6_addr;

    for (int n=15;n>=0;n--) {
        for (int b=0;b<8;b++) {
            uint8_t m = 1<<b;
            uint8_t v = value[n] & m;

            if (knee) {
                if (v==0) {
                    return -1;
                }
            }
            else {
                if (v==0) {
                    num++;
                }
                else {
                    knee=true;
                }
            }
        }
    }

    return 128-num;
}

int edupals::network::maskbits(struct sockaddr* addr)
{
    switch (addr->sa_family) {

        case AF_INET: {
            struct sockaddr_in* in = (struct sockaddr_in*)addr;
            return network::maskbits(in->sin_addr);
        }
        break;

        case AF_INET6: {
            struct sockaddr_in6* in6 = (struct sockaddr_in6*)addr;
            return network::maskbits(in6->sin6_addr);
        }

        default:
            return 0;
    }
}

bool edupals::network::in_range(struct in_addr& ip,struct in_addr& subnet,struct in_addr& mask)
{
    for (int n=0;n<4;n++) {
        uint8_t v = ~(subnet.s_addr[n] ^ ip.s_addr[n]);
        v = mask.s_addr[n] & v;
        if (v!=0xff) {
            return false;
        }
    }

    return true;
}

void CachedInterface::push_address(struct ifaddrs* addr)
{
    IFAddress ifa(addr->ifa_addr,addr->ifa_netmask,addr->ifa_ifu.ifu_broadaddr);
    this->addresses.push_back(ifa);

    if (addr->ifa_addr->sa_family == AF_PACKET) {
        
        this->flags = addr->ifa_flags;
        std::memcpy(&this->address, addr->ifa_addr,sizeof(struct sockaddr_ll));
        std::memcpy(&this->broadcast, addr->ifa_ifu.ifu_broadaddr,sizeof(struct sockaddr_ll));
    }

}

Interface::Interface(string name)
{
    path="/sys/class/net/"+name;
    this->name=name;
    this->cache=nullptr;
    
    for (CachedInterface& q : thread_cache) {
        if (q.name==name) {
            this->cache=&q;
            break;
        }
    }
}

string Interface::read_str(string prop)
{
    if(!exists()) {
        throw InterfaceNotFound(name);
    }
    
    ifstream file;
    string tmp;
    
    fs::path sysfs = path;
    fs::path carrier = sysfs / prop;
    
    file.open(carrier);
    std::getline(file,tmp);
    file.close();
    
    return tmp;
}

uint32_t Interface::read_u32(string prop)
{
    string tmp = read_str(prop);
    return std::stoi(tmp);
}

bool Interface::carrier()
{
    try {
        uint32_t value = read_u32("carrier");
        return (value==1);
    }
    catch (std::exception e) {
        return false;
    }
}

uint32_t Interface::mtu()
{
    return read_u32("mtu");
}

uint32_t Interface::type()
{
    return read_u32("type");
}

bool Interface::up()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_UP != 0);
}

bool Interface::loopback()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_LOOPBACK != 0);
}

bool Interface::p2p()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_POINTOPOINT != 0);
}

bool Interface::broadcast()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_BROADCAST != 0);
}

bool Interface::multicast()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_MULTICAST != 0);
}

bool Interface::exists()
{
    fs::path sysfs = path;
    
    return fs::exists(sysfs);
}

struct sockaddr_ll& Interface::hwaddress()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return cache->address;
}

struct sockaddr_ll& Interface::hwbroadcast()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return cache->broadcast;
}

std::vector<IFAddress>& Interface::addresses()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return cache->addresses;
}

vector<Interface> Interface::list()
{
    vector<Interface> ifaces;
    
    fs::path sysfs("/sys/class/net");
    
    for (auto& dev: fs::directory_iterator(sysfs)) {
        ifaces.push_back(Interface(dev.path().filename()));
    }
    
    return ifaces;
}

void Interface::update()
{
    update_count++;

    for (CachedInterface& q : thread_cache) {
        q.addresses.clear();
    }
    
    struct ifaddrs* ifaces;

    if (getifaddrs(&ifaces) == 0) {
        
        struct ifaddrs* iface = ifaces;

        bool found;

        while (iface!=nullptr) {
            found = false;
            /* suboptimal search */
            for (CachedInterface& q : thread_cache) {
                if (q.name == string(iface->ifa_name)) {
                    q.update_id=update_count;
                    found = true;
                    q.push_address(iface);
                    break;
                }
            }
            
            if (!found) {
                CachedInterface q;
                q.name = string(iface->ifa_name);
                q.update_id=update_count;
                q.push_address(iface);
                
                thread_cache.push_back(q);
            }
            
            iface=iface->ifa_next;
        }
        
        freeifaddrs(ifaces);
        
    }
    
}

ostream& operator<<(ostream& os,struct in6_addr& addr)
{
    char buffer[INET6_ADDRSTRLEN];
    const char* ret = inet_ntop(AF_INET6,&addr,buffer,INET6_ADDRSTRLEN);
    os<<buffer;

    return os;
}

ostream& operator<<(ostream& os,struct in_addr& addr)
{
    char buffer[INET_ADDRSTRLEN];
    const char* ret = inet_ntop(AF_INET,&addr,buffer,INET_ADDRSTRLEN);
    os<<buffer;

    return os;
}

ostream& operator<<(ostream& os,struct sockaddr_ll& addr)
{
    stringstream s;
    int n = 0;
    L1:
        s<<hex<<setw(2)<<setfill('0')<<(int)addr.sll_addr[n];

        n++;
        if (n != addr.sll_halen) {
            s<<":";
            goto L1;
        }

    os<<s.str();
    return os;
}

ostream& operator<<(ostream& os,struct sockaddr& addr)
{
    switch (addr.sa_family) {
        case AF_PACKET: {
            sockaddr_ll* ll = (sockaddr_ll*)&addr;
            os<<*ll;
        }
        break;

        case AF_INET6: {
            sockaddr_in6* in = (sockaddr_in6*)&addr;
            os<<in->sin6_addr;
        }
        break;

        case AF_INET: {
            sockaddr_in* in = (sockaddr_in*)&addr;

            os<<in->sin_addr;
        }
        break;
    }

    return os;
}

ostream& operator<<(ostream& os,struct sockaddr* addr)
{
    os<<*addr;
    return os;
}
