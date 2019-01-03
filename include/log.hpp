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
#include <string>

#define debug dbg<<__FILE__<<" "<<__LINE__<<":"

namespace edupals
{
    namespace log
    {
        /*!
            Custom std::streambuf with folowing features:
            - Thread safe, output is granted from start to first
            end line character
            - outputs to stderr (unoptimized now)
            - custom header
        */
        class SyncBuf : public std::streambuf
        {
            public:
            
            SyncBuf(std::string header);
            
            protected:
            
            std::string header;
            std::string back;
            
            int overflow (int c = EOF) override;
        };

        /*! debug stream (better use debug macro) */
        extern std::ostream dbg;
        
        /*! common output messages */
        extern std::ostream info;
        
        /*! blue output messages */
        extern std::ostream notice;
        
        /*!  yellow warnings */
        extern std::ostream warning;
        
        /*! red errors */
        extern std::ostream error;
    }
}

#endif
