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
 

#include <base64.hpp>

#include <iostream>

using namespace std;

const char* encode_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char to_b64(int code)
{
    if (code<26) {
        return 'A'+code;
    }
    else {
        if (code<52) {
            return 'a'+(code-26);
        }
        else {
            return '0'+(code-52);
        }
    }
}

void edupals::base64::decode(string& in,vector<uint8_t>& out)
{
}

void edupals::base64::encode(vector<uint8_t>& in,string& out)
{
    out.clear();
    
    size_t isize=in.size();
    size_t remain=isize%3;
    size_t osize=4*(isize/3);
    
    if (remain>0) {
        osize+=4;
    }
    
    clog<<"input size:"<<isize<<endl;
    clog<<"output size:"<<osize<<endl;
    
    out.reserve(osize);
    
    for (size_t n=0;n<isize-remain;n+=3) {
        uint32_t raw = (in[n+0]<<16) | (in[n+1]<<8) | (in[n+2]);
        uint32_t c1=(raw & 0x00fd0000)>>18;
        uint32_t c2=(raw & 0x0003f000)>>12;
        uint32_t c3=(raw & 0x00000fd0)>>6;
        uint32_t c4=(raw & 0x0000003f);
        
        cout<<"block: "<<to_b64(c1)<<":"<<to_b64(c2)<<":"<<to_b64(c3)<<":"<<to_b64(c4)<<endl;
    }
    
    //TODO: compute remain here
    
}
