/*
 * Copyright (C) 2020 Edupals project
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

#include <user.hpp>
#include <group.hpp>

#include <unistd.h>

#include <stdexcept>

using namespace edupals::system;
using namespace std;

User::User(struct passwd* pw)
{
    build(pw);
}

User::User(uid_t uid)
{
    struct passwd* pw=getpwuid(uid);
    
    if(pw==nullptr) {
        throw exception::UserNotFound();
    }
    
    build(pw);
}

User::User(const char* name)
{
    struct passwd* pw=getpwnam(name);
    
    if(pw==nullptr) {
        throw exception::UserNotFound();
    }
    
    build(pw);
}

User::User() : uid(-1)
{
}

User User::me()
{
    return User(getuid());
}

void User::build(struct passwd* pw)
{
    name=pw->pw_name;
    password=pw->pw_passwd;
    uid=pw->pw_uid;
    gid=pw->pw_gid;
    gecos=pw->pw_gecos;
    home=pw->pw_dir;
    shell=pw->pw_shell;
}

vector<User> User::list()
{
    vector<User> users;
    
    struct passwd* pw;
    
    L1:
    errno=0;
    pw=getpwent();
    
    if(!pw) {
        if (errno!=0) {
            throw runtime_error("Error reading passwd database");
        }
        
        endpwent();
    }
    else {
        users.push_back(User(pw));
        goto L1;
    }
    
    return users;
}

Group User::group()
{
    Group gr(gid);
    
    return gr;
}

vector<Group> User::groups()
{
    vector<Group> ret;
    
    //push main gid
    ret.push_back(Group(this->gid));
    
    struct group* gr;
    
    setgrent();
    
    L1:
    
    errno=0;
    gr=getgrent();
    
    if (!gr) {
        if (errno!=0) {
            throw runtime_error("Error reading group database");
        }
        endgrent();
    }
    else {
        
        if (gr->gr_gid!=this->gid) {
            char** member=gr->gr_mem;
            
            while(*member) {
                if (this->name==*member) {
                    ret.push_back(Group(gr));
                    break;
                }
                member++;
            }
        }
        
        goto L1;
    }
    
    return ret;
}