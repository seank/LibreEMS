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
 * @ingroup enginePositionRPMDecoders
 *
 * @brief XGATE decoder output scheduling
 *
 * @author Sean Keys
 */


#define OFFSET	0x1EUL // number of ticks to adjust by, this is just for testing

/* temp *safety semaphore, to keep me from chasing my tail, just in case */
unsigned char i = 0;
do {
	XGSEM = 0x0101;
	if (i) {
		__asm__("wai");  /* put the processor to sleep, so we know something bad could have happened if we continue, TODO see if there is a flag for global shutdown */
	}
	i++;
} while (!(XGSEM & 0x01));
XGSEM = 0x0100;

unsigned char savedRPage = RPAGE;
RPAGE = RPAGE_TUNE_TWO;
*xgsInStamp = timeStamp.timeShorts[1]; // this should be the value of TC0 at the time the decoder ISR started, Fred feel free to correct me.
*xgsEventsToSch = 1;
XGOutputEvents[0].channelID = outputEventNumber;
//ATOMIC_START();
if(outputEventDelayTotalPeriod[outputEventNumber] > OFFSET){
	outputEventDelayTotalPeriod[outputEventNumber] -= OFFSET;
}
XGOutputEvents[0].delay = outputEventDelayTotalPeriod[outputEventNumber] - OFFSET;
XGOutputEvents[0].runtime = outputEventPulseWidthsMath[outputEventNumber];
//ATOMIC_END();
XGSCHEDULE();
RPAGE = savedRPage;


