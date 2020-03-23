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
 
#ifndef EDUPALS_BSON
#define EDUPALS_BSON

#include "variant.hpp"

#include <ostream>
#include <exception>
#include <sstream>

namespace edupals
{
    namespace bson
    {
        namespace exception
        {
            class UnexpectedEOF : public std::exception
            {
                public:
                
                const char* what () const throw ()
                {
                    return "Unexpected end of file";
                }
            };
            
            class UnsupportedExport : public std::exception
            {
                private:
                    variant::Type type;
                    
                public:
                    
                UnsupportedExport(variant::Type type)
                {
                    this->type=type;
                }
                
                const char* what () const throw ()
                {
                    std::stringstream ss;
                    ss<<"Unsupported export to BSON:"<<static_cast<int>(type);
                    
                    return ss.str().c_str();
                }
            };
        }
        
        /*!
            Serialize Variant container as a bson
            \param stream valid output stream
            \param value data to serialize
        */
        void dump(std::ostream& stream,variant::Variant& value);
        
        /*!
            Load a bson into a Variant
            \param stream valid input stream
            \return Bson loaded as Variant container
        */
        variant::Variant load(std::istream& stream);
    }
}

#endif
