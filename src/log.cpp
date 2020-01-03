/*
 * Copyright (C) 2018 Edupals project
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

#include "log.hpp"

using namespace edupals;
using namespace edupals::log;
using namespace std;

std::mutex SyncBuf::io_mutex;

SyncBuf::SyncBuf()
{
}

SyncBuf::SyncBuf(string header)
{
    set_header(header);
}

int SyncBuf::sync()
{
    std::lock_guard<std::mutex> lock(SyncBuf::io_mutex);
    
    cerr<<header<<str()<<back<<std::flush;
    str("");
    
    return 0;
}

void SyncBuf::set_header(string header)
{
    this->header=header;
    back=console::reset::all;
}


Log::Log(): std::ostream(&buffer)
{
}

Log::Log(string header): buffer(header),std::ostream(&buffer)
{
}

Log::Log(string format,string header): buffer(format+header), std::ostream(&buffer)
{
}
