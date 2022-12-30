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
#include <user.hpp>
#include <group.hpp>
#include <workqueue.hpp>
#include <token.hpp>
#include <lexer.hpp>
#include <log.hpp>
#include <console.hpp>
#include <variant.hpp>
#include <json.hpp>
#include <bson.hpp>
#include <base64.hpp>
#include <uri.hpp>

#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <chrono>
#include <fstream>
#include <functional>

using namespace edupals;
using namespace edupals::variant;
using namespace edupals::network;
using namespace std;

struct Test
{
    string name;
    std::function<bool()> fn;
    bool passed;
    
    Test(string name,std::function<bool()> fn)
    {
        this->name=name;
        this->fn=fn;
        this->passed=false;
    }
};

bool test_network()
{
    vector<network::Interface> ifaces;
    
    network::Interface::update();
    ifaces = network::Interface::list();
    
    clog<<"Network interfaces:"<<endl;
    
    for (network::Interface& iface:ifaces) {
        clog<<"* "<<console::fg::red<<iface.path<<console::reset::all<<endl;
        
        clog<<"\tName:"<<iface.name<<endl;
        clog<<"\tDevice:"<<iface.path<<endl;
        clog<<"\tType:"<<iface.type()<<endl;
        clog<<"\tUp:"<<iface.up()<<endl;
        clog<<"\tLoopback:"<<iface.loopback()<<endl;
        clog<<"\tBroadcast/p2p:"<<iface.broadcast()<<"/"<<iface.p2p()<<endl;
        clog<<"\tMulticast:"<<iface.multicast()<<endl;
        clog<<"\tAddress:"<<iface.hwaddress()<<endl;
        clog<<"\tBroadcast:"<<iface.hwbroadcast()<<endl;
        clog<<"\tCarrier:"<<iface.carrier()<<endl;
        clog<<"\tMTU:"<<iface.mtu()<<endl;

        clog<<endl;
        clog<<"\tAddresses:"<<endl;

        map<int,string> family = {
            {AF_PACKET,"link"},
            {AF_INET,"ipv4"},
            {AF_INET6,"ipv6"}
        };

        for (network::IFAddress& address : iface.addresses()) {
            clog<<"\t\t* "<<family[address.family()]<<" "<<address.address()<<"/"<<network::maskbits(address.netmask())<<endl;
        }
    }

    struct in_addr ip1 = network::ip4("192.168.0.32");
    struct in_addr ip2 = network::ip4("192.168.1.31");
    struct in_addr m1 = network::ip4("255.255.255.0");
    struct in_addr s1 = network::ip4("192.168.0.0");



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
    clog<<"* total memory: "<<system::total_memory()/1024<<" kB"<<endl;
    clog<<"* free memory: "<<system::free_memory()/1024<<" kB"<<endl;
    return true;
}

bool test_system_modules()
{
    vector<string> modules = system::modules();
    
    clog<<"* modules:"<<endl;
    for (string& module : modules) {
        clog<<"->"<<module<<endl;
    }
    
    return true;
}

bool test_system_pids()
{
    vector<int32_t> pids = system::pids();
    
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
    clog<<"    - cmdline: ["<<p.cmdline()<<"]"<<endl;
    clog<<"    - comm: ["<<p.comm()<<"]"<<endl;
    clog<<"    - state: ["<<p.state()<<"]"<<endl;
    clog<<"    - ppid: ["<<p.ppid()<<"]"<<endl;
    
    return true;
}

bool test_user()
{
    system::User user=system::User::me();
    
    clog<<"user:"<<user.uid<<endl;
    clog<<"\tname:"<<user.name<<endl;
    clog<<"\tgecos:"<<user.gecos<<endl;
    clog<<"\thome:"<<user.home<<endl;
    clog<<"\tshell:"<<user.shell<<endl;
    clog<<endl;
    
    vector<system::User> users = system::User::list();
    
    for (system::User& user:users) {
        clog<<"* "<<user.name<<":"<<user.password<<":"<<user.uid<<":"<<user.group().name<<endl;
    }
    
    return true;
}

bool test_group()
{
    vector<system::Group> groups;
    
    groups = system::Group::list();
    
    for (system::Group& group:groups) {
        clog<<"* "<<group.gid<<":"<<group.name<<endl;
    }
    
    clog<<endl;
    
    clog<<"sudo:"<<endl;
    system::Group group("adm");
    vector<system::User> users = group.users();
    
    for (system::User& user:users) {
        clog<<"* "<<user.name<<endl;
    }
    
    clog<<endl;
    system::User user=system::User::me();
    clog<<"User:"<<user.name<<endl;
    
    groups=user.groups();
    for (system::Group& group:groups) {
        clog<<"* "<<group.gid<<":"<<group.name<<endl;
    }
    
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
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
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
    bool status = true;
    
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
    else {
        status=false;
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
    else {
        status=false;
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
    else {
        status=false;
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
    else {
        status=false;
    }
    
    return status;
}

bool test_log()
{
    std::thread workers[4];
    
    for (int n=0;n<4;n++) {
        workers[n]=std::thread([]() {
            stringstream s;
            s<<"["<<std::this_thread::get_id()<<"] ";
            log::Log out(console::fg::blue,s.str());
            out<<"created thread"<<endl;
            
            for (int m=0;m<10;m++) {
                out<<"* "<<m<<endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            
            out<<"bye"<<endl;
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
    
    const char* index="alfa";
    message[index]="A";
    message["beta"]=1;
    message["gamma"]=2.2f;
    clog<<"p1"<<endl;
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
    clog<<"gamma: "<<message["gamma"].to_int32()<<endl;
    clog<<"delta: "<<message["delta"]["k1"].get_int32()<<endl;
    
    auto tmp = g.get_bytes();
    clog<<"0x"<<hex<<(int)tmp[0]<<(int)tmp[1]<<(int)tmp[2]<<(int)tmp[3]<<endl;
    clog<<dec;
    
    clog<<"iteration:"<<endl;
    if (message.is_struct()) {
        for (auto kv : message.get_struct()) {
            clog<<kv.second<<endl;
        }
    }
    
    Variant lima = Variant::create_struct();
    Variant mike = lima;
    Variant lima_a = lima["a"];
    clog<<"lima:"<<lima<<endl;
    lima["a"]=3;
    lima["a"]=4;
    clog<<"lima:"<<lima<<endl;
    clog<<"lima_a:"<<lima_a<<endl;
    clog<<"mike:"<<mike<<endl;
    
    return true;
}

bool test_json()
{
    bool status = true;
    
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
    
    try {
        
        Variant tmp;
        
        tmp = parsed/"alfa"/Type::Int32;
        if (tmp.get_int32()!=33) {
            status = false;
        }
        tmp = parsed/"beta"/Type::Float;
        if (tmp.get_float()<4.0f or tmp.get_float()>5.0f) {
            status = false;
        }
        tmp = parsed/"status"/Type::Boolean;
        
        if (tmp.get_boolean()==true) {
            status = false;
        }
        
        status = true;
    }
    catch(...) {
        status = false;
    }
    
    json::dump(parsed,cout);
    cout<<endl;
    
    return status;
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
    
    bson::dump(msg,ofile);
    
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

bool test_uri()
{
    string address = "http://noname@80.20.200.1/alfa/beta/gamma";
    
    clog<<"url: "<<address<<endl;
    uri::Uri url(address);
    
    clog<<"scheme:"<<url.scheme<<endl;
    clog<<"user:"<<url.user<<endl;
    clog<<"host:"<<url.host<<endl;
    clog<<"ip:"<<url.ip<<endl;
    clog<<"port:"<<url.port<<endl;
    clog<<"path:"<<url.path<<endl;
    
    clog<<endl;
    address = "my-protocol://net.lliurex/nopath";
    
    clog<<"url: "<<address<<endl;
    url=uri::Uri(address);
    
    clog<<"scheme:"<<url.scheme<<endl;
    clog<<"user:"<<url.user<<endl;
    clog<<"host:"<<url.host<<endl;
    clog<<"ip:"<<url.ip<<endl;
    clog<<"port:"<<url.port<<endl;
    clog<<"path:"<<url.path<<endl;
    
    return true;
}

bool evaluate(std::function<bool()> test_function)
{
    try {
        return test_function();
    }
    catch(std::exception& e) {
        cerr<<e.what()<<endl;
        return false;
    }
}

int main (int argc,char* argv[])
{
    map<string,bool> to_test;
    
    map<string,vector<Test> > tests;
    
    // system tests
    tests["system"].push_back(Test("version",test_system_version));
    tests["system"].push_back(Test("uptime",test_system_uptime));
    tests["system"].push_back(Test("cmdline",test_system_cmdline));
    tests["system"].push_back(Test("memory",test_system_memory));
    tests["system"].push_back(Test("pids",test_system_pids));
    tests["system"].push_back(Test("modules",test_system_modules));
    
    tests["workqueue"].push_back(Test("workqueue",test_threading));
    
    tests["process"].push_back(Test("process",test_process));
    
    tests["network"].push_back(Test("network",test_network));
    
    tests["user"].push_back(Test("user",test_user));
    
    tests["group"].push_back(Test("group",test_group));
    
    //parser
    tests["parser"].push_back(Test("parser",test_parser));
    
    //variant
    tests["variant"].push_back(Test("variant",test_variant));
    
    //json
    tests["json"].push_back(Test("json",test_json));
    
    //bson
    tests["bson"].push_back(Test("bson",test_bson));
    
    //base64
    tests["base64"].push_back(Test("base64",test_base64));
    
    //base64
    tests["log"].push_back(Test("log",test_log));
    
    //uri
    tests["uri"].push_back(Test("uri",test_uri));
    
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
        to_test[s]=true;
    }
    
    clog<<"Testing..."<<endl<<endl;
    int test_done=0;
    int test_passed=0;
    
    map<string,bool> test_results;
    
    for (auto t:to_test) {
        if (t.second) {
            clog<<"**********"<<endl;
            clog<<"  "<<t.first<<endl;
            clog<<"**********"<<endl<<endl;
            
            for (Test& test: tests[t.first]) {
                test_done++;
                bool result = evaluate(test.fn);
                if (result) {
                    test_passed++;
                    test.passed=result;
                }
                
                test_results[t.first+":"+test.name]=result;
            }
        }
    }
    
    clog<<endl<<"Results:"<<endl;
    
    for (auto r: test_results) {
        clog<<"* "<<r.first<<" "<<(r.second ? "pass" : "fail")<<endl;
    }
    
    clog<<std::dec<<"Test passed: ("<<test_passed<<"/"<<test_done<<")"<<endl;
    
    return ((test_done-test_passed)==0) ? 0:1;
}
