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
#include <experimental/filesystem>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
namespace fs=std::experimental::filesystem;
using namespace edupals::network;


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
    this->address[0]=address & 0x000000FF;
    this->address[1]=(address & 0x0000FF00)>>8;
    this->address[2]=(address & 0x00FF0000)>>16;
    this->address[3]=(address & 0xFF000000)>>24;
}

IP4::IP4(array<uint8_t,4> address) : address(address)
{
}

string IP4::to_string()
{
    stringstream s;
    
    s<<address[3]<<"."<<address[2]<<"."<<address[1]<<"."<<address[0];
    
    return s.str();
}

uint8_t IP4::operator [] (int n)
{
    return address[n];
}

Device::Device(string name)
{
    
    this->path="/sys/class/net/"+name;
    
    update();
}

void Device::update()
{
    // ignore empty device path
    if (this->path.size()==0) {
        return;
    }
    
    fs::path sysfs = this->path;
    
    // name is just final filename of path
    this->name=sysfs.filename();
    
     ifstream file;
     string tmp;

    // mac address
    fs::path address = sysfs / "address";
    
    file.open(address);
    std::getline(file,tmp);
    file.close();
    
    this->address=MAC(tmp);
    
    // carrier status
    fs::path carrier = sysfs / "carrier";
    
    file.open(carrier);
    std::getline(file,tmp);
    file.close();
    
    this->carrier=(tmp=="1");
        
    //mtu
    fs::path mtu = sysfs / "mtu";
    
    file.open(mtu);
    std::getline(file,tmp);
    file.close();
    
    this->mtu=std::stoi(tmp);
    
    //type
    fs::path type = sysfs / "type";
    
    file.open(type);
    std::getline(file,tmp);
    file.close();
    
    this->type=std::stoi(tmp);
}

vector<string> edupals::network::get_devices()
{
    vector<string> devices;
    
    fs::path sysfs("/sys/class/net");
    
    for (auto& dev: fs::directory_iterator(sysfs)) {
        devices.push_back(dev.path().filename());
    }
    
    return devices;
}
