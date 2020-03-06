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

#ifndef EDUPALS_GROUP
#define EDUPALS_GROUP

#include <grp.h>

#include <string>
#include <vector>

namespace edupals
{
    namespace system
    {
        namespace exception
        {
            class GroupNotFound : public std::exception
            {
                public:
                
                const char* what() const throw()
                {
                    return "Group not found";
                }
            };
        }
        
        class User;
        
        class Group
        {
            protected:
            
            void build(struct group* gr);
            
            public:
            
            /*! Group name */
            std::string name;
            
            /*! posix group id */
            gid_t gid;
            
            /*! creates a group object from posix struct */
            Group(struct group* gr);
            
            /*! 
            * creates a group object from its posix group id 
            * \throws GroupNotFound
            */
            Group(gid_t gid);
            
            /*!
            * creates a group object from its name
            * \throws GroupNotFound
            */
            Group(const char* name);
            
            /*!
            * creates an empty group object
            */
            Group();
            
            /*!
             * gets all available groups
            */
            static std::vector<Group> list();
            
            /*!
            * gets all users that belongs to the group
            */
            std::vector<User> users();
        };
    }
}

#endif