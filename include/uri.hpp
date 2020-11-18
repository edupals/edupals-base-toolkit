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

#ifndef EDUPALS_URI
#define EDUPALS_URI

#include "network.hpp"

#include <string>

namespace edupals
{
    namespace uri
    {
        namespace exception
        {
            class SyntaxError : public std::exception
            {
                public:
                
                std::string message;
                
                SyntaxError(std::string message)
                {
                    this->message=message;
                }
                
                const char* what () const throw ()
                {
                    return message.c_str();
                }
            };
        }
        
        class Uri
        {
            public:
            
            /*!
                Parses a URI string (not fully RFC compliant yet)
                throws a SyntaxError if string cannot be properly parsed
            */
            Uri(std::string uri);
            
            /*! Scheme used ie:http. This field is always present */
            std::string scheme;
            
            /*! User name or empty string if not defined */
            std::string user;
            
            /*! Host address, both hostname based or ip4 */
            std::string host;
            
            /*! IP4 object if hostname is defined as ip address, 0.0.0.0 otherwise */
            network::IP4 ip;
            
            /*! Port number, or 0 if not defined */
            int port;
            
            /*! Path. This field is always present */
            std::string path;
            
        };
    }
}

#endif
