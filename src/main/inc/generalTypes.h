/* FreeEMS - the open source engine management system
 *
 * Copyright 2008-2011 Fred Cooke
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
 * @ingroup dataStructures
 *
 * @brief General struct typedefs
 *
 * This file contains all of the general purpose and smaller struct typedefs.
 */


/* Header file multiple inclusion protection courtesy eclipse Header Template */
/* and http://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/ C pre processor manual    */
#ifndef FILE_GENERAL_TYPES_H_SEEN
#define FILE_GENERAL_TYPES_H_SEEN


/* block_ prefixed defines have Masks for the flag field in the blockDetails struct. */

/** Memory block has parent */
#define BLOCK_HAS_PARENT           BIT0_16
#define BLOCK_IS_IN_RAM            BIT1_16
#define BLOCK_IS_IN_FLASH          BIT2_16
#define BLOCK_IS_INDEXABLE         BIT3_16
#define BLOCK_IS_READ_ONLY         BIT4_16
#define BLOCK_GETS_VERIFIED        BIT5_16
#define BLOCK_FOR_BACKUP_RESTORE   BIT6_16
#define BLOCK_SPARE_FLAG_7         BIT7_16
#define BLOCK_SPARE_FLAG_8         BIT8_16
#define BLOCK_SPARE_FLAG_9         BIT9_16
#define BLOCK_SPARE_FLAG_10        BIT10_16
#define BLOCK_SPARE_FLAG_11        BIT11_16
#define BLOCK_IS_2DUS_TABLE        BIT12_16
#define BLOCK_IS_MAIN_TABLE        BIT13_16
#define BLOCK_IS_LOOKUP_DATA       BIT14_16
#define BLOCK_IS_CONFIGURATION     BIT15_16

/** Addressable memory block description.
 *
 * This structure holds all details required to manipulate a particular block.
 *
 * It is used for passing around the details of a memory block to allow serial
 * operations on RAM and flash data structures in a safe and protected way.
 *
 * block_ prefixed defines are masks for the flag field in the this struct.
 */
typedef struct {
	unsigned short flags;    ///< Property flags, see flag masks above
	unsigned short parent;   ///< Location ID of parent block if sub block
	unsigned char RAMPage;   ///< RAM page number
	unsigned char FlashPage; ///< Flash page number
	void* RAMAddress;        ///< RAM address
	void* FlashAddress;      ///< Flash address
	unsigned short size;     ///< Memory block size
} blockDetails;


#else
	/* let us know if we are being untidy with headers */
	#warning "Header file GENERAL_TYPES_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
