/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2011-2013 Sean Keys
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
 * along with any LibreEMS software.  If not, see http://www.gnu.org/licenses/
 *
 * Thank you for choosing LibreEMS to run your engine!
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


#include "inc/utils.h"
#include "inc/interrupts.h"

/* debug flags */
#define XGATE_SCHEDULER_OUTRUN  0x04

/* debug macros */
#define SET_DEBUG_FLAG(VARIABLE, BIT)		(KeyUserDebugs.VARIABLE |= BIT)
#define UNSET_DEBUG_FLAG(VARIABLE, BIT)		(KeyUserDebugs.VARIABLE &= ~(BIT))

unsigned char xgateOutputChannel = fixedConfigs1.schedulingSettings.xgateOutputChannel[outputEventNumber];

if(xgateOutputChannel != 0xFF) {
	extern KeyUserDebug KeyUserDebugs;
	/* temp *safety semaphore, to keep me from chasing my tail while testing. */
	unsigned char i = 0;
	do {
		XGSEM = 0x0101;
		if (i) {
			SET_DEBUG_FLAG(zsp8, XGATE_SCHEDULER_OUTRUN);
		}
		i++;
	}while (!(XGSEM & 0x01));
	XGSEM = 0x0100;
	unsigned char savedRPage = RPAGE;
	RPAGE = RPAGE_TUNE_TWO;
    *xgsInStamp = timeStamp.timeShorts[1]; // This should be the value of TC0 at the time the decoder ISR started.

    if(outputEventPulseWidthsMathSecondary[outputEventNumber] && fixedConfigs1.schedulingSettings.schedulingConfigurationBits[outputEventNumber]) {
		*xgsEventsToSch = 2;
		XGOutputEvents[1].channelID = xgateOutputChannel + fixedConfigs1.engineSettings.cylinderCount;
		XGOutputEvents[1].delay = outputEventDelayTotalPeriod[outputEventNumber];
		XGOutputEvents[1].runtime = outputEventPulseWidthsMathSecondary[outputEventNumber];

	} else {
		*xgsEventsToSch = 1;
	}
	XGOutputEvents[0].channelID = xgateOutputChannel;
	XGOutputEvents[0].delay = outputEventDelayTotalPeriod[outputEventNumber];
	XGOutputEvents[0].runtime = outputEventPulseWidthsMath[outputEventNumber];
	XGSCHEDULE();
	RPAGE = savedRPage;
}
