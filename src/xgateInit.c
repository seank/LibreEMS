// Temporary file to make Sean's life easy!


/* FreeEMS - the open source engine management system
 *
 * Copyright 2011 Sean Keys
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
 * @ingroup xgateFiles
 *
 * @brief XGATE init routine
 *
 * @author Sean Keys
 */


/** @brief Xgate module setup
 *
 * Configure XGATE setup registers and prepare XGATE code to be run by copying
 * it from flash hcs12mem can write to to flash XGATE can read from.
 *
 * @author Sean Keys
 *
 * @note A thanks goes out to Edward Karpicz for helping me get xgate configured
 * properly.
 *
 * @warning If executing from RAM you must copy the code from Flash to RAM before
 * starting XGATE
 *
 */
void initXgate(){
	/* route interrupt to xgate, vector address = channel_id * 2 */
	ROUTE_INTERRUPT(0x39, XGATE_INTERRUPT, PRIORITY_LEVEL_ONE) /*enable xgate int on software0 interrupt */
	//ROUTE_INTERRUPT(0x3A, XGATE_INTERRUPT, PRIORITY_LEVEL_ONE) /*enable xgate int on PIT3 */
	//ROUTE_INTERRUPT(0x3B, XGATE_INTERRUPT, PRIORITY_LEVEL_ONE) /*enable xgate int on PIT2 */
	ROUTE_INTERRUPT(0x3C, XGATE_INTERRUPT, PRIORITY_LEVEL_ONE) /*enable xgate int on PIT1 */
	ROUTE_INTERRUPT(0x3D, XGATE_INTERRUPT, PRIORITY_LEVEL_TWO) /*enable xgate int on PIT0 */

	/* XGATE sees flash starting at paged address 0xE0+0x8800 to + 30Kb*/
	unsigned char savedRPAGE = RPAGE;
	unsigned char savedPPAGE = PPAGE;
	// XGATE threads execute from RAM
	//RPAGE = RPAGE_TUNE_TWO;
	PPAGE = 0xE1;
	// we can't use the symbols for the memcpy part because the symbols need to contain xgate relevant values
	//memcpy(START_OF_RAM_WINDOW, START_OF_FLASH_WINDOW, XGATE_RAM_ALLOCATION_SIZE);
	memcpy((unsigned short *)0x3030, START_OF_FLASH_WINDOW, XGATE_RAM_ALLOCATION_SIZE);
	//TODO set RAM protection
	//RPAGE = savedRPAGE;
	PPAGE = savedPPAGE;
	// Set the XGVBR register to its start address in flash (page 0xE0 after 2K register space)
	XGVBR = (unsigned short )0x0800; // EO region is divided to ensure vectors end up here visible to xgate

	// Enable XGate and XGate interrupts
	XGMCTL= (unsigned short)0x8181;

	/* Enable PIT TODO move back to proper section once unit tested */
//	PITMTLD0 = 0x1F; /* 32 prescaler gives 0.8uS resolution and max period of 52.4288ms measured */
	/* Measurements from logic analizer PITMTLD0
	 * 0x01 = 3.28ms@16-bits
	 */

	/* A prescalar of 4 yeilds .1ms resolution and max times of 6.5535ms@16-bit, 429,483.622ms@32-bit */
	/* TODO figure out why they behave odd if they carry different values */

	PITMTLD0 = 0x1F; /* 32 prescaler (1 / ((40 MHz) / PRESCALAR)) */
	PITMTLD1 = 0x1F; /* 32 prescaler  (1 / ((40 MHz) / PRESCALAR)) */

	//PITMTLD0 = 0x03; /* 4 prescaler (1 / ((40 MHz) / PRESCALAR)) */
	//PITMTLD1 = 0x03; /* 4 prescaler  (1 / ((40 MHz) / PRESCALAR)) */

	PITMUX = 0xC0; /* set chan 0-1 to use PITMTLD0 base and chan 2-3 to use PITMTLD1 */
	PITLD0 = 0x0000; // set timers running //TEST ONLY
	PITLD1 = 0xFFFF; // set timers running //TEST ONLY
	//PITLD2 = 0xFFFF; /* This is our metronome so this number is static */
	//PITLD3 = 0x01; // set timers running //TEST ONLY

	PITCFLMT = 0x80; // enable module
	PITCE = 0x02; // countdown enable on our metronome
	PITINTE = 0x03; // enable interrupt on 0, 1
	PITFLT = ONES; // clear flags

	/* Initialize our global xgate pointers */
	XGOutputEvents = (XGOutputEvent*) (parametersBase - RPAGE_TUNE_TWO_WINDOW_DIFFERENCE);
	xgsInStamp = (unsigned short*) (xGSInputEdgeStamp - RPAGE_TUNE_TWO_WINDOW_DIFFERENCE);
	xgsEventsToSch = (unsigned short*) (xgsNumOfEventsToSchedule - RPAGE_TUNE_TWO_WINDOW_DIFFERENCE);
}
