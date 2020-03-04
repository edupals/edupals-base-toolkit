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

void User::build(struct passwd* pw)
{
    name=pw->pw_name;
    password=pw->pw_passwd;
    uid=pw->pw_uid;
    gid=pw->pw_gid;
    gecos=pw->pw_gecos;
    dir=pw->pw_dir;
    shell=pw->pw_shell;
}