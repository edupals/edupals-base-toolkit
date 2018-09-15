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

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include <fstream>
#include <string>
#include <iostream>

using namespace edupals::system;
using namespace std;

static void read_single_line(string path,string& dest)
{
    ifstream file;
    
    file.open(path.c_str());
    std::getline(file,dest);
    file.close();
}

Process::Process()
{
    pid=-1;
}

Process::Process(int32_t pid)
{
    this->pid=pid;
}

Process Process::me()
{
    return Process(getpid());
}

Process Process::parent()
{
    return Process(getppid());
}

string Process::get_proc()
{
    string path;
    
    if (pid<0) {
        path="/proc/self";
    }
    else {
        path="/proc/"+to_string(pid);
    }
    
    return path;
}

string Process::get_name()
{
    string cmdline = get_cmdline();
    
    return cmdline.substr(0,cmdline.find(' ',0));
}

string Process::get_cmdline()
{

    string dest;
    
    string path=get_proc()+"/cmdline";
    
    
    ifstream file;
    
    file.open(path.c_str());
    
    char c;
    while (file.get(c)) {
        
        if (c=='\0') {
            c=' ';
        }
        dest+=c;
    }
    
    file.close();
    
    dest.pop_back();
    
    return dest;
}

string Process::get_comm()
{
    string path = get_proc()+"/comm";
    string dest;
    
    read_single_line(path,dest);
    
    return dest;
}

char Process::get_state()
{
    string path = get_proc()+"/stat";
    string dest;
    
    read_single_line(path,dest);
    
    size_t p=dest.find(' ',0);
    p=dest.find(' ',p+1);
    return dest[p+1];
}

int32_t Process::get_pid()
{
    return this->pid;
}

int32_t Process::get_ppid()
{
    string path = get_proc()+"/stat";
    string dest;
    
    read_single_line(path,dest);
    
    size_t p=dest.find(' ',0);
    p=dest.find(' ',p+1);
    p=dest.find(' ',p+1);
    size_t q=dest.find(' ',p+1);
    
    string tmp = dest.substr(p,q-p);
    
    return stoi(tmp);
}

bool Process::exists()
{
    int ret=kill(pid,0);
    
    return (ret==0);
}
