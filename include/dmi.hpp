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
            std::string system_vendor();

            std::string product_family();
            std::string product_name();
            std::string product_serial();
            std::string product_sku();
            std::string product_uuid();
            std::string product_version();

            std::string board_name();
        }
    }
}

#endif
