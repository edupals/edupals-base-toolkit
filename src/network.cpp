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

#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <experimental/filesystem>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace edupals::network;
using namespace edupals::network::exception;
using namespace std;
namespace fs=std::experimental::filesystem;

MAC::MAC(array<uint8_t,6> address) : address(address)
{
}

MAC::MAC(string address)
{

    //TODO: check for errors!

    string hex="";
    int n=5;
    
    for (char c:address) {
        if (c==':') {
            this->address[n]=stoi(hex,0,16);
            n--;
            hex="";
        }
        else {
            hex+=c;
        }
    }
    
    this->address[n]=stoi(hex,0,16);
    
}

string MAC::to_string()
{
    stringstream s;
    
    s<<hex<<setw(2)<<setfill('0')<<(int)address[5]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)address[4]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)address[3]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)address[2]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)address[1]<<":";
    s<<hex<<setw(2)<<setfill('0')<<(int)address[0];
    
    return s.str();
}

uint8_t MAC::operator [] (int n)
{
    return address[n];
}

IP4::IP4(uint32_t address)
{
    this->address[3]=address & 0x000000FF;
    this->address[2]=(address & 0x0000FF00)>>8;
    this->address[1]=(address & 0x00FF0000)>>16;
    this->address[0]=(address & 0xFF000000)>>24;
}

IP4::IP4(array<uint8_t,4> address) : address(address)
{
}

string IP4::to_string()
{
    stringstream s;
    
    s<<(int)address[3]<<"."<<(int)address[2]<<"."<<(int)address[1]<<"."<<(int)address[0];
    
    return s.str();
}

uint8_t IP4::operator [] (int n)
{
    return address[n];
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

MAC Interface::address()
{
    string tmp = read_str("address");
    return MAC(tmp);
}

IP4 Interface::ip4()
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    ifr.ifr_addr.sa_family = AF_INET;
    std::strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    struct sockaddr_in* sin = (struct sockaddr_in*)&ifr.ifr_addr;

    return IP4(sin->sin_addr.s_addr);
}

bool Interface::exists()
{
    fs::path sysfs = path;
    
    return fs::exists(sysfs);
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
