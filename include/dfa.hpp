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
 
#ifndef EDUPALS_DFA
#define EDUPALS_DFA

#include <cstdint>
#include <string>
#include <cstring>

namespace edupals
{
    namespace parser
    {
        class DFA
        {
            protected:
            
            int8_t* stack;
            size_t capacity;
            int cursor;
            int last;
            bool _accept;
            bool _end;
            bool _valid;
                
            public:
            
            DFA();
            
            virtual ~DFA();
            
            /*!
                Push a character into dfa
                this will trigger either start or step methods
            */
            void push(int8_t c)
            {
                if (cursor==capacity) {
                    size_t new_capacity=capacity*1.5f;
                    int8_t* tmp = new int8_t[new_capacity];
                    
                    std::memcpy(tmp,stack,capacity);
                    
                    delete [] stack;
                    stack=tmp;
                    capacity=new_capacity;
                }
                
                cursor++;
                stack[cursor]=c;

                if (cursor==0) {
                    start();
                }
                else {
                    step();
                }
                
                if (_end) {
                    last=cursor;
                }
                
                _valid=_accept;
            }
            
            /*!
                Resets dfa state
            */
            virtual void reset();
            
            /*!
                Whenever dfa is accepting current pushed string
            */
            bool accept()
            {
                return _accept;
            }
            
            /*!
                Whenever dfa is ready to end
            */
            bool end()
            {
                return _end;
            }
            
            /*!
                Whenever dfa still valid
            */
            bool valid()
            {
                return _valid;
            }
            
            /*!
                Triggered with first character pushed
            */
            virtual void start()=0;
            
            /*!
                Triggered after a character been pushed
            */
            virtual void step()=0;
            
            /*!
                Returns accepted string
            */
            std::string value();
            
            /*!
                size of current stack
            */
            size_t size();
        };
    }
}

#endif
