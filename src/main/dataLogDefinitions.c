/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys
 *
 * This file is part of the LibreEMS project.
 *
 * LibreEMS software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LibreEMS software is distributed in the hope that it will be useful,
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
//TODO Create macro(s) to remove duplicates. Eg .start and .size take the same parameters
//TODO once all done enable CASSERT
//TODO adjust copyright to reflect all authors


const dataBlockDescriptor coreVarsDescriptor[] PPAGE_E5 ={
		{
			.start = offsetof(CoreVar, IAT), 
			.size = bitSize(CoreVar, IAT), 
			.is_signed = 0, 
			.name = "IAT", 
			.description = "Inlet Air Temperature (Kelvin)", 
			.multiplier = "0.01" , 
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = NULL,
			.suffix = "degrees",
		},
		{
			.start = offsetof(CoreVar, CHT), 
			.size = bitSize(CoreVar, CHT), 
			.is_signed = 0,
			.name = "CHT", 
			.description = "Coolant or Head Temperature (Kelvin)", 
			.multiplier = "0.01" ,
			.adder = "0",
			.transfer_function = "x/100",
			.flags = NULL,
			.suffix = "degrees",
		},
		{
			.start = offsetof(CoreVar, TPS), 
			.size = bitSize(CoreVar, TPS), 
			.is_signed = 0,
			.name = "TPS", 
			.description = "Throttle Position Sensor", 
			.multiplier = "0.0015625",
			.adder = "0", 
			.transfer_function = "x/640",
			.flags = "temperature",
			.suffix = "%",
		},
		{
			.start = offsetof(CoreVar, EGO), 
			.size = bitSize(CoreVar, EGO), 
			.is_signed = 0,
			.name = "EGO", 
			.description = "Exhaust Gas Oxygen (Lambda)", 
			.multiplier = "0.0000305175781",
			.adder = "0", 
			.transfer_function = "x/32768",
			.flags = "",
			.suffix = "lambda",
		},
		{
			.start = offsetof(CoreVar, MAP), 
			.size = bitSize(CoreVar, MAP), 
			.is_signed = 0,
			.name = "MAP", 
			.description = "Manifold Absolute Pressure", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "kPa",
		},
		{
			.start = offsetof(CoreVar, AAP), 
			.size = bitSize(CoreVar, AAP), 
			.is_signed = 0,
			.name = "AAP", 
			.description = "Atmosphere Absolute Pressure", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "kPa",
		},
		{
			.start = offsetof(CoreVar, BRV), 
			.size = bitSize(CoreVar, BRV), 
			.is_signed = 0,
			.name = "BRV", 
			.description = "Battery Reference Voltage", 
			.multiplier = "0.001",
			.adder = "0", 
			.transfer_function = "x/1000",
			.flags = "",
			.suffix = "Volts",
		},
		{
			.start = offsetof(CoreVar, MAT), 
			.size = bitSize(CoreVar, MAT), 
			.is_signed = 0,
			.name = "MAT", 
			.description = "Manifold Air Temperature", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "temperature",
			.suffix = "degrees",
		},
		{
			.start = offsetof(CoreVar, EGO2), 
			.size = bitSize(CoreVar, EGO2), 
			.is_signed = 0,
			.name = "EGO2", 
			.description = "Exhaust Gas Oxygen (sensor2, Lambda)", 
			.multiplier = "0.0000305175781",
			.adder = "0", 
			.transfer_function = "x/32768",
			.flags = "",
			.suffix = "lambda",
		},
		{
			.start = offsetof(CoreVar, IAP), 
			.size = bitSize(CoreVar, IAP), 
			.is_signed = 0,
			.name = "IAP", 
			.description = "Intercooler Absolute Pressure", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "kPa",
		},
		{
			.start = offsetof(CoreVar, MAF), 
			.size = bitSize(CoreVar, MAF), 
			.is_signed = 0,
			.name = "MAF", 
			.description = "Mass Air Flow", 
			.multiplier = "1",
			.adder = "0", 
			.transfer_function = "x",
			.flags = "",
			.suffix = "raw",
		},
		{
			.start = offsetof(CoreVar, DMAP), 
			.size = bitSize(CoreVar, DMAP), 
			.is_signed = 0,
			.name = "DMAP", 
			.description = "MAP rate of change", 
			.multiplier = "1",
			.adder = "0", 
			.transfer_function = "x",
			.flags = "",
			.suffix = "kPa/second",
		},
		{
			.start = offsetof(CoreVar, DTPS), 
			.size = bitSize(CoreVar, DTPS), 
			.is_signed = 0,
			.name = "DMAP", 
			.description = "TPS rate of change", 
			.multiplier = "1",
			.adder = "0", 
			.transfer_function = "x",
			.flags = "",
			.suffix = "%/second",
		},
		{
			.start = offsetof(CoreVar, RPM), 
			.size = bitSize(CoreVar, RPM), 
			.is_signed = 0,
			.name = "RPM", 
			.description = "Engine RPM", 
			.multiplier = "0.5",
			.adder = "0", 
			.transfer_function = "x/2",
			.flags = "",
			.suffix = "RPM",
		},
		{
			.start = offsetof(CoreVar, DRPM), 
			.size = bitSize(CoreVar, DRPM), 
			.is_signed = 0,
			.name = "DRPM", 
			.description = "Engine RPM rate of change", 
			.multiplier = "0.5",
			.adder = "0", 
			.transfer_function = "x/2",
			.flags = "",
			.suffix = "RPM/second",
		},
		{
			.start = offsetof(CoreVar, DDRPM), 
			.size = bitSize(CoreVar, DDRPM), 
			.is_signed = 0,
			.name = "DDRPM", 
			.description = "RPM Acceleration", 
			.multiplier = "0.5",
			.adder = "0", 
			.transfer_function = "x/2",
			.flags = "",
			.suffix = "RPM/second^2",
		},
};
//CASSERT(sizeof(CoreVar) / sizeof(unsigned short) == sizeof(coreVarsDescriptor), DATALOGDEFINITIONS_C) // At least check for correct number of entries

const dataBlockDescriptor derrivedVarsDescriptor[] PPAGE_E5 ={
		{.start = offsetof(DerivedVar, LoadMain), .size = bitSize(DerivedVar, LoadMain), .name = "LoadMain", .description = "Algorithm dependent representation of engine load", 0,0},
		{.start = offsetof(DerivedVar, VEMain), .size = bitSize(DerivedVar, VEMain), .name = "CHT", .description = "VE value from table", 0,0}
};
//CASSERT(sizeof(CoreVar) / sizeof(unsigned short) == sizeof(derrivedVarsDescriptor), DATALOGDEFINITIONS_C) // ditto

const dataBlockDescriptor KeyUserVarsDescriptor[] PPAGE_E5 ={
		{.start = offsetof(KeyUserDebug, tempClock), .size = bitSize(KeyUserDebug, tempClock), .name = "tempClock", .description = "Incremented once per log sent", 0,0},
		{.start = offsetof(KeyUserDebug, spareChar), .size = bitSize(KeyUserDebug, spareChar), .name = "spareChar", .description = "Spare char variable", 0,0}
};
//CASSERT(sizeof(CoreVar) / sizeof(unsigned short) == sizeof(KeyUserVarsDescriptor), DATALOGDEFINITIONS_C) // like wise

