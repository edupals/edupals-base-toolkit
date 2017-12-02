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

#include <cstring>

#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <glob.h>

using namespace edupals::filesystem;
using namespace std;

void Path::sanitize()
{
    string sane;
    
    bool slash=false;
    
    for (char c:value) {
        if (c=='/') {
            if (slash==false) {
                sane+=c;
                slash=true;
            }
        }
        else {
            sane+=c;
            slash=false;
        }
    }
    
    value=sane;
}

Path::Path(string path)
{
    this->value=path;
    sanitize();
}

Path::Path(const char* path) : Path(string(path))
{

}

string Path::name()
{
    return value;
}

string Path::base()
{
    char * dup = new char[value.length()+1];
    strcpy(dup,value.c_str());
    string ret(basename(dup));
    
    delete [] dup;
    
    return ret;
}

string Path::dir()
{
    char * dup = new char[value.length()+1];
    strcpy(dup,value.c_str());
    string ret(dirname(dup));
    
    delete [] dup;
    
    return ret;
}

vector<Path> Path::list()
{
    vector<Path> content;
    
    DIR *dp;
    struct dirent *dirp;
    
    dp  = opendir(value.c_str());
    
    while ((dirp = readdir(dp)) != nullptr) {
        string path=this->value+"/"+dirp->d_name;
        
        content.push_back(Path(path));
    }
    
    return content;
}

vector<Path> Path::list(string expression)
{
    vector<Path> content;
    
    //TODO: Almost copy pasted, needs a deeper undestanding
    
    glob_t glob_result;
    
    string full = this->value+"/"+expression;
    glob(full.c_str(),GLOB_TILDE,nullptr,&glob_result);

    for (unsigned int n=0;n<glob_result.gl_pathc;n++) {
        content.push_back(Path(glob_result.gl_pathv[n]));
    }
    
    globfree(&glob_result);
    
    return content;
}

bool Path::exists()
{
    return (access( value.c_str(), F_OK )!=-1);
}

bool Path::is_dir()
{
    struct stat fflags;
    stat(value.c_str(),&fflags);
    
    return S_ISDIR(fflags.st_mode);
}

bool Path::is_relative()
{
    bool ret=false;
    
    if (value.size()>0) {
        ret=(value[0]!='/');
    }
    
    return ret;
}

Path Path::home()
{

    struct passwd *pw = getpwuid(getuid());

    return Path(pw->pw_dir);
}

Path Path::operator + (Path& right)
{
    string path;
    
    path = this->value + '/' + right.value;
    
    return Path(path);
}

Path Path::operator + (string& right)
{
    string path;
    
    path = this->value + '/' + right;
    
    return Path(path)
}

Path Path::operator + (const char* right)
{
    return (*this + string(right));
}

Path& operator += (Path& right)
{
    this->value=this->value + '/' + right.value;
    this->sanitize();
    
    return *this;
}

Path& operator += (const char* right)
{
    this->value=this->value + '/' + right;
    this->sanitize();
    
    return *this;
}

Path& operator += (string& right)
{
    this->value=this->value + '/' + right;
    this->sanitize();
    
    return *this;
}

bool Path::operator == (Path& right)
{
    return (value==right.value);
}

bool Path::operator != (Path& right)
{
    return (value!=right.value);
}

