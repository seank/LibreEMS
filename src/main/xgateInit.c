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

void initXgate(){
	/* route interrupt to xgate, vector address = channel_id * 2 */
	ROUTE_INTERRUPT(0x39, XGATE_INTERRUPT, PRIORITY_LEVEL_ONE) /*enable xgate int on software0 interrupt */
	ROUTE_INTERRUPT(0x3C, XGATE_INTERRUPT, PRIORITY_LEVEL_ONE) /*enable xgate int on PIT1 */
	ROUTE_INTERRUPT(0x3D, XGATE_INTERRUPT, PRIORITY_LEVEL_TWO) /*enable xgate int on PIT0 */

	/* XGATE sees flash starting at paged address 0xE0+0x8800 to + 30Kb*/
	unsigned char savedRPAGE = RPAGE;
	unsigned char savedPPAGE = PPAGE;

	RPAGE = RPAGE_TUNE_TWO;
	PPAGE = 0xE1;
	// XGATE threads execute from RAM
	//TODO set RAM protection
	// we can't use the symbols for the memcpy part because the symbols need to contain xgate relevant values
	memcpy(START_OF_RAM_WINDOW, START_OF_FLASH_WINDOW, XGATE_RAM_ALLOCATION_SIZE);
	/* TODO again test symbol offset linker feature that I added over a year ago so the code below can be used
	 * Also if those symbols can be used we can then map xg-channels to physical pins outside asm code vs having
	 * them hardcoded in the xgate sources
	 */
	//memcpy(startXGATECode, START_OF_FLASH_WINDOW, XGATE_RAM_ALLOCATION_SIZE);
	RPAGE = savedRPAGE;
	PPAGE = savedPPAGE;
	// Set the XGVBR register to its start address in flash (page 0xE0 after 2K register space)
	XGVBR = (unsigned short)0x0800; // EO region is divided to ensure vectors end up here visible to xgate
	// Enable XGate and XGate interrupts
	XGMCTL= (unsigned short)0x8181;

	/* A prescalar of 4 yeilds .1ms resolution and max times of 6.5535ms@16-bit, 429,483.622ms@32-bit */
	PITMTLD0 = 0x1F; /* 32 prescaler (1 / ((40 MHz) / PRESCALAR)) */
	PITMTLD1 = 0x1F; /* 32 prescaler  (1 / ((40 MHz) / PRESCALAR)) */

	PITMUX = 0xC0; /* set chan 0-1 to use PITMTLD0 base and chan 2-3 to use PITMTLD1 */
	PITLD0 = 0x0000; // set timers running //TEST ONLY
	PITLD1 = 0xFFFF; /* This is our metronome so this number is static */

	PITCFLMT = 0x80; // enable module
	PITCE = 0x02; 	// enable count down on our metronome
	PITINTE = 0x03; // enable interrupts on 0, 1
	PITFLT = ONES;	// clear flags

	/* Initialize our global xgate pointers */
	XGOutputEvents = (XGOutputEvent*) (parametersBase - RPAGE_TUNE_TWO_WINDOW_DIFFERENCE);
	xgsInStamp = (unsigned short*) (xGSInputEdgeStamp - RPAGE_TUNE_TWO_WINDOW_DIFFERENCE);
	xgsEventsToSch = (unsigned short*) (xgsNumOfEventsToSchedule - RPAGE_TUNE_TWO_WINDOW_DIFFERENCE);
	/* once we are in shared RAM these can look something like this */
	//XGOutputEvents = (XGOutputEvent*) parametersBase;
	//xgsInStamp = (unsigned short*) xGSInputEdgeStamp;
	//xgsEventsToSch = (unsigned short*) xgsNumOfEventsToSchedule;
}
