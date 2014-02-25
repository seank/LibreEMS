/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys
 *
 * This file is part of the FreeEMS project.
 *
 * FreeEMS software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeEMS software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any LibreEMS software.  If not, see http://www.gnu.org/licenses/
 *
 * Thank you for choosing LibreEMS to run your engine!
 */


/** @file
 *
 * @ingroup TODO
 *
 * @brief TODO
 *
 *
 */

#include <stddef.h>

#include "inc/freeEMS.h"
#include "inc/dataLogDefinitions.h"
#include "inc/libreDefs.h"


const dataLogDefinition dataLogDefinitions PPAGE_E5 = {
		version:  PROTOCOL_VERSION,
		sizeOfEntry:  sizeof(dataLogField),
		entries: {
				 {.startingPosition = offsetof(CoreVar, IAT), .size = bitSize(CoreVar, IAT), .name = "IAT", .description = "Inlet Air Temperature", 0,0},
				 {.startingPosition = offsetof(CoreVar, CHT), .size = bitSize(CoreVar, CHT), .name = "CHT", .description = "Coolant or Head Temperature", 0,0},
		}
};

