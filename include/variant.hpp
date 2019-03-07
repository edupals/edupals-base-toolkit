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

namespace edupals
{
    enum class VariantException
    {
        Unitialized,
        InvalidType
    };
    
    enum class VariantType
    {
        None,
        Int32,
        Float32,
        String,
        Array
    };
    
    class VariantContainer
    {
        public:
            
        VariantType type;
        
        virtual size_t size()
        {
            return 0;
        }
        
        VariantContainer()
        {
            type = VariantType::None;
        };
        
        virtual ~VariantContainer()
        {
        }
    };
    
    class VariantContainerInt32: public VariantContainer
    {
        
        public:
        int32_t value;
        
        VariantContainerInt32(int32_t value)
        {
            type=VariantType::Int32;
            this->value=value;
        }
        
        size_t size() override
        {
            return 4;
        }
        
    };
    
    class VariantContainerFloat32: public VariantContainer
    {
        
        public:
        float value;
        
        VariantContainerFloat32(float value)
        {
            type=VariantType::Float32;
            this->value=value;
        }
        
        size_t size() override
        {
            return 4;
        }
        
    };
    
    class VariantContainerString: public VariantContainer
    {
        
        public:
        std::string value;
        
        VariantContainerString(std::string value)
        {
            type=VariantType::String;
            this->value=value;
        }
        
        size_t size() override
        {
            return value.size();
        }
        
    };
    
    class Variant;
    
    class VariantContainerArray: public VariantContainer
    {
        
        public:
        Variant* value;
        size_t count;
        
        VariantContainerArray(std::vector<Variant> value);
        ~VariantContainerArray();
        
        size_t size() override;
        
    };
    
    class Variant
    {
        protected:
            
        std::shared_ptr<VariantContainer> data;
            
        public:
            
        Variant();
        Variant(int value);
        Variant(float value);
        Variant(std::string value);
        Variant(const char* value);
        Variant(std::vector<Variant> value);
        
        ~Variant();
        
        size_t size();
        
        int32_t get_int32();
        float get_float32();
        std::string get_string();
        
        Variant& operator=(int value);
        Variant& operator=(float value);
        Variant& operator=(std::string value);
        Variant& operator=(const char* value);
        
        Variant& operator[] (const int index);
        
    };
}

#endif