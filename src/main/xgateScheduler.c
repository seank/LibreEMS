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


#include "inc/utils.h"
#include "inc/interrupts.h"

/* debug flags */
#define PRIMARY_DC_EXCEEDED		0x01
#define SECONDARY_DC_EXCEEDED	0x02
#define XGATE_SCHEDULER_OUTRUN 0x04

/* debug macros */
#define SET_DEBUG_FLAG(VARIABLE, BIT)		(KeyUserDebugs.VARIABLE |= BIT)
#define UNSET_DEBUG_FLAG(VARIABLE, BIT)		(KeyUserDebugs.VARIABLE &= ~(BIT))

/* testing values */
#define OFFSET	0x1EUL

/* temp *safety semaphore, to keep me from chasing my tail while testing, just in case */
unsigned char i = 0;
do {
	XGSEM = 0x0101;
	if (i) {
		SET_DEBUG_FLAG(zsp8, XGATE_SCHEDULER_OUTRUN);
	}
	i++;
} while (!(XGSEM & 0x01));
XGSEM = 0x0100;
UNSET_DEBUG_FLAG(zsp8, XGATE_SCHEDULER_OUTRUN);

unsigned char savedRPage = RPAGE;
RPAGE = RPAGE_TUNE_TWO;

/* The following #if block contains code that performs staged injection based on duty-cycle parameters that you set below.
 * This code has been bench-tested, run on a street-bike engine but not throughly run though the ringer. There are *limitations
 * to ranges and percentages. As always, test with YOUR parameters BEFORE ever running any engine with it. -SK */
#if (CONFIG == SEANKR1_ID) || (CONFIG == SEANKLT1_ID)

/* staged config settings -
 * max duty cycle takes a bit of thought in some situations(high RPM) IE don't forget you need *enough dead-time to cover
 * injector dead-time or you may experience flutter. */
#define STAGED_INJECTION_THRESHOLD	8500UL /* duty cycle in 0.01 percent to start staging injection */
#define SECONDARY_MAX_DUTY_CYCLE	8500UL /* max duty cycle allowed on the second set before cutting all fuel */
#define STAGED_RPM_THRESHOLD		5000  /* keep this reasonable since variables may over flow at really slow speeds */
#define SECONDARY_INJECTOR_FLOW		CC_PER_MINUTE(190) /* the flow rate of your secondary injectors */
#define PRIMARY_INJECTOR_FLOW		CC_PER_MINUTE(230) /* tackiness! this is a duplicate of fixedConfigs1.engineSettings.injectorFlow */
#define SECONDARY_INJECTOR_OFFSET	4 /* eg. you have injector 1 & 5 on the same cylinder ditto for 2 & 6 */
/* factors */
#define FLOW_SCALE_FACTOR			10000UL /* .01 % */
#define DEGRESS_PERCYCLE_FACTOR		72UL	/* 720/10 */
#define FLOW_DIFFERENCE				(((PRIMARY_INJECTOR_FLOW * FLOW_SCALE_FACTOR) / (SECONDARY_INJECTOR_FLOW) \
										* PRIMARY_INJECTOR_FLOW) / FLOW_SCALE_FACTOR) /* difference of flow in % */
#define INVERSE_TICK_VALUE			125UL /* inverse of .8 x 10 */

unsigned short calculatedPW = outputEventPulseWidthsMath[outputEventNumber];
//unsigned short dutyCycle = (CoreVars->RPM * (calculatedPW * 10UL) / INVERSE_TICK_VALUE) / 2400UL; //in percent for engines running in sequential mode
unsigned short dutyCycle = ((CoreVars->RPM / 2) * (calculatedPW * 5UL) / INVERSE_TICK_VALUE) / 600UL; //in percent for engines running in sequential mode
unsigned long  tickPerEngineCycle = (CoreVars->DRPM * DEGRESS_PERCYCLE_FACTOR);
unsigned short secondaryDutyCycle;
unsigned short secondaryPulsewidthToUseForThisChannel;
unsigned short newPrimaryPulsewidth;
*xgsInStamp = timeStamp.timeShorts[1]; // This should be the value of TC0 at the time the decoder ISR started.

/* is this event for fuel or ign ? */
if(fixedConfigs1.schedulingSettings.schedulingConfigurationBits[outputEventNumber]) {
	KeyUserDebugs.zsp7 = dutyCycle;
	if(dutyCycle > STAGED_INJECTION_THRESHOLD && CONFIG == SEANKR1_ID && CoreVars->RPM > STAGED_RPM_THRESHOLD) {
		KeyUserDebugs.zsp8 |= PRIMARY_DC_EXCEEDED;
		/* this formula pushes any *extra PW onto the second corresponding injector */
		calculatedPW -= DerivedVars->IDT; /* remove any section of the PW that is not part of the fuel calc */
		newPrimaryPulsewidth = (unsigned short)((tickPerEngineCycle * STAGED_INJECTION_THRESHOLD) / FLOW_SCALE_FACTOR);
		secondaryPulsewidthToUseForThisChannel = calculatedPW - newPrimaryPulsewidth;
		secondaryPulsewidthToUseForThisChannel += DerivedVars->IDT; //Move *lost fuel to second channel WARNING THIS CURRENTLY ASSUMES BOTH INJECTORS have the same dead-time TODO FIX
		/* factor in any injector size difference */
		secondaryPulsewidthToUseForThisChannel = (unsigned short)((secondaryPulsewidthToUseForThisChannel * FLOW_DIFFERENCE) / FLOW_SCALE_FACTOR);
		secondaryPulsewidthToUseForThisChannel += DerivedVars->IDT; //ideally we need a second lookup, incase the characteristics are different from our first set of injectors
		/* wet the intake tract(crude!) */
		//	if(numOfEventsSinceStartOfStaging < 20) {
		//		secondaryPulsewidthToUseForThisChannel += 0x1500;
		//		if(outputEventNumber == (fixedConfigs1.schedulingSettings.numberOfConfiguredOutputEvents - 1)) {
		//			numOfEventsSinceStartOfStaging++;
		//		}
		//		KeyUserDebugs.zsp8 = numOfEventsSinceStartOfStaging;
		//	}
		/* schedule two events for XGS to schedule */
		*xgsEventsToSch = 2;
		XGOutputEvents[0].channelID = outputEventNumber;
		XGOutputEvents[0].delay = outputEventDelayTotalPeriod[outputEventNumber];
		XGOutputEvents[0].runtime = newPrimaryPulsewidth;
		/* set second injection PW */
		XGOutputEvents[1].channelID = outputEventNumber + SECONDARY_INJECTOR_OFFSET;
		XGOutputEvents[1].delay = outputEventDelayTotalPeriod[outputEventNumber];
		XGOutputEvents[1].runtime = secondaryPulsewidthToUseForThisChannel;
		secondaryDutyCycle = ((CoreVars->RPM / 2) * (calculatedPW * 5UL) / INVERSE_TICK_VALUE) / 600UL; //in percent for engines running in sequential mode
		//secondaryDutyCycle = (CoreVars->RPM * (secondaryPulsewidthToUseForThisChannel * 10UL) / INVERSE_TICK_VALUE) / 2400UL; //in percent for engines running in sequential mode
		KeyUserDebugs.zsp6 = secondaryDutyCycle;
		/* if we cant deliver the required fuel, don't deliver anything */
		if(secondaryDutyCycle < SECONDARY_MAX_DUTY_CYCLE) {
			XGSCHEDULE();
			KeyUserDebugs.zsp8 &= ~(SECONDARY_DC_EXCEEDED);
		} else {
			KeyUserDebugs.zsp8 |= SECONDARY_DC_EXCEEDED;
		}
	} else {
		/* single injector event */
		numOfEventsSinceStartOfStaging = 0;
		*xgsEventsToSch = 1;
		XGOutputEvents[0].channelID = outputEventNumber;
		XGOutputEvents[0].delay = outputEventDelayTotalPeriod[outputEventNumber];
		XGOutputEvents[0].runtime = calculatedPW;
		XGSCHEDULE();
		KeyUserDebugs.zsp8 &= ~(SECONDARY_DC_EXCEEDED | PRIMARY_DC_EXCEEDED);
	}
} else {
	/* Just schedule as normal */
	*xgsEventsToSch = 1;
	XGOutputEvents[0].channelID = outputEventNumber;
	XGOutputEvents[0].delay = outputEventDelayTotalPeriod[outputEventNumber];
	XGOutputEvents[0].runtime = outputEventPulseWidthsMath[outputEventNumber];
	XGSCHEDULE();
}

#else
/* Business as usual */
*xgsInStamp = timeStamp.timeShorts[1]; // This should be the value of TC0 at the time the decoder ISR started.
*xgsEventsToSch = 1;
XGOutputEvents[0].channelID = outputEventNumber;
XGOutputEvents[0].delay = outputEventDelayTotalPeriod[outputEventNumber];
XGOutputEvents[0].runtime = outputEventPulseWidthsMath[outputEventNumber];
XGSCHEDULE();
#endif

RPAGE = savedRPage;

