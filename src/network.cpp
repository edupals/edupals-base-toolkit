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
    
    fs::path sysfs("/sys/class/net");
    
    for (auto& dev: fs::directory_iterator(sysfs)) {
    
        Device device;
        
        //device kernel name
        device.name=dev.path().filename();

        ifstream file;
        string tmp;

        // mac address
        fs::path address = dev.path() / "address";
        
        file.open(address);
        std::getline(file,tmp);
        file.close();
        
        device.address=MAC(tmp);
        
        //carrier
        fs::path carrier = dev.path() / "carrier";
        
        file.open(carrier);
        std::getline(file,tmp);
        file.close();
        
        device.carrier=(tmp=="1");
        
        //mtu
        fs::path mtu = dev.path() / "mtu";
        
        file.open(mtu);
        std::getline(file,tmp);
        file.close();
        
        device.mtu=std::stoi(tmp);
        
        devices.push_back(device);
    }
    
    
   return devices;
}
