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

#include <stdexcept>

using namespace edupals::system;
using namespace std;

Group::Group(struct group* gr)
{
    build(gr);
}

Group::Group(gid_t gid)
{
    struct group* gr=getgrgid(gid);
    
    if (gr==nullptr) {
        throw exception::GroupNotFound();
    }
    
    build(gr);
}

Group::Group(const char* name)
{
    struct group* gr=getgrnam(name);
    
    if (gr==nullptr) {
        throw exception::GroupNotFound();
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
    
    L1:
    errno=0;
    gr=getgrent();
    
    if(!gr) {
        if (errno!=0) {
            throw runtime_error("Error reading group database");
        }
        
        endgrent();
    }
    else {
        groups.push_back(Group(gr));
        goto L1;
    }
    
    return groups;
}