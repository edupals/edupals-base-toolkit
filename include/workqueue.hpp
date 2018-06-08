/*
 * Copyright (C) 2017 Edupals project
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
 
 
#ifndef EDUPALS_WORKQUEUE
#define EDUPALS_WORKQUEUE

#include <mutex>
#include <condition_variable>
#include <queue>

namespace edupals
{
    namespace threading
    {
        template <typename T>
        class WorkQueue
        {
            public:
            
            std::queue<T> queue;
            std::mutex qmutex;
            
            std::condition_variable cond_full;
            std::condition_variable cond_empty;
            
            size_t max_size;
            
            WorkQueue()
            {
                max_size=32767;
            }
            
            WorkQueue(size_t max_size)
            {
                this->max_size=max_size;
            }
            
            void push(T& t)
            {
                std::unique_lock<std::mutex> lck(qmutex);
                
                if(queue.size()>=max_size) {
                    cond_full.wait(lck);
                }
                queue.push(t);
                
                lck.unlock();
                
                cond_empty.notify_one();
            }
            
            T pop()
            {
                std::unique_lock<std::mutex> lck(qmutex);
                
                if (queue.empty()) {
                    cond_empty.wait(lck);
                }
                
                T& t = queue.front();
                queue.pop();
                
                lck.unlock();
                
                cond_full.notify_one();
                
                return t;
            }
        
        };
    }
}

#endif
