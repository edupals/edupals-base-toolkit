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
                Default constructor
            */
            Path(){};
            
            /*!
                Constructor
            */
            Path(std::string path);
            
            /*!
                Constructor
            */
            Path(const char* path);
            
            /*!
                Gets full path
                \return full path as a string
            */
            std::string name();
            
            /*!
                Gets path basename
                \return string with basename
            */
            std::string base();
            
            /*!
                Gets path dirname
                \return string with dirname
            */
            std::string dir();
            
            /*!
                Lists directory content
                \return vector of Paths
            */
            std::vector<Path> list();
            
            /*!
                List directory using glob expression
                \return vector of Paths matching expression
            */
            std::vector<Path> list(std::string expression);
            
            /*!
                Check if exists path
            */
            bool exists();
            
            /*!
                Whenever path is a directory
            */
            bool is_dir();
            
            /*!
                Whenever path is relative
            */
            bool is_relative();

            /*!
                Gets the current user home
                \return Path 
            */
            static Path home();
            
            /*!
                Concatenates two Paths
            */
            Path operator + (Path& right);
            
            /*!
                Concatenates two Paths
            */
            Path operator + (const char* right);
            
            /*!
                Concatenates two Paths
            */
            Path operator + (std::string& right);
            
            /*!
                Concatenates two Paths
            */
            Path& operator += (Path& right);
            
            /*!
                Concatenates two Paths
            */
            Path& operator += (const char* right);
            
            /*!
                Concatenates two Paths
            */
            Path& operator += (std::string& right);
            
            /*!
                Checks whenever both paths are the same
            */
            bool operator == (Path& right);
            
            /*!
                Checks whenever both paths are not the same
            */
            bool operator != (Path& right);
        };
    }
}

#endif
