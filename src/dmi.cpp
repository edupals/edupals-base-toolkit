/*
 * Copyright (C) 2022 Edupals project
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

#include <dmi.hpp>

#include <fstream>
#include <map>

#define SYSFS_DMI "/sys/devices/virtual/dmi/id/"

using namespace edupals::system::dmi;

using namespace std;

std::map<Key,string> endpoint = {
    { SystemVendor , "sys_vendor" },

    { ProductFamily , "product_family" },
    { ProductName , "product_name" },
    { ProductSerial , "product_serial" },
    { ProductSKU , "product_sku" },
    { ProductUUID , "product_uuid" },
    { ProductVersion , "product_version" },

    { BiosDate , "bios_date" },
    { BiosRelease , "bios_release" },
    { BiosVendor , "bios_vendor" },
    { BiosVersion , "bios_version" },

    { BoardAssetTag , "board_asset_tag" },
    { BoardName , "board_name" },
    { BoardSerial , "board_serial" },
    { BoardVendor , "board_vendor" },
    { BoardVersion , "board_version" },

    { ChassisAssetTag , "chassis_asset_tag" },
    { ChassisSerial , "chassis_serial" },
    { ChassisType , "chassis_type" },
    { ChassisVendor , "chassis_vendor" },
    { ChassisVersion , "chassis_version" }
};

static void read_single_line(string path,string& dest)
{
    ifstream file;

    file.open(path.c_str());
    std::getline(file,dest);
    file.close();
}

string edupals::system::dmi::get(Key key)
{
    string path = SYSFS_DMI;

    string tmp = endpoint[key];

    path = path + tmp;
    read_single_line(path,tmp);

    return tmp;
}
