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
#include <system.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <fstream>
#include <string>
#include <iostream>
#include <exception>

using namespace edupals::system;
using namespace std;

static void read_single_line(string path,string& dest)
{
    ifstream file;
    
    file.open(path.c_str());
    std::getline(file,dest);
    file.close();
}

Process::Process() : m_pid(-1)
{
    
}

Process::Process(int32_t pid) : m_pid(pid)
{
    
}

Process Process::me()
{
    return Process(getpid());
}

Process Process::parent()
{
    return Process(getppid());
}

vector<Process> Process::list()
{
    vector<Process> ret;
    vector<int32_t> pids = system::pids();
    
    for (int32_t p:pids) {
        ret.push_back(Process(p));
    }
    
    return ret;
}

string Process::proc()
{
    string path;
    
    if (m_pid<0) {
        path="/proc/self";
    }
    else {
        path="/proc/"+to_string(m_pid);
    }
    
    return path;
}

string Process::name()
{
    string tmp = cmdline();
    
    return tmp.substr(0,tmp.find(' ',0));
}

string Process::cmdline()
{
    string dest;
    
    string path=proc()+"/cmdline";
     
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

string Process::comm()
{
    string path = proc()+"/comm";
    string dest;
    
    read_single_line(path,dest);
    
    return dest;
}

char Process::state()
{
    string path = proc()+"/stat";
    string dest;
    
    read_single_line(path,dest);
    
    size_t p=dest.find(' ',0);
    p=dest.find(' ',p+1);
    return dest[p+1];
}

int32_t Process::pid()
{
    return m_pid;
}

int32_t Process::ppid()
{
    string path = proc()+"/stat";
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
    int ret = kill(m_pid, 0);
    
    return (ret == 0);
}

Process Process::spawn(string filename, vector<string> args, int* outfd, int* infd, int* errfd)
{
    int outpipe[2];
    int inpipe[2];
    int errpipe[2];

    if (outfd) {
        if (pipe(outpipe) == -1) {
            throw std::runtime_error("Failed to create output pipe");
        }
    }

    if (infd) {
        if (pipe(inpipe) == -1) {
            throw std::runtime_error("Failed to create input pipe");
        }
    }

    if (errfd) {
        if (pipe(errpipe) == -1) {
            throw std::runtime_error("Failed to create error pipe");
        }
    }

    pid_t pid = fork();

    if (pid == -1) {
        throw std::runtime_error("Failed to fork process");
    }

    if (pid == 0) {
        /* prepare args */
        char* argv[32];
        argv[0] = (char*)filename.c_str();
        int n = 1;
        int argc = std::min((size_t)31,args.size() + 1);

        while (n<argc) {
            argv[n] = (char*)args[n-1].c_str();
            n++;
        }
        argv[n] = nullptr;

        /* connect pipes */
        if (outfd) {
            dup2(outpipe[1], STDOUT_FILENO);
            close(outpipe[0]);
            close(outpipe[1]);
        }

        if (errfd) {
            dup2(errpipe[1], STDERR_FILENO);
            close(errpipe[0]);
            close(errpipe[1]);
        }

        if (infd) {
            dup2(inpipe[0], STDIN_FILENO);
            close(inpipe[0]);
            close(inpipe[1]);
        }

        execvp(filename.c_str(),argv);
    }
    else {
        if (outfd) {
            close(outpipe[1]);
            *outfd = outpipe[0];
        }

        if (errfd) {
            close(errpipe[1]);
            *errfd = errpipe[0];
        }

        if (infd) {
            close(inpipe[0]);
            *infd = inpipe[1];
        }

        return Process(pid);
    }

    return Process(0); // just here to fool compiler
}

int Process::wait()
{
    int status;
    int value;

    waitpid(m_pid, &status, 0);

    if (WIFEXITED(status)) {
        value = WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        value = -WTERMSIG(status);
    }

    return value;
}
