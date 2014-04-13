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
 * @ingroup communicationsFiles
 *
 * @brief Core communications functions.
 *
 * This file contains most of the core comms functionality. Currently that is
 * only for UART serial style communication. It is already too big and needs
 * to be split up somewhat. This will happen fairly soon during the serial
 * refactoring and protocol fine tuning.
 *
 * @todo TODO function to setup a packet and send it fn(populateBodyFunctionPointer(), header, other, fields, here, and, use, or, not, within){}
 * @todo TODO factor many things into functions and move the receive delegator to its own file
 */


#define COMMSCORE_C
#include "inc/freeEMS.h"
#include "inc/flashWrite.h"
#include "inc/interrupts.h"
#include "inc/utils.h"
#include "inc/tableLookup.h"
#include "inc/locationIDs.h"
#include "inc/blockDetailsLookup.h"
#include "inc/decoderInterface.h"
#include "inc/commsCore.h"
#include "inc/init.h"
#include <string.h> /// @todo TODO this is pulling in the system string.h not the m68hc1x version, and functions other than memcpy do not work because they are not in crt1.o or other included-by-default libs
#include "decoders/inc/BenchTest.h"
#include "inc/dataLogDefinitions.h"
#include "inc/librePacketTypes.h"
#include "inc/commsUtils.h"


/* Global Comms Variables */
unsigned char  RXBufferContentSourceID;
unsigned char* RXBufferCurrentPosition; 
unsigned short RXCalculatedPayloadLength;
unsigned char  RXHeaderFlags;
unsigned short RXHeaderPayloadID;
unsigned short RXHeaderPayloadLength;
unsigned char  RXStateFlags;
unsigned char* TXHeaderFlags;
unsigned char* TXBufferCurrentPositionCAN0;
unsigned char* TXBufferCurrentPositionHandler;
unsigned char* TXBufferCurrentPositionSCI0;
unsigned char  TXBufferInUseFlags;

unsigned char  commsCoreStateFlags;
unsigned char  currentChunk;
unsigned char  currentDescription;
unsigned short baseOffset;
unsigned char  packetPayloadEnum;
unsigned short RXHeaderPayloadIDInProcess;
unsigned char  format;
unsigned short payloadID;

/** @brief Populate a basic datalog packet
 *
 * Copies various chunks of data to the transmission buffer and truncates to
 * the configured length. If changing this, update the maxBasicDatalogLength.
 */
unsigned short populateBasicDatalog(){
	/// @todo TODO setup proper sequence and clock with some sort of differential measurement log to log. insert in front of actual data because these are part of the log itself.

//	KeyUserDebugs.zsp10 = Counters.pinScheduledWithTimerExtension;
	KeyUserDebugs.coreStatusA = coreStatusA;
	KeyUserDebugs.clockIn8thsOfAMilli = Clocks.realTimeClockMain;
	KeyUserDebugs.clockInMilliSeconds = Clocks.realTimeClockMillis;

	unsigned short confSize = 0;
	unsigned char chunkLimit = TablesB.SmallTablesB.loggingSettings.firstChunk + TablesB.SmallTablesB.loggingSettings.numberOfChunks;
	unsigned char chunks;
	for(chunks=TablesB.SmallTablesB.loggingSettings.firstChunk;chunks<chunkLimit;chunks++){
		unsigned short localSize = TablesB.SmallTablesB.loggingSettings.logChunks[chunks].size;
		confSize += localSize;
		if(confSize > 2048){
			confSize -= localSize;
			break;
		}
		memcpy(TXBufferCurrentPositionHandler, TablesB.SmallTablesB.loggingSettings.logChunks[chunks].address, localSize);
		TXBufferCurrentPositionHandler += localSize;
	}
	// After copying data, otherwise tempClock is NEVER zero and reset detection does NOT work
	KeyUserDebugs.tempClock++;
	return confSize;
}


// All of these require some range checking, eg only some registers, and all RAM, not flash, not other regs
// TODO pointer for one byte
// TODO pointer for one short
// TODO function to log generic memory region by location and size ? requires length!
// Ranges are :
// RAM window
// bss/data region
// IO registers etc that can't be altered simply by reading from.
// NOT :
// flash makes no sense
// some regs are sensitive
// some RAM is unused
// serial buffers make no sense
// eeprom makes no sense
//
// 2k of regs max - user beware for now
// 12k of RAM max
//
//init :
//logaddr = fixed.addr
//loglen = fixed.len
//
//len = loglen OR 1 OR 2
//
//check :
//if((addr < 0x0800) && (length < (0x0800 - addr))){
//	// reg space is OK
//}else if(((0x1000 < addr) && (addr < 0x4000)) && (length < (0x4000 - addr))){
//	// RAM space is OK
//}else{
//	// send an error instead
//}
//
//run check at init and set time, not run time or just not check?? maybe its silly to check at all
//
// /* Just dump the ADC channels as fast as possible */
//void populateScopeLogADCAll(){
//	sampleBlockADC(TXBufferCurrentPositionHandler);
//	TXBufferCurrentPositionHandler += sizeof(ADCBuffer);
//}


// what does this mean >> ??? TODO Look at the time stamps and where to write them, also whether to function call these simple blocks or write one function that handles all the logic.


/** @brief Finalise a packet and send it
 *
 * This functions job is to finalise the main loop part of the packet sending
 * process. It configures the pos/neg ack header bit, adds the code if neg,
 * runs a checksum over the packet data and tags it to the end before
 * configuring the various ISRs that need to send the data out.
 *
 * @bug http://issues.freeems.org/view.php?id=81
 * @todo TODO fix the double/none start byte bug and remove the hack!
 */
void finaliseAndSend(unsigned short errorID){

	if(errorID != 0){
		*TXHeaderFlags |= HEADER_IS_NACK;
		*((unsigned short*)TXBufferCurrentPositionHandler) = errorID;
		TXBufferCurrentPositionHandler += 2;
	}

	/* Tag the checksum on the end */
	*TXBufferCurrentPositionHandler = checksum((unsigned char*)&TXBuffer, ((unsigned short)TXBufferCurrentPositionHandler - (unsigned short)&TXBuffer));

	/* Send it out on all the channels required. */

	/* SCI0 - Main serial interface */
	if(TXBufferInUseFlags & COM_SET_SCI0_INTERFACE_ID){
		/* Initiate transmission */
		SCI0DRL = START_BYTE;

		/* Note : Order Is Important! */
		/* TX empty flag is already set, so we must clear it by writing out before enabling the interrupt */
		SCI0CR2 |= (SCICR2_TX_ENABLE | SCICR2_TX_ISR_ENABLE);
	}
	/* CAN0 - Main CAN interface */
	if(TXBufferInUseFlags & COM_SET_CAN0_INTERFACE_ID){
		// just clear up front for now
		TXBufferInUseFlags &= COM_CLEAR_CAN0_INTERFACE_ID;
	}
	/* spare2 */
	if(TXBufferInUseFlags & COM_SET_SPARE2_INTERFACE_ID){
		// just clear up front for now
		TXBufferInUseFlags &= COM_CLEAR_SPARE2_INTERFACE_ID;
	}
	/* spare3 */
	if(TXBufferInUseFlags & COM_SET_SPARE3_INTERFACE_ID){
		// just clear up front for now
		TXBufferInUseFlags &= COM_CLEAR_SPARE3_INTERFACE_ID;
	}
	/* spare4 */
	if(TXBufferInUseFlags & COM_SET_SPARE4_INTERFACE_ID){
		// just clear up front for now
		TXBufferInUseFlags &= COM_CLEAR_SPARE4_INTERFACE_ID;
	}
	/* spare5 */
	if(TXBufferInUseFlags & COM_SET_SPARE5_INTERFACE_ID){
		// just clear up front for now
		TXBufferInUseFlags &= COM_CLEAR_SPARE5_INTERFACE_ID;
	}
	/* spare6 */
	if(TXBufferInUseFlags & COM_SET_SPARE6_INTERFACE_ID){
		// just clear up front for now
		TXBufferInUseFlags &= COM_CLEAR_SPARE6_INTERFACE_ID;
	}
	/* spare7 */
	if(TXBufferInUseFlags & COM_SET_SPARE7_INTERFACE_ID){
		// just clear up front for now
		TXBufferInUseFlags &= COM_CLEAR_SPARE7_INTERFACE_ID;
	}
}


/** @brief Decode a packet and respond
 *
 * This is the core function that controls which functionality is run when a
 * packet is received in full by the ISR code and control is passed back to the
 * main loop code. The vast majority of communications action happens here.
 */
void decodePacketAndRespond(){
	// How big was the packet that we got back
	unsigned short RXPacketLengthReceived = (unsigned short)RXBufferCurrentPosition - (unsigned short)&RXBuffer;

	/* Check that the packet is big enough for header,ID,checksum */
	if(RXPacketLengthReceived < 4){
		resetReceiveState(CLEAR_ALL_SOURCE_ID_FLAGS);
		FLAG_AND_INC_FLAGGABLE(FLAG_SERIAL_PACKETS_UNDER_LENGTH_OFFSET);
		KeyUserDebugs.serialAndCommsCodeErrors++;
		return;
	}

	/* Pull out the received checksum and calculate the real one, then check */
	unsigned char RXReceivedChecksum = (unsigned char)*(RXBufferCurrentPosition - 1);
	unsigned char RXCalculatedChecksum = checksum((unsigned char*)&RXBuffer, RXPacketLengthReceived - 1);
	if(RXCalculatedChecksum != RXReceivedChecksum){
		resetReceiveState(CLEAR_ALL_SOURCE_ID_FLAGS);
		FLAG_AND_INC_FLAGGABLE(FLAG_SERIAL_CHECKSUM_MISMATCHES_OFFSET);
		KeyUserDebugs.serialAndCommsCodeErrors++;
		return;
	}

	/* Start this off as full packet length and build down to the actual length */
	RXCalculatedPayloadLength = RXPacketLengthReceived;

	/* Grab the RX header flags out of the RX buffer */
	RXBufferCurrentPosition = (unsigned char*)&RXBuffer;
	RXHeaderFlags = *RXBufferCurrentPosition;
	RXBufferCurrentPosition++;
	RXCalculatedPayloadLength--;

	/* Grab the payload ID for processing and load the return ID */
	RXHeaderPayloadID = *((unsigned short*)RXBufferCurrentPosition);

	prepForTX();

	RXBufferCurrentPosition += 2;
	RXCalculatedPayloadLength -= 2;

	/* Check that the length is sufficient for the fields configured. Packets
	 * that are too long will be caught and rejected on an individual payload
	 * ID basis as the information required to handle that is not available at
	 * this point. Packets that are too short are rejected immediately!
	 */
	if(((RXHeaderFlags & HEADER_HAS_LENGTH) && (RXHeaderFlags & HEADER_HAS_SEQUENCE) && (RXPacketLengthReceived < 7))
		|| ((RXHeaderFlags & HEADER_HAS_LENGTH) && (RXPacketLengthReceived < 6))
		|| ((RXHeaderFlags & HEADER_HAS_SEQUENCE) && (RXPacketLengthReceived < 5))){
		finaliseAndSend(PACKET_TOO_SHORT_FOR_SPECIFIED_FIELDS);
		resetReceiveState(CLEAR_ALL_SOURCE_ID_FLAGS);
		return;
	}

	/* Subtract checksum to get final length */
	RXCalculatedPayloadLength--;

	if(RXHeaderFlags & HEADER_HAS_SEQUENCE){
		*TXBufferCurrentPositionHandler = *RXBufferCurrentPosition;
		RXBufferCurrentPosition++;
		TXBufferCurrentPositionHandler++;
		RXCalculatedPayloadLength--;
		*TXHeaderFlags |= HEADER_HAS_SEQUENCE;
	}

	if(RXHeaderFlags & HEADER_HAS_LENGTH){
		RXHeaderPayloadLength = *((unsigned short*)RXBufferCurrentPosition);
		RXBufferCurrentPosition += 2;
		RXCalculatedPayloadLength -= 2;
		/* Already subtracted one for checksum */
		if(RXHeaderPayloadLength != RXCalculatedPayloadLength){
			finaliseAndSend(PAYLOAD_LENGTH_HEADER_MISMATCH);
			resetReceiveState(CLEAR_ALL_SOURCE_ID_FLAGS);
			return;
		}
	}

	/* Calculate the position of the end of the stored packet for later use as a buffer */
	void* leftOverBuffer = (void*)((unsigned short)&RXBuffer + RXPacketLengthReceived);

	unsigned short errorID = 0;
	/* This is where all the communication logic resides.
	 *
	 * Please Note: Length and its flag should be set by each return packet
	 * type handler if required or desired. If an ack has been requested,
	 * ensure the negative ack flag is set if the operation failed.
	 */

	//tempHack:
	switch (RXHeaderPayloadID){

	// LibreEMS Core Comms Interface cases
	   case REQUEST_LIST_OF_DATASTREAM_IDS:
           //Reply packet RESPONSE_LIST_OF_DATASTREAM_IDS
		   // Here we return a list of Datalog IDs, 8-bit IDs gives us more then enough possibilities IMO, do you think storage for 5 IDs is enough?
		   // PACKET PAYLOAD RESPONSE = ...[ID,ID,ID,ID,ID]...
		   // ID 0 should be reserved for ALL, so valid IDs are 1-255
		   break;
	   case REQUEST_DEFINE_DATASTREAM_ID:
	       //(A)
		   // Here we get a datalog ID along with what variables we want. Now since the user application is aware of the total available stream
		   //variables I'm inclined to have the user application send a multi-byte bit-mask. One bit for each variable, 0 = I dont want it,
		   // 1 = yes give it to me.
		   //Example Playload:  [STREAMID, MULTIBYTE MASK]
		   //(B)
		   //Alternative, since each variable ID is only 1 byte, we can afford to send a list of IDs. If you prefer this just let me know.
		   //now that I think about it, this is more reasonable IMO. It's not like we are pressed for serial time etc.
		   //Example Playload:  [STREAMID, vID,vID,vID,vID,vID....]
	   	   break;
	   case REQUEST_SET_DATASTREAM_ID:
           //Easy enough, just set the stream ID. Once this is set the application should send REQUEST_DATALOG_DESCRIPTOR
		   //On success return the ID request
		   //On fail send the default id of 0
		   //We should doc these protocols as a LibreEMS supplemental in addition to the underlying freeems coms
		   break;
	   case RETRIEVE_DATALOG_DESCRIPTOR: /* return a data-stream descriptor in the requested format for the current datastreamID */
		   /* If format is 0 that means this is a new request */
		   	if (format == 0) {
		   		if (RXCalculatedPayloadLength != 1) {
		   			errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
		   			break;
		   		}
		   		format = *((unsigned char*) RXBufferCurrentPosition);
		   		RXBufferCurrentPosition += 1;
		   	}
		   	sendDescriptor();
	    	break;
	// FreeEMS Core Comms Interface cases
		case REQUEST_INTERFACE_VERSION:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			/* This type must have a length field, set that up */
			*((unsigned short*)TXBufferCurrentPositionHandler) = sizeof(interfaceVersion);
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;
			/* Load the body into place */
			memcpy((void*)TXBufferCurrentPositionHandler, (void*)&interfaceVersion, sizeof(interfaceVersion));
			TXBufferCurrentPositionHandler += sizeof(interfaceVersion);
			break;
		}
		case REQUEST_FIRMWARE_VERSION:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}
			/* This type must have a length field, set that up */
			*((unsigned short*)TXBufferCurrentPositionHandler) = sizeof(firmwareVersion);
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;
			/* Load the body into place */
			memcpy((void*)TXBufferCurrentPositionHandler, (void*)&firmwareVersion, sizeof(firmwareVersion));
			TXBufferCurrentPositionHandler += sizeof(firmwareVersion);
			break;
		}
		case REQUEST_MAX_PACKET_SIZE:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}
			/* Load the size into place */
			*((unsigned short*)TXBufferCurrentPositionHandler) = RX_BUFFER_SIZE;
			TXBufferCurrentPositionHandler += 2;
			break;
		}
		case REQUEST_ECHO_PACKET_RETURN:
		{
			/* This type must have a length field, set that up */
			*((unsigned short*)TXBufferCurrentPositionHandler) = RXPacketLengthReceived;
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;
			/* Load the body into place */
			memcpy((void*)TXBufferCurrentPositionHandler, (void*)&RXBuffer, RXPacketLengthReceived);
			/* Note, there is no overflow check here because the TX buffer is slightly       */
			/* bigger than the RX buffer and there is overflow checking for receives anyway. */
			TXBufferCurrentPositionHandler += RXPacketLengthReceived;
			break;
		}
		case REQUEST_SOFT_SYSTEM_RESET:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
			}else{ // Perform soft system reset
				_start();
			}
			break;
		}
		case REQUEST_HARD_SYSTEM_RESET:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
			}else{
				/* This is how the serial monitor does it. */
				COPCTL = 0x01; /* Arm with shortest time */
				ARMCOP = 0xFF; /* Write bad value, should cause immediate reset */
				/* Using _start() only resets the app ignoring the monitor switch. It does not work */
				/* properly because the location of _start is not the master reset vector location. */
			}
			break;
		}
		case REQUEST_RE_INIT_OF_SYSTEM:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
			}else{
				init();
			}
			break;
		}
	// FreeEMS Vanilla Firmware Specific cases
		case CLEAR_COUNTERS_AND_FLAGS_TO_ZERO:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			unsigned short zeroCounter;
			unsigned char* counterPointer;

			counterPointer = (unsigned char*) &Counters;
			for(zeroCounter = 0;zeroCounter < sizeof(Counter);zeroCounter++){
				*counterPointer = 0;
				counterPointer++;
			}

			KeyUserDebugs.flaggableFlags = 0;
			counterPointer = (unsigned char*) &Flaggables;
			for(zeroCounter = 0;zeroCounter < sizeof(Flaggable);zeroCounter++){
				*counterPointer = 0;
				counterPointer++;
			}

			KeyUserDebugs.flaggableFlags2 = 0;
			counterPointer = (unsigned char*) &Flaggables2;
			for(zeroCounter = 0;zeroCounter < sizeof(Flaggable2);zeroCounter++){
				*counterPointer = 0;
				counterPointer++;
			}
			break;
		}
		case REQUEST_DECODER_NAME:
		case REQUEST_FIRMWARE_BUILD_DATE:
		case REQUEST_COMPILER_VERSION:
		case REQUEST_OPERATING_SYSTEM:
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			unsigned char* stringToSend = 0;
			switch (RXHeaderPayloadID) {
				case REQUEST_DECODER_NAME:
					stringToSend = (unsigned char*)decoderName;
					break;
				case REQUEST_FIRMWARE_BUILD_DATE:
					stringToSend = (unsigned char*)buildTimeAndDate;
					break;
				case REQUEST_COMPILER_VERSION:
					stringToSend = (unsigned char*)compilerVersion;
					break;
				case REQUEST_OPERATING_SYSTEM:
					stringToSend = (unsigned char*)operatingSystem;
					break;
			}
			/* This type must have a length field, set that up and load the body into place at the same time */
			*((unsigned short*)TXBufferCurrentPositionHandler) = stringCopy((TXBufferCurrentPositionHandler + 2), stringToSend);
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			// Update with length field and string length.
			TXBufferCurrentPositionHandler += 2 + *((unsigned short*)TXBufferCurrentPositionHandler);
			break;
		}
		case UPDATE_BLOCK_IN_RAM:
		{
			// Subtract six to allow for the locationID, size, offset
			if(RXCalculatedPayloadLength < 7){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// Extract the RAM location ID
			unsigned short locationID = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the offset to place the data at
			unsigned short offset = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the size of the data to be stored
			unsigned short size = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Look up the memory location details
			blockDetails details;
			lookupBlockDetails(locationID, &details);

			// Don't let anyone write to running variables unless we are running BenchTest firmware!
			if((details.flags & BLOCK_IS_READ_ONLY) && compare((unsigned char*)&decoderName, (unsigned char*)BENCH_TEST_NAME, sizeof(BENCH_TEST_NAME))){
				errorID = ATTEMPT_TO_WRITE_TO_READ_ONLY_BLOCK;
				break;
			}

			// Subtract six to allow for the locationID, size, offset
			if((RXCalculatedPayloadLength - 6) != size){
				errorID = PAYLOAD_NOT_EQUAL_TO_SPECIFIED_VALUE;
				break;
			}

			// If either of these is zero then this block is not in RAM!
			if((details.RAMPage == 0) || (details.RAMAddress == 0)){
				errorID = INVALID_MEMORY_ACTION_FOR_ID;
				break;
			}

			// Check that size and offset describe a region that is not out of bounds
			if((size == 0) || (offset > (details.size - 1)) || (size > (details.size - offset))){
				errorID = INVALID_SIZE_OFFSET_COMBINATION;
				break;
			}

			// Don't allow sub region manipulation where it does not make sense or is unsafe.
			if((size != details.size) && !(details.flags & BLOCK_IS_INDEXABLE)){
				errorID = UNCHECKED_TABLE_MANIPULATION_NOT_ALLOWED;
				break;
			}

			// Save page values for restore
			unsigned char oldRamPage = RPAGE;
			// Set the viewable RAM page
			RPAGE = details.RAMPage;

			/// TODO @todo factor this out into validation delegation function once the number of types increases somewhat
			//
			if((details.flags & BLOCK_IS_MAIN_TABLE) || (details.flags & BLOCK_IS_2DUS_TABLE)){
				void* bufferToCheck;

				// For sub regions, construct an image for verification
				if(size != details.size){
					// Copy data from destination location to buffer
					memcpy(leftOverBuffer, details.RAMAddress, details.size);

					// Copy data from rx buffer to buffer over writing old data
					memcpy(leftOverBuffer + offset, RXBufferCurrentPosition, size);

					bufferToCheck = leftOverBuffer;
				}else{
					bufferToCheck = RXBufferCurrentPosition;
				}

				// Verify all tables
				if(details.flags & BLOCK_IS_MAIN_TABLE){
					errorID = validateMainTable((mainTable*)bufferToCheck);
				}else if(details.flags & BLOCK_IS_2DUS_TABLE){
					errorID = validateTwoDTable((twoDTableUS*)bufferToCheck);
				}// TODO add other table types here

				// If the validation failed, report it
				if(errorID != 0){
					RPAGE = oldRamPage; // Restore the original RAM page, even when getting an error condition.
					break;
				}
			}

			// Copy from the RX buffer to the block of RAM
			memcpy((unsigned char*)(details.RAMAddress + offset), RXBufferCurrentPosition, size);

			// Check that the write was successful
			unsigned char index = compare(RXBufferCurrentPosition, (unsigned char*)(details.RAMAddress + offset), size);

			// Restore the original RAM and flash pages
			RPAGE = oldRamPage;

			if(index != 0){
				errorID = MEMORY_WRITE_ERROR;
			}
			break;
		}
		case UPDATE_BLOCK_IN_FLASH:
		{
			// Subtract six to allow for the locationID, size, offset
			if(RXCalculatedPayloadLength < 7){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// Extract the RAM location ID
			unsigned short locationID = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the offset to place the data at
			unsigned short offset = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the size of the data to be stored
			unsigned short size = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Look up the memory location details
			blockDetails details;
			lookupBlockDetails(locationID, &details);

			// Subtract six to allow for the locationID, size, offset
			if((RXCalculatedPayloadLength - 6) != size){
				errorID = PAYLOAD_NOT_EQUAL_TO_SPECIFIED_VALUE;
				break;
			}

			// If either of these is zero then this block is not in flash!
			if((details.FlashPage == 0) || (details.FlashAddress == 0)){
				errorID = INVALID_MEMORY_ACTION_FOR_ID;
				break;
			}

			// Check that size and offset describe a region that is not out of bounds
			if((size == 0) || (offset > (details.size - 1)) || (size > (details.size - offset))){
				errorID = INVALID_SIZE_OFFSET_COMBINATION;
				break;
			}

			// Don't allow sub region manipulation where it does not make sense or is unsafe.
			if((size != details.size) && !(details.flags & BLOCK_IS_INDEXABLE)){
				errorID = UNCHECKED_TABLE_MANIPULATION_NOT_ALLOWED;
				break;
			}

			/// TODO @todo factor this out into validation delegation function once the number of types increases somewhat
			//
			if((details.flags & BLOCK_IS_MAIN_TABLE) || (details.flags & BLOCK_IS_2DUS_TABLE)){
				void* bufferToCheck;

				// For sub regions, construct an image for verification
				if(size != details.size){
					/* Save page value for restore and set the visible page */
					unsigned char oldFlashPage = PPAGE;
					PPAGE = details.FlashPage;

					// Copy data from destination location to buffer
					memcpy(leftOverBuffer, details.FlashAddress, details.size);

					/* Restore the original flash page */
					PPAGE = oldFlashPage;

					// Copy data from rx buffer to buffer over writing old data
					memcpy(leftOverBuffer + offset, RXBufferCurrentPosition, size);

					bufferToCheck = leftOverBuffer;
				}else{
					bufferToCheck = RXBufferCurrentPosition;
				}

				// Verify all tables
				if(details.flags & BLOCK_IS_MAIN_TABLE){
					errorID = validateMainTable((mainTable*)bufferToCheck);
				}else if(details.flags & BLOCK_IS_2DUS_TABLE){
					errorID = validateTwoDTable((twoDTableUS*)bufferToCheck);
				}// TODO add other table types here

				// If the validation failed, report it
				if(errorID != 0){
					break;
				}
			}

			/* Copy the flash details and populate the RAM details with the buffer location */
			blockDetails burnDetails;
			burnDetails.FlashPage = details.FlashPage;
			burnDetails.FlashAddress = details.FlashAddress + offset;
			burnDetails.RAMPage = RPAGE;
			burnDetails.RAMAddress = RXBufferCurrentPosition;
			burnDetails.size = size;

			/* Copy from the RX buffer to the block of flash */
			errorID = writeBlock(&burnDetails, leftOverBuffer);
			if(errorID != 0){
				break;
			}

			/* If present in RAM, update that too */
			if((details.RAMPage != 0) && (details.RAMAddress != 0)){
				/* Save page values for restore */
				unsigned char oldRamPage = RPAGE;
				/* Set the viewable RAM page */
				RPAGE = details.RAMPage;

				/* Copy from the RX buffer to the block of RAM */
				memcpy((unsigned char*)(details.RAMAddress + offset), RXBufferCurrentPosition, size);

				/* Check that the write was successful */
				unsigned char index = compare(RXBufferCurrentPosition, (unsigned char*)(details.RAMAddress + offset), size);

				/* Restore the original RAM and flash pages */
				RPAGE = oldRamPage;

				if(index != 0){
					errorID = MEMORY_WRITE_ERROR;
				}
			}

			break;
		}
		case RETRIEVE_BLOCK_FROM_RAM:
		{
			if(RXCalculatedPayloadLength != 6){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// Extract the RAM location ID
			unsigned short locationID = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the offset to place the data at
			unsigned short offset = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the size of the data to be stored
			unsigned short size = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			/* Look up the memory location details */
			blockDetails details;
			lookupBlockDetails(locationID, &details);

			if((details.RAMPage == 0) || (details.RAMAddress == 0)){
				errorID = INVALID_MEMORY_ACTION_FOR_ID;
				break;
			}

			// Special behaviour for size of zero which returns the whole block
			if((size == 0) && (offset == 0)){
				size = details.size;
			}

			// Check that size and offset describe a region that is not out of bounds
			if((size == 0) || (offset > (details.size - 1)) || (size > (details.size - offset))){
				errorID = INVALID_SIZE_OFFSET_COMBINATION;
				break;
			}

			// Don't allow sub region retrieval where it does not make sense or is unsafe. (keep it symmetric for djandruczyk)
			if((size != details.size) && !(details.flags & BLOCK_IS_INDEXABLE)){
				errorID = DOES_NOT_MAKE_SENSE_TO_RETRIEVE_PARTIALLY;
				break;
			}

			// This type must have a length field, set that up
			*((unsigned short*)TXBufferCurrentPositionHandler) = size;
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;

			/* Save page value for restore and set the visible page */
			unsigned char oldRamPage = RPAGE;
			RPAGE = details.RAMPage;

			/* Copy the block of RAM to the TX buffer */
			memcpy(TXBufferCurrentPositionHandler, (unsigned char*)(details.RAMAddress + offset), size);
			TXBufferCurrentPositionHandler += size;

			/* Restore the original RAM and flash pages */
			RPAGE = oldRamPage;

			break;
		}
		case RETRIEVE_BLOCK_FROM_FLASH:
		{
			if(RXCalculatedPayloadLength != 6){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// Extract the RAM location ID
			unsigned short locationID = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the offset to place the data at
			unsigned short offset = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the size of the data to be stored
			unsigned short size = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			/* Look up the memory location details */
			blockDetails details;
			lookupBlockDetails(locationID, &details);

			if((details.FlashPage == 0) || (details.FlashAddress == 0)){
				errorID = INVALID_MEMORY_ACTION_FOR_ID;
				break;
			}

			// Special behaviour for size of zero which returns the whole block
			if((size == 0) && (offset == 0)){
				size = details.size;
			}

			// Check that size and offset describe a region that is not out of bounds
			if((size == 0) || (offset > (details.size - 1)) || (size > (details.size - offset))){
				errorID = INVALID_SIZE_OFFSET_COMBINATION;
				break;
			}

			// Don't allow sub region retrieval where it does not make sense or is unsafe. (keep it symmetric for djandruczyk)
			if((size != details.size) && !(details.flags & BLOCK_IS_INDEXABLE)){
				errorID = DOES_NOT_MAKE_SENSE_TO_RETRIEVE_PARTIALLY;
				break;
			}

			// This type must have a length field, set that up
			*((unsigned short*)TXBufferCurrentPositionHandler) = size;
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;

			/* Save page value for restore and set the visible page */
			unsigned char oldFlashPage = PPAGE;
			PPAGE = details.FlashPage;

			/* Copy the block of flash to the TX buffer */
			memcpy(TXBufferCurrentPositionHandler, (unsigned char*)(details.FlashAddress + offset), size);
			TXBufferCurrentPositionHandler += size;

			/* Restore the original RAM and flash pages */
			PPAGE = oldFlashPage;

			break;
		}
		case BURN_BLOCK_FROM_RAM_TO_FLASH:
		{
			if(RXCalculatedPayloadLength != 6){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// Extract the RAM location ID
			unsigned short locationID = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the offset to place the data at
			unsigned short offset = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// Extract the size of the data to be stored
			unsigned short size = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			/* Look up the memory location details */
			blockDetails details;
			lookupBlockDetails(locationID, &details);

			/* Check that all data we need is present */
			if((details.RAMPage == 0) || (details.RAMAddress == 0) || (details.FlashPage == 0) || (details.FlashAddress == 0)){
				errorID = INVALID_MEMORY_ACTION_FOR_ID;
				break;
			}

			// Special behaviour for size of zero which burns the whole block
			if((size == 0) && (offset == 0)){
				size = details.size;
			}

			// Check that size and offset describe a region that is not out of bounds
			if((size == 0) || (offset > (details.size - 1)) || (size > (details.size - offset))){
				errorID = INVALID_SIZE_OFFSET_COMBINATION;
				break;
			}

			// Don't allow sub region retrieval where it does not make sense or is unsafe. (keep it symmetric for djandruczyk)
			if((size != details.size) && !(details.flags & BLOCK_IS_INDEXABLE)){
				errorID = DOES_NOT_MAKE_SENSE_TO_RETRIEVE_PARTIALLY;
				break;
			}


			// adjust details block to feed to represent the subsection of ram and flash that we want to burn down.
			details.RAMAddress += offset;
			details.FlashAddress += offset;
			details.size = size;

			/* Write the block down from RAM to Flash */
			errorID = writeBlock(&details, leftOverBuffer);
			break;
		}
		case REQUEST_DATALOG_PACKET: // Set type through standard configuration methods
		{
			if(RXCalculatedPayloadLength != 0){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			/* Set the length field up */
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			unsigned short* localLength = (unsigned short*)TXBufferCurrentPositionHandler;
			TXBufferCurrentPositionHandler += 2;

			/* Fill out the log and send */
			*localLength = populateBasicDatalog();
			break;
		}
		case SET_ASYNC_DATALOG_TYPE:
		{
			if(RXCalculatedPayloadLength != 1){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			unsigned char newDatalogType = *((unsigned char*)RXBufferCurrentPosition);
			if(newDatalogType > ASYNCDATALOGLASTTYPE){
				errorID = NO_SUCH_ASYNC_DATALOG_TYPE;
				break;
			}

			TablesB.SmallTablesB.loggingSettings.datalogStreamType = newDatalogType;
			break;
		}
		case RETRIEVE_ARBITRARY_MEMORY:
		{
			if(RXCalculatedPayloadLength != 6){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			unsigned short length = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;
			// Make sure the buffer can handle the block
			if(length > TX_MAX_PAYLOAD_SIZE){
				errorID = REQUESTED_LENGTH_TOO_LARGE;
				break;
			}

			void* address = (void*) *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;
			// Ensure we don't try to read past the end of the address space
			if(((unsigned short)address) <= ((0xFFFF - length) + 1)){
				// TODO Possibly check and limit ranges
				errorID = REQUESTED_ADDRESS_DISALLOWED;
				break;
			}

			unsigned char RAMPage = *((unsigned char*)RXBufferCurrentPosition);
			RXBufferCurrentPosition++;
			// Ensure RAM page is valid. Being too high is not possible.
			if(RAMPage < RPAGE_MIN){
				errorID = REQUESTED_RAM_PAGE_INVALID;
				break;
			}

			unsigned char FlashPage = *((unsigned char*)RXBufferCurrentPosition);
			RXBufferCurrentPosition++;
			// Ensure Flash page is valid. Being too high is not possible.
			if(FlashPage < PPAGE_MIN){
				errorID = REQUESTED_FLASH_PAGE_INVALID;
				break;
			}

			/* This type must have a length field, set that up */
			*((unsigned short*)TXBufferCurrentPositionHandler) = length + 6;
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;

			/* Put the request payload into the reply */
			*((unsigned short*)TXBufferCurrentPositionHandler) = (unsigned short) address;
			TXBufferCurrentPositionHandler += 2;
			*((unsigned short*)TXBufferCurrentPositionHandler) = length;
			TXBufferCurrentPositionHandler += 2;
			*((unsigned char*)TXBufferCurrentPositionHandler) = RAMPage;
			TXBufferCurrentPositionHandler++;
			*((unsigned char*)TXBufferCurrentPositionHandler) = FlashPage;
			TXBufferCurrentPositionHandler++;

			/* Load the body into place */
			memcpy((void*)TXBufferCurrentPositionHandler, address, length);
			TXBufferCurrentPositionHandler += length;

			break;
		}
		case RETRIEVE_LIST_OF_LOCATION_IDS:
		{
			if(RXCalculatedPayloadLength != 3){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// Extract the type of list that we want
			unsigned char listType = *((unsigned char*)RXBufferCurrentPosition);
			RXBufferCurrentPosition++;

			// Extract the mask for the qualities that we want
			unsigned short blockDetailsMask = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// This type must have a length field, set that up
			unsigned short * listLength = (unsigned short*)TXBufferCurrentPositionHandler;
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;

			// Zero the counter before we start, woops!
			*listLength = 0;

			unsigned long locationID;
			blockDetails details;
			for(locationID = 0;locationID < 65536;locationID++){
				unsigned short locationIDDoesntExist;
				locationIDDoesntExist = lookupBlockDetails((unsigned short)locationID, &details);

				if(!locationIDDoesntExist){
					if((listType == 0x00) || // get all
							((listType == 0x01) && (details.flags & blockDetailsMask)) || // get OR of bits
							((listType == 0x02) && (!(~(details.flags) & blockDetailsMask)))){ // get AND of bits
						*((unsigned short*)TXBufferCurrentPositionHandler) = (unsigned short)locationID;
						TXBufferCurrentPositionHandler += 2;
						*listLength += 2;
					}
				}
			}

			break;
		}
		case RETRIEVE_LOCATION_ID_DETAILS:
		{
			if(RXCalculatedPayloadLength != 2){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// Extract the RAM location ID
			unsigned short locationID = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			// This type must have a length field, set that up
			*((unsigned short*)TXBufferCurrentPositionHandler) = sizeof(blockDetails);
			*TXHeaderFlags |= HEADER_HAS_LENGTH;
			TXBufferCurrentPositionHandler += 2;

			// Write straight to output buffer to save time/code
			errorID = lookupBlockDetails(locationID, (blockDetails*)TXBufferCurrentPositionHandler);

			if(errorID != 0){
				break;
			}

			// Adjust TX buffer position if successful
			TXBufferCurrentPositionHandler += sizeof(blockDetails);

			break;
		}
		case REQUEST_UNIT_TEST_OVER_SERIAL:
		{
			/*
			 * The idea here is to call this function with arguments, and data
			 * and have the result sent back for comparison with an expected
			 * result that isn't divulged to the firmware.
			 *
			 * It is intended that all testable functions be callable through
			 * this mechanism and that any number of test executions can be
			 * performed by an external suite using different parameters and
			 * data sets and matching expected results.
			 *
			 * The usual error mechanism shall be used to indicate some sort of
			 * either internal or test failure and returned errors shall be
			 * suitably descriptive to allow diagnosis and fixing of issues.
			 */

			// Must at least have test ID
			if(RXCalculatedPayloadLength < 2){
				errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
				break;
			}

			// grab unit test ID from payload
			unsigned short unitTestID = *((unsigned short*)RXBufferCurrentPosition);
			RXBufferCurrentPosition += 2;

			switch(unitTestID){
				case TESTEMPTYTEST:
				{
					// Must be only the ID
					if(RXCalculatedPayloadLength != 2){
						errorID = PAYLOAD_SHORTER_THAN_REQUIRED_FOR_TEST;
						break;
					}

					*((unsigned short*)TXBufferCurrentPositionHandler) = unitTestID;
					TXBufferCurrentPositionHandler +=2;

					break;
				}
				case TESTTWODTABLEUSLOOKUP:
				{
					// ID + Value + Table
					if(RXCalculatedPayloadLength != (2 + 2 + sizeof(twoDTableUS))){
						errorID = PAYLOAD_SHORTER_THAN_REQUIRED_FOR_TEST;
						break;
					}

					unsigned short Value = *((unsigned short*)RXBufferCurrentPosition);
					RXBufferCurrentPosition += 2;

					twoDTableUS* Table = ((twoDTableUS*)RXBufferCurrentPosition);
					RXBufferCurrentPosition += sizeof(twoDTableUS);

					unsigned short result = lookupTwoDTableUS(Table, Value);

					*((unsigned short*)TXBufferCurrentPositionHandler) = result;
					TXBufferCurrentPositionHandler +=2;

					break;
				}
				// http://issues.freeems.org/view.php?id=156
				//
				/// TODO @todo test all things listed below:
				// lookupPagedMainTableCellValue - pass this RPAGE so that it remains unchanged
				// validateMainTable
				// validateTwoDTable
				// set table values - leave this till last, currently unused by mtx, likely to be removed anyway
				// generateDerivedVars - convert to pointers, remove headers, privatise a lot of data!
				// calculateFuelAndIgnition - ditto
				// scheduling algorithm - ditto
				// safeAdd
				// safeTrim
				// safeScale
				// sleep (milliseconds)
				// sleepMicro (microseconds)
				// checksum
				// stringCopy
				// compare
				// utils that can't be checked: sampleLoopADC sampleBlockADC sampleEachADC - can check for how long each takes! adjustPWM (test only anyway), resetToNonRunningState and setupPagedRAM (would interfere with functioning of device)
				// init code may be able to be partially checked
				// most other code at this stage is ISR code, flash writing code, or could interfere with the running of the engine
				// more testable code will appear with time, such as the HAL layer, and most accessory functions.
				default:
				{
					errorID = NO_SUCH_UNIT_TEST_ID;
				}

			// each case:
				// checks length, fails if wrong
				// parses data into args
				// calls function on data/args
				// assembles response OR sets error
				// breaks
			}
			break;
		}
		case START_BENCH_TEST_SEQUENCE:
		{
			// see TODO on include at top and modify this line appropriately
			if(!(compare((unsigned char*)&decoderName, (unsigned char*)BENCH_TEST_NAME, sizeof(BENCH_TEST_NAME)))){
				if(RXCalculatedPayloadLength < 1){
					errorID = PAYLOAD_LENGTH_TYPE_MISMATCH;
					break;
				}

				unsigned char localTestMode = *((unsigned char*)RXBufferCurrentPosition); //1; // The only mode, for now.
				RXBufferCurrentPosition++;
				if(localTestMode > TEST_MODE_BUMP_UP_CYCLES){
					errorID = UNIMPLEMENTED_TEST_MODE;
					break;
				}else if((localTestMode == TEST_MODE_STOP) && (RXCalculatedPayloadLength == 1)){
					if(!(coreStatusA & BENCH_TEST_ON)){
						errorID = BENCH_TEST_NOT_RUNNING_TO_STOP;
						break;
					}

					// Ensure we succeed at stopping it as quickly as possible.
					ATOMIC_START();
					KeyUserDebugs.currentEvent = testEventsPerCycle - 1; // Gets incremented then compared with testEventsPerCycle
					testNumberOfCycles = 1;                              // Gets decremented then compared with zero
					ATOMIC_END();

					// eventually save and return where it got to
					break;
				}else if((localTestMode == TEST_MODE_BUMP_UP_CYCLES) && (RXCalculatedPayloadLength == 2)){
					if(!(coreStatusA & BENCH_TEST_ON)){
						errorID = BENCH_TEST_NOT_RUNNING_TO_BUMP;
						break;
					}

					// Get bump value from payload
					unsigned char bumpCycles = *((unsigned char*)RXBufferCurrentPosition); //1; // The only mode, for now.
					RXBufferCurrentPosition++;

					if(bumpCycles == 0){
						errorID = BUMPING_BY_ZERO_MAKES_NO_SENSE;
						break;
					}

					// Bump count by value from payload
					testNumberOfCycles += bumpCycles;
					// Given that this function is only for situations when A it's getting near to
					// zero and B the user is watching, not checking for overflow is reasonable.
					break;
				}else if((localTestMode == TEST_MODE_ITERATIONS) && (RXCalculatedPayloadLength == 24)){
					testMode = localTestMode;
					// do nothing to fall through, or move other code into here
				}else{
					errorID = PACKET_SIZE_WRONG_FOR_TEST_MODE;
					break;
				}

				if(coreStatusA & BENCH_TEST_ON){
					errorID = BENCH_TEST_ALREADY_RUNNING;
					break;
				}

				testEventsPerCycle = *((unsigned char*)RXBufferCurrentPosition); //100;  // @ 10ms  =  1s
				RXBufferCurrentPosition++;
				if(testEventsPerCycle == 0){
					errorID = INVALID_EVENTS_PER_CYCLE;
					break;
				}

				testNumberOfCycles = *((unsigned short*)RXBufferCurrentPosition); //20;   // @ 1s    = 20s
				RXBufferCurrentPosition += 2;
				if(testNumberOfCycles == 0){
					errorID = INVALID_NUMBER_OF_CYCLES;
					break;
				}

				testTicksPerEvent = *((unsigned short*)RXBufferCurrentPosition); //12500; // @ 0.8us = 10ms
				RXBufferCurrentPosition += 2;
				if(testTicksPerEvent < decoderMaxCodeTime){
					errorID = TOO_SHORT_OF_AN_EVENT_PERIOD;
					break;
				}

				// Pluck the arrays out of the packet for the loop below
				unsigned char* testEventNumbers = RXBufferCurrentPosition;
				RXBufferCurrentPosition += 6;
				unsigned short* testPulseWidths = (unsigned short*)RXBufferCurrentPosition;
				RXBufferCurrentPosition += 12;

				// Reset the clock for reading timeout
				Clocks.timeoutADCreadingClock = 0; // make this optional, such that we can use real inputs to determine pw and/or dwell.

				// Validate and transfer the per-channel data
				unsigned char channel;
				unsigned char configuredChannels = 6;
				for(channel = 0;channel < 6;channel++){
					if(testPulseWidths[channel] > injectorSwitchOnCodeTime){ // See next block for warning.
						// use as-is
						outputEventDelayFinalPeriod[channel] = decoderMaxCodeTime;
						outputEventPulseWidthsMath[channel] = testPulseWidths[channel];
						outputEventInputEventNumbers[channel] = testEventNumbers[channel];
					}else if(testPulseWidths[channel] > 3){
						// less than the code time, and not special, error!
						errorID = TOO_SHORT_OF_A_PULSE_WIDTH_TO_TEST;
						// Warning, PWs close to this could be slightly longer than requested, that will change in later revisions.
						break;
					}else if(testPulseWidths[channel] == 3){
						testMode++; // Dirty hack to avoid dealing with Dave for the time being.
						testNumberOfMissing = channel;
					}else if(testPulseWidths[channel] == 2){
						// use the dwell from the core maths and input vars.
						outputEventDelayFinalPeriod[channel] = decoderMaxCodeTime;
						outputEventPulseWidthsMath[channel] = DerivedVars->Dwell;
						outputEventInputEventNumbers[channel] = testEventNumbers[channel];
					}else if(testPulseWidths[channel] == 1){
						// use the reference pulse width from the core maths and input vars.
						outputEventDelayFinalPeriod[channel] = decoderMaxCodeTime;
						outputEventPulseWidthsMath[channel] = DerivedVars->RefPW;
						outputEventInputEventNumbers[channel] = testEventNumbers[channel];
					}else{ // is zero
						// Set this channel to zero for and therefore off, don't set this channel.
						outputEventInputEventNumbers[channel] = 0xFF; // Off.
						configuredChannels--;
					}
				}

				if(configuredChannels == 0){
					errorID = NO_CHANNELS_CONFIGURED_TO_TEST;
					break;
				}

				if(errorID == 0){
					// Let the first iteration roll it over to zero.
					KeyUserDebugs.currentEvent = 0xFF; // Needs to be here in case of multiple runs, init is not sufficient

					if(testMode == TEST_MODE_DODGY_MISSING_TOOTH){
						if(testEventsPerCycle <= 127){
							testEventsPerCycle *= 2;
						}else{
							errorID = TOO_MANY_EVENTS_PER_CYCLE_MISSING_TTH;
							break;
						}

						// Store the time per event in RPM such that it can be updated dynamically
						CoreVars->RPM = testTicksPerEvent;

						// The channels to use rely on the defaults from initialisers! Custom builds can break BenchTest mode!

						// Un-schedule anything that got scheduled
						outputEventInputEventNumbers[2] = 0xFF;
						outputEventInputEventNumbers[3] = 0xFF;
						outputEventInputEventNumbers[4] = 0xFF;
						outputEventInputEventNumbers[5] = 0xFF;
					}else if(testMode > TEST_MODE_DODGY_MISSING_TOOTH){
						errorID = UNIMPLEMENTED_TEST_MODE;
						break;
					}

					// Trigger decoder interrupt to fire thus starting the loop!
					TIE = 0x01; // The ISR does the rest!

					// Nothing went wrong, now set flag.
					coreStatusA |= BENCH_TEST_ON;
				}else{
					break;
				}


/* http://issues.freeems.org/view.php?id=155
 *
 * The following block has been left in, as I still do not know why it won't work as intended:
 *
 * - It should fire all 6 output pins with a 52ms duration pulse, exactly once.
 * - The SAME code run from anywhere else (pre main loop, in main loop, in rtc, in decoder) works fine, just not here in commsCore.c
 * - The interrupts run, but the pin doesn't change state, despite the registers being configured correctly
 *
 * I've tried quite a bit:
 *
 * - Moving this code around
 * - Checking memory definitions
 * - Completely rewriting the output ISR
 * - Adding significant debug to output ISR
 * - Checking for register contents in output ISR
 * - Checking for key things modified in this file
 * - General head scratching and confused searching
 */

//				outputEventPinNumbers[0] = 0; // 1 ign
//				outputEventPinNumbers[1] = 1; // 2 ign
//				outputEventPinNumbers[2] = 2; // 3 ign/1 fuel
//				outputEventPinNumbers[3] = 3; // 4 ign/2 fuel
//				outputEventPinNumbers[4] = 4; // 3 fuel
//				outputEventPinNumbers[5] = 5; // 4 fuel
//				outputEventDelayFinalPeriod[0] = decoderMaxCodeTime;
//				outputEventDelayFinalPeriod[1] = decoderMaxCodeTime;
//				outputEventDelayFinalPeriod[2] = decoderMaxCodeTime;
//				outputEventDelayFinalPeriod[3] = decoderMaxCodeTime;
//				outputEventDelayFinalPeriod[4] = decoderMaxCodeTime;
//				outputEventDelayFinalPeriod[5] = decoderMaxCodeTime;
//				outputEventPulseWidthsMath[0] = SHORTMAX;
//				outputEventPulseWidthsMath[1] = SHORTMAX;
//				outputEventPulseWidthsMath[2] = SHORTMAX;
//				outputEventPulseWidthsMath[3] = SHORTMAX;
//				outputEventPulseWidthsMath[4] = SHORTMAX;
//				outputEventPulseWidthsMath[5] = SHORTMAX;
//
//				unsigned short edgeTimeStamp = TCNT;
//				// call sched output with args
//				LongTime timeStamp;
//				/* Install the low word */
//				timeStamp.timeShorts[1] = edgeTimeStamp;
//				/* Find out what our timer value means and put it in the high word */
//				if(TFLGOF && !(edgeTimeStamp & 0x8000)){ /* see 10.3.5 paragraph 4 of 68hc11 ref manual for details */
//					timeStamp.timeShorts[0] = timerExtensionClock + 1;
//				}else{
//					timeStamp.timeShorts[0] = timerExtensionClock;
//				}
//
//				schedulePortTPin(0, timeStamp);
//				schedulePortTPin(1, timeStamp);
//				schedulePortTPin(2, timeStamp);
//				schedulePortTPin(3, timeStamp);
//				schedulePortTPin(4, timeStamp);
//				schedulePortTPin(5, timeStamp);
//
//				sleep(1000);
			}else{
				errorID = THIS_IS_NOT_THE_BENCH_TEST_DECODER;
			}
			break;
		}
		default:
		{
			if((RXHeaderPayloadID % 2) == 1){
				errorID = INVALID_PAYLOAD_ID;
			}else{
				errorID = UNRECOGNISED_PAYLOAD_ID;
			}
			break;
		}
	}

	// Always reply, if errorID is zero it's just an ack.
	finaliseAndSend(errorID);

	/* Switch reception back on now that we are done with the received data */
	resetReceiveState(CLEAR_ALL_SOURCE_ID_FLAGS);
}

void sendDescriptor() {

	unsigned char savedPPage = PPAGE;
	unsigned char* currentTXBufferPosition = TXBufferCurrentPositionHandler + 3; //FIXME create macro for the 2 +1
	unsigned char* lastTXBufferPosition;
	unsigned char numChunks = TablesB.SmallTablesB.loggingSettings.numberOfChunks;
	const dataBlockDescriptor* descriptorPTR;
	unsigned char full = 0;

	switch (format) {
	case DESCRIPTOR_JSON:
	case DESCRIPTOR_YAML:
		PPAGE = 0xE5;
		if ((currentChunk == 0) && (currentDescription == 0)) {
			/* Add JSON header */
			if (format == DESCRIPTOR_JSON)
				currentTXBufferPosition = addJSONHeader(currentTXBufferPosition);
			if (format == DESCRIPTOR_YAML)
				currentTXBufferPosition = addYAMLHeader(currentTXBufferPosition);
			packetPayloadEnum = 0;
		}
		lastTXBufferPosition = currentTXBufferPosition;
		//TODO fix first log chunk index as it is not guaranteed to be zero
		/* Pick up where we left off */
		while (currentChunk < numChunks) {
			//TODO if current descriptor = 0 maybe add another sub ID/name
			descriptorPTR =	&(TablesB.SmallTablesB.loggingSettings.logChunks[currentChunk].descriptor[currentDescription]);
			while (currentDescription < *(TablesB.SmallTablesB.loggingSettings.logChunks[currentChunk].numDescriptions)) {
				if (format == DESCRIPTOR_JSON)
					currentTXBufferPosition = addJSONRecord(currentTXBufferPosition, descriptorPTR, baseOffset);
				else if (format == DESCRIPTOR_YAML)
					currentTXBufferPosition = addYAMLRecord(currentTXBufferPosition, descriptorPTR, baseOffset);

				if (currentTXBufferPosition) {
					++currentDescription;
					++descriptorPTR;
					lastTXBufferPosition = currentTXBufferPosition;
				} else {
					full = 1;
					break;
				}
			}
			if (full) {
				break;
			} else {
				currentDescription = 0;
			}
			baseOffset += TablesB.SmallTablesB.loggingSettings.logChunks[currentChunk].size;
			++currentChunk;
		}
		/* If we aren't full yet, try to append footer, if we can't 
		 * set full flag so we catch it on the next iteration around
		 */
		if (!full)
		{
			if (format == DESCRIPTOR_JSON)
				currentTXBufferPosition = addJSONFooter(currentTXBufferPosition);
			if (format == DESCRIPTOR_YAML)
				currentTXBufferPosition = addYAMLFooter(currentTXBufferPosition);
			if (!currentTXBufferPosition)
				full = 1;
		}
		/* once everything is sent reset our indexes */
		if (!full) {
			currentChunk = 0;
			currentDescription = 0;
			baseOffset = 0;
			commsCoreStateFlags &= ~(PROCESSING_MULTI_PACKET_PAYLOAD);
			RXHeaderPayloadIDInProcess = 0;
			*TXHeaderFlags |= HEADER_IS_COMPLETE;
		} else {
			*TXHeaderFlags |= HEADER_IS_PARTIAL;
			commsCoreStateFlags |= PROCESSING_MULTI_PACKET_PAYLOAD;
			RXHeaderPayloadIDInProcess = RXHeaderPayloadID;
		}
		/* write length into packet */
		*((unsigned short*) TXBufferCurrentPositionHandler) = lastTXBufferPosition - TXBufferCurrentPositionHandler;
		/* write payload number into packet */
		*(TXBufferCurrentPositionHandler + 2) = packetPayloadEnum;
		/* fast forward buffer to end */
		TXBufferCurrentPositionHandler = lastTXBufferPosition;
		/* This type must have a length field, set that up and load the body into place at the same time */
		*TXHeaderFlags |= HEADER_HAS_LENGTH;
		++packetPayloadEnum;
		break;
	default:
		//invalid format requested
		break;
	}
	PPAGE = savedPPage;
}

/* Here we prepare for sending a TX packet. This is more or less just temp code before
 * we can rewrite the whole lot. This does enable us to send multi-packet payloads, but
 * not in a generic way just yet.  */

void prepForTX(){
	/* Rewind all pointers to start of buffer */
	TXBufferCurrentPositionHandler = (unsigned char*)&TXBuffer;
	TXBufferCurrentPositionSCI0 = (unsigned char*)&TXBuffer;
	TXBufferCurrentPositionCAN0 = (unsigned char*)&TXBuffer;

	/* Flag that we are transmitting! */
	TXBufferInUseFlags |= COM_SET_SCI0_INTERFACE_ID;
	// SCI0 only for now...

	/* Load a blank header into the TX buffer ready for masking */
	TXHeaderFlags = TXBufferCurrentPositionHandler;
	*TXHeaderFlags = 0;
	TXBufferCurrentPositionHandler++;
	*((unsigned short*)TXBufferCurrentPositionHandler) = RXHeaderPayloadID + 1;
	TXBufferCurrentPositionHandler += 2;
}

/* This function should be period limited to about 10 seconds internally (or by scheduler) */
//void checkCountersAndSendErrors(){
	// compare time stamps  with current time stamps and execute if old enough. (if no scheduler)

	// compare counters with counters cache (from last time) sending an error packet when they differ

	// copy counters to counters cache for next time

	// send errors with busy wait on the basis that all errors should be taken care of and not be sent in fairly short order?

	// or send with isr but just busy wait for it to finish before sending the next?

	// debug messages, busy wait or isr or both, perhaps busy wait till able to send, lock sending (need semaphore for this as well as sending one?) and initiate send, then carry on? investigate timeframes for sends of smallish 100byte packets.

	// need to figure out how to queue received packets for processing when we are currently sending stuff out.

	// above notes don't belong here really.
//}

