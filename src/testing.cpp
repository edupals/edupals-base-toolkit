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
#include "filesystem.hpp"

#include <iostream>


using namespace edupals;
using namespace std;

bool test_filesystem()
{

    filesystem::Path usr("/usr");
    filesystem::Path share("share");

    filesystem::Path path=usr+share;
    
    if (path.exists()) {
    
        clog<<"list:"<<path.name()<<endl;
        for (auto & d:path.list()) {
            if (d.is_dir()) {
                clog<<"[D]"<<d.name()<<endl;
            }
            else {
                clog<<d.name()<<endl;
            }
        }
    }
    
    return true;
}

bool test_system_version()
{

    clog<<"* version: "<<system::version()<<endl;
    
    return true;
}

bool test_system_uptime()
{

    clog<<"* uptime: "<<system::uptime()<<endl;
    
    return true;
}

bool test_system_cmdline()
{

    clog<<"* cmdline: "<<system::cmdline()<<endl;
    
    return true;
}

bool test_system_memory()
{
    clog<<"* total memory: "<<system::get_total_memory()/1024<<" kB"<<endl;
    clog<<"* free memory: "<<system::get_free_memory()/1024<<" kB"<<endl;
    return true;
}

bool test_system_get_modules()
{
    vector<string> modules = system::get_modules();
    
    clog<<"* modules:"<<endl;
    for (string& module : modules) {
        clog<<"->"<<module<<endl;
    }
    
    return true;
}


int main (int argc,char* argv[])
{

    test_filesystem();

    test_system_version();
    test_system_uptime();
    test_system_cmdline();
    test_system_memory();
    test_system_get_modules();
    
    return 0;
}
