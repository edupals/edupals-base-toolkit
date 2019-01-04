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
 

#include <filesystem.hpp>

#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <glob.h>
#include <sys/inotify.h>

#include <cstring>
#include <stdexcept>

using namespace std;
namespace fs=std::experimental::filesystem;

INotify::INotify(string path)
{
    ifd = inotify_init();
    
    if (ifd==-1) {
        string err=std::to_string(errno);
        
        throw runtime_error("inotify_init: "+err);
    }
    
    wfd = notify_add_watch(ifd, path.c_str(), IN_ALL_EVENTS);

    if (wfd==-1) {
        string err=std::to_string(errno);
        
        throw runtime_error("inotify_add_watch: "+err);
    }
}

INotify::~INotify()
{
    if (wfd>-1) {
        inotify_rm_watch( ifd, wfd );
    }
    
    if (ifd>-1) {
        close(ifd);
    }
}

void INotify::push()
{
}

void INotify::event()
{
}

vector<fs::path> edupals::filesystem::glob(string expression)
{
    vector<fs::path> tmp;
    
    glob_t glob_result;
    glob(expression.c_str(),GLOB_TILDE,nullptr,&glob_result);

    for (unsigned int n=0;n<glob_result.gl_pathc;n++) {
        tmp.push_back(fs::path(glob_result.gl_pathv[n]));
    }
    
    globfree(&glob_result);

    
    return tmp;
}

