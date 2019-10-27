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
#include <iostream>
#include <initializer_list>

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
            Bytes,
            Array,
            Struct
        };
        
        namespace exception
        {
            class Unitialized : public std::exception
            {
                const char* what () const throw ()
                {
                    return "Variant not initialized";
                }
            };
            
            class InvalidType : public std::exception
            {
                const char* what () const throw ()
                {
                    return "Invalid type";
                }
            };
            
            class OutOfBounds : public std::exception
            {
                const char* what () const throw ()
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
                
                virtual void serialize(std::ostream& stream);
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
                
                void serialize(std::ostream& stream) override
                {
                    if (value) {
                        stream<<"true";
                    }
                    else {
                        stream<<"false";
                    }
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
                
                void serialize(std::ostream& stream) override
                {
                    ostream<<value;
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
                
                void serialize(std::ostream& stream) override
                {
                    ostream<<value;
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
                
                void serialize(std::ostream& stream) override
                {
                    ostream<<value;
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
                
                void serialize(std::ostream& stream) override
                {
                    ostream<<"\""<<value<<"\"";
                }
            };
            
            class Bytes: public Base
            {
                public:
                std::vector<uint8_t> value;
                
                Bytes(std::vector<uint8_t>& value);
                Bytes(uint8_t* value,size_t size);
                
                size_t size() override
                {
                    return value.size();
                }
                
                void serialize(std::ostream& stream) override;
            };
            
            class Array: public Base
            {
                
                public:
                std::vector<variant::Variant> value;
                
                Array(std::vector<variant::Variant>& value);
                Array(size_t count);
                ~Array();
                
                size_t size() override;
                size_t count();
                
                void serialize(std::ostream& stream) override;
            };
            
            class Struct: public Base
            {
                
                public:
                std::map<std::string,variant::Variant> value;
                
                Struct();
                
                size_t size() override;
                
                void serialize(std::ostream& stream) override;
            };
        }
        
        class Variant
        {
            protected:
                
            std::shared_ptr<container::Base> data;
            
            private:
                
            Variant& get_value_from_key(std::string key);
            
            public:
            
            /*!
                Create an empty Variant
            */
            Variant();
            
            Variant(bool value);
            Variant(int value);
            Variant(float value);
            Variant(double value);
            Variant(std::string value);
            Variant(const char* value);
            Variant(std::vector<Variant>& value);
            Variant(std::vector<uint8_t>& value);
            Variant(uint8_t* value,size_t size);
            Variant(std::initializer_list<Variant> list);
            
            virtual ~Variant();
            
            /*!
                Create an array variant with 'count' empty values
            */
            static Variant create_array(size_t count);
            
            /*!
                Get number of array elements
                throws InvalidType exception in case of type missmatch
            */
            size_t count();
            
            /*!
                Grow array variants by one, adding and empty value
            */
            void append();
            
            /*!
                Adds a new Variant to an array
            */
            void append(Variant value);
            
            /*!
                Create a struct (aka map) Variant
            */
            static Variant create_struct();
            
            /*!
                Get a vector with struct keys
                throws InvalidType exception in case of type missmatch
            */
            std::vector<std::string> keys();
            
            /*!
                Compute size (in bytes) of Variant container, included children
            */
            size_t size();
            
            /*!
                Get type of Variant
            */
            Type type();
            
            /*!
                Serialize contents to a standard output stream
            */
            void serialize(std::ostream& stream);
            
            /*!
                get Variant as boolean
                throws InvalidType exception in case of type missmatch
            */
            bool get_boolean();
            
            /*!
                get Variant as a 32 bit signed integer
                throws InvalidType exception in case of type missmatch
            */
            int32_t get_int32();
            
            /*!
                get Variant as single precission float
                throws InvalidType exception in case of type missmatch
            */
            float get_float();
            
            /*!
                get Variant as double precission float
                throws InvalidType exception in case of type missmatch
            */
            double get_double();
            
            /*!
                get Variant as string
                throws InvalidType exception in case of type missmatch
            */
            std::string get_string();
            
            /*!
                get Variant as a vector of bytes
                throws InvalidType exception in case of type missmatch
            */
            std::vector<uint8_t>& get_bytes();
            
            Variant& operator=(bool value);
            Variant& operator=(int value);
            Variant& operator=(float value);
            Variant& operator=(double value);
            Variant& operator=(std::string value);
            Variant& operator=(const char* value);
            Variant& operator=(std::vector<uint8_t>& value);
            
            /*!
                get Variant from an array Variant using index
                throws InvalidType exception in case of type missmatch
            */
            Variant& operator[](const int index);
            
            /*!
                get Variant from a struct Variant using key
                throws InvalidType exception in case of type missmatch
            */
            Variant& operator[](const char* key);
            Variant& operator[](std::string key);
            
            
            
        };
        
        /*!
                std stream support
        */
        std::ostream& operator<<(std::ostream& os,Variant& v);
    }
}

#endif
