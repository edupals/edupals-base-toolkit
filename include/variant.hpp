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
 
#ifndef EDUPALS_VARIANT
#define EDUPALS_VARIANT

#include <string>
#include <memory>
#include <cstdint>
#include <vector>
#include <exception>
#include <map>

namespace edupals
{
    namespace variant
    {
        
        class Variant;
        
         enum class Type
        {
            None,
            Boolean,
            Int32,
            Float,
            Double,
            String,
            Array,
            Struct
        };
        
        namespace exception
        {
            class Unitialized : public std::exception
            {
                const char * what () const throw ()
                {
                    return "Variant not initialized";
                }
            };
            
            class InvalidType : public std::exception
            {
                const char * what () const throw ()
                {
                    return "Invalid type";
                }
            };
            
            class OutOfBounds : public std::exception
            {
                const char * what () const throw ()
                {
                    return "Index out of bounds";
                }
            };
        }
    
        namespace container
        {
                class Base
                {
                    public:
                        
                    variant::Type type;
                    
                    virtual size_t size()
                    {
                        return 0;
                    }
                    
                    Base()
                    {
                        type = variant::Type::None;
                    }
                    
                    virtual ~Base()
                    {
                    }
                };
                
                class Boolean: public Base
                {
                    
                    public:
                    bool value;
                    
                    Boolean(bool value)
                    {
                        type=variant::Type::Boolean;
                        this->value=value;
                    }
                    
                    size_t size() override
                    {
                        return sizeof(bool);
                    }
                    
                };
                
                class Int32: public Base
                {
                    
                    public:
                    int32_t value;
                    
                    Int32(int32_t value)
                    {
                        type=variant::Type::Int32;
                        this->value=value;
                    }
                    
                    size_t size() override
                    {
                        return sizeof(int32_t);
                    }
                    
                };
                
                class Float: public Base
                {
                    
                    public:
                    float value;
                    
                    Float(float value)
                    {
                        type=variant::Type::Float;
                        this->value=value;
                    }
                    
                    size_t size() override
                    {
                        return sizeof(float);
                    }
                    
                };
                
                class Double: public Base
                {
                    
                    public:
                    double value;
                    
                    Double(double value)
                    {
                        type=variant::Type::Double;
                        this->value=value;
                    }
                    
                    size_t size() override
                    {
                        return sizeof(double);
                    }
                    
                };
                
                 class String: public Base
                {
                    
                    public:
                    std::string value;
                    
                    String(std::string value)
                    {
                        type=variant::Type::String;
                        this->value=value;
                    }
                    
                    size_t size() override
                    {
                        return value.size();
                    }
                    
                };
                
                class Array: public Base
                {
                    
                    public:
                    variant::Variant* value;
                    size_t count;
                    
                    Array(std::vector<variant::Variant> value);
                    Array(size_t count);
                    ~Array();
                    
                    size_t size() override;
                    
                };
                
                class Struct: public Base
                {
                    
                    public:
                    std::map<std::string,variant::Variant> value;
                    
                    Struct();
                    
                    size_t size() override;
                };
        }
        
        
        class Variant
        {
            protected:
                
            std::shared_ptr<container::Base> data;
                
            public:
                
            Variant();
            Variant(bool value);
            Variant(int value);
            Variant(float value);
            Variant(double value);
            Variant(std::string value);
            Variant(const char* value);
            Variant(std::vector<Variant> value);
            
            ~Variant();
            
            static Variant create_array(size_t count);
            
            size_t size();
            
            bool get_boolean();
            int32_t get_int32();
            float get_float();
            double get_double();
            std::string get_string();
            
            Variant& operator=(bool value);
            Variant& operator=(int value);
            Variant& operator=(float value);
            Variant& operator=(double value);
            Variant& operator=(std::string value);
            Variant& operator=(const char* value);
            
            Variant& operator[](const int index);
            
        };
    }
}

#endif
