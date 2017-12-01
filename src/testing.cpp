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
 
 
#include <system.hpp>
#include <filesystem.hpp>
#include <network.hpp>
#include <cmd.hpp>

#include <iostream>


using namespace edupals;
using namespace std;

bool test_network_get_devices()
{
    vector<network::Device> devices;
    
    devices = network::get_devices();
    
    clog<<"Network devices:"<<endl;
    
    for (auto& dev:devices) {
        clog<<"* "<<dev.name<<endl;
    }
}

bool test_filesystem()
{

    filesystem::Path usr("/usr");
    filesystem::Path share("share");

    filesystem::Path path=usr+share;
    
    if (path.exists()) {
    
        clog<<"list:"<<path.name()<<endl;
        for (auto & d:path.list("*gnome*")) {
            if (d.is_dir()) {
                clog<<"[D]"<<d.name()<<endl;
            }
            else {
                clog<<d.name()<<endl;
            }
        }
    }
    
    filesystem::Path usrshare("/usr/share");
    
    clog<<usrshare.name()<<"=="<<path.name()<<":"<<(usrshare==path)<<endl;
    
    
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

bool test_system_get_pids()
{
    vector<int32_t> pids = system::get_pids();
    
    clog<<"Process id:"<<endl;
    
    for (int32_t pid : pids) {
        clog<<pid<<endl;
    }
    
    return true;
}

int main (int argc,char* argv[])
{

    cmd::ArgumentParser parser;
    cmd::ParseResult result;
    
    parser.add_option(cmd::Option('a',"append",cmd::ArgumentType::Required));
    parser.add_option(cmd::Option('n',"level",cmd::ArgumentType::Optional));
    parser.add_option(cmd::Option('h',"help",cmd::ArgumentType::None));
    parser.add_option(cmd::Option('v',"version",cmd::ArgumentType::None));
    
    parser.add_option(cmd::Option('1',cmd::ArgumentType::None));
    parser.add_option(cmd::Option('2',cmd::ArgumentType::None));
    parser.add_option(cmd::Option('3',cmd::ArgumentType::None));
    
    
    result=parser.parse(argc,argv);
    
    clog<<"options:"<<endl;
    
    for (cmd::Option& o:result.options) {
        clog<<"* ["<<o.short_name<<"]"<<o.long_name<<":"<<o.value<<endl;
    }
    
    clog<<endl<<"args:"<<endl;
    
    for (string s:result.args) {
        clog<<"* "<<s<<endl;
    }
    
    test_network_get_devices();
    /*
    test_filesystem();

    test_system_version();
    test_system_uptime();
    test_system_cmdline();
    test_system_memory();
    test_system_get_pids();
    test_system_get_modules();
    */
    return 0;
}
