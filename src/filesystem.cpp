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
 

#include "filesystem.hpp"

#include <dirent.h>

using namespace edupals::filesystem;
using namespace std;


Path::Path(const char* path)
{
    this->path=path;
}

vector<Path> Path::list()
{
    vector<Path> content;
    
    DIR *dp;
    struct dirent *dirp;
    
    dp  = opendir(path.c_str());
    
    while ((dirp = readdir(dp)) != nullptr) {
        content.push_back(dirp->d_name);
    }
    
    return content;
}
