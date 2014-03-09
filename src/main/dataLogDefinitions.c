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

#define DATALOGDEFINITIONS_C

#include <stddef.h>

//#include "inc/typeChecks.h"
#include "inc/freeEMS.h"
#include "inc/dataLogDefinitions.h"
#include "inc/libreDefs.h"

//TODO Fill these tables out
//TODO Potentially change linker so that they are guaranteed to have a successive LMA(load memory address)
//TODO Create macro(s) to remove duplicates. Eg .startingPosition and .size take the same parameters
//TODO once all done enable CASSERT
//TODO adjust copyright to reflect all authors


const dataBlockDescriptor coreVarsDescriptor[] PPAGE_E5 ={
		{.startingPosition = offsetof(CoreVar, IAT), .size = bitSize(CoreVar, IAT), .name = "IAT", .description = "Inlet Air Temperature", 0,0},
		{.startingPosition = offsetof(CoreVar, CHT), .size = bitSize(CoreVar, CHT), .name = "CHT", .description = "Coolant or Head Temperature", 0,0}
};
//CASSERT(sizeof(CoreVar) / sizeof(unsigned short) == sizeof(coreVarsDescriptor), DATALOGDEFINITIONS_C) // At least check for correct number of entries

const dataBlockDescriptor derrivedVarsDescriptor[] PPAGE_E5 ={
		{.startingPosition = offsetof(DerivedVar, LoadMain), .size = bitSize(DerivedVar, LoadMain), .name = "LoadMain", .description = "Algorithm dependent representation of engine load", 0,0},
		{.startingPosition = offsetof(DerivedVar, VEMain), .size = bitSize(DerivedVar, VEMain), .name = "CHT", .description = "VE value from table", 0,0}
};
//CASSERT(sizeof(CoreVar) / sizeof(unsigned short) == sizeof(derrivedVarsDescriptor), DATALOGDEFINITIONS_C) // ditto

const dataBlockDescriptor KeyUserVarsDescriptor[] PPAGE_E5 ={
		{.startingPosition = offsetof(KeyUserDebug, tempClock), .size = bitSize(KeyUserDebug, tempClock), .name = "tempClock", .description = "Incremented once per log sent", 0,0},
		{.startingPosition = offsetof(KeyUserDebug, spareChar), .size = bitSize(KeyUserDebug, spareChar), .name = "spareChar", .description = "Spare char variable", 0,0}
};
//CASSERT(sizeof(CoreVar) / sizeof(unsigned short) == sizeof(KeyUserVarsDescriptor), DATALOGDEFINITIONS_C) // like wise

