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

using namespace edupals;
using namespace edupals::log;
using namespace std;

atomic_flag lock = ATOMIC_FLAG_INIT;

SyncBuf::SyncBuf(string header)
{
    this->header=header;
    back=console::reset::all;
    
    start=true;
}

streamsize SyncBuf::xsputn(const char* s,streamsize n)
{
/*
    if (start) {
        cerr<<header;
        start=false;
        n+=header.size();
    }
    */
    cerr<<s;
    cout<<"-size:"<<n<<endl;
    /*
    if (s[n-2]=='\n') {
        cerr<<back;
        n+=back.size();
        start=true;
        lock.clear();
    }
    */
    return n;
}
