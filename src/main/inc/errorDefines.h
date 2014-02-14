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
 * @ingroup globalHeaders
 *
 * @brief Error ID hash defines
 *
 * This file contains all error codes for propagation up and out to a PC.
 * When adding new codes, ensure that they are unique and also that you
 * group them appropriately in ranges for easy visual identification.
 */


/* Header file multiple inclusion protection courtesy eclipse Header Template */
/* and http://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/ C pre processor manual    */
#ifndef FILE_ERRORDEFINES_H_SEEN
#define FILE_ERRORDEFINES_H_SEEN


/** @todo TODO Introduce some structure to the numbering such that they can be interpreted somewhat without a chart */

// A OK Nothing is wrong No error error.
#define NO_PROBLEMO                     0x0666

/* Unconfigured options */
#define IAT_NOT_CONFIGURED_CODE  0x1000
#define CHT_NOT_CONFIGURED_CODE  0x1001
#define BRV_NOT_CONFIGURED_CODE  0x1002
#define LOAD_NOT_CONFIGURED_CODE 0x1003


/* Badly configured options */
#define VE_TABLE_MAIN_LOAD_LENGTH_TOO_LONG 0x2000
#define VE_TABLE_MAIN_RPM_LENGTH_TOO_LONG  0x2001
#define VE_TABLE_MAIN_MAIN_LENGTH_TOO_LONG 0x2002
#define BRV_MAX_TOO_LARGE                  0x2003


/* Flash burning error codes */
#define SIZE_NOT_MULTIPLE_OF_SECTOR_SIZE     0x3000
#define SIZE_OF_BLOCK_TO_BURN_IS_ZERO         0x3001
#define SMALL_BLOCK_CROSSES_SECTOR_BOUNDARY 0x3002
#define ADDRESS_NOT_SECTOR_ALIGNED         0x3003
#define ADDRESS_NOT_WORD_ALIGNED           0x3004
#define ADDRESS_NOT_FLASH_REGION           0x3005
#define FLASH_ERASE_FAILED                0x3006
#define FLASH_ACCESS_ERROR                0x3007
#define FLASH_PROTECTION_ERROR            0x3008


// memory check error
#define MEMORY_WRITE_ERROR                   0x3009

/* Communications error codes */
#define UNIMPLEMENTED_FUNCTION                0x4000
#define PACKET_CHECKSUM_MISMATCH               0x4001
#define PACKET_TOO_SHORT_FOR_SPECIFIED_FIELDS     0x4002
#define DOES_NOT_MAKE_SENSE_TO_RETRIEVE_PARTIALLY  0x4003
#define PAYLOAD_LENGTH_TYPE_MISMATCH            0x4004
#define PAYLOAD_LENGTH_HEADER_MISMATCH          0x4005
#define INVALID_PAYLOAD_ID                     0x4006
#define UNRECOGNISED_PAYLOAD_ID                0x4007
#define INVALID_MEMORY_ACTION_FOR_ID             0x4008
#define INVALID_ID_FOR_MAIN_TABLE_ACTION          0x4009
#define INVALID_ID_FOR_TWOD_TABLE_ACTION          0x400A
#define NO_SUCH_ASYNC_DATALOG_TYPE               0x400B
#define DATALOG_LENGTH_EXCEEDS_MAX              0x400C
#define LOCATION_ID_NOT_FOUND                   0x400D
#define REQUESTED_RAM_PAGE_INVALID              0x400E
#define REQUESTED_FLASH_PAGE_INVALID            0x400F
#define REQUESTED_LENGTH_TOO_LARGE              0x4010
#define REQUESTED_ADDRESS_DISALLOWED           0x4011
#define INVALID_SIZE_OFFSET_COMBINATION         0x4012
#define UNCHECKED_TABLE_MANIPULATION_NOT_ALLOWED 0x4013
#define PAYLOAD_NOT_EQUAL_TO_SPECIFIED_VALUE      0x4014
#define NO_SUCH_LOCATION_ID_LIST_TYPE             0x4015
#define PAYLOAD_SHORTER_THAN_REQUIRED_FOR_TEST    0x4016
#define NO_SUCH_UNIT_TEST_ID                     0x4017
#define ATTEMPT_TO_WRITE_TO_READ_ONLY_BLOCK        0x4018

#define INVALID_AXIS_ORDER 1 /* prevent parsing */
#define INVALID_AXIS_INDEX 2 /* prevent parsing */

#define ERROR_BASE_MAIN_TABLE_RPM           0x6000
#define INVALID_MAIN_TABLE_RPM_ORDER        0x6001
#define INVALID_MAIN_TABLE_RPM_INDEX        0x6002
#define INVALID_MAIN_TABLE_RPM_LENGTH       0x6003
#define ERROR_BASE_MAIN_TABLE_LOAD          0x6004
#define INVALID_MAIN_TABLE_LOAD_ORDER       0x6005
#define INVALID_MAIN_TABLE_LOAD_INDEX       0x6006
#define INVALID_MAIN_TABLE_LOAD_LENGTH      0x6007
#define INVALID_MAIN_TABLE_MAIN_LENGTH      0x6008

#define ERROR_BASE_TWO_D_TABLE_AXIS          0x6010
#define INVALID_TWO_D_TABLE_AXIS_ORDER       0x6011
#define INVALID_TWO_D_TABLE_INDEX           0x6012

#define THIS_IS_NOT_THE_BENCH_TEST_DECODER    0x7777
#define TOO_SHORT_OF_A_PULSE_WIDTH_TO_TEST     0x7778
#define UNIMPLEMENTED_TEST_MODE           0x7779
#define INVALID_EVENTS_PER_CYCLE           0x777A
#define INVALID_NUMBER_OF_CYCLES           0x777B
#define TOO_SHORT_OF_AN_EVENT_PERIOD         0x777C
#define NO_CHANNELS_CONFIGURED_TO_TEST      0x777D
#define BENCH_TEST_ALREADY_RUNNING         0x777E
#define PACKET_SIZE_WRONG_FOR_TEST_MODE      0x777F
#define BUMPING_BY_ZERO_MAKES_NO_SENSE       0x7780
#define BENCH_TEST_NOT_RUNNING_TO_STOP       0x7781
#define BENCH_TEST_NOT_RUNNING_TO_BUMP       0x7782
#define TOO_MANY_EVENTS_PER_CYCLE_MISSING_TTH 0x7783


#else
	/* let us know if we are being untidy with headers */
	#warning "Header file ERRORDEFINES_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
