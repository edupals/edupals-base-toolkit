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
#include <log.hpp>
#include <console.hpp>
#include <variant.hpp>
#include <json.hpp>
#include <bson.hpp>
#include <base64.hpp>

#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <chrono>
#include <fstream>

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
    parser::token::Boolean boolean;
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
    
    stringstream sa;
    sa<<"( alfa (alfabeta=false) )(alfa)alfa=true) bet alfa";
    
    clog<<"Parsing: "<<sa.str()<<endl;
    
    lexer.set_input(&sa);
    
    while (lexer.step()) {
        clog<<"["<<lexer.get_token()<<"]:["<<lexer.get_dfa()->value()<<"]"<<endl;
    }
    
    if (!lexer.eof()) {
        clog<<"Unknown token"<<endl;
    }
    
    clog<<endl;
    
    stringstream sb;
    sb<<"alfabetaalfabetabetabe";
    clog<<"Parsing: "<<sb.str()<<endl;
    
    lexer.set_input(&sb);
    
    while (lexer.step()) {
        clog<<"["<<lexer.get_token()<<"]:["<<lexer.get_dfa()->value()<<"]"<<endl;
    }
    
    if (lexer.eof() and lexer.pending()) {
        clog<<"Unexpected EOF"<<endl;
    }
    
    clog<<endl;
    
    stringstream sc;
    sc<<"alfabeta alfa beta alfa";
    clog<<"Parsing: "<<sc.str()<<endl;
    
    lexer.set_input(&sc);
    
    while (lexer.step()) {
        clog<<"["<<lexer.get_token()<<"]:["<<lexer.get_dfa()->value()<<"]"<<endl;
    }
    
    if (lexer.eof() and !lexer.pending()) {
        clog<<"Parsed succesfully"<<endl;
    }
    
    clog<<endl;
    
    stringstream sd;
    sd<<"alfa gamma";
    clog<<"Parsing: "<<sd.str()<<endl;
    
    lexer.set_input(&sd);
    
    while (lexer.step()) {
        clog<<"["<<lexer.get_token()<<"]:["<<lexer.get_dfa()->value()<<"]"<<endl;
    }
    
    if (!lexer.eof() and !lexer.pending()) {
        clog<<"No match"<<endl;
    }
    
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
    Variant d=20L;
    Variant e=12.4;
    Variant f="come get some!";
    
    clog<<"std::to_string "<<std::to_string(e.get_double())<<endl;
    
    Variant args={1,{"two","three"},5,7.0,"9"};
    clog<<"list:"<<args<<endl;
    
    vector<Variant> data={10,20,30.0,"40"};
    vector<uint8_t> bytes={0xde,0xad,0xbe,0xef};
    
    Variant g=bytes;
    Variant h=true;
    
    bool ih=h;
    int32_t ia=a;
    int64_t id=d;
    float ib=b;
    double ie=e;
    string isf=f;
    
    clog<<"implicit conversions:"<<ih<<","<<ia<<","<<id<<","<<ib<<","<<ie<<","<<isf<<endl;
    
    Variant nv={h,a,d,b,e,f};
    clog<<nv<<endl;
    
    Variant array(data);
    Variant stuff=Variant::create_array(6);
    stuff.append();
    
    array[1]=15;
    a=64L;
    
    stuff[6]="Amazing variant";
    
    Variant message=Variant::create_struct();
    
    message["alfa"]="A";
    message["beta"]=1;
    message["gamma"]=2.0f;
    message["delta"]=Variant::create_struct();
    message["delta"]["k1"]=32767;
    message["delta"]["k2"]=Variant({11,22,33,44});
    
    clog<<array<<endl;
    clog<<message<<endl;
    Variant find =message["delta"]["k2"].find(1); 
    clog<<"find:"<<find<<endl;
    
    find = message/"delta"/"k2"/2/Type::Int32;
    clog<<"find:"<<find<<endl;
    
    clog<<"a: "<<a<<endl;
    clog<<"b: "<<b<<endl;
    clog<<"c: "<<c<<endl;
    clog<<"d: "<<d<<endl;
    clog<<"e: "<<e<<endl;
    clog<<"f: "<<f<<endl;
    clog<<"g: "<<g<<endl;
    clog<<"h: "<<h<<endl;
    clog<<"0: "<<array[0].get_int32()<<endl;
    clog<<"1: "<<array[1].get_int32()<<endl;
    clog<<"2: "<<array[2].get_double()<<endl;
    clog<<"3: "<<array[3].get_string()<<endl;
    clog<<"6: "<<stuff[6].get_string()<<endl;
    
    clog<<"alfa: "<<message["alfa"].get_string()<<endl;
    clog<<"beta: "<<message["beta"].get_int32()<<endl;
    clog<<"gamma: "<<message["gamma"].get_float()<<endl;
    clog<<"delta: "<<message["delta"]["k1"].get_int32()<<endl;
    
    auto tmp = g.get_bytes();
    clog<<"0x"<<hex<<(int)tmp[0]<<(int)tmp[1]<<(int)tmp[2]<<(int)tmp[3]<<endl;
    
    return true;
}

bool test_json()
{
    Variant msg=Variant::create_struct();
    
    msg["status"]=true;
    msg["value"]=Variant::create_struct();
    msg["value"]["name"]="edupals";
    msg["value"]["uid"]=1000;
    
    msg["value"]["groups"]=(Variant){100,200,201,202,203};
    
    json::dump(msg,cout);
    cout<<endl;
    
    stringstream input;

    input<<" { \"alfa\" : 33 , \"beta\": 4.325, \"gamma\":[7,8,9,[11,22,33]],\"status\":false,\"utf8\":\"¡ñ!\"}";
    Variant parsed=json::load(input);
    
    json::dump(parsed,cout);
    cout<<endl;
    
    return true;
}

bool test_bson()
{
    Variant msg=Variant::create_struct();
    
    msg["status"]=true;
    msg["value"]=Variant::create_struct();
    msg["value"]["name"]="edupals";
    msg["value"]["uid"]=1000;
    
    vector<Variant> groups={100,200,201,202,203};
    msg["value"]["groups"]=groups;
    
    /*
    msg["A"]=1;
    msg["B"]=2;
    msg["value"]=Variant::create_struct();
    msg["value"]["name"]="edupals";
    vector<Variant> groups={100,200,201,202,203};
    msg["value"]["groups"]=groups;
    */
    ofstream ofile("/tmp/edupals.bson");
    
    bson::dump(ofile,msg);
    
    ofile.close();
    
    ifstream ifile("/tmp/edupals.bson");
    
    msg = bson::load(ifile);
    
    ifile.close();
    
    json::dump(msg,cout);
    
    return true;
}

bool test_base64()
{
    const string expected_ascii="edupals.base64";
    const string expected_b64="ZWR1cGFscy5iYXNlNjQ=";
    
    clog<<"references:"<<endl;
    clog<<"Ascii:"<<expected_ascii<<endl;
    clog<<"Base64:"<<expected_b64<<endl;
    
    vector<uint8_t> data(expected_ascii.begin(),expected_ascii.end());
    string b64;
    
    base64::encode(data,b64);
    
    clog<<"Encoded:"<<b64<<endl;
    
    if (b64!=expected_b64) {
        cerr<<console::fg::red<<"Base64 encoding fail"<<console::reset::all<<endl;
    }
    
    data.clear();
    b64=expected_b64;
    
    base64::decode(b64,data);
    
    clog<<"Decoded:";
    for (uint8_t c:data) {
        clog<<c;
    }
    clog<<endl;
    
    string ascii(data.begin(),data.end());
    
    if (ascii!=expected_ascii) {
        cerr<<console::fg::red<<"Base64 decoding fail"<<console::reset::all<<endl;
    }
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
        
        if (s=="log") {
            test_log();
        }
        
        if (s=="variant") {
            test_variant();
        }
        
        if (s=="json") {
            test_json();
        }
        
        if (s=="bson") {
            test_bson();
        }
        
        if (s=="base64") {
            test_base64();
        }
    }
    
    return 0;
}
