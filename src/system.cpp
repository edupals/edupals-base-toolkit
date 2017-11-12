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
 

#include "system.hpp"

#include <fstream>

#include <unistd.h>

using namespace std;

static void read_single_line(string path,string& dest)
{
    ifstream file;
    
    file.open(path.c_str());
    std::getline(file,dest);
    file.close();
}

string edupals::system::version()
{
    string version;
    
    read_single_line("/proc/version",version);
    
    return version;
}

double edupals::system::uptime()
{
    ifstream file;
    string uptime;
    
    file.open("/proc/uptime");
    file>>uptime;
    file.close();
    
    return std::stod(uptime);
}

string edupals::system::cmdline()
{
    string cmdline;
    
    read_single_line("/proc/cmdline",cmdline);
    
    return cmdline;
}

uint64_t edupals::system::get_total_memory()
{
    uint64_t pages = sysconf(_SC_PHYS_PAGES);
    uint64_t page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

uint64_t edupals::system::get_free_memory()
{
    uint64_t pages = sysconf(_SC_AVPHYS_PAGES);
    uint64_t page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

vector<string> edupals::system::get_modules()
{
    //TODO: Crappy implementation
    vector<string> modules;
    ifstream file;
    char buffer[256];
    
    file.open("/proc/modules");
    
    while (!file.eof()) {
        
        string name;
        
        file>>name;
        file.getline(buffer,256);
        
        if (name.size()>0) {
            modules.push_back(name);
        }
    }
    
    file.close();
    
    return modules;
}
