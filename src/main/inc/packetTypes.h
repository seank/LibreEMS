/* FreeEMS - the open source engine management system
 *
 * Copyright 2008-2012 Fred Cooke
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
 * along with any FreeEMS software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at admin(at)diyefi(dot)org or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeEMS to run your engine!
 */


/** @file
 *
 * @ingroup allHeaders
 */


/* Header file multiple inclusion protection courtesy eclipse Header Template */
/* and http://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/ C pre processor manual    */
#ifndef FILE_PACKET_TYPES_H_SEEN
#define FILE_PACKET_TYPES_H_SEEN


/*&&&&&&&&&&&&&&&&&&&&  Payload Type ID Name Value Pairs  &&&&&&&&&&&&&&&&&&&&*/

/* Please note, requests use even ID numbers and their matching responses use */
/* the odd number immediately above the original request ID. There are a few  */
/* examples where either a request is not required or a response not sent. In */
/* such cases the odd ID may be used for an packet asyncronously sent from    */
/* the EMS to the PC. These will be well commented when present.              */


/*&&&&&&&&&&&&&&&&&&&&&&&&  Protocol Payload Type IDs  &&&&&&&&&&&&&&&&&&&&&&&*/

/* Firmware Independent functions */
#define REQUEST_INTERFACE_VERSION     0x0000
#define REQUEST_FIRMWARE_VERSION      0x0002
#define REQUEST_MAX_PACKET_SIZE        0x0004
#define REQUEST_ECHO_PACKET_RETURN     0x0006
#define REQUEST_SOFT_SYSTEM_RESET      0x0008 /* System comes up fresh, so no response */
//efine REPLYTO_SOFT_SYSTEM_RESET      0x0009 /* This is reserved */
#define REQUEST_HARD_SYSTEM_RESET      0x000A /* System comes up fresh, so no response */
//efine REPLYTO_HARD_SYSTEM_RESET      0x000B /* This is reserved */
#define REQUEST_RE_INIT_OF_SYSTEM       0x000C /* EXPERIMENTAL, currently responds like normal */
//efine REPLYTO_RE_INIT_OF_SYSTEM       0x000D /* This is reserved */


//efine REQUEST_ASYNC_DEBUG_INFO       0x000E /* This is reserved */
#define ASYNC_DEBUG_INFO_PACKET        0x000F /* NOTE : Unrequested debug information packet */
//efine REQUEST_ASYNC_ERROR_CODE       0x0010 /* This is reserved */
#define ASYNC_ERROR_CODE_PACKET        0x0011 /* NOTE : Unrequested error event code packet */


/*&&&&&&&&&&&&&&&&&&&&&&&&  Firmware Payload Type IDs  &&&&&&&&&&&&&&&&&&&&&&&*/

/* Data block manipulation */
#define UPDATE_BLOCK_IN_RAM            0x0100
#define UPDATE_BLOCK_IN_FLASH          0x0102
#define RETRIEVE_BLOCK_FROM_RAM        0x0104
#define RETRIEVE_BLOCK_FROM_FLASH      0x0106
#define BURN_BLOCK_FROM_RAM_TO_FLASH     0x0108

/* Datalog request packets */
#define REQUEST_DATALOG_PACKET        0x0190
#define RESPONSE_BASIC_DATALOG        0x0191 /* Defined because it can be used both synchronously and asynchronously */
#define REQUEST_BYTE_LA_DATALOG        0x0196 // logic analyser
#define RESPONSE_BYTE_LA_DATALOG       0x0197 // logic analyser

/* Generic memory grabber for debugging */
#define RETRIEVE_ARBITRARY_MEMORY     0x0258

/* Experimental unit testing interface */
#define REQUEST_UNIT_TEST_OVER_SERIAL   0x6666

// Bench testing init calls
#define START_BENCH_TEST_SEQUENCE      0x8888

// Data interrogation gold
#define RETRIEVE_LIST_OF_LOCATION_IDS   0xDA5E // Idea is Dave's, impl is Fred's
#define RETRIEVE_LOCATION_ID_DETAILS   0xF8E0 // Idea is Fred's, impl is Fred's
#define REQUEST_DECODER_NAME          0xEEEE // Makes interrogation unique within a single build
#define REQUEST_FIRMWARE_BUILD_DATE    0xEEF0 // When it was built
#define REQUEST_COMPILER_VERSION      0xEEF2 // Which GCC built it
#define REQUEST_OPERATING_SYSTEM      0xEEF4 // Which OS was it built on
#define CLEAR_COUNTERS_AND_FLAGS_TO_ZERO 0xFFF0 // Clean slate to perform tests of any type. Clears various flags too, should not affect running

// DEPRECATED TODO Remove once certain no users exist
#define ADJUST_MAIN_TABLE_CELL         0x012C
#define ADJUST_MAIN_TABLE_RPM_AXIS      0x012E
#define ADJUST_MAIN_TABLE_LOAD_AXIS     0x0130
#define ADJUST2D_TABLE_AXIS           0x0132
#define ADJUST2D_TABLE_CELL           0x0134
#define SET_ASYNC_DATALOG_TYPE         0x0194

#else
	/* let us know if we are being untidy with headers */
	#warning "Header file PACKET_TYPES_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
