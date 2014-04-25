/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys, David J. Andruczyk
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

//TODO Potentially change linker so that they are guaranteed to have a successive LMA(load memory address)
//TODO Create macro(s) to remove duplicates. Eg .start and .size take the same parameters
//TODO once all done enable CASSERT


const dataBlockDescriptor coreVarsDescriptor[] PPAGE_E5 ={
		{
			.start = offsetof(CoreVar, IAT), 
			.size = bitSize(CoreVar, IAT), 
			.isSigned = 0,
			.name = "IAT", 
			.description = "Inlet Air Temperature (Kelvin)", 
			.multiplier = "0.01" , 
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "degrees",
			.bitFieldDescription = {{0}}
		},
		{
			.start = offsetof(CoreVar, CHT), 
			.size = bitSize(CoreVar, CHT), 
			.isSigned = 0,
			.name = "CHT", 
			.description = "Coolant or Head Temperature (Kelvin)", 
			.multiplier = "0.01" ,
			.adder = "0",
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "degrees",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, TPS), 
			.size = bitSize(CoreVar, TPS), 
			.isSigned = 0,
			.name = "TPS", 
			.description = "Throttle Position Sensor", 
			.multiplier = "0.0015625",
			.adder = "0", 
			.transfer_function = "x/640",
			.flags = "temperature",
			.suffix = "%",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, EGO), 
			.size = bitSize(CoreVar, EGO), 
			.isSigned = 0,
			.name = "EGO", 
			.description = "Exhaust Gas Oxygen (Lambda)", 
			.multiplier = "0.0000305175781",
			.adder = "0", 
			.transfer_function = "x/32768",
			.flags = "",
			.suffix = "lambda",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, MAP), 
			.size = bitSize(CoreVar, MAP), 
			.isSigned = 0,
			.name = "MAP", 
			.description = "Manifold Absolute Pressure", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "kPa",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, AAP), 
			.size = bitSize(CoreVar, AAP), 
			.isSigned = 0,
			.name = "AAP", 
			.description = "Atmosphere Absolute Pressure", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "kPa",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, BRV), 
			.size = bitSize(CoreVar, BRV), 
			.isSigned = 0,
			.name = "BRV", 
			.description = "Battery Reference Voltage", 
			.multiplier = "0.001",
			.adder = "0", 
			.transfer_function = "x/1000",
			.flags = "",
			.suffix = "Volts",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, MAT), 
			.size = bitSize(CoreVar, MAT), 
			.isSigned = 0,
			.name = "MAT", 
			.description = "Manifold Air Temperature", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "temperature",
			.suffix = "degrees",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, EGO2), 
			.size = bitSize(CoreVar, EGO2), 
			.isSigned = 0,
			.name = "EGO2", 
			.description = "Exhaust Gas Oxygen (sensor2, Lambda)", 
			.multiplier = "0.0000305175781",
			.adder = "0", 
			.transfer_function = "x/32768",
			.flags = "",
			.suffix = "lambda",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, IAP), 
			.size = bitSize(CoreVar, IAP), 
			.isSigned = 0,
			.name = "IAP", 
			.description = "Intercooler Absolute Pressure", 
			.multiplier = "0.01",
			.adder = "0", 
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "kPa",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, MAF), 
			.size = bitSize(CoreVar, MAF), 
			.isSigned = 0,
			.name = "MAF", 
			.description = "Mass Air Flow", 
			.multiplier = "1",
			.adder = "0", 
			.transfer_function = "x",
			.flags = "",
			.suffix = "raw",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, DMAP), 
			.size = bitSize(CoreVar, DMAP), 
			.isSigned = 0,
			.name = "DMAP", 
			.description = "MAP rate of change", 
			.multiplier = "1",
			.adder = "0", 
			.transfer_function = "x",
			.flags = "",
			.suffix = "kPa/second",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, DTPS), 
			.size = bitSize(CoreVar, DTPS), 
			.isSigned = 0,
			.name = "DMAP", 
			.description = "TPS rate of change", 
			.multiplier = "1",
			.adder = "0", 
			.transfer_function = "x",
			.flags = "",
			.suffix = "%/second",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, RPM), 
			.size = bitSize(CoreVar, RPM), 
			.isSigned = 0,
			.name = "RPM", 
			.description = "Instant Engine RPM",
			.multiplier = "0.5",
			.adder = "0", 
			.transfer_function = "x/2",
			.flags = "",
			.suffix = "RPM",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, DRPM), 
			.size = bitSize(CoreVar, DRPM), 
			.isSigned = 0,
			.name = "DRPM", 
			.description = "Engine RPM rate of change", 
			.multiplier = "0.5",
			.adder = "0", 
			.transfer_function = "x/2",
			.flags = "",
			.suffix = "RPM/second",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(CoreVar, DDRPM), 
			.size = bitSize(CoreVar, DDRPM), 
			.isSigned = 0,
			.name = "DDRPM", 
			.description = "RPM Acceleration", 
			.multiplier = "0.5",
			.adder = "0", 
			.transfer_function = "x/2",
			.flags = "",
			.suffix = "RPM/second^2",
			.bitFieldDescription = {{0}},
		}
};
CASSERT((sizeof(CoreVar) / sizeof(unsigned short)) == (sizeof(coreVarsDescriptor) / sizeof(dataBlockDescriptor)), DATALOGDEFINITIONS_C) // At least check for correct number of entries
const unsigned char coreVarsDescriptorCnt = (sizeof(coreVarsDescriptor) / sizeof(dataBlockDescriptor));

const dataBlockDescriptor derrivedVarsDescriptor[] PPAGE_E5 ={
		{
			.start = offsetof(DerivedVar, LoadMain), 
			.size = bitSize(DerivedVar, LoadMain), 
			.isSigned = 0,
			.name = "LoadMain", 
			.description = "Algorithm dependent representation of engine load", 
			.multiplier = "0.01",
			.adder = "0",
			.transfer_function = "x/100",
			.flags = "",
			.suffix = "Load",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, VEMain), 
			.size = bitSize(DerivedVar, VEMain), 
			.isSigned = 0,
			.name = "VEMain", 
			.description = "Current Volumetric Efficiency",
			.multiplier = "0.001953125",
			.adder = "0",
			.transfer_function = "x/512",
			.flags = "",
			.suffix = "VE %",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, Lambda),
			.size = bitSize(DerivedVar, Lambda),
			.isSigned = 0,
			.name = "Lambda",
			.description = "FIXME ADDME",
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, AirFlow), 
			.size = bitSize(DerivedVar, AirFlow), 
			.isSigned = 0,
			.name = "AirFlow", 
			.description = "Top half of the equation (to be removed?)", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, densityAndFuel), 
			.size = bitSize(DerivedVar, densityAndFuel), 
			.isSigned = 0,
			.name = "densityAndFuel", 
			.description = "Bottom half of the equation (to be removed?)", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, BasePW), 
			.size = bitSize(DerivedVar, BasePW), 
			.isSigned = 0,
			.name = "BasePW", 
			.description = "Raw PW before corrections", 
			.multiplier = "0.0008",
			.adder = "0",
			.transfer_function = "x/1250",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, ETE), 
			.size = bitSize(DerivedVar, ETE), 
			.isSigned = 0,
			.name = "ETE", 
			.description = "Engine Temperature Enrichment percentage", 
			.multiplier = "0.00305175",
			.adder = "0",
			.transfer_function = "x/327.68",
			.flags = "",
			.suffix = "%",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, TFCTotal), 
			.size = bitSize(DerivedVar, TFCTotal), 
			.isSigned = 1,
			.name = "TFCTotal", 
			.description = "Transient Fuel correction PW", 
			.multiplier = "0.0008",
			.adder = "0",
			.transfer_function = "x/1250",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, EffectivePW), 
			.size = bitSize(DerivedVar, EffectivePW), 
			.isSigned = 0,
			.name = "EffectivePW", 
			.description = "Actual PW for fuel delivery before per channel corrections", 
			.multiplier = "0.0008",
			.adder = "0",
			.transfer_function = "x/1250",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, IDT), 
			.size = bitSize(DerivedVar, IDT), 
			.isSigned = 0,
			.name = "IDT", 
			.description = "Injector Dead Time (time before fuel flow begins)", 
			.multiplier = "0.0008",
			.adder = "0",
			.transfer_function = "x/1250",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, RefPW), 
			.size = bitSize(DerivedVar, RefPW), 
			.isSigned = 0,
			.name = "RefPW", 
			.description = "Reference PW (i.e. actual PW before traims are applied)", 
			.multiplier = "0.0008",
			.adder = "0",
			.transfer_function = "x/1250",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, Advance), 
			.size = bitSize(DerivedVar, Advance), 
			.isSigned = 0,
			.name = "Advance", 
			.description = "Ignition Timing Advance", 
			.multiplier = "0.02",
			.adder = "0",
			.transfer_function = "x/50",
			.flags = "",
			.suffix = "deg",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(DerivedVar, Dwell), 
			.size = bitSize(DerivedVar, Dwell), 
			.isSigned = 0,
			.name = "Dwell", 
			.description = "Dwell Period", 
			.multiplier = "0.0008",
			.adder = "0",
			.transfer_function = "x/1250",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		}
};
CASSERT((sizeof(DerivedVar) / sizeof(unsigned short)) == (sizeof(derrivedVarsDescriptor) / sizeof(dataBlockDescriptor)), DATALOGDEFINITIONS_C) // At least check for correct number of entries
const unsigned char derrivedVarsDescriptorCnt = (sizeof(derrivedVarsDescriptor) / sizeof(dataBlockDescriptor));

const dataBlockDescriptor KeyUserVarsDescriptor[] PPAGE_E5 ={
		{
			.start = offsetof(KeyUserDebug, tempClock), 
			.size = bitSize(KeyUserDebug, tempClock), 
			.isSigned = 0,
			.name = "tempClock", 
			.description = "The number of datalog's sent", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, spareChar), 
			.size = bitSize(KeyUserDebug, spareChar), 
			.isSigned = 0,
			.name = "spareChar", 
			.description = "Unused at this time", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, coreStatusA), 
			.size = bitSize(KeyUserDebug, coreStatusA), 
			.isSigned = 0,
			.name = "coreStatusA", 
			.description = "Status Bits Register 'A'", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {
					{.fieldName = "FuelPumpPrime", .bitPosition = 0}
			}
		},
		{
			.start = offsetof(KeyUserDebug, decoderFlags), 
			.size = bitSize(KeyUserDebug, decoderFlags), 
			.isSigned = 0,
			.name = "decoderFlags", 
			.description = "Decoder status bits, sync levels, and other details", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
			.bitFieldDescription = {
					{.fieldName = "LastPeriodValid", .bitPosition = 4},
					{.fieldName = "LastStampValid", .bitPosition = 3},
					{.fieldName = "CamSync", .bitPosition = 2},
					{.fieldName = "CrankSync", .bitPosition = 1},
					{.fieldName = "CombustionSync", .bitPosition = 0}
			}
		},
		{
			.start = offsetof(KeyUserDebug, flaggableFlags), 
			.size = bitSize(KeyUserDebug, flaggableFlags), 
			.isSigned = 0,
			.name = "flaggableFlags", 
			.description = "Counter incremented status bits for various things like comms and decoders etc.", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {
					{.fieldName = "SerialPacketsUnderLength", .bitPosition = 13},
					{.fieldName = "SerialChecksumMismatches", .bitPosition = 12},
					{.fieldName = "SerialPacketsOverLength", .bitPosition = 11},
					{.fieldName = "SerialStartsInsideAPacket", .bitPosition = 10},
					{.fieldName = "SerialEscapePairMismatches", .bitPosition = 9},
					{.fieldName = "SerialOverrunErrors", .bitPosition = 8},
					{.fieldName = "SerialParityErrors", .bitPosition = 7},
					{.fieldName = "SerialFramingErrors", .bitPosition = 6},
					{.fieldName = "SerialNoiseErrors", .bitPosition = 5},
					{.fieldName = "DecoderSyncStateClears", .bitPosition = 4},
					{.fieldName = "DecoderSyncCorrections", .bitPosition = 3},
					{.fieldName = "DecoderSyncLosses", .bitPosition = 2},
					{.fieldName = "LowVoltageConditions", .bitPosition = 1},
					{.fieldName = "CallsToUnimplmentedISRs", .bitPosition = 0}
			}
		},
		{
			.start = offsetof(KeyUserDebug, currentEvent), 
			.size = bitSize(KeyUserDebug, currentEvent), 
			.isSigned = 0,
			.name = "currentEvent", 
			.description = "Which event was the latest to come in", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, syncLostWithThisID), 
			.size = bitSize(KeyUserDebug, syncLostWithThisID), 
			.isSigned = 0,
			.name = "syncLostWithThisID", 
			.description = "A unique identifier for the reason behind a loss of sync", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, syncLostOnThisEvent), 
			.size = bitSize(KeyUserDebug, syncLostOnThisEvent), 
			.isSigned = 0,
			.name = "syncLostOnThisEvent", 
			.description = "Where in the input pattern it all went very badly wrong", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, syncCaughtOnThisEvent), 
			.size = bitSize(KeyUserDebug, syncCaughtOnThisEvent), 
			.isSigned = 0,
			.name = "syncCaughtOnThisEvent", 
			.description = "Where in the input pattern that things started making sense", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, syncResetCalls), 
			.size = bitSize(KeyUserDebug, syncResetCalls), 
			.isSigned = 0,
			.name = "syncResetCalls", 
			.description = "Sum of sync losses and state clears", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, primaryTeethSeen), 
			.size = bitSize(KeyUserDebug, primaryTeethSeen), 
			.isSigned = 0,
			.name = "primaryTeethSeen", 
			.description = "Free running counters for number of input events, useful at lower RPM", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, secondaryTeethSeen), 
			.size = bitSize(KeyUserDebug, secondaryTeethSeen), 
			.isSigned = 0,
			.name = "secondaryTeethSeen", 
			.description = "Free running counters for number of input events, useful at lower RPM", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, serialOverrunErrors), 
			.size = bitSize(KeyUserDebug, serialOverrunErrors), 
			.isSigned = 0,
			.name = "serialOverrunErrors", 
			.description = "Incremented when an overrun occurs due to high interrupt load, yes it is a fault FIXME",
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, serialHardwareErrors), 
			.size = bitSize(KeyUserDebug, serialHardwareErrors), 
			.isSigned = 0,
			.name = "serialHardwareErrors", 
			.description = "Sum of noise, parity, and framing errors", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, serialAndCommsCodeErrors), 
			.size = bitSize(KeyUserDebug, serialAndCommsCodeErrors), 
			.isSigned = 0,
			.name = "serialAndCommsCodeErrors", 
			.description = "Sum of checksum, escape mismatches, starts inside, and over/under length", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, inputEventTimeTolerance), 
			.size = bitSize(KeyUserDebug, inputEventTimeTolerance), 
			.isSigned = 0,
			.name = "inputEventTimeTolerance", 
			.description = "Required to tune noise rejection over RPM TODO add to LT1 and MissingTeeth", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, flaggableFlags2), 
			.size = bitSize(KeyUserDebug, flaggableFlags2), 
			.isSigned = 0,
			.name = "flaggableFlags2", 
			.description = "Counter incremented status bits for various things like comms and decoders etc applicable to flaggables2 struct.", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, zsp9), 
			.size = bitSize(KeyUserDebug, zsp9), 
			.isSigned = 0,
			.name = "zsp9", 
			.description = "Spare 9", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, zsp8), 
			.size = bitSize(KeyUserDebug, zsp8), 
			.isSigned = 0,
			.name = "zsp8", 
			.description = "Spare 8", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, zsp7), 
			.size = bitSize(KeyUserDebug, zsp7), 
			.isSigned = 0,
			.name = "zsp7", 
			.description = "Spare 7", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, zsp6), 
			.size = bitSize(KeyUserDebug, zsp6), 
			.isSigned = 0,
			.name = "zsp6", 
			.description = "Spare 6", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, blendAlphaNPercent), 
			.size = bitSize(KeyUserDebug, blendAlphaNPercent), 
			.isSigned = 0,
			.name = "blendAlphaNPercent", 
			.description = "Not documented yet", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, speedDensityAirFlow), 
			.size = bitSize(KeyUserDebug, speedDensityAirFlow), 
			.isSigned = 0,
			.name = "speedDensityAirflow", 
			.description = "Not documented yet", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, alphaNAirFlow), 
			.size = bitSize(KeyUserDebug, alphaNAirFlow), 
			.isSigned = 0,
			.name = "alphaNAirFlow", 
			.description = "Not documented yet", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, clockInMilliSeconds), 
			.size = bitSize(KeyUserDebug, clockInMilliSeconds), 
			.isSigned = 0,
			.name = "clockInMilliSeconds", 
			.description = "Internal Clock in milliseconds", 
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, clockIn8thsOfAMilli), 
			.size = bitSize(KeyUserDebug, clockIn8thsOfAMilli), 
			.isSigned = 0,
			.name = "clockIn8thsofAMilli", 
			.description = "Internal Clock in 1/8th millisecond increments", 
			.multiplier = "0.125",
			.adder = "0",
			.transfer_function = "x/8",
			.flags = "",
			.suffix = "ms",
			.bitFieldDescription = {{0}},
		},
		{
			.start = offsetof(KeyUserDebug, ignitionCuts), 
			.size = bitSize(KeyUserDebug, ignitionCuts), 
			.isSigned = 0,
			.name = "ignitionCuts", 
			.description = "Bit-field of reasons why ignition was cut",
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {
					{.fieldName = "IgnitioncurOverBoost", .bitPosition = 1},
					{.fieldName = "IgnitionCutOverRPM", .bitPosition = 0}
			}
		},
		{
			.start = offsetof(KeyUserDebug, injectionCuts), 
			.size = bitSize(KeyUserDebug, injectionCuts), 
			.isSigned = 0,
			.name = "injectionCuts", 
			.description = "Bit-field of reasons why injection was cut",
			.multiplier = "1",
			.adder = "0",
			.transfer_function = "x",
			.flags = "",
			.suffix = "",
			.bitFieldDescription = {
					{.fieldName = "InjectionLostSelfClock", .bitPosition = 3},
					{.fieldName = "InjectionLostPLLSync", .bitPosition = 2},
					{.fieldName = "InjectionCutOverBoost", .bitPosition = 1},
					{.fieldName = "InjectionCutOverRPM", .bitPosition = 0}
			}
		}
};
//CASSERT((sizeof(KeyUserDebug) / sizeof(unsigned short)) == (sizeof(KeyUserVarsDescriptor) / sizeof(dataBlockDescriptor)), DATALOGDEFINITIONS_C) // At least check for correct number of entries
const unsigned char KeyUserVarsDescriptorCnt = (sizeof(KeyUserVarsDescriptor) / sizeof(dataBlockDescriptor));


