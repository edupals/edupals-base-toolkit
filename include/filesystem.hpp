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
 
#ifndef EDUPALS_FILESYSTEM
#define EDUPALS_FILESYSTEM

#include <string>
#include <vector>

namespace edupals
{
    namespace filesystem
    {
        class Path
        {
            private:
            
            std::string value;
            
            void sanitize();
            
            public:
            
            
            /*!
            
            */
            Path(){};
            
            /*!
            
            */
            Path(std::string path);
            
            /*!
            
            */
            Path(const char* path);
            
            /*!
            
            */
            std::string name();
            
            /*!
            
            */
            std::vector<Path> list();

            /*!
            
            */
            bool exists();
            
            /*!
            
            */
            bool is_dir();
            
            /*!
            
            */
            bool is_relative();
            /*!
            
            */
            static Path home();
            
            Path operator + (Path right);
        };
    }
}

#endif
