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

#include <group.hpp>
#include <user.hpp>

#include <stdexcept>

using namespace edupals::system;
using namespace std;

Group::Group(struct group* gr)
{
    if (gr) {
        build(gr);
    }
}

Group::Group(gid_t gid)
{
    errno=0;
    struct group* gr=getgrgid(gid);
    
    if (!gr) {
        throw exception::GroupNotFound(gid,errno);
    }
    else {
        build(gr);
    }
    
}

Group::Group(const char* name)
{
    errno=0;
    struct group* gr=getgrnam(name);
    
    if (!gr) {
        throw exception::GroupNotFound(name,errno);
    }
    
    build(gr);
}

Group::Group() : gid(-1)
{
}

void Group::build(struct group* gr)
{
    name=gr->gr_name;
    gid=gr->gr_gid;
}

vector<Group> Group::list()
{
    vector<Group> groups;
    
    struct group* gr;
    
    setgrent();
    
    L1:
    errno=0;
    gr=getgrent();
    
    if(!gr) {
        endgrent();
    }
    else {
        groups.push_back(Group(gr));
        goto L1;
    }
    
    return groups;
}

vector<User> Group::users()
{
    vector<User> ret;
    
    errno = 0;
    struct group* gr = getgrgid(gid);
    
    if(!gr) {
        throw exception::GroupDatabaseError(errno);
    }
    
    size_t n=0;
    
    char* name;
    
    L1:
    
    name=gr->gr_mem[n];
    
    if(name) {
        n++;
        ret.push_back(User(name));
        goto L1;
    }
    
    return ret;
}
