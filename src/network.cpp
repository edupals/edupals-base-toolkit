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
#include <filesystem.hpp>

#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
using namespace edupals::network;
using namespace edupals::filesystem;


MACAddress::MACAddress(array<uint8_t,6> address) : address(address)
{
}

string MACAddress::to_string()
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

uint8_t MACAddress::operator [] (int n)
{
    return address[n];
}

IP4Address::IP4Address(uint32_t address)
{
    this->address[0]=address & 0x000000FF;
    this->address[1]=(address & 0x0000FF00)>>8;
    this->address[2]=(address & 0x00FF0000)>>16;
    this->address[3]=(address & 0xFF000000)>>24;
}

IP4Address::IP4Address(array<uint8_t,4> address) : address(address)
{
}

string IP4Address::to_string()
{
    stringstream s;
    
    s<<address[3]<<"."<<address[2]<<"."<<address[1]<<"."<<address[0];
    
    return s.str();
}

uint8_t IP4Address::operator [] (int n)
{
    return address[n];
}

vector<Device> edupals::network::get_devices()
{
    vector<Device> devices;
    
    Path sysfs("/sys/class/net");
    
    for (Path& dev:sysfs.list()) {
        
        string name;
        name=dev.base();
        
        if (name=="." or name=="..") {
            continue;
        }
        
        Device device;
        
        device.name=name;
        
        Path property("address");
        
        Path address=dev+property;
        
        ifstream file;
        string tmp;
        
        clog<<address.name()<<endl;
        
        file.open(address.name());
        std::getline(file,tmp);
        file.close();
        
        clog<<"mac:"<<tmp<<endl;
        
        devices.push_back(device);
    }
    
    /*
    int sck;
    struct ifconf ifc;
    struct ifreq* ifr;
    
    char* buffer;
    const size_t len=sizeof(ifreq)*32; //room for 32 devices
    
    sck = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sck<0) {
        //TODO: error
    }
    
    buffer = new char[len];
    
    ifc.ifc_len = len;
    ifc.ifc_buf = buffer;
    
    if (ioctl(sck, SIOCGIFCONF, &ifc) < 0) {
        //TODO: error
    }
    
    ifr = ifc.ifc_req;
    int num_ifaces = ifc.ifc_len / sizeof(struct ifreq);
    
    for (int n=0;n<num_ifaces;n++) {
    
        struct ifreq *item = &ifr[n];
        
        Device device;
        
        device.name=item->ifr_name;
        
        devices.push_back(device);
    
    }
    
    close(sck);
    
    delete [] buffer;
    */
    return devices;
}
