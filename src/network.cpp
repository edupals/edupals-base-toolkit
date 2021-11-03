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

#include <sys/ioctl.h>
#include <linux/if_packet.h>
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
#include <experimental/filesystem>

using namespace edupals::network;
using namespace edupals::network::exception;
using namespace std;
namespace fs=std::experimental::filesystem;

thread_local std::vector<CachedInterface> thread_cache;
thread_local uint32_t update_count;

RawAddress::RawAddress(struct sockaddr* addr)
{
    family = AF_UNSPEC;
    
    if (addr!=nullptr) {
        family = addr->sa_family;
        
        switch (family) {
            case AF_PACKET: {
                struct sockaddr_ll* link=(struct sockaddr_ll*)addr;
                uint8_t* ptr = (uint8_t*)link->sll_addr;
                for (size_t n = 0;n<6;n++) {
                    value.push_back(ptr[n]);
                }
            }
            break;
            
            case AF_INET: {
                struct sockaddr_in* in=(struct sockaddr_in*)addr;
                uint8_t* ptr = (uint8_t*)&in->sin_addr.s_addr;
                for (size_t n = 0;n<4;n++) {
                    value.push_back(ptr[n]);
                }
            }
            
            case AF_INET6: {
                struct sockaddr_in6* in=(struct sockaddr_in6*)addr;
                uint8_t* ptr = (uint8_t*)&in->sin6_addr.s6_addr;
                clog<<"ipv6:";
                for (size_t n = 0;n<16;n++) {
                    clog<<std::hex<<(int)ptr[n]<<" ";
                    value.push_back(ptr[n]);
                }
                clog<<endl;
            }
        }
    }
}

uint8_t RawAddress::data(int n)
{
    return value[n];
}

string RawAddress::to_string()
{
    stringstream s;
    
    size_t n = 0;
    
    while (true) {
        s<<hex<<setw(2)<<setfill('0')<<(int)value[n];
        n++;
        
        if (n==value.size()) {
            break;
        }
        s<<":";
    } 
    
    return s.str();
}

MAC::MAC(RawAddress& address)
{
    family=address.family;
    value=address.value;
}

MAC::MAC(array<uint8_t,6> address)
{
    family = AF_PACKET;
    std::copy(address.begin(),address.end(),value.begin());
}

MAC::MAC(string address)
{
    family = AF_PACKET;

    //TODO: check for errors!

    string hex="";
    int n=5;
    
    for (char c:address) {
        if (c==':') {
            value[n]=stoi(hex,0,16);
            n--;
            hex="";
        }
        else {
            hex+=c;
        }
    }
    
    value[n]=stoi(hex,0,16);
    
}

string MAC::to_string()
{
    stringstream s;
    
    s<<hex<<setw(2)<<setfill('0')<<(int)value[0]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[1]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[2]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[3]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[4]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[5];
    
    return s.str();
}

IP4::IP4(RawAddress& address)
{
    family=address.family;
    value=address.value;
}

IP4::IP4(uint32_t address)
{
    family = AF_INET;
    value[0]=address & 0x000000FF;
    value[1]=(address & 0x0000FF00)>>8;
    value[2]=(address & 0x00FF0000)>>16;
    value[3]=(address & 0xFF000000)>>24;
}

IP4::IP4(array<uint8_t,4> address)
{
    family = AF_INET;
    
    for (size_t n = 0;n<4;n++) {
        value.push_back(address[n]);
    }
}

string IP4::to_string()
{
    stringstream s;
    
    s<<(int)value[0]<<"."<<(int)value[1]<<"."<<(int)value[2]<<"."<<(int)value[3];
    
    return s.str();
}

uint32_t IP4::get_uint32()
{
    uint32_t tmp;
    
    tmp=value[0] | (value[1]<<8) | (value[2]<<16) | (value[3]<<24);
    
    return tmp;
}

Mask4::Mask4(RawAddress& address) : IP4(address)
{
}

Mask4::Mask4(uint32_t address) : IP4(address)
{
}

Mask4::Mask4(array<uint8_t,4> address) : IP4(address)
{
}

int32_t Mask4::bits()
{
    int32_t num=0;
    bool knee=false;
    
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

bool Mask4::valid()
{
    int32_t v=bits();
    
    return (v!=-1);
}

bool Mask4::in_range(IP4 subnet,IP4 ip)
{
    for (int n=0;n<3;n++) {
        uint8_t v = ~(subnet[n] ^ ip[n]);
        v = value[n] & v;
        if (v!=0xff) {
            return false;
        }
    }
    
    return true;
}

IP6::IP6(RawAddress& address)
{
    family=address.family;
    value=address.value;
}

IP6::IP6(array<uint16_t,8> address)
{
    family = AF_INET6;
    
    for (size_t n = 0;n<8;n++) {
        value.push_back((address[n] & 0xff00)>>8);
        value.push_back(address[n] & 0x00ff);

    }
}

string IP6::to_string()
{
    stringstream s;
    

    s<<std::hex;
    uint16_t tmp;
    size_t n = 0;
    
    l1:
    tmp = value[n*2]<<8 | value[(n*2)+1];
    s<<tmp;
    n++;
    if (n<8) {
        s<<":";
        goto l1;
    }
    
    return s.str();
}

uint16_t IP6::operator [] (int n)
{
    uint16_t tmp = value[n*2]<<8 | value[(n*2)+1];
    return tmp;
}


void CachedInterface::push_address(struct ifaddrs* addr)
{
    
    if (addr->ifa_addr->sa_family == AF_PACKET) {
        
        this->flags = addr->ifa_flags;
        RawAddress tmp = RawAddress(addr->ifa_addr);
        this->address = MAC(tmp);
        tmp = RawAddress(addr->ifa_ifu.ifu_broadaddr);
        this->broadcast = MAC(tmp);
    }
    else {
        this->addresses.push_back(AddressSetup(addr->ifa_addr,addr->ifa_netmask,addr->ifa_ifu.ifu_broadaddr));
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

bool Interface::is_up()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_UP != 0);
}

bool Interface::is_loopback()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_LOOPBACK != 0);
}

bool Interface::is_p2p()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_POINTOPOINT != 0);
}

bool Interface::is_broadcast()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return (cache->flags & IFF_BROADCAST != 0);
}

bool Interface::is_multicast()
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

MAC Interface::address()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return cache->address;
}

MAC Interface::broadcast()
{
    if (cache==nullptr or cache->update_id!=update_count) {
        throw exception::InterfaceNotFound(name);
    }
    
    return cache->broadcast;
}

std::vector<AddressSetup>& Interface::addresses()
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

ostream& edupals::network::operator<<(ostream& os,edupals::network::MAC& addr)
{
    os<<addr.to_string();
    return os;
}

ostream& edupals::network::operator<<(ostream& os,edupals::network::MAC addr)
{
    os<<addr.to_string();
    return os;
}

ostream& edupals::network::operator<<(ostream& os,edupals::network::IP4& addr)
{
    os<<addr.to_string();
    return os;
}

ostream& edupals::network::operator<<(ostream& os,edupals::network::IP4 addr)
{
    os<<addr.to_string();
    return os;
}

ostream& edupals::network::operator<<(ostream& os,edupals::network::IP6& addr)
{
    os<<addr.to_string();
    return os;
}

ostream& edupals::network::operator<<(ostream& os,edupals::network::IP6 addr)
{
    os<<addr.to_string();
    return os;
}
