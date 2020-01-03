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
 
#ifndef EDUPALS_LOG
#define EDUPALS_LOG

#include "console.hpp"

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <mutex>

#define debug dbg<<__FILE__<<" "<<__LINE__<<":"

namespace edupals
{
    namespace log
    {
        
        /*!
            Custom std::stringbuf with folowing features:
            - Object must be thread local to work properly
            - outputs to stderr synchronized (mutex) at flush event
            - custom header
        */
        class SyncBuf : public std::stringbuf
        {
            public:
            
            SyncBuf();
            SyncBuf(std::string header);
            
            void set_header(std::string header);
            
            protected:
            
            static std::mutex io_mutex;
            
            std::string header;
            std::string back;
            
            int sync() override;
        };
        
        class Log : public std::ostream
        {
            public:
            
            SyncBuf buffer;
            
            Log();
            Log(std::string header);
            Log(std::string style, std::string header);
            
        };
    }
}

#endif
