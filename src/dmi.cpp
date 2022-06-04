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

#define SYSFS_DMI "/sys/devices/virtual/dmi/id"

using namespace edupals::system::dmi;

using namespace std;

static void read_single_line(string path,string& dest)
{
    ifstream file;

    file.open(path.c_str());
    std::getline(file,dest);
    file.close();
}

string edupals::system::dmi::system_vendor()
{
    string tmp;

    read_single_line(SYSFS_DMI"/sys_vendor",tmp);

    return tmp;
}

string edupals::system::dmi::product_family()
{
    string tmp;

    read_single_line(SYSFS_DMI"/product_family",tmp);

    return tmp;
}

string edupals::system::dmi::product_name()
{
    string tmp;

    read_single_line(SYSFS_DMI"/product_name",tmp);

    return tmp;
}

string edupals::system::dmi::product_serial()
{
    string tmp;

    read_single_line(SYSFS_DMI"/product_serial",tmp);

    return tmp;
}

string edupals::system::dmi::product_sku()
{
    string tmp;

    read_single_line(SYSFS_DMI"/product_sku",tmp);

    return tmp;
}

string edupals::system::dmi::product_uuid()
{
    string tmp;

    read_single_line(SYSFS_DMI"/product_uuid",tmp);

    return tmp;
}

string edupals::system::dmi::product_version()
{
    string tmp;

    read_single_line(SYSFS_DMI"/product_version",tmp);

    return tmp;
}

string edupals::system::dmi::board_name()
{
    string tmp;

    read_single_line(SYSFS_DMI"/board_name",tmp);

    return tmp;
}
