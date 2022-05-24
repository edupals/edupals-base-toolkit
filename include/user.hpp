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
#include <sstream>
#include <exception>
#include <vector>
#include <map>

namespace edupals
{
    namespace system
    {
        namespace exception
        {

            class UserDatabaseError : public std::exception
            {
                public:
                std::string message;

                int errnum;

                UserDatabaseError(int error) : errnum(error)
                {
                    std::stringstream ss;
                    std::map<int,std::string> enames;
                    enames[EAGAIN] = "EAGAIN";
                    enames[ERANGE] = "ERANGE";
                    enames[ENOENT] = "ENOENT";

                    ss<<"User database error: "<<errnum<<" ("<<enames[errnum]<<")";

                    message = ss.str();
                }

                const char* what() const throw()
                {
                    return message.c_str();
                }
            };

            class UserNotFound : public UserDatabaseError
            {
                public:

                UserNotFound(int uid, int error = ENOENT) : UserDatabaseError(error)
                {
                    std::stringstream ss;

                    ss<<"User "<<uid<<" not found:\n"<<message;
                    message = ss.str();
                }

                UserNotFound(const char* name, int error = ENOENT) : UserDatabaseError(error)
                {
                    std::stringstream ss;

                    ss<<"User "<<name<<" not found:\n"<<message;
                    message = ss.str();
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
            
            /*! create user object from posix uid */
            User(uid_t uid);
            
            /*! create user object from user name */
            User(const char* name);
            
            /*! Create an empty user */
            User();
            
            /*! gets current (real) user */
            static User me();
            
            /*! gets available users */
            static std::vector<User> list();
            
            /*! get user main group */
            Group group();
            
            /*! gets all group user belongs to */
            std::vector<Group> groups();
            
            bool operator == (User& a)
            {
                return uid==a.uid;
            }
            
            bool operator != (User& a)
            {
                return uid!=a.uid;
            }
        };
    }
}

#endif
