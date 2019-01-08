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
 
#include <cmd.hpp>

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace edupals::cmd;

static int find_long_option(vector<Option> & options,string long_name)
{
    
    for (size_t n=0;n<options.size();n++) {
        if (options[n].long_name==long_name) {
            return n;
        }
    }
    
    return -1;
}

static int find_short_option(vector<Option>& options,char short_name)
{
    for (size_t n=0;n<options.size();n++) {
        if (options[n].short_name==short_name) {
            return n;
        }
    }
    
    return -1;
}

Option::Option(char short_name,string long_name,ArgumentType argument_type)
{
    this->short_name=short_name;
    this->long_name=long_name;
    this->argument_type=argument_type;
}

Option::Option(char short_name,ArgumentType argument_type): Option(short_name,"",argument_type)
{
}

Option::Option(string long_name,ArgumentType argument_type) : Option('\0',long_name,argument_type)
{
}

ParseResult::ParseResult()
{
    
}

bool ParseResult::success()
{
    return (unknowns.size()==0 and missings.size()==0);
}

void ArgumentParser::add_option(Option option)
{
    options.push_back(option);
}

ParseResult ArgumentParser::parse(int argc,char* argv[])
{
    ParseResult result;
    bool eatr=false;
    bool eato=false;
    string left,right;
    string tmp;
    
    for (int n=0;n<argc;n++) {
        
        tmp = argv[n];
        
        
        // single letter
        if (tmp.size()==1) {
            result.args.push_back(tmp);
        }
        else {
            
            // short option(s)
            if (tmp[0]=='-') {
                
                //optional argument is not comming
                eato=false;
                
                //Error: missing long option required argument 
                if (eatr) {
                    result.missings.push_back(left);
                    eatr=false;
                }
                
                //long option
                if (tmp[1]=='-') {
                    
                    //stop parsing options
                    if (tmp.size()==2) {
                        //TODO
                    }
                    else {
                        
                        //check for equal character
                        size_t equal=tmp.find('=');
                        
                        
                        
                        if (equal!=string::npos) {
                            left=tmp.substr(2,equal-2);
                            right=tmp.substr(equal+1);
                        }
                        else {
                            left=tmp.substr(2);
                        }
                        
                        int oindex=find_long_option(options,left);
                        
                        //Error: unknown long option
                        if (oindex==-1) {
                            result.unknowns.push_back(left);
                        }
                        else {
                            Option option=options[oindex];
                            
                            //required argument
                            if (option.argument_type==ArgumentType::Required) {
                                //there is equal
                                if (right.size()>0) {
                                    option.value=right;
                                    result.options.push_back(option);
                                }
                                else {
                                    //eat next
                                    eatr=true;
                                    result.options.push_back(option);
                                }
                            }
                            
                            //no argument
                            if (option.argument_type==ArgumentType::None) {
                                result.options.push_back(option);
                            }
                            
                            //optional argument
                            if (option.argument_type==ArgumentType::Optional) {
                                //there is equal
                                if (right.size()>0) {
                                    option.value=right;
                                    result.options.push_back(option);
                                }
                                else {
                                    //eat next
                                    eato=true;
                                    result.options.push_back(option);
                                }
                            }
                        }
                    
                    }
                }
                else {
                    // short option processing
                    
                    for (size_t sn=1;sn<tmp.size();sn++) {
                        int oindex = find_short_option(options,tmp[sn]);
                        
                        if (oindex>=0) {
                            Option option=options[oindex];
                            
                            if (option.argument_type==ArgumentType::None) {
                                result.options.push_back(option);
                            }
                            
                            if (option.argument_type==ArgumentType::Required) {
                                string arg = tmp.substr(sn+1);

                                //Error: missing short option required argument
                                if (arg.size()==0) {
                                    result.missings.push_back(string(1,tmp[sn]));
                                }
                                
                                
                                option.value=arg;
                                result.options.push_back(option);
                                break;
                            }
                            
                            if (option.argument_type==ArgumentType::Optional) {
                                
                                option.value = tmp.substr(sn+1);
                                
                                result.options.push_back(option);
                                break;
                            }
                        }
                        else {
                            //Error: unknown short option
                            result.unknowns.push_back(string(1,tmp[sn]));
                        }
                    }
                }
            }
            else {
            
                //eat argument
                if (eatr or eato) {
                    result.options.back().value=tmp;
                    eato=false;
                    eatr=false;
                }
                else {
                    result.args.push_back(tmp);
                }
            }
            
        }
    }
    
    //Error: missing required long option argument
    if (eatr) {
        result.missings.push_back(left);
    }
    
    return result;
}
