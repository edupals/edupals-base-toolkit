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

static int8_t to_b64(uint8_t code)
{
    code=code & 0x3f;
    
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

static void shiftmask(uint8_t a,uint8_t b,uint8_t c,uint32_t* out)
{
    out[0]=(a & 0xfc)>>2;
    out[1]=((a & 0x03)<<4) | ((b & 0xf0)>>4);
    out[2]=((b & 0x0f)<<2) | ((c & 0xc0)>>6);
    out[3]=c & 0x3f;
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
    clog<<"remain: "<<remain<<endl;
    
    out.reserve(osize);
    
    uint32_t c[4];
    for (size_t n=0;n<isize-remain;n+=3) {
        
        shiftmask(in[n+0],in[n+1],in[n+2],c);
        
        for (int m=0;m<4;m++) {
            out.append(1,to_b64(c[m]));
        }
    }
    
    if (remain==1) {
        size_t n= isize-remain;
        uint32_t c[4];
        shiftmask(in[n],0,0,c);
        out.append(1,to_b64(c[0]));
        out.append(1,to_b64(c[1]));
        out.append("==");
    }
    if (remain==2) {
        size_t n= isize-remain;
        uint32_t c[4];
        shiftmask(in[n-1],in[n],0,c);
        out.append(1,to_b64(c[0]));
        out.append(1,to_b64(c[1]));
        out.append(1,to_b64(c[2]));
        out.append("=");
    }
    
}
