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

std::mutex data_mutex;
std::vector<Interface> data;

MAC::MAC(struct sockaddr addr)
{
    std::copy(&addr,&addr+6,value.begin());
}

MAC::MAC(array<uint8_t,6> address) : value(address)
{
}

MAC::MAC(string address)
{

    //TODO: check for errors!

    string hex="";
    int n=5;
    
    for (char c:address) {
        if (c==':') {
            this->value[n]=stoi(hex,0,16);
            n--;
            hex="";
        }
        else {
            hex+=c;
        }
    }
    
    this->value[n]=stoi(hex,0,16);
    
}

string MAC::to_string()
{
    stringstream s;
    
    s<<hex<<setw(2)<<setfill('0')<<(int)value[5]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[4]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[3]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[2]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[1]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)value[0];
    
    return s.str();
}

uint8_t MAC::operator [] (int n)
{
    return value[n];
}

IP4::IP4(struct sockaddr addr)
{
    std::copy(&addr,&addr+4,value.begin());
}

IP4::IP4(uint32_t address)
{
    value[0]=address & 0x000000FF;
    value[1]=(address & 0x0000FF00)>>8;
    value[2]=(address & 0x00FF0000)>>16;
    value[3]=(address & 0xFF000000)>>24;
}

IP4::IP4(array<uint8_t,4> address) : value(address)
{
}

string IP4::to_string()
{
    stringstream s;
    
    s<<(int)value[0]<<"."<<(int)value[1]<<"."<<(int)value[2]<<"."<<(int)value[3];
    
    return s.str();
}

uint8_t IP4::operator [] (int n)
{
    return address[n];
}

uint32_t IP4::get_uint32()
{
    uint32_t tmp;
    
    tmp=value[0] | (value[1]<<8) | (value[2]<<16) | (value[3]<<24);
    
    return tmp;
}

Mask4::Mask4(struct sockaddr addr) : IP4(addr)
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

Interface::Interface(string name)
{
    path="/sys/class/net/"+name;
    this->name=name;
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

void Interface::push_address(struct ifaddrs* addr)
{
    if (addr->ifa_addr->sa_family == AF_LINK) {
        this->flags = addr->ifa_flags;
        this->_address = MAC(*addr->ifa_addr);
        this->_broadcast = MAC(*adr->ifa_ifu.ifu_broadaddr);
    }
    else {
        _addresses.push_back(Address(*addr->ifa_addr,*addr->ifa_netmask,*adr->ifa_ifu.ifu_broadaddr));
    }
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

bool Interface::exists()
{
    fs::path sysfs = path;
    
    return fs::exists(sysfs);
}

vector<Interface>& Interface::list()
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
    std::lock_guard<std::mutex> lock(data_mutex);

    data.clear();
    fs::path sysfs("/sys/class/net");
    
    for (auto& dev: fs::directory_iterator(sysfs)) {
        data.push_back(Interface(dev.path().filename()));
    }
    
    struct ifaddrs* ifaces;

	if (getifaddrs(&ifaces) == 0) {
        
        struct ifaddrs* iface = ifaces;

        while (iface!=nullptr) {
            
            /* suboptimal search */
            for (Interface& q : data) {
                if (q->name == string(iface->ifa_name)) {
                    q.push_address(iface);
                    break;
                }
            }
            
            iface=iface->ifa_next;
        }
        
        freeifaddrs(ifaces);
        
    }
    
}
