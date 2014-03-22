/* FreeEMS - the open source engine management system
 *
 * Copyright 2008-2013 Fred Cooke
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
#ifndef FILE_COMMS_CORE_H_SEEN
#define FILE_COMMS_CORE_H_SEEN


// Buffer size minus the overhead of a maximal packet header
#define TX_MAX_PAYLOAD_SIZE   (TX_BUFFER_SIZE - 32)


/* Function declarations */
/* This function accesses paged flash and thus must be in linear space. Set explicitly to text. */
void decodePacketAndRespond(void) TEXT;

void resetReceiveState(unsigned char) FPAGE_FE;
void finaliseAndSend(unsigned short) FPAGE_FE;

unsigned short populateBasicDatalog(void) FPAGE_FE;


// Shared serial comms stuff TODO move this to a commsCommon.h header
#define START_BYTE          0xAA
#define ESCAPE_BYTE         0xBB
#define STOP_BYTE           0xCC

#define ESCAPED_START_BYTE  0x55
#define ESCAPED_ESCAPE_BYTE 0x44
#define ESCAPED_STOP_BYTE   0x33


/* Masks for TXBufferInUseFlags and RXBufferContentSourceID */
#define COM_SET_SCI0_INTERFACE_ID     BIT0
#define COM_SET_CAN0_INTERFACE_ID     BIT1
#define COM_SET_SPARE2_INTERFACE_ID   BIT2
#define COM_SET_SPARE3_INTERFACE_ID   BIT3
#define COM_SET_SPARE4_INTERFACE_ID   BIT4
#define COM_SET_SPARE5_INTERFACE_ID   BIT5
#define COM_SET_SPARE6_INTERFACE_ID   BIT6
#define COM_SET_SPARE7_INTERFACE_ID   BIT7
#define COM_CLEAR_SCI0_INTERFACE_ID   NBIT0
#define COM_CLEAR_CAN0_INTERFACE_ID   NBIT1
#define COM_CLEAR_SPARE2_INTERFACE_ID NBIT2
#define COM_CLEAR_SPARE3_INTERFACE_ID NBIT3
#define COM_CLEAR_SPARE4_INTERFACE_ID NBIT4
#define COM_CLEAR_SPARE5_INTERFACE_ID NBIT5
#define COM_CLEAR_SPARE6_INTERFACE_ID NBIT6
#define COM_CLEAR_SPARE7_INTERFACE_ID NBIT7
#define CLEAR_ALL_SOURCE_ID_FLAGS     ZEROS


/* Masks for SCIRXStateFlags */
//#define RX_BUFFER_IN_USE          BIT0
#define RX_READY_TO_PROCESS       BIT1
#define RX_SCI_ESCAPED_NEXT       BIT2
//#define RX_SCI_INSIDE_PACKET      BIT3
//#define RX_BUFFER_NOT_IN_USE      NBIT0
#define RX_CLEAR_READY_TO_PROCESS NBIT1
#define RX_SCI_NOT_ESCAPED_NEXT   NBIT2
//#define RX_SCI_NOT_INSIDE_PACKET  NBIT3

/* Global CAN specific variables */
// TODO can id filters routing etc


/* Comms Control Enable and Disable Masks */
/* SCI masks */
#define SCICR2_RX_ENABLE      BIT2
#define SCICR2_TX_ENABLE      BIT3
#define SCICR2_RX_ISR_ENABLE  BIT5
#define SCICR2_TX_ISR_ENABLE  BIT7
#define SCICR2_RX_DISABLE     NBIT2
#define SCICR2_TX_DISABLE     NBIT3
#define SCICR2_RX_ISR_DISABLE NBIT5
#define SCICR2_TX_ISR_DISABLE NBIT7
/* CAN masks */
// TODO probably 8 of these too
// TODO probably 8 of these too
// TODO probably 8 of these too
// TODO probably 8 of these too
// TODO probably 8 of these too
// TODO probably 8 of these too
// TODO probably 8 of these too
// TODO probably 8 of these too


/* Header flag masks */
/* Always has flags (obviously)     */
/* Always has payload ID so no flag */
/* Always has checksum, so no flag  */
#define HEADER_HAS_LENGTH       BIT0
#define HEADER_IS_NACK          BIT1
#define HEADER_HAS_SEQUENCE     BIT2
#define HEADER_PARTIAL_PAYLOAD  BIT3
#define HEADER_RESERVED_D       BIT4
#define HEADER_RESERVED_C       BIT5
#define HEADER_RESERVED_B       BIT6
#define HEADER_RESERVED_A       BIT7


#define ASYNCDATALOGOFF        0x00 ///< Logs by polling only, reduces CPU load a little but gives much lower data rate
#define ASYNCDATALOGBASIC      0x01 ///< Good old default log, always contains the normal stuff, good for most people, most of the time
#define ASYNCDATALOGSCRATCHPAD 0x02 ///< User log of anything, any subset of a block allowed, start offset, size, content pointers/flags
#define ASYNCDATALOGSTRUCTS    0x03 ///< Key structs, or subsets of them, or chunk to chunk, streamed, more efficient than scratch pad and bigger if needed
#define ASYNCDATALOGPOSITION   0x04 ///< Record a buffer of position information, send when full: http://forum.diyefi.org/viewtopic.php?f=8&t=1339
#define ASYNCDATALOGBLOCKBYTES 0x05 ///< Populate a large block with bytes as fast as possible, send when full
#define ASYNCDATALOGBLOCKWORDS 0x06 ///< Populate a large block with bytes as fast as possible, send when full
#define ASYNCDATALOGBLOCKLONGS 0x07 ///< Populate a large block with bytes as fast as possible, send when full
#define ASYNCDATALOGSTREAMBYTE 0x08 ///< Send out a single byte as often as possible, ~1kHz with occasional ~3ms gaps from math running instead
#define ASYNCDATALOGSTREAMWORD 0x09 ///< Send out a single word as often as possible, ~1kHz with occasional ~3ms gaps from math running instead
#define ASYNCDATALOGSTREAMLONG 0x0A ///< Send out a single long as often as possible, ~1kHz with occasional ~3ms gaps from math running instead
#define ASYNCDATALOGLASTTYPE   ASYNCDATALOGSTREAMLONG

extern Clock Clocks;
extern KeyUserDebug KeyUserDebugs;
extern Clock Clocks;
extern Counter Counters;
extern KeyUserDebug KeyUserDebugs;
extern Flaggable Flaggables;
extern Flaggable2 Flaggables2;
extern const unsigned char interfaceVersion[INTERFACE_VERSION_LENGTH];
extern const unsigned char firmwareVersion[FIRMWARE_VERSION_LENGTH];
extern const unsigned char buildTimeAndDate[FIRMWARE_BUILD_DATE_LENGTH];
extern const unsigned char compilerVersion[COMPILER_VERSION_LENGTH];
extern const unsigned char operatingSystem[OPERATING_SYSTEM_LENGTH];
extern const unsigned short injectorSwitchOnCodeTime;

// Stuff that should only be found in this file.
#include "packetTypes.h"
#include "unitTestIDs.h"


/* end of the wrapper ifdef from the very top */
#endif
