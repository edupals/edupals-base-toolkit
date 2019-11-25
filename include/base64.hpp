/*
 * Copyright (C) 2019 Edupals project
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
 
#ifndef EDUPALS_BASE64
#define EDUPALS_BASE64

#include <string>
#include <vector>
#include <cstdint>
#include <exception>

namespace edupals
{
    namespace base64
    {
        namespace exception
        {
            class BadSize : public std::exception
            {
                const char* what () const throw ()
                {
                    return "Input base64 string is not multiple of 4";
                }
            };
            
            class BadChar : public std::exception
            {
                const char* what () const throw ()
                {
                    return "Invalid base64 character";
                }
            };
        }
        
        /*!
            Decode an input ascii string encoded with base64
            \param in input string
            \param out output std::vector of bytes
        */
        void decode(std::string& in,std::vector<uint8_t>& out);
        
        /*!
            Encode a raw data into a base64 string
            \param in std::vector of bytes with input raw data
            \param out output string filled with encoded base64 data
        */
        void encode(std::vector<uint8_t>& in,std::string& out);
    }
}

#endif
