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
#include <network.hpp>
#include <filesystem.hpp>
#include <cmd.hpp>
#include <process.hpp>
#include <workqueue.hpp>
#include <token.hpp>
#include <lexer.hpp>

#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>


using namespace edupals;
using namespace std;

bool test_network_get_devices()
{
    vector<network::Device> devices;
    
    devices = network::get_devices();
    
    clog<<"Network devices:"<<endl;
    
    for (auto& dev:devices) {
        clog<<"* "<<dev.name<<endl;
        clog<<"\tAddress:"<<dev.address.to_string()<<endl;
        clog<<"\tcarrier:"<<dev.carrier<<endl;
        clog<<"\tmtu:"<<dev.mtu<<endl;
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

bool test_system_get_pids()
{
    vector<int32_t> pids = system::get_pids();
    
    clog<<"Process id:"<<endl;
    
    for (int32_t pid : pids) {
        clog<<pid<<endl;
    }
    
    return true;
}

bool test_process()
{
    system::Process p;
    
    clog<<"* process:"<<endl;
    clog<<"    - cmdline: ["<<p.get_cmdline()<<"]"<<endl;
    clog<<"    - comm: ["<<p.get_comm()<<"]"<<endl;
    clog<<"    - state: ["<<p.get_state()<<"]"<<endl;
    clog<<"    - ppid: ["<<p.get_ppid()<<"]"<<endl;
    
    return true;
}

bool test_threading()
{
    threading::WorkQueue<int> queue(8);
    mutex cm;
    
    clog<<"* threading:"<<endl;
    
    
    thread producer([&]() mutable {
    
        for (int n=0;n<32;n++) {
            queue.push(n);
            cm.lock();
            clog<<"pushed "<<n<<endl;
            cm.unlock();
        }
    
    });
    
    
    thread consumer([&]() mutable {
    
        int v=0;
        
        while(v!=31) {
            v=queue.pop();
            cm.lock();
            clog<<"poped "<<v<<endl;
            cm.unlock();
        }
        
       
    });
    
    
    producer.join();
    consumer.join();
    
    return true;
}

bool test_filesystem()
{
    auto files=filesystem::glob("/dev/tty*");
    
    clog<<"* filesystem: "<<endl;
    
    for (auto file : files) {
        clog<<"- "<<file<<endl;
    }
    
    return true;
}

bool test_parser()
{

    stringstream ss;
    
    ss<<"( alfa (alfabeta=false) )(alfa)alfa=true) bet alfa";
    
    parser::Lexer lexer;
    
    parser::token::Char ws(' ');
    parser::token::Char left('(');
    parser::token::Char right(')');
    parser::token::Char equal('=');
    parser::token::Char cr('\n');
    parser::token::Bool boolean;
    parser::token::Word alfa("alfa");
    parser::token::Word beta("beta");
    parser::token::Word alfabeta("alfabeta");
    
    lexer.add_token("WS",&ws);
    lexer.add_token("LEFT",&left);
    lexer.add_token("RIGHT",&right);
    lexer.add_token("EQUAL",&equal);
    lexer.add_token("CR",&cr);
    lexer.add_token("BOOL",&boolean);
    lexer.add_token("ALFABETA",&alfabeta);
    lexer.add_token("ALFA",&alfa);
    lexer.add_token("BETA",&beta);
    
    
    lexer.parse(ss);
    
    return true;
}

int main (int argc,char* argv[])
{

    cmd::ArgumentParser parser;
    cmd::ParseResult result;
    
    //parser.add_option(cmd::Option('a',"append",cmd::ArgumentType::Required));
    //parser.add_option(cmd::Option('n',"level",cmd::ArgumentType::Optional));
    parser.add_option(cmd::Option('h',"help",cmd::ArgumentType::None));
    parser.add_option(cmd::Option('v',"version",cmd::ArgumentType::None));
    
    result=parser.parse(argc,argv);
    /*
    clog<<"options:"<<endl;
    
    for (cmd::Option& o:result.options) {
        clog<<"* ["<<o.short_name<<"]"<<o.long_name<<":"<<o.value<<endl;
    }
    
    clog<<endl<<"args:"<<endl;
    
    for (string s:result.args) {
        clog<<"* "<<s<<endl;
    }
    */
    
    for (string s:result.args) {
        if (s=="system") {
            test_system_version();
            test_system_uptime();
            test_system_cmdline();
            test_system_memory();
            test_system_get_pids();
            test_system_get_modules();
        }
        
        if (s=="network") {
            test_network_get_devices();
        }
        
        if (s=="process") {
            test_process();
        }
        
        if (s=="threading") {
            test_threading();
        }
        
        if (s=="filesystem") {
            test_filesystem();
        }
        
        if (s=="parser") {
            test_parser();
        }
    }
    
    return 0;
}
