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

#ifndef EDUPALS_USER
#define EDUPALS_USER

#include <pwd.h>

#include <string>
#include <exception>

namespace edupals
{
    namespace system
    {
        namespace exception
        {
            class UserNotFound : public std::exception
            {
                public:
                
                const char * what () const throw ()
                {
                    return "User not found";
                }
            };
        }
        
        class User
        {
            protected:
            
            void build(struct passwd* pw);
            
            public:
            
            std::string name;
            std::string password;
            uid_t uid;
            gid_t gid;
            std::string gecos;
            std::string dir;
            std::string shell;
            
            User(struct passwd* pw);
            User(uid_t uid);
            User(const char* name);
        };
    }
}

#endif