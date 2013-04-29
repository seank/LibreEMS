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
 * @ingroup testing
 * @ingroup xgateFiles
 *
 * @brief XGate Timing and Bit Bang Tests
 *
 * @author Sean Keys
 */


#define XGATETESTS_C
EXTERN void sleepMicro(unsigned short) FPAGE_FE;
EXTERN void generateCoreVars(void) LOOKUPF;

/* gcc is one of the few compilers that supports nested functions in C ! */

//static unsigned char waitCycles = 44;
static unsigned short testCH = 0;
static unsigned long delay = 2500;
static unsigned short runtime = 5000; //0x0FFF; //0x0FFE - 40;
static unsigned long delay2 = 10000;
static unsigned long sleep = 10;
static unsigned short numRuns = 0;
static unsigned char once = 1;
static unsigned char debug = 0;

/**
 * This test will schedule the first eight channel in sequential fashion. Incrementing after each channel has been
 * scheduled both the delay and runtime
 */
void incrementalTests() {
	if (Clocks.millisToTenths % 1 == 0) {
		numRuns++;
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 1;
		if (testCH == 0) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime + numRuns;
		} else if (testCH == 1) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 2) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime - numRuns;
		} else if (testCH == 3) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 4) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime + (numRuns*2);
		} else if (testCH == 5) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 6) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime - (numRuns*2);
		} else if (testCH == 7) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = runtime;
		}
		XGSCHEDULE();
		testCH++;
		sleep++;
		if (testCH > 7) {
			testCH = 0;
		}
		RPAGE = savedRPage;
	}
	return;
}

void wiringTest() {
	if (Clocks.millisToTenths % 100 == 0) {
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 1;
		XGOutputEvents[0].channelID = testCH;
		XGOutputEvents[0].delay = delay;
		XGOutputEvents[0].runtime = 0x2FFF;
		XGSCHEDULE();
		RPAGE = savedRPage;
		numRuns++;
		if (numRuns == 10) {
			++testCH;
			if (testCH > 11) {
				testCH = 0;
			}
			numRuns = 0;
		}
	}

	return;
}


/**
 * This test will allow you to measure the latency difference between your first and last event
 */
void firstLastDiffTest() {
	if (Clocks.millisToTenths % 100 == 0) {
		PORTB = 0xFF;
		//sleep(1);
		PORTB = 0;
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 2;
		XGOutputEvents[0].channelID = 0;
		XGOutputEvents[0].delay = 2500;
		XGOutputEvents[0].runtime = 2500;
		XGOutputEvents[1].channelID = 35;
		XGOutputEvents[1].delay = 2500;
		XGOutputEvents[1].runtime = 2500;
		XGSCHEDULE();
		RPAGE = savedRPage;
	}
	return;
}

/**
 * A test.
 */
void singleTest() {
	if (Clocks.millisToTenths % 100 == 0) {
		PORTB = 0xFF;
		PORTB = 0xFF;
		PORTB = 0xFF;
		PORTB = 0xFF;
		PORTB = 0xFF;
		PORTB = 0xFF;

		//sleep(1);
		PORTB = 0;
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 2;
		XGOutputEvents[0].channelID = testCH;
		XGOutputEvents[0].delay = 2500;
		XGOutputEvents[0].runtime = 2500;
		XGOutputEvents[1].channelID = testCH + 1;
		XGOutputEvents[1].delay = 5000;
		XGOutputEvents[1].runtime = 5000;
		XGSCHEDULE();
		RPAGE = savedRPage;
	}
	testCH++;
	if (testCH > 2) {
		testCH = 0;
	}
	return;
}

/**
 * This test will allow you to observe overlaps be varying the delays
 */
void delayShiftDiffTest() {
	if (Clocks.millisToTenths % 2 == 0) {
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 1;
		if (testCH == 0) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		} else if (testCH == 1) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay2;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		} else if (testCH == 2) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay + 1;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		} else if (testCH == 3) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay2 - 1;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		} else if (testCH == 4) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay + 2;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		} else if (testCH == 5) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay2 - 2;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		} else if (testCH == 6) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay + 3;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		} else if (testCH == 7) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay2 - 3;
			XGOutputEvents[0].runtime = DELAYTESTPW;
		}
		XGSCHEDULE();
		testCH++;
		delay++;
		delay2--;
		sleep++;
		if (delay > 10000) {
			delay = 200;
		}
		if (delay2 > 10000) {
			delay2 = 10000;
		}

		if (testCH > 7) {
			testCH = 0;
		}
		RPAGE = savedRPage;
	}
}

/**
 * This test will allow you to observe how log it takes to schedule 1-6 events
 */
void scheduleLatencyTest() {
	if (Clocks.millisToTenths % 100 == 0) {
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		PORTB = 0xFF;
		PORTB = 0xFF;
		PORTB = 0xFF;
		PORTB = 0xFF;
		PORTB = 0;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 6;
		delay = 2500;
		runtime = 2500;
		XGOutputEvents[0].channelID = testCH;
		XGOutputEvents[0].delay = delay;
		XGOutputEvents[0].runtime = runtime;
		XGOutputEvents[1].channelID = testCH + 1;
		XGOutputEvents[1].delay = delay;
		XGOutputEvents[1].runtime = runtime;
		XGOutputEvents[2].channelID = testCH + 2;
		XGOutputEvents[2].delay = delay;
		XGOutputEvents[2].runtime = runtime;
		XGOutputEvents[3].channelID = testCH + 3;
		XGOutputEvents[3].delay = delay;
		XGOutputEvents[3].runtime = runtime;
		XGOutputEvents[4].channelID = testCH + 4;
		XGOutputEvents[4].delay = delay;
		XGOutputEvents[4].runtime = runtime;
		XGOutputEvents[5].channelID = testCH + 5;
		XGOutputEvents[5].delay = delay;
		XGOutputEvents[5].runtime = runtime;
		XGSCHEDULE();
		RPAGE = savedRPage;
	}
}

/**
 * This test will allow you to observe how log it takes to schedule 1-6 events
 */
//void competingInterruptsTest() {
//	if (Clocks.millisToTenths % 10 == 0) {
//		unsigned int i = 0;
//
//		do {
//			XGSEM = 0x0101;
//			if (i) {
//				PORTA = 0xFF;
//			}
//			i++;
//		} while (!(XGSEM & 0x01));
//		XGSEM = 0x0100;
//
//		PORTB |= 0x80;
//		unsigned char savedRPage = RPAGE;
//		RPAGE = RPAGE_TUNE_TWO;
//		*xgsInStamp = TC0;
//		*xgsEventsToSch = 6;
//		//*(unsigned short*)(eventFlags1 - 0x8000) = (unsigned short)0;
//		//*(unsigned short*)(eventFlags1 - 0x8000) = (unsigned short)0;
//		delay++;
//		//runtime++;
//		XGOutputEvents[0].channelID = testCH;
//		XGOutputEvents[0].delay = delay;
//		XGOutputEvents[0].runtime = runtime;
//		XGOutputEvents[1].channelID = testCH + 1;
//		XGOutputEvents[1].delay = delay + 200;
//		XGOutputEvents[1].runtime = runtime;
//		XGOutputEvents[2].channelID = testCH + 2;
//		XGOutputEvents[2].delay = delay + 3 + numRuns;
//		XGOutputEvents[2].runtime = runtime;
//		XGOutputEvents[3].channelID = testCH + 3;
//		XGOutputEvents[3].delay = delay + numRuns;
//		XGOutputEvents[3].runtime = runtime + 4;
//		XGOutputEvents[4].channelID = testCH + 4;
//		XGOutputEvents[4].delay = 0;
//		XGOutputEvents[4].runtime = 2500;
//		XGOutputEvents[5].channelID = testCH + 35;
//		XGOutputEvents[5].delay = delay + 5 + numRuns;
//		XGOutputEvents[5].runtime = runtime;
//		XGSCHEDULE();
//		RPAGE = savedRPage;
//		PORTB &= 0x7F;
//	}
//	if (runtime > 10500) {
//		runtime = 250;
//	}
//	numRuns++;
//}

/**
 *
 */
void metronomeOverlapTests() {
	if (once) {
		delay = 45000;
		--once;
	}

	if (Clocks.millisToTenths % 10 == 0) {
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 1;
		if (Clocks.millisToTenths % 100 == 0) {

			if(numRuns % 10 == 0) {
				PORTB |= (1 << 1);
				sleepMicro(20);
				PORTB &= ~(1 << 1);
				XGOutputEvents[0].channelID = 1;
				XGOutputEvents[0].delay = runtime;
				XGOutputEvents[0].runtime = runtime;
				XGSCHEDULE()
			} else {

			if(testCH > 1) {
			PORTB |= (1 << testCH);
			sleepMicro(20);
			PORTB &= ~(1 << testCH);
			}
			if (testCH == 2) {
				XGOutputEvents[0].channelID = testCH;
				XGOutputEvents[0].delay = delay;
				XGOutputEvents[0].runtime = runtime - numRuns;
			} else if (testCH == 3) {
				XGOutputEvents[0].channelID = testCH;
				XGOutputEvents[0].delay = delay;
				XGOutputEvents[0].runtime = runtime;
			} else if (testCH == 4) {
				XGOutputEvents[0].channelID = testCH;
				XGOutputEvents[0].delay = delay;
				XGOutputEvents[0].runtime = runtime + (numRuns * 2);
			} else if (testCH == 5) {
				XGOutputEvents[0].channelID = testCH;
				XGOutputEvents[0].delay = delay;
				XGOutputEvents[0].runtime = runtime;
			} else if (testCH == 6) {
				XGOutputEvents[0].channelID = testCH;
				XGOutputEvents[0].delay = delay;
				XGOutputEvents[0].runtime = runtime - (numRuns * 2);
			} else if (testCH == 7) {
				XGOutputEvents[0].channelID = testCH;
				XGOutputEvents[0].delay = delay;
				XGOutputEvents[0].runtime = runtime;
			}
			XGSCHEDULE()
			numRuns++;
			delay++;
			testCH++;
			}
		}else {
			XGOutputEvents[0].channelID = 0;
			XGOutputEvents[0].delay = runtime;
			XGOutputEvents[0].runtime = runtime;
			XGSCHEDULE()
		}

		if (testCH > 7) {
			testCH = 0;
		}
		RPAGE = savedRPage;
	}

	return;
}

void metronomeTests() {
	if (Clocks.millisToTenths % 100 == 0) {
		++numRuns;
		if(numRuns % 10 == 0) {
		unsigned char savedRPage = RPAGE;
		RPAGE = RPAGE_TUNE_TWO;
		*xgsInStamp = TC0;
		*xgsEventsToSch = 1;
		PORTB |= (1 << testCH);
		sleepMicro(20);
		PORTB &= ~(1 << testCH);
		if (testCH == 0) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = 0xFFFAUL + debug++;
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 1) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = 0xFFFFUL + (0xFFFF / 2);
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 2) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = 0xFFFFUL + (0xFFFF / 3);
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 3) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay * (numRuns / 10);
			XGOutputEvents[0].runtime = runtime * (numRuns / 10);
		} else if (testCH == 4) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay * (numRuns / 10);
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 5) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay * (numRuns / 10);
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 6) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay * (numRuns / 10);
			XGOutputEvents[0].runtime = runtime;
		} else if (testCH == 7) {
			XGOutputEvents[0].channelID = testCH;
			XGOutputEvents[0].delay = delay * (numRuns / 10);
			XGOutputEvents[0].runtime = runtime;
		}
		XGSCHEDULE();
		testCH++;
		sleep++;
		if (testCH > 7) {
			testCH = 0;
		}
		RPAGE = savedRPage;
	}
	}
	return;
}

//void schdulerTests() {
//	if (Clocks.millisToTenths % 100 == 0) {
//		++numRuns;
//		if(numRuns % 10 == 0) {
//ATOMIC_START();
//		unsigned long potentialDelay = 0x0FFF;
//	//	outputEventInputEventNumbers[numRuns] = mappedEvent;
//		outputEventDelayFinalPeriod[testCH] = (unsigned short)potentialDelay;
//		outputEventPulseWidthsMath[testCH] = 0x0FFF;
//		outputEventExtendNumberOfRepeats[testCH] = 0;
//		outputEventDelayTotalPeriod[testCH] = potentialDelay; // No async accesses occur
//		unsigned short edgeTimeStamp = TC0;    /* Save the edge time stamp */
//		LongTime timeStamp;
//		/* Install the low word */
//		timeStamp.timeShorts[1] = edgeTimeStamp;
//		/* Find out what our timer value means and put it in the high word */
////		if(TFLGOF && !(edgeTimeStamp & 0x8000)){ /* see 10.3.5 paragraph 4 of 68hc11 ref manual for details */
////			timeStamp.timeShorts[0] = timerExtensionClock + 1;
////		}else{
////			timeStamp.timeShorts[0] = timerExtensionClock;
////		}
//		//unsigned long thisEventTimeStamp = timeStamp.timeLong;
//		schedulePortTPin(testCH, timeStamp);
//ATOMIC_END();
//	testCH++;
//	if(testCH == fixedConfigs1.schedulingSettings.numberOfConfiguredOutputEvents) testCH = 0;
//	}
//}
//}

void stagedInjectionTests() {
#define TIME_DIVISOR	50UL //6000 rpm
	if (Clocks.millisToTenths % TIME_DIVISOR == 0) {
		ATOMIC_START();
		*ticksPerDegree = (unsigned short)((ticks_per_degree_multiplier * (1250UL * TIME_DIVISOR) ) / (720UL * ANGLE_FACTOR));
		//CoreVars->DRPM = #define ticks_per_degree_multiplier (10 * ANGLE_FACTOR) // FIX <<< shouldn't be done like this.
		generateCoreVars();
		//CoreVars->RPM = RPM(6000); //these need to match TIME_DIVISOR or your scope results wont
		unsigned char outputEventNumber = 0;
		outputEventDelayTotalPeriod[outputEventNumber] = 200;
		outputEventPulseWidthsMath[outputEventNumber] = runtime + DerivedVars->IDT;
		unsigned short edgeTimeStamp = TC0;
		LongTime timeStamp;
		/* Install the low word */
		timeStamp.timeShorts[1] = edgeTimeStamp;
		/* Find out what our timer value means and put it in the high word */
		if (TFLGOF && !(edgeTimeStamp & 0x8000)) { /* see 10.3.5 paragraph 4 of 68hc11 ref manual for details */
			timeStamp.timeShorts[0] = timerExtensionClock + 1;
		} else {
			timeStamp.timeShorts[0] = timerExtensionClock;
		}
		schedulePortTPin(outputEventNumber, timeStamp);
		/* roll back and fwd */
		if(once){
			runtime += 30;
		}else {
			runtime -= 30;
		}
		if(runtime > 34000) {
			once = 0;
		}
		if(runtime < 5000){
			once = 1;
		}
		ATOMIC_END();
	}
}
wiringTest();
//incrementalTests();
//metronomeTests();
//metronomeOverlapTests();
//schdulerTests();
//stagedInjectionTests();
