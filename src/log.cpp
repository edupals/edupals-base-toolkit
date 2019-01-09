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

#include <atomic>
#include <thread>
#include <mutex>

using namespace edupals;
using namespace edupals::log;
using namespace std;

thread_local bool locked = false;
mutex output;

SyncBuf dbg_buf(string(console::style::dim)+"[debug] ");
SyncBuf info_buf("");
SyncBuf notice_buf(string(console::fg::blue)+
                    string(console::style::bold));
                    
SyncBuf warning_buf(string(console::fg::yellow)+"[warning] ");
SyncBuf error_buf(string(console::fg::red)+"[error] ");

ostream edupals::log::dbg(&dbg_buf);
ostream edupals::log::info(&info_buf);
ostream edupals::log::notice(&notice_buf);
ostream edupals::log::warning(&warning_buf);
ostream edupals::log::error(&error_buf);

SyncBuf::SyncBuf(string header)
{
    this->header=header;
    back=console::reset::all;
    
}

int SyncBuf::overflow (int c)
{
    
    if (!locked) {
        output.lock();
        locked=true;
        cerr<<header;
    }
    
    cerr.put(c);
    
    if (c=='\n') {
        cerr<<back;
        locked=false;
        output.unlock();
    }
    
    return c;
}

