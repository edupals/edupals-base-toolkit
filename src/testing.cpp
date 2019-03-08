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
#include <configfile.hpp>
#include <log.hpp>
#include <console.hpp>
#include <variant.hpp>

#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <chrono>

using namespace edupals;
using namespace edupals::variant;
using namespace std;

bool test_network()
{
    vector<string> devices;
    
    devices = network::get_devices();
    
    clog<<"Network devices:"<<endl;
    
    for (string& path:devices) {
        clog<<"* "<<console::fg::red<<path<<console::reset::all<<endl;
        
        network::Device dev(path);
        
        clog<<"\tName:"<<dev.name<<endl;
        clog<<"\tDevice:"<<dev.path<<endl;
        clog<<"\tType:"<<dev.type<<endl;
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
    
    lexer.signal_accepted(
        [](parser::DFA* dfa,string name) {
            clog<<"-- token: "<<name<<"="<<dfa->value()<<endl;
        }
    );
    
    lexer.signal_rejected(
        [](string expression) {
            clog<<"-- syntax error: "<<expression<<endl;
        }
    );
    
    stringstream sa;
    sa<<"( alfa (alfabeta=false) )(alfa)alfa=true) bet alfa";
     
    clog<<"Parsing: "<<sa.str()<<endl;
        
    lexer.parse(sa);
    
    
    stringstream sb;
    sb<<"( ) z";
    clog<<"Parsing: "<<sb.str()<<endl;
    lexer.parse(sb);
    
    
    stringstream sc;
    sc<<"alfabetaalfabetabeta";
    clog<<"Parsing: "<<sc.str()<<endl;
    lexer.parse(sc);

    return true;
}

bool test_config()
{

    configfile::Config ini("crap.ini");

    return true;
}

bool test_log()
{
    log::info<<"Testing colors:"<<endl<<endl;
    
    log::debug<<"testing debug"<<endl;
    log::info<<"testing info"<<endl;
    log::notice<<"testing notice"<<endl;
    log::warning<<"testing warning"<<endl;
    log::error<<"testing error"<<endl;
    
    log::info<<endl<<"Testing locks:"<<endl<<endl;

    std::thread workers[4];
    
    
    for (int n=0;n<4;n++) {
        workers[n]=std::thread([]() {
            log::notice<<"Created thread"<<endl;
            
            for (int m=0;m<10;m++) {
                log::debug<<"* "<<m<<endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            
            log::warning<<"bye"<<endl;
        }
        );
        
    }
    
    for (int n=0;n<4;n++) {
        workers[n].join();
    }
    
    return true;
}

bool test_variant()
{
    Variant a(32);
    Variant b(3.14f);
    Variant c=a;
    Variant d=20;
    Variant e=12.4f;
    Variant f="come get some!";
    
    vector<Variant> data={10,20,30.0,"40"};
    
    Variant array(data);
    Variant stuff=Variant::create_array(6);
    
    array[1]=15;
    a=64;
    stuff[0]="Amazing variant";
    
    clog<<"a: "<<a.get_int32()<<endl;
    clog<<"b: "<<b.get_float()<<endl;
    clog<<"c: "<<c.get_int32()<<endl;
    clog<<"d: "<<d.get_int32()<<endl;
    clog<<"e: "<<e.get_float()<<endl;
    clog<<"f: "<<f.get_string()<<endl;
    clog<<"0: "<<array[0].get_int32()<<endl;
    clog<<"1: "<<array[1].get_int32()<<endl;
    clog<<"2: "<<array[2].get_double()<<endl;
    clog<<"3: "<<array[3].get_string()<<endl;
    clog<<"0: "<<stuff[0].get_string()<<endl;
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
    
    if (!result.success()) {
        if (result.unknowns.size()>0) {
            cout<<"Unknown options: "<<endl;
            for (auto s: result.unknowns) {
                clog<<"\t"<<s<<endl;
            }
        }
        
        if (result.missings.size()>0) {
            clog<<"Missing argument for: "<<endl;
            for (auto s:result.missings) {
                clog<<"\t"<<s<<endl;
            }
        }
    }
    
    
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
            test_network();
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
        
        if (s=="config") {
            test_config();
        }
        
        if (s=="log") {
            test_log();
        }
        
        if (s=="variant") {
            test_variant();
        }
    }
    
    return 0;
}
