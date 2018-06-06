/*
 * Copyright (C) 2018 Edupals project
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
 

#include <process.hpp>

#include <fstream>
#include <string>

using namespace edupals::system;
using namespace std;

Process::Process()
{
    pid=-1;
}

Process::Process(int32_t pid)
{
    this->pid=pid;
}

string Process::get_name()
{
    return get_cmdline();
}

string Process::get_cmdline()
{

    string dest;
    string path;
    
    if (pid<0) {
        path="/proc/self/cmdline";
    }
    else {
        path="/proc/"+to_string(pid)+"/cmdline";
    }

    ifstream file;
    
    file.open(path.c_str());
    std::getline(file,dest);
    file.close();
    
    return dest;
}
