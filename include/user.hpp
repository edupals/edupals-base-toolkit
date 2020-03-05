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
#include <vector>

namespace edupals
{
    namespace system
    {
        namespace exception
        {
            class UserNotFound : public std::exception
            {
                public:
                
                const char* what() const throw()
                {
                    return "User not found";
                }
            };
        }
        
        class Group;
        
        class User
        {
            protected:
            
            void build(struct passwd* pw);
            
            public:
            
            /*! user name */
            std::string name;
            
            /*! user password, usually blank */
            std::string password;
            
            /*! unix user id */
            uid_t uid;
            
            /*! unix user main group id */
            gid_t gid;
            
            /*! see: https://en.wikipedia.org/wiki/Gecos_field */
            std::string gecos;
            
            /*! user home */
            std::string home;
            
            /*! shell binary path */
            std::string shell;
            
            /*! Create user from posix passwd struct */
            User(struct passwd* pw);
            
            /*! Create user from posix uid */
            User(uid_t uid);
            
            /*! Create user from user name */
            User(const char* name);
            
            /*! Create an empty user */
            User();
            
            /*! gets current (real) user */
            static User me();
            
            static std::vector<User> list();
            
            Group group();
        };
    }
}

#endif