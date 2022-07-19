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

#ifndef EDUPALS_DMI
#define EDUPALS_DMI

#include <string>

namespace edupals
{
    namespace system
    {
        namespace dmi
        {
            enum Key
            {
                SystemVendor = 0,

                ProductFamily,
                ProductName,
                ProductSerial,
                ProductSKU,
                ProductUUID,
                ProductVersion,

                BiosDate,
                BiosRelease,
                BiosVendor,
                BiosVersion,

                BoardAssetTag,
                BoardName,
                BoardSerial,
                BoardVendor,
                BoardVersion,

                ChassisAssetTag,
                ChassisSerial,
                ChassisType,
                ChassisVendor,
                ChassisVersion
            };

            /*!
                Gets a DMI value from given key
            */
            std::string get(Key key);
        }
    }
}

#endif
