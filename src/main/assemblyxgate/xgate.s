/* FreeEMS - the open source engine management system
 *
 * Copyright 2010-2011 Sean Keys
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

/* @defgroup xgateThreads */

/**
 * @cond DOXYGEN_IGNORE_ASM
*/
; *****************************************************************************
;
; Thanks to Edward Karpicz for help with getting all the xgate registers sorted.
;
; *****************************************************************************
;debug code
.global eventFlags1
.global xgatePORTPFlip
.global xgatePORTBFlip

;	.sect .data
    .sect .ppageE1

include "assemblyxgate/xgate.inc"
startXGATECode:
/**
 * @endcond
*/
;xgateSchdule()

/**
 * @brief xgateSchedule()
 *
 * @brief
 * This subroutine loops though a shared array of parameters. The s12 core writes them and the xgate core reads them.
 * As it loops though each parameter set it applies the formulas below to get the final countdown for the bangOn
 * and bangOff times.
 * @note bangOn = XGOutputEvents[0].delay - (TCNT - inputEvent);
 * @note bangOff =  XGOutputEvents[0].runtime + bangOn;
 *
 * @brief Schedule logic works as follows:
 * @note If EVENT_FLAG_RUNNING  bit IS NOT set bangOn and bangOff times are programmed as calculated
 * @note If EVENT_FLAG_RUNNING  bit IS set bangOn is ignored and the runtime is *adjusted to the calculated bangOff value
 *
 * @param unsigned char *xgsEventsToSch; The number of event parameter sets you have pushed by the s12 core.
 * @param unsigned short *xgsInStamp; The decoder's inputEvent TCNT timestamp.
 * @param struct XGOutputEvents[]; An array of XGOutputEvents that hold the channelIDs,
 * delays and runtimes.
 *
 * @brief Example: Schedule 1 channel to generate a 2ms pulseWidth 8ms from the inputStartTime
 * @note *xgsInStamp = inputStartTime;
 * @note *xgsEventToSch = 1;
 * @note XGOutputEvents[0].channelID = 0;
 * @note XGOutputEvents[0].delay = 5000;
 * @note XGOutputEvents[0].runtime = 20000;
 *
 * @warning you must set *xgsInStamp or you may get a pulsewidth without a valid reference
 * @warning do not exceede MAX_NUMBER_OF_PARAMETERS or NUMBER_OF_EVENT_STRUCTURES
 * @todo TODO consider optimizations
 *
 * @author Sean Keys
 * @par
 * @cond DOXYGEN_IGNORE_ASM
*/

xgateSchedule:
LDD R2, xgateSchedule
	;lock semaphore
	SSEM #0
	;pull a pin high to mark the start of our thread
	;LDD R1, #1
	;LDD R2, #2
	;STB R2, R1, #0;
;debug code
;LDW R4, EVENT_0_REGISTER_MASK
;LDW R5, xgatePORTBMaskOn
;JAL R5
;end debug code
	LDD R3, PITCNT1 ;load current metronome value
	LDD R5, TC0 ;load current TC0 value to factor decoder latency
	LDW R3, R3, #ZERO_OFFSET
	LDW R5, R5, #ZERO_OFFSET
	LDD R4, xGSSystemVars
	STW R3, R4, #XGS_VARIABLE_METROSTARTTIMESTAMP_OFFSET ; save metronome stamp
	STW R5, R4, #XGS_VARIABLE_TCNTSTARTSTAMP_OFFSET; sk why does this need to be saved ?
	;default our countdown to highest possobile value
	LDD R5, #0xFFFF
	STW R5, R4, #XGS_VARIABLE_SOONESTCOUNTDOWN_OFFSET
	;TODO think/finish the conditional branch and the end of this ISR
	;clear the check PIT0 flag
	;LDW R5, R4, #XGS_VARIABLE_XGS_FLAGS_OFFSET
	;ANDL R5, #XGS_FLAG_CHECKPIT0_XOR
	;STW R5, R4, #XGS_VARIABLE_XGS_FLAGS_OFFSET
	;clear interrupt flag
	LDD R3, #SW_ISR_FLAG_REG
	LDD R2, #SW_ISR_ZERO_CFLAG_MASK
	STW R2, R3, #ZERO_OFFSET
	;initialize our parameter loop
	LDD R1, xGSSystemVars
	LDL R2, #0
	STW R2, R1, #XGS_VARIABLE_NUM_OF_EVENTS_SCHEDULED_OFFSET
	LDD R2, parametersBase
	STW R2, R1, #XGS_VARIABLE_PARAMETER_BASE_ADDRESS_OFFSET; #XGS_VARIABLE_EVENT_ADDRESS_OFFSET
BRA xgsParameterLoopCheck
	xgsParameterLoop:
;debug code
;LDD R5, xgatePORTPFlip
;JAL R5; jump to xgatePORTPFlip
 	; TODO check this new code
 	;Check to see if our PIT timer went off, if it did we need to service it NOW
;debug
BRA xgsPIT0Clear ; skip past the optimization that checks to see if your bang loop went off
 	LDD R3, #PITTF
	LDB R3, R3, #ZERO_OFFSET
	BITL R3, #0x01
BEQ xgsPIT0Clear
	;we need to push loop sensitive registers to tmp vars
	LDD R4, xGRegisters
	STW R2, R4, #XG_COREREGISTERTWO_OFFSET
	;set xGScheduler flag busy
	LDD R4, xGSSystemVars
	LDW R2, R4, #XGS_VARIABLE_XGS_FLAGS_OFFSET
	ORL R2, #XGS_FLAG_JUMPED_TO_PIT0_LOOP
	STW R2,  R4, #XGS_VARIABLE_XGS_FLAGS_OFFSET
	BRA xgatePITBangLoop
	xgsContinue:
	;clear jumped flag
	LDD R4, xGSSystemVars
	LDW R2, R4, #XGS_VARIABLE_XGS_FLAGS_OFFSET
	ANDL R2, #XGS_FLAG_JUMPED_TO_PIT0_LOOP_XOR
	STW R2,  R4, #XGS_VARIABLE_XGS_FLAGS_OFFSET
	;we need to pull loop sensitive registers
	LDD R4, xGRegisters
	LDW R2, R4, #XG_COREREGISTERTWO_OFFSET
    xgsPIT0Clear:
	;diff TCNT against inputEdgeStamp to get elapsed time
	;LDD R4, TCNT ;load current TCNT
	LDD R4, xGSScheduleTCNTStartStamp ;load saved TCNT stamp, sk this should be moved inside the schedule loop below and the live TCNT should be used for the diff
	LDW R4, R4, #ZERO_OFFSET
	LDD R5, xGSInputEdgeStamp ;load saved TCNT
	LDW R5, R5, #ZERO_OFFSET
	SUB R6, R4, R5 ;R6 now contains our elapsed time since the input edge
	;load our parameter pointer that points to the correct position in our parameters array
	LDD R3, xGSSystemVars
	LDW R3, R3, #XGS_VARIABLE_PARAMETER_BASE_ADDRESS_OFFSET
	;check to see if ChannelID is within range
	;NOTE when properly used we actually dont need this code, but there is a chance of mem corruption without it
	LDW R4, R3, #PARAMETER_CHANNEL_ID_OFFSET
 	SUBL R4, #NUMBER_OF_EVENT_STRUCTURES
BLS xgsChannelIDOk
	BRA xgsParameterLoopNext
 	xgsChannelIDOk:
;debug
;bra xgsInputEventStampFactorDone ; skip TCNT correct for debug testing
	;correct againt our decoder ISR TCNT stamp
	LDW R4, R3, #PARAMETER_DELAY_LOWWORD_OFFSET ;load
	LDW R7, R3, #PARAMETER_DELAY_HIGHWORD_OFFSET ;load
	SUB R1, R4, R6 ; subtract time elapsed since inputEdge from parameter low word
	SBC R5, R7, R0 ; subtract carry from high word if any
	SUB R2, R4, R1 ; is low word zero?
	SBC R2, R7, R5 ; is high word zero?
	;if carry has been set that means our countdown needs to be set to zero
BCS xgsNeedsToBeZero
	STW R1, R3, #PARAMETER_DELAY_LOWWORD_OFFSET
	STW R5, R3, #PARAMETER_DELAY_HIGHWORD_OFFSET
BRA xgsInputEventStampFactorDone
xgsNeedsToBeZero:
	STW R0, R3, #PARAMETER_DELAY_LOWWORD_OFFSET
	STW R0, R3, #PARAMETER_DELAY_HIGHWORD_OFFSET
xgsInputEventStampFactorDone:
	;take our delayTime and add our runTime to get "runtimeDelay"
	LDW R4, R3, #PARAMETER_RUNTIME_LOWWORD_OFFSET ;Low Word
	LDW R1, R3, #PARAMETER_DELAY_LOWWORD_OFFSET
	ADD R2, R4, R1 ;Add our delays to get runtime low word countdown
	STW R2, R3, #PARAMETER_RUNTIME_LOWWORD_OFFSET ;save low
	LDW R2, R3, #PARAMETER_DELAY_HIGHWORD_OFFSET ;High Word
	ADC R2, R0, R2 ;Add the carry to the high word if any
	STW R2, R3, #PARAMETER_RUNTIME_HIGHWORD_OFFSET;save final runtime
	;If our times are long enough to use the metronome we need to factor is current counter value for each countdown
	TST R2
BEQ xgsFixUpDone ;if our runtime doesnt have a highword value then our delay doesnt either
	;calculate and apply our metronome offset
	LDD R1, xGSScheduleMetroStartStamp
	LDW R1, R1, #ZERO_OFFSET
	COM R1 ;Diff our Metronome stamp against itself we need to know how close to "full" it was since there
				;is a very small 1 in 16 bits chance it is FFFF
	;fix our RunTime according to how much we got shorted from 0xFFFF
	LDW R4, R3, #PARAMETER_RUNTIME_LOWWORD_OFFSET ;Low Word
	ADD R2, R4, R1 ;Add diff to our parameter countdown(low)
	STW R2, R3, #PARAMETER_RUNTIME_LOWWORD_OFFSET ;save low
	LDW R2, R3, #PARAMETER_RUNTIME_HIGHWORD_OFFSET ;High Word
	ADC R2, R0, R2 ;Add the carry to the high word if any ;TODO a check for 16-range could be added here
	STW R2, R3, #PARAMETER_RUNTIME_HIGHWORD_OFFSET ;save high
	;fix our DelayTime, if needed
	LDW R4, R3, #PARAMETER_DELAY_HIGHWORD_OFFSET
	TST R4
BEQ xgsFixUpDone
		LDW R4, R3, #PARAMETER_DELAY_LOWWORD_OFFSET ;Low Word
		ADD R2, R4, R1 ;Add diff to our parameter countdown(low)
		STW R2, R3, #PARAMETER_DELAY_LOWWORD_OFFSET ;save low
		LDW R2, R3, #PARAMETER_DELAY_HIGHWORD_OFFSET ;High Word
		ADC R2, R0, R2 ;Add the carry to the high word if any ;TODO a check for 16-range could be added here
		STW R2, R3, #PARAMETER_DELAY_HIGHWORD_OFFSET ;save high
    xgsFixUpDone:
 ;calculate our pointer to our event of interest
	LDL R2, #0x00 ;initialize loop register
	LDL R3, #0x00 ;initialize our offset register
	LDD R4, xGSSystemVars
	LDW R4, R4, #XGS_VARIABLE_PARAMETER_BASE_ADDRESS_OFFSET
	LDW R4, R4, #PARAMETER_CHANNEL_ID_OFFSET
BRA XSLoop1
	XSLoop1Block:
		ADDL R2, #0x01 ;i++
		ADDL R3, #EVENT_STRUCTURE_SIZE;++
	XSLoop1:
		CMP R4, R2
BNE XSLoop1Block
	;we can now update and arm our event
	LDD R4, eventsStructStart
	ADD R4, R3, R4 ;add offset to base address R3 still contains offset at this point
	LDD R5, xGSSystemVars
	STW R4, R5, #XGS_VARIABLE_EVENT_ADDRESS_OFFSET
	LDW R5, R5, #XGS_VARIABLE_PARAMETER_BASE_ADDRESS_OFFSET
	;If the event has not happened yet go ahead and over write its delays
	;If the event is running extent only its runtime
	LDW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
BITL R6, #EVENT_FLAG_RUNNING ;if our event is already running just modify the runtime
BNE xgsAdjustRuntime
	;Incase the event was previously scheduled clear all flags
	LDW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
	ANDL R6, #EVENT_FLAG_AVAILABLE
	STW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
	;Write our delay and set our flags accordingly
	LDW R6, R5, #PARAMETER_DELAY_LOWWORD_OFFSET;Load low word of parameter
	STW R6, R4, #EVENT_DELAY_LOW_WORD_OFFSET
	LDW R6, R5, #PARAMETER_DELAY_HIGHWORD_OFFSET;Load high word of parameter
	STW R6, R4, #EVENT_DELAY_HIGH_WORD_OFFSET
 	TST R6
BNE xgsDelayNotApproachable ;if zero flag event as approachable
 	LDW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
 	ORL R6, #EVENT_FLAG_APPROACHABLE
 	STW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
    ;get/save start time stamp so we can diff later on
	LDD R6, xGSSystemVars
	LDW R1, R6, #XGS_VARIABLE_METROSTARTTIMESTAMP_OFFSET
	STW R1, R4, #EVENT_DELAY_LATENCY_METROSTAMP_OFFSET
    ;see if this events has the soonest countdown if it does set PIT0 check flag and store new countdown
	;disabled there seems to be a bug
	;LDW R1, R6, #XGS_VARIABLE_SOONESTCOUNTDOWN_OFFSET
	;LDW R2, R5, #PARAMETER_DELAY_LOWWORD_OFFSET
	;CMP R2, R1
;BHI xgsNotApproachable ;if currentEvent(R6) < nextEventTime(R4) update it!
;		STW R2, R6, #XGS_VARIABLE_SOONESTCOUNTDOWN_OFFSET ;save soonest time
		;LDW R2, R6, #XGS_VARIABLE_XGS_FLAGS_OFFSET
		;ORL R2, #XGS_FLAG_CHECKPIT0
		;STW R2, R6, #XGS_VARIABLE_XGS_FLAGS_OFFSET
 	xgsDelayNotApproachable:
 	;save delay parameter to proper event
	LDW R6, R5, #PARAMETER_RUNTIME_HIGHWORD_OFFSET;Load high word of parameter
	STW R6, R4, #EVENT_RUNTIME_HIGH_WORD_OFFSET
 	TST R6
BNE xgsNotApproachableShutdown ;if zero flag event as approachable
	LDW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
	ORL R6, #EVENT_FLAG_APPROACHABLE_SHUTDOWN
	STW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
    ;get/save start time stamp so we can diff later
	LDD R6, xGSScheduleMetroStartStamp
	LDW R6, R6, #ZERO_OFFSET
	STW R6, R4, #EVENT_RUNTIME_LATENCY_METROSTAMP_OFFSET
	xgsNotApproachableShutdown:
	BRA xgsSaveAllParameters

xgsAdjustRuntime:
;debug
;BRA xgsApproachableCheck
	;clear shutdown approachable flag
	LDW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
	ANDL R6, #EVENT_FLAG_APPROACHABLE_SHUTDOWN_XOR
	STW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
    ;get/save start time stamp so we can diff latency later on
	LDD R6, xGSScheduleMetroStartStamp
	LDW R6, R6, #ZERO_OFFSET
	STW R6, R4, #EVENT_RUNTIME_LATENCY_METROSTAMP_OFFSET
	;save runtime parameter to proper event
	LDW R6, R5, #PARAMETER_RUNTIME_LOWWORD_OFFSET ;Load low word of parameter
	STW R6, R4, #EVENT_RUNTIME_LOW_WORD_OFFSET
	LDW R6, R5, #PARAMETER_RUNTIME_HIGHWORD_OFFSET ;Load high word of parameter
	STW R6, R4, #EVENT_RUNTIME_HIGH_WORD_OFFSET
TST R6 ;if the event is already running and we dont need more than a 16-bit runtime we should not need this check
BNE xgsDoneExtendingRuntime ;if zero flag event as approachable
	LDW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
	ORL R6, #EVENT_FLAG_APPROACHABLE_SHUTDOWN
	STW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
	;see if this events has the soonest countdown if it does set PIT0 check flag and store new countdown
;disable this optimization
;	LDD R6, xGSSystemVars
;	LDW R1, R6, #XGS_VARIABLE_SOONESTCOUNTDOWN_OFFSET
;	LDW R2, R5, #PARAMETER_RUNTIME_LOWWORD_OFFSET
;	CMP R2, R1
;BHI xgsDoneExtendingRuntime ;if currentEvent(R6) < nextEventTime(R4) update it!
;		STW R2, R6, #XGS_VARIABLE_SOONESTCOUNTDOWN_OFFSET ;save soonest time
		;LDW R2, R6, #XGS_VARIABLE_XGS_FLAGS_OFFSET
		;ORL R2, #XGS_FLAG_CHECKPIT0
		;STW R2, R6, #XGS_VARIABLE_XGS_FLAGS_OFFSET
xgsDoneExtendingRuntime:
BRA xgsParameterLoopNext
xgsSaveAllParameters:
	;save runtime parameter to proper event
	LDW R6, R5, #PARAMETER_RUNTIME_LOWWORD_OFFSET ;Load low word of parameter
	STW R6, R4, #EVENT_RUNTIME_LOW_WORD_OFFSET
	LDW R6, R5, #PARAMETER_RUNTIME_HIGHWORD_OFFSET ;Load high word of parameter
	STW R6, R4, #EVENT_RUNTIME_HIGH_WORD_OFFSET
	;clear parameter variables
	STW R0, R5, #PARAMETER_DELAY_HIGHWORD_OFFSET ;Load high word of parameter
	STW R0, R5, #PARAMETER_DELAY_LOWWORD_OFFSET ;Load high word of parameter
	STW R0, R5, #PARAMETER_RUNTIME_HIGHWORD_OFFSET ;Load high word of parameter
	STW R0, R5, #PARAMETER_RUNTIME_LOWWORD_OFFSET ;Load high word of parameter
	STW R0, R5, #PARAMETER_CHANNEL_ID_OFFSET ;channel id
	;flag our event as armed ;
	LDW R6, R4, #EVENT_STATUS_FLAGS_OFFSET ;set our event ARMED bit
	ORL	R6, #EVENT_FLAG_ARMED
	STW R6, R4, #EVENT_STATUS_FLAGS_OFFSET
	xgsParameterLoopNext:
	;perform loop increments
	LDD R4, xGSSystemVars ;inc our number of events processed
	LDW R2, R4, #XGS_VARIABLE_NUM_OF_EVENTS_SCHEDULED_OFFSET
	ADDL R2, #1 ;i++
	STW R2, R4, #XGS_VARIABLE_NUM_OF_EVENTS_SCHEDULED_OFFSET
	;inc our event parameter address offset
	LDW R2, R4, #XGS_VARIABLE_PARAMETER_BASE_ADDRESS_OFFSET
	ADDL R2, #PARAMETER_STRUCTURE_SIZE ;i++
	STW R2, R4, #XGS_VARIABLE_PARAMETER_BASE_ADDRESS_OFFSET
xgsParameterLoopCheck:
	LDD R4, xGSSystemVars ;inc our number of events processed
	LDW R2, R4, #XGS_VARIABLE_NUM_OF_EVENTS_SCHEDULED_OFFSET
	LDW R1, R4, #XGS_VARIABLE_NUM_OF_EVENTS_TO_SCHEDULE_OFFSET
	CMP R1, R2
BEQ xgsParameterLoopDone
	SUBL R2, #MAX_NUMBER_OF_PARAMETERS ;make sure we are operating withing our parameter allocation
BNE xgsParameterLoop
xgsParameterLoopDone:
	;see if we have approachable events
xgsApproachableCheck:
;pull a pin low to mark the end of our thread
;LDD R1, #1
;LDD R2, #0
;STB R2, R1, #0;
;as of 11-2-11 it takes 3.166 to sch one event and 14.708us to schedule 6 events



;for debugging/testing just branch reguardless
BRA xgsDone;

;TODO fix the optimization below. Right now it doesnt seem to get the lowest count into the down counter causing lates!
	LDD R5, xGSSystemVars
	LDW R6, R5, #XGS_VARIABLE_SOONESTCOUNTDOWN_OFFSET
	LDD R1, #PITCNT0 ;load PIT0 counter register address
	;Check to see if PIT0 is Enabled
	LDD R3, #PITCE
	LDB R4, R3, #ZERO_OFFSET
    BITL R4, #PIT0_ENABLE_MASK
BEQ xgsUseOurSoonest ;use our soonest countdown if PIT0 is idle
	;if PIT0 is enabled see if its down counter is sooner than our soonest countdown
	LDW R7, R1, #ZERO_OFFSET
	CMP R7, R6
BCC xgsDone ;PIT0 already has a countdown less than our shortest that was scheduled
	MOV R6, R7
xgsUseOurSoonest:
    ;enable PIT0 down counter interrupt
	LDB R2, R3, #ZERO_OFFSET
	ORL R2, #PIT0_ENABLE_MASK
	LDD R5, PITLD0
	STW R6, R5, #ZERO_OFFSET ;push our countdown into the down counter register
	STB R2, R3, #ZERO_OFFSET
;	LD
;	ANDL R2, #XGS_FLAG_CHECKPIT0
;BNE xgatePITBangLoopCall

;for debugging/testing just branch reguardless
;BRA xgatePITBangLoopCall
;	BITL R6, #EVENT_FLAG_APPROACHABLE;
;BNE xgatePITBangLoopCall ;if bit set jump
;	BITL R6, #EVENT_FLAG_APPROACHABLE_SHUTDOWN;
;BNE xgatePITBangLoopCall ;if bit set jump

	;debug code we have not yet handeled an overlapping schedule
	;TODO just inc a skipped events counter
	;LDD R7, #PORTBA
	;LDD R6, #0xFefe
	;STW R6, R7, #0
	;brk
xgsDoNotSchedule:
	xgsDone:
	;clear semaphore
	CSEM #0
;debug code
;LDW R4, EVENT_0_REGISTER_MASK
;LDW R5, xgatePORTBMaskOff
;JAL R5
;for debugging/testing just branch reguardless
BRA xgatePITBangLoopCall
	RTS
xgateScheduleEnd:

/**
 * @endcond DOXYGEN_IGNORE_ASM
*/

;xgateMetronome()

/**
 * @brief xgateMetronome()
 *
 * @brief Loop though our events and decrement the high words of our coutdowns.  If the
 *
 *
 * @note Schedule logic
 *
 * @todo TODO consider moving code to PIT ISR
 *
 *
 * @warning Test warn
 *
 * @author Sean Keys
 *
 * @cond DOXYGEN_IGNORE_ASM
*/

xgateMetronome: ; PIT 2 ISR, called by PIT2 interrupt. Decrement out delayCounter.
;debug heartbeat code
;LDD R5, xgatePORTBFlip
;JAL R5
    ;TODO save start time stamp
	LDD R2, xGMStartTime
	LDD R3, PITCNT1
	LDW R3, R3, #ZERO_OFFSET
	STW R3, R2, #ZERO_OFFSET
    ;CIF
    LDL R2, #PIT1_CFLAG_MASK
	LDD R3, #PITTF
	STB R2, R3, #ZERO_OFFSET
	;decrement our high word delays and runtimes value by 1
	;calculate our pointer offsets to our structure of interest
	LDL R2, #0x00 ;initialize loop register
	LDL R7, #FALSE ;initialize our branch to update short test var
	LDD R3, eventsStructStart ;initialize our structure pointer to start of onEvents array
	LDD R4, #NUMBER_OF_EVENT_STRUCTURES ;ld the number of structures we need to loop though
BRA XMLoop1
	XMLoop1Block: ; decrement all the high words of our countdown
	LDW R5, R3, #EVENT_STATUS_FLAGS_OFFSET
	;if our event is not armed check the next event
	BITL R5, #EVENT_FLAG_ARMED
	BEQ XMNoApproachableShutdownSet ;skip to the next event
	;if our event is running dont bother doing anything with the delay
	BITL R5, #EVENT_FLAG_RUNNING
	BNE XMNoApproachableFlag
		LDW R5, R3, #EVENT_DELAY_HIGH_WORD_OFFSET
		SUBL R5, #1 ;subtract 1 from our high word delay
		STW R5, R3, #EVENT_DELAY_HIGH_WORD_OFFSET
		;test to see if our event needs to be armed and schedule
		TST R5 ;see if our countdown is zero if it is see if its flagged for action
		BNE XMNoApproachableFlag ;branch if our channel armed flag is not set
		LDW R5, R3, #EVENT_STATUS_FLAGS_OFFSET
		;set EVENT_FLAG_APPROACHABLE bit
		ORL	R5, #EVENT_FLAG_APPROACHABLE
		STW R5, R3, #EVENT_STATUS_FLAGS_OFFSET
		;place metro stamp
		LDD R5, xGMStartTime
		LDW R5, R5, #ZERO_OFFSET
		STW R5, R3, #EVENT_DELAY_LATENCY_METROSTAMP_OFFSET
		LDL R7, #TRUE ;Flag R7 true if we need to branch to process a near countdown
		XMNoApproachableFlag:
		;do the same for our runtime high words
		LDW R5, R3, #EVENT_RUNTIME_HIGH_WORD_OFFSET
		SUBL R5, #1 ;subtract 1 from our high word delay
		STW R5, R3, #EVENT_RUNTIME_HIGH_WORD_OFFSET
		;test to see if our event needs to be armed and schedule
		TST R5 ;see if our countdown is zero if it is see if its flagged for action
		BNE XMNoApproachableShutdownSet ;branch if we are still not approachable
		LDW R5, R3, #EVENT_STATUS_FLAGS_OFFSET
		;set EVENT_FLAG_APPROACHABLE_SHUTDOWN bit
		ORL	R5, #EVENT_FLAG_APPROACHABLE_SHUTDOWN
		STW R5, R3, #EVENT_STATUS_FLAGS_OFFSET
		;place RuntimeMetronomeStamp
		LDD R5, xGMStartTime
		LDW R5, R5, #ZERO_OFFSET
		STW R5, R3, #EVENT_RUNTIME_LATENCY_METROSTAMP_OFFSET
		LDL R7, #TRUE ;Flag R7 true if we need to branch to process a near countdown
		XMNoApproachableShutdownSet: ;jump here if countdown is greater than 16 bits
		ADDL R3, #EVENT_STRUCTURE_SIZE
		ADDL R2, #0x01 ;i++
	XMLoop1:
		CMP R4, R2
		BHI XMLoop1Block  ; if R4 >= R2

;Debug flip PORTP everytime we overflow
;LDD R5, xgatePORTPFlip
;JAL R5; jump to xgatePORTPFlip
	TST R7
		BNE xgatePITBangLoopCall ;branch to another function, it will issue the RTS when it completes
	RTS
xgateMetronomeEnd:
/**
 * @endcond DOXYGEN_IGNORE_ASM
*/

; xgatePITBangLoop()
/**
 * @brief xgatePITBangLoop()
 *
 * @note
 *
 *
 * @note Schedule logic
 *
 * @todo TODO consider moving code to PIT ISR
 *
 *
 * @warning Test warn
 *
 * @author Sean Keys
 *
 * @cond DOXYGEN_IGNORE_ASM
*/
xgatePITBangLoop: ; PIT 0 ISR, called by PIT0 interrupt. Loop though our que and see if we need to bang any registers
  ;CIF was here
;Either the MetroNome or XGScheduler can send us here
xgatePITBangLoopCall:
;
;pull a pin high to mark the start of our thread
;LDD R1, #1
;LDD R2, #0x80
;STB R2, R1, #0
;debug code
;LDD R5, xgatePORTPFlip
;JAL R5; jump to xgatePORTPFlip
	;save current metro stamp
	LDD R3, #PITCNT1
	LDW R3, R3, #ZERO_OFFSET
	LDD R2, pitDelaySubSystemVars
	STW R3, R2, #PDS_VARIABLE_PIT0THREADSTARTSTAMP_OFFSET
	;Disable pit channel 0
	LDD R2, #PITCE
	LDB R4, R2, #ZERO_OFFSET
    ANDL R4, #PIT0_DISABLE_MASK
    STB R4, R2, #ZERO_OFFSET
	;CIF
	LDL R2, #PIT0_CFLAG_MASK
	LDD R1, #PITTF
	STB R2, R1, #ZERO_OFFSET
	;clear our SELF_UPDATE bit
	LDD R2, pitDelaySubSystemVars
	LDW R1, R2, #PDS_VARIABLE_PITDELAYFLAGS_OFFSET
	ANDL R1, #PDS_FLAG_SELF_UPDATE_BIT_XOR
	STW R1, R2, #PDS_VARIABLE_PITDELAYFLAGS_OFFSET

BRA XGQEND ;Debug this skips the optimized bang loop
;;;;;;;;START FAST BANG LOOP
	LDL R1, #0x00 ;initialize loop count register
	LDD R2, eventsStructStart ;initialize our structure pointer to start of onEvents array
;	LDD R3, pitDelaySubSystemVars
;	LDW R3, R3, #PDS_VARIABLE_PIT0THREADSTARTSTAMP_OFFSET
BRA XGQPIT0BangLoop
		XGQPIT0CheckBangOn:
		LDW R4, R2, #EVENT_STATUS_FLAGS_OFFSET
		BITL R4, #EVENT_FLAG_ARMED
	BEQ	 XGQPIT0CheckNextEvent
		BITL R4, #EVENT_FLAG_APPROACHABLE ;see if your
	BEQ XGQPIT0CheckBangOff ;skip subtracting our last countdown if our approachable flag is not set
		LDW R5, R2, #EVENT_DELAY_LATENCY_METROSTAMP_OFFSET ;load last time stamp
		SUB R5, R5, R3 ;R5 now contains elapsed time
		LDW R6, R2, #EVENT_DELAY_LOW_WORD_OFFSET ;load time remaining
		SUB R5, R6, R5
	BCS XGQPIT0BangOn ;did we carry(are we late)?
		SUBL R5, #LATENCY_ZEROTIME_TOLLERANCE ;
	BCS XGQPIT0BangOn ;are we on-time?
		;TODO if our metronome rolledover this could happen with HIGH counts near 16 bits FIXME
	BRA XGQPIT0CheckBangOff ;branch if greater than zero
		XGQPIT0BangOn: ;bang register pin(s) on
		LDW R5, R2, #EVENT_BANG_REGISTER_OFFSET ;load the address of the register we wish to bang from the event structure
		LDB R6, R5, #ZERO_OFFSET ;save the register value
		LDW	R7, R2, #EVENT_BANG_MASK_OFFSET ;load the mask we want to apply
		OR R6, R6, R7
		STB R6, R5, #ZERO_OFFSET ;TODO add config to maybe bang a 16-bit register if necessary
		ANDL R4, #EVENT_FLAG_APPROACHABLE_XOR
		STW R4, R2, #EVENT_STATUS_FLAGS_OFFSET ;clear approachable flag
	BRA XGQPIT0CheckNextEvent
		XGQPIT0CheckBangOff:
        LDW R4, R2, #EVENT_STATUS_FLAGS_OFFSET
		BITL R4, #EVENT_FLAG_APPROACHABLE_SHUTDOWN ;see if your
	BRA XGQPIT0CheckNextEvent
	BEQ XGQPIT0CheckBangOff ;skip subtracting our last countdown if our approachable flag is not set
		LDW R5, R2, #EVENT_RUNTIME_LATENCY_METROSTAMP_OFFSET ;load time left
		SUB R5, R5, R3 ;R5 now contains our time left
		LDW R6, R2, #EVENT_RUNTIME_LOW_WORD_OFFSET ;load time remaining
		SUB R5, R6, R5
	BCS XGQPIT0BangOff ;did we carry?
		SUBL R5, #LATENCY_ZEROTIME_TOLLERANCE
	BCS XGQPIT0BangOff	;did we carry?
		;TODO if our metronome rolledover this could happen with HIGH counts near 16 bits FIXME
	BRA XGQPIT0CheckNextEvent ;branch if greater than zero
		XGQPIT0BangOff: ;bang register pin(s) off
		LDW R5, R2, #EVENT_BANG_REGISTER_OFFSET;load the address of the register we wish to bang from the event structure
		LDB R6, R5, #ZERO_OFFSET ;save the register value
		LDW	R7, R2, #EVENT_BANG_MASK_OFFSET ;load the mask we want to apply
		COM R7 ;make into AND mask
		AND R6, R6, R7
		STB R6, R5, #ZERO_OFFSET ;TODO add config to maybe bang a 16-bit register
		;ANDL R4, #EVENT_FLAG_APPROACHABLE_SHUTDOWN_XOR
		;STW R4, R2, #EVENT_STATUS_FLAGS_OFFSET ;clear approachable flag
		STW R0, R2, #EVENT_STATUS_FLAGS_OFFSET ;clear approachable flag
		XGQPIT0CheckNextEvent:
		;advance our loop counter and structure
		ADDL R2, #EVENT_STRUCTURE_SIZE
		ADDL R1, #0x01 ;i++
	XGQPIT0BangLoop:
		MOV R4, R1
		SUBL R4, #NUMBER_OF_EVENT_STRUCTURES
		BNE XGQPIT0CheckBangOn
;;;;;END FAST BANG LOOP
XGQEND:

; Set this to a high value initially so any value will be the soonest.
; But not so high things could roll over
	LDD R1, pitDelaySubSystemVars
	LDD R3, 0xFF00
	STW R3, R1, #PDS_VARIABLE_PITDELAYNEXTCOUNTDOWN_OFFSET
	;************************************************************************************
	; loop thought events if event is flagged as EVENT_FLAG_APPROACHABLE and countdown is zero
	; after we Subtract our last runtime from other events bang the register clear the masks
	;************************************************************************************
	; loop though our events, if the event is zero-time(or withing tollerance)bang the register
	;subtract last runtime from events flagged APPROACHABLE
	LDL R1, #0x00 ;initialize loop register
	LDD R2, eventsStructStart ;initialize our structure pointer to start of onEvents array
;	LDL R3, #NUMBER_OF_EVENT_STRUCTURES ;ld the number of structures we need to loop though
BRA XGPITOnLoop1
	XGPITOnLoop1Block:
		LDW R4, R2, #EVENT_STATUS_FLAGS_OFFSET
		BITL R4, #EVENT_FLAG_APPROACHABLE ;see if your
	BEQ PIT0CheckBangOff ;skip subtracting our last countdown if our approachable flag is not set
		LDD R5, pitDelaySubSystemVars
		LDW R5, R5, #PDS_VARIABLE_PIT0THREADSTARTSTAMP_OFFSET ;earlier verson used the current #PITCNT1
		LDW R6, R2, #EVENT_DELAY_LATENCY_METROSTAMP_OFFSET
		STW R5, R2, #EVENT_DELAY_LATENCY_METROSTAMP_OFFSET ;store new metro value
		SUB R5, R6, R5 ;R5 now contains our elapsed time
		LDW R6, R2, #EVENT_DELAY_LOW_WORD_OFFSET
		SUB R7, R6, R5
	BCS PITOnBang
		;TODO if our metronome rolledover this could happen with HIGH counts near 16 bits FIXME
		STW R7, R2, #EVENT_DELAY_LOW_WORD_OFFSET ;save the updated event approacable delay incase we arent at zero time
		;see if we are now zero time or past zero time
		MOV R3, R7
		SUBL R7, #LATENCY_ZEROTIME_TOLLERANCE
	BCS PITOnBang
	BRA XGPITOnProcessDelayTime ;branch if greater than zero
		PITOnBang:
		;bang register
		LDW R4, R2, #EVENT_BANG_REGISTER_OFFSET ;load the address of the register we wish to bang from the event structure
		LDB R5, R4, #ZERO_OFFSET ;save the register value
		LDW	R6, R2, #EVENT_BANG_MASK_OFFSET ;load the mask we want to apply
		OR R5, R6, R5
		STB R5, R4, #ZERO_OFFSET ;TODO add config to maybe bang a 16-bit register if necessary
		;clear approach flag
		LDW R4, R2, #EVENT_STATUS_FLAGS_OFFSET
		ANDL R4, #EVENT_FLAG_APPROACHABLE_XOR
		;flag event as running
		ORL R4, #EVENT_FLAG_RUNNING
		STW R4, R2, #EVENT_STATUS_FLAGS_OFFSET
	BRA PIT0CheckBangOff
		XGPITOnProcessDelayTime: ;we end up here if our event was approachable but not zero time
;TODO check to see if this event is the soonest to be scheduled
		LDD R5, pitDelaySubSystemVars
		LDW R4, R5, #PDS_VARIABLE_PITDELAYNEXTCOUNTDOWN_OFFSET
		CMP R7, R4
	BHI PIT0ThisDelayNotSoonest ;if currentEvent(R6) < nextEventTime(R4) update it!
		STW R7, R5, #PDS_VARIABLE_PITDELAYNEXTCOUNTDOWN_OFFSET ;its ok to store zero because the APPROACHABLE bit needs to be set just to get here
		PIT0ThisDelayNotSoonest:
		LDW R4, R5, #PDS_VARIABLE_PITDELAYFLAGS_OFFSET
		ORL R4, #PDS_FLAG_SELF_UPDATE_BIT
		STW R4, R5, #PDS_VARIABLE_PITDELAYFLAGS_OFFSET
		BRA XGPITCheckNextEvent
;-------continue loop and see what events need to be banged off ;TODO see if its wiser to check for bang off events
		PIT0CheckBangOff:
        LDW R4, R2, #EVENT_STATUS_FLAGS_OFFSET
	BITL R4, #EVENT_FLAG_APPROACHABLE_SHUTDOWN ;see if your
	BEQ XGPITCheckNextEvent ;skip to next event if we are not to do anything with this ones runtime
		;subtract our elapsed ticks from our approachable shutdown events
		LDD R5, pitDelaySubSystemVars
		LDW R5, R5, #PDS_VARIABLE_PIT0THREADSTARTSTAMP_OFFSET
		LDW R6, R2, #EVENT_RUNTIME_LATENCY_METROSTAMP_OFFSET
		STW R5, R2, #EVENT_RUNTIME_LATENCY_METROSTAMP_OFFSET
		SUB R5, R6, R5 ;R5 now contains our elapsed time
		LDW R6, R2, #EVENT_RUNTIME_LOW_WORD_OFFSET
		;TODO if our metronome rolledover this could happen with HIGH counts near 16 bits FIXME
		SUB R7, R6, R5
	BCS PIT0BangOff
		STW R7, R2, #EVENT_RUNTIME_LOW_WORD_OFFSET ;save the updated event approacable delay incase we arent at zero time
		;see if we are close to zerotime
		MOV R3, R7
		SUBL R7, #LATENCY_ZEROTIME_TOLLERANCE
	BCS PIT0BangOff
	BRA XGPITOnProcessRunTime ;branch if greater than zero
		PIT0BangOff:
		;bang register off
		LDW R4, R2, #EVENT_BANG_REGISTER_OFFSET;load the address of the register we wish to bang from the event structure
		LDB R5, R4, #ZERO_OFFSET ;save the register value
		LDW	R6, R2, #EVENT_BANG_MASK_OFFSET ;load the mask we want to apply
		COM R6 ;make into an AND mask
		AND R5, R6, R5
		STB R5, R4, #ZERO_OFFSET ;TODO add config to maybe bang a 16-bit register
		;clear approach shutdown flag
		;LDW R4, R2, #EVENT_STATUS_FLAGS_OFFSET
		;ANDL R4, #EVENT_FLAG_APPROACHABLE_SHUTDOWN_XOR
		;clear the armed flag
		;ANDL R4, #EVENT_FLAG_ARMED_XOR
		;clear event to availiable state
		;LDD R4, #EVENT_FLAG_AVAILABLE
		;LDL R4, #0 ;since we are done with the event we can just push one value
		STW R0, R2, #EVENT_STATUS_FLAGS_OFFSET
		BRA XGPITCheckNextEvent
		XGPITOnProcessRunTime:
		;check to see if this event is the soonest to be scheduled
		LDD R5, pitDelaySubSystemVars
		LDW R4, R5, #PDS_VARIABLE_PITDELAYNEXTCOUNTDOWN_OFFSET
		;LDW R6, R2, #EVENT_DELAY_LOW_WORD_OFFSET
		CMP R7, R4
	BHI PIT0ThisRuntimeNotSoonest ;if currentEvent(R6) < nextEventTime(R4) update it!
		STW R7, R5, #PDS_VARIABLE_PITDELAYNEXTCOUNTDOWN_OFFSET ;its ok to store zero because the APPROACHABLE bit needs to be set just to get here
		PIT0ThisRuntimeNotSoonest:
		LDW R4, R5, #PDS_VARIABLE_PITDELAYFLAGS_OFFSET
		ORL R4, #PDS_FLAG_SELF_UPDATE_BIT
		STW R4, R5, #PDS_VARIABLE_PITDELAYFLAGS_OFFSET
		PITOnNotApproachableShutdown: ;jump here if we are to not allowed to approach an event for shutdown
		XGPITCheckNextEvent:
		;advance our loop counter and structure
		ADDL R2, #EVENT_STRUCTURE_SIZE
		ADDL R1, #0x01 ;i++
	XGPITOnLoop1:
		;CMP R3, R1
		;BHI XGPITOnLoop1Block
		MOV R3, R1
		SUBL R3, #NUMBER_OF_EVENT_STRUCTURES
		BNE XGPITOnLoop1Block
	;Check our SELF_UPDATE bit and set downcounter if we have an approchable event
	LDD R2, pitDelaySubSystemVars
	LDW R3, R2, #PDS_VARIABLE_PITDELAYFLAGS_OFFSET
	BITL R3, #PDS_FLAG_SELF_UPDATE_BIT
BEQ xgPIT0Return
	;set counter and enable interrupt
;	LDD R1, pitDelaySubSystemVars
	LDW R2, R2, #PDS_VARIABLE_PITDELAYNEXTCOUNTDOWN_OFFSET
;	STW R2, R1, #PDS_VARIABLE_PITONLASTCOUNTDOWN_OFFSET
	;TODO subtract the ammount of time elapsed from start of thread instead of applying the live CNT
	LDD R1, #PITLD0
	STW R2, R1, #ZERO_OFFSET ;store count down value in register
;debug echo countdown value
;LDD R5, #PORTBA
;STW R2, R5, #ZERO_OFFSET
;brk
	LDD R1, #PITCE
	LDB R2, R1, #ZERO_OFFSET
	ORL R2, #PIT0_ENABLE_MASK
	STB R2, R1, #ZERO_OFFSET
;debug echo channels enabled value
;LDD R5, PORTBA
;LDD R2, #0xFFFF
;STW R2, R5, #ZERO_OFFSET
;brk
	xgPIT0Return:
;debug code
;LDD R5, xgatePORTPFlip
;JAL R5; jump to xgatePORTPFlip

	;see if we need to jump back to the scheduler
	LDD R4, xGSSystemVars
	LDW R2, R4, #XGS_VARIABLE_XGS_FLAGS_OFFSET
	BITL R2, #XGS_FLAG_JUMPED_TO_PIT0_LOOP
	BEQ noJump
	LDD R7, xgsContinue
	JAL R7
	;brk
	noJump:
;pull a pin high to mark the start of our thread
;LDD R1, #1
;LDD R2, #0
;STB R2, R1, #0
		;For Debugging ++ our thread counter ;TODO comment out when not in use
		;LDD R1, pitDelaySubSystemVars
		;LDW R2, R1, #PDS_VARIABLE_PITDELAYTHREADRUNS_OFFSET
		;ADDL R2, #1
		;STW R2, R1, #PDS_VARIABLE_PITDELAYTHREADRUNS_OFFSET
	RTS
xgatePITBangLoopEnd:

/**
 * @endcond DOXYGEN_IGNORE_ASM
*/

;xgateErrorThread()

/**
 * @brief xgateErrorThread()
 *
 * @note
 *
 *
 * @note Schedule logic
 *
 * @todo TODO consider moving code to PIT ISR
 *
 *
 * @warning Test warn
 *
 * @author Sean Keys
 *
 * @cond DOXYGEN_IGNORE_ASM
*/
xgateErrorThread:
	;R1 will have the xgate channel number that was executed
	;TODO build some defualt code to increment an error counter, Fred can you give me the addres of a a status var that will show in the log output !
/**
 * @endcond DOXYGEN_IGNORE_ASM
*/

**************************************************
;  utility threads
**************************************************

/**
 * @cond DOXYGEN_IGNORE_ASM
*/

;IMULU: ; multiply two registers(R1*R2)
;	STW R1, (R0,-R7)
;	STW R4, (R0,-R7)
;    STW R5, (R0,-R7)
;	MOV R4, R2
;	BPL *+6       ;abs = 0x0000403A
;	NEG R4
;    NEG R3
;    LDL R2, #0
;    BRA *+10       ;abs = 0x00004046
;    BFINS R4, R0, R5
;    MOV R1, R3
;    LSL R1, R5
;    ADD R2, R1, R2
;    BFFO R5, R4
;	BCC *-10       ;abs = 0x0000403E
;    LDW R5, (R0,R7+)
;    LDW R4, (R0,R7+)
;    LDW R1, (R0,R7+)
;    JAL       R6

; *****************************************************************************
;
;	VARIABLES
;
;	Remember we are dealing with a 16-bit RISC CPU, alignment care should be
;	taken when defining 8-bit vars/constants
;
; *****************************************************************************
parametersBase:
	parameterChannelID: ;an index to our array of events
		.word 0
	parameterDelayH: ; How long from now to trigger
		.word 0 ;high word
	parameterDelay: ; How long from now to trigger
		.word 0 ;Low word countdown
	parameterRuntimeH: ;How long to run once triggered
		.word 0 ;High word countdown
	parameterRuntime: ;How long to run once triggered
		.word 0 ;Low word  countdown
	parameterInputStamp:
		.word 0
	;second parameter
	parameterChannelID1: ;an index to our array of events
		.word 0
	parameterDelayH1: ; How long from now to trigger
		.word 0 ;high byte
	parameterDelay1: ; How long from now to trigger
		.word 0 ;Low word countdown
	parameterRuntimeH1: ;How long to run once triggered
		.word 0 ;High word countdown
	parameterRuntime1: ;How long to run once triggered
		.word 0 ;Low word  countdown
	parameterInputStamp1:
		.word 0
	;third parameter
	parameterChannelID2: ;an index to our array of events
		.word 0
	parameterDelayH2: ; How long from now to trigger
		.word 0 ;high byte
	parameterDelay2: ; How long from now to trigger
		.word 0 ;Low word countdown
	parameterRuntimeH2: ;How long to run once triggered
		.word 0 ;High word countdown
	parameterRuntime2: ;How long to run once triggered
		.word 0 ;Low word  countdown
	parameterInputStamp2:
		.word 0
	;fourth parameter
	parameterChannelID3: ;an index to our array of events
		.word 0
	parameterDelayH3: ; How long from now to trigger
		.word 0;high byte
	parameterDelay3: ; How long from now to trigger
		.word 0 ;Low word countdown
	parameterRuntimeH3: ;How long to run once triggered
		.word 0 ;High word countdown
	parameterRuntime3: ;How long to run once triggered
		.word 0 ;Low word  countdown
	parameterInputStamp3:
		.word 0
	;fifth parameter
	parameterChannelID4: ;an index to our array of events
		.word 0
	parameterDelayH4: ; How long from now to trigger
		.word 0;high byte
	parameterDelay4: ; How long from now to trigger
		.word 0 ;Low word countdown
	parameterRuntimeH4: ;How long to run once triggered
		.word 0 ;High word countdown
	parameterRuntime4: ;How long to run once triggered
		.word 0 ;Low word  countdown
	parameterInputStamp4:
		.word 0
	;sixth parameter
	parameterChannelID5: ;an index to our array of events
		.word 0
	parameterDelayH5: ; How long from now to trigger
		.word 0;high byte
	parameterDelay5: ; How long from now to trigger
		.word 0 ;Low word countdown
	parameterRuntimeH5: ;How long to run once triggered
		.word 0 ;High word countdown
	parameterRuntime5: ;How long to run once triggered
		.word 0 ;Low word  countdown
	parameterInputStamp5:
		.word 0

pitDelaySubSystemVars:
	pitOnLastCountDown:
		.word 0 ;used to hold the pit count value when cals are needed
	pitDelayNextCountDown:
		.word 0
	pitDelayFlags:
		.word 0 ;keeps status of whats going on
	pitDelayThreadStartStamp:
		.word 0
;	pitDelayThreadRuns: ;keep track of the number of times the ISR is called
;		.word 0
;	pitDelayThreadEXECTime: ;this variable holds the amount of time in ticks that the thread previously took to run
;		.word
;	pitDelayThreadStartStamp:
;		.word
;	pitDelayThreadStopStamp:
;		.word

; xgateSchedule variables
xGSSystemVars:
	xGSScheduleMetroStartStamp: ;timestamp that contains the metronomes count at the time of the schedule interrupt
		.word 0
	xgsEventAddressBase:
		.word 0
	xgsNumOfEventsToSchedule:
		.word 0
	xgsNumOfEventsScheduled: ;numer of events scheduled the xgs has scheduled in one run
		.word 0
	xGSFlags:
		.word 0
	xGSParameterAddressBase:
		.word 0
	xGSInputEdgeStamp:
		.word 0
	xGSScheduleTCNTStartStamp:
		.word 0
	xGSScheduleSoonestCountDown:
		.word 0

;variables used by the metronome function
xGMSysytemVars:
	xGMStartTime: ;timestamp that contains the metronomes current count
		.word

XGBBCounters:
	xGBBBadChannelRequest: ;num of times an invalid channel tried to be scheduled
		.word

;register variables these can hold the values of registers incase we need to jump and return
xGRegisters:
	savedRegOne:
		.word 0
	savedRegTwo:
		.word 0
	savedRegThree:
		.word 0
	savedRegFour:
		.word 0
	savedRegFive:
		.word 0
	savedRegSix:
		.word 0
	savedRegSeven:
		.word 0
	savedCCR:
		.word 0

****************************************************************************************
;  Event Structures
****************************************************************************************
/* array of ON event structures */
eventsStructStart:
	eventChannelID0: /* a const that contains this events ENUM ID */
    	.word 0
	eventDelay0:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime0:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags0: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress0: ;The target register we wish to bang
    	.word EVENT_0_REGISTER_ADDRESS
    eventRegisterMask0: ;The mask to apply to the target register when we bang
    	.word EVENT_0_REGISTER_MASK
    eventXGSTicks0: ;TODO this isnt really needed and NOT used right nowhold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp0: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp0: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
    	.word 0
	;second event
	eventChannelID1: /* a const that contains this events ENUM ID */
    	.word 1
	eventDelay1:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime1:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags1: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress1: ;The target register we wish to bang
    	.word EVENT_1_REGISTER_ADDRESS
    eventRegisterMask1: ;The mask to apply to the target register when we bang
    	.word EVENT_1_REGISTER_MASK
    eventXGSTicks1: ;TODO this isnt really needed and NOT used right nowhold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp1: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp1: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
    	.word 0
    ;third event
	eventChannelID2: /* a const that contains this events ENUM ID */
    	.word 2
	eventDelay2:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime2:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags2: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress2: ;The target register we wish to bang
    	.word EVENT_2_REGISTER_ADDRESS
    eventRegisterMask2: ;The mask to apply to the target register when we bang
    	.word EVENT_2_REGISTER_MASK
    eventXGSTicks2: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp2: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp2: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
    	.word 0
    ;forth event
	eventChannelID3: /* a const that contains this events ENUM ID */
    	.word 3
	eventDelay3:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime3:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags3: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress3: ;The target register we wish to bang
    	.word EVENT_3_REGISTER_ADDRESS
    eventRegisterMask3: ;The mask to apply to the target register when we bang
    	.word EVENT_3_REGISTER_MASK
    eventXGSTicks3: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp3: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp3: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
    	.word 0
    ;fifth event
	eventChannelID4: /* a const that contains this events ENUM ID */
    	.word 4
	eventDelay4:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime4:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags4: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress4: ;The target register we wish to bang
    	.word EVENT_4_REGISTER_ADDRESS
    eventRegisterMask4: ;The mask to apply to the target register when we bang
    	.word EVENT_4_REGISTER_MASK
    eventXGSTicks4: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp4: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp4: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
    	.word 0
    ;sixth event
	eventChannelID5: /* a const that contains this events ENUM ID */
    	.word 5
	eventDelay5:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime5:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags5: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress5: ;The target register we wish to bang
    	.word EVENT_5_REGISTER_ADDRESS
    eventRegisterMask5: ;The mask to apply to the target register when we bang
    	.word EVENT_5_REGISTER_MASK
    eventXGSTicks5: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp5: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp5: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
    	.word 0
    ;seventh event
	eventChannelID6: /* a const that contains this events ENUM ID */
    	.word 6
	eventDelay6:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime6:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags6: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress6: ;The target register we wish to bang
    	.word EVENT_6_REGISTER_ADDRESS
    eventRegisterMask6: ;The mask to apply to the target register when we bang
    	.word EVENT_6_REGISTER_MASK
    eventXGSTicks6: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp6: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp6: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
    	.word 0
    ;eigth event
	eventChannelID7: /* a const that contains this events ENUM ID */
    	.word 7
	eventDelay7:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime7:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags7: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress7: ;The target register we wish to bang
    	.word EVENT_7_REGISTER_ADDRESS
    eventRegisterMask7: ;The mask to apply to the target register when we bang
    	.word EVENT_7_REGISTER_MASK
    eventXGSTicks7: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp7: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp7: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;ninth event
	eventChannelID8: /* a const that contains this events ENUM ID */
    	.word 8
	eventDelay8:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime8:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags8: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress8: ;The target register we wish to bang
    	.word EVENT_8_REGISTER_ADDRESS
    eventRegisterMask8: ;The mask to apply to the target register when we bang
    	.word EVENT_8_REGISTER_MASK
    eventXGSTicks8: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp8: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp8: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;tenth event
	eventChannelID9: /* a const that contains this events ENUM ID */
    	.word 9
	eventDelay9:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime9:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags9: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress9: ;The target register we wish to bang
    	.word EVENT_9_REGISTER_ADDRESS
    eventRegisterMask9: ;The mask to apply to the target register when we bang
    	.word EVENT_9_REGISTER_MASK
    eventXGSTicks9: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp9: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp9: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;eleventh event
	eventChannelID10: /* a const that contains this events ENUM ID */
    	.word 10
	eventDelay10:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime10:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags10: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress10: ;The target register we wish to bang
    	.word EVENT_10_REGISTER_ADDRESS
    eventRegisterMask10: ;The mask to apply to the target register when we bang
    	.word EVENT_10_REGISTER_MASK
    eventXGSTicks10: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp10: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp10: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twelveth event
	eventChannelID11: /* a const that contains this events ENUM ID */
    	.word 11
	eventDelay11:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime11:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags11: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress11: ;The target register we wish to bang
    	.word EVENT_11_REGISTER_ADDRESS
    eventRegisterMask11: ;The mask to apply to the target register when we bang
    	.word EVENT_11_REGISTER_MASK
    eventXGSTicks11: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp11: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp11: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thirteenth event
	eventChannelID12: /* a const that contains this events ENUM ID */
    	.word 12
	eventDelay12:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime12:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags12: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress12: ;The target register we wish to bang
    	.word EVENT_12_REGISTER_ADDRESS
    eventRegisterMask12: ;The mask to apply to the target register when we bang
    	.word EVENT_12_REGISTER_MASK
    eventXGSTicks12: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp12: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp12: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;fourteenth event
	eventChannelID13: /* a const that contains this events ENUM ID */
    	.word 13
	eventDelay13:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime13:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags13: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress13: ;The target register we wish to bang
    	.word EVENT_13_REGISTER_ADDRESS
    eventRegisterMask13: ;The mask to apply to the target register when we bang
    	.word EVENT_13_REGISTER_MASK
    eventXGSTicks13: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp13: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp13: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;fifteenth event
	eventChannelID14: /* a const that contains this events ENUM ID */
    	.word 14
	eventDelay14:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime14:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags14: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress14: ;The target register we wish to bang
    	.word EVENT_14_REGISTER_ADDRESS
    eventRegisterMask14: ;The mask to apply to the target register when we bang
    	.word EVENT_14_REGISTER_MASK
    eventXGSTicks14: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp14: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp14: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;sixteenth event
	eventChannelID15: /* a const that contains this events ENUM ID */
    	.word 15
	eventDelay15:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime15:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags15: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress15: ;The target register we wish to bang
    	.word EVENT_15_REGISTER_ADDRESS
    eventRegisterMask15: ;The mask to apply to the target register when we bang
    	.word EVENT_15_REGISTER_MASK
    eventXGSTicks15: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp15: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp15: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;seventeenth event
	eventChannelID16: /* a const that contains this events ENUM ID */
    	.word 16
	eventDelay16:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime16:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags16: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress16: ;The target register we wish to bang
    	.word EVENT_16_REGISTER_ADDRESS
    eventRegisterMask16: ;The mask to apply to the target register when we bang
    	.word EVENT_16_REGISTER_MASK
    eventXGSTicks16: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp16: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp16: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;eighteenth event
	eventChannelID17: /* a const that contains this events ENUM ID */
    	.word 17
	eventDelay17:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime17:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags17: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress17: ;The target register we wish to bang
    	.word EVENT_17_REGISTER_ADDRESS
    eventRegisterMask17: ;The mask to apply to the target register when we bang
    	.word EVENT_17_REGISTER_MASK
    eventXGSTicks17: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp17: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp17: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;nineteenth event
	eventChannelID18: /* a const that contains this events ENUM ID */
    	.word 18
	eventDelay18:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime18:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags18: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress18: ;The target register we wish to bang
    	.word EVENT_18_REGISTER_ADDRESS
    eventRegisterMask18: ;The mask to apply to the target register when we bang
    	.word EVENT_18_REGISTER_MASK
    eventXGSTicks18: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp18: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp18: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenthieth event
	eventChannelID19: /* a const that contains this events ENUM ID */
    	.word 19
	eventDelay19:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime19:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags19: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress19: ;The target register we wish to bang
    	.word EVENT_19_REGISTER_ADDRESS
    eventRegisterMask19: ;The mask to apply to the target register when we bang
    	.word EVENT_19_REGISTER_MASK
    eventXGSTicks19: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp19: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp19: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty first event
	eventChannelID20: /* a const that contains this events ENUM ID */
    	.word 20
	eventDelay20:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime20:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags20: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress20: ;The target register we wish to bang
    	.word EVENT_20_REGISTER_ADDRESS
    eventRegisterMask20: ;The mask to apply to the target register when we bang
    	.word EVENT_20_REGISTER_MASK
    eventXGSTicks20: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp20: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp20: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty second event
	eventChannelID21: /* a const that contains this events ENUM ID */
    	.word 21
	eventDelay21:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime21:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags21: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress21: ;The target register we wish to bang
    	.word EVENT_21_REGISTER_ADDRESS
    eventRegisterMask21: ;The mask to apply to the target register when we bang
    	.word EVENT_21_REGISTER_MASK
    eventXGSTicks21: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp21: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp21: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty third event
	eventChannelID22: /* a const that contains this events ENUM ID */
    	.word 22
	eventDelay22:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime22:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags22: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress22: ;The target register we wish to bang
    	.word EVENT_22_REGISTER_ADDRESS
    eventRegisterMask22: ;The mask to apply to the target register when we bang
    	.word EVENT_22_REGISTER_MASK
    eventXGSTicks22: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp22: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp22: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty fourth event
	eventChannelID23: /* a const that contains this events ENUM ID */
    	.word 23
	eventDelay23:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime23:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags23: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress23: ;The target register we wish to bang
    	.word EVENT_23_REGISTER_ADDRESS
    eventRegisterMask23: ;The mask to apply to the target register when we bang
    	.word EVENT_23_REGISTER_MASK
    eventXGSTicks23: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp23: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp23: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty fifth event
	eventChannelID24: /* a const that contains this events ENUM ID */
    	.word 24
	eventDelay24:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime24:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags24: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress24: ;The target register we wish to bang
    	.word EVENT_24_REGISTER_ADDRESS
    eventRegisterMask24: ;The mask to apply to the target register when we bang
    	.word EVENT_24_REGISTER_MASK
    eventXGSTicks24: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp24: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp24: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty sixth event
	eventChannelID25: /* a const that contains this events ENUM ID */
    	.word 25
	eventDelay25:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime25:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags25: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress25: ;The target register we wish to bang
    	.word EVENT_25_REGISTER_ADDRESS
    eventRegisterMask25: ;The mask to apply to the target register when we bang
    	.word EVENT_25_REGISTER_MASK
    eventXGSTicks25: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp25: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp25: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty seventh event
	eventChannelID26: /* a const that contains this events ENUM ID */
    	.word 26
	eventDelay26:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime26:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags26: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress26: ;The target register we wish to bang
    	.word EVENT_26_REGISTER_ADDRESS
    eventRegisterMask26: ;The mask to apply to the target register when we bang
    	.word EVENT_26_REGISTER_MASK
    eventXGSTicks26: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp26: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp26: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty eighth event
	eventChannelID27: /* a const that contains this events ENUM ID */
    	.word 27
	eventDelay27:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime27:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags27: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress27: ;The target register we wish to bang
    	.word EVENT_27_REGISTER_ADDRESS
    eventRegisterMask27: ;The mask to apply to the target register when we bang
    	.word EVENT_27_REGISTER_MASK
    eventXGSTicks27: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp27: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp27: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;twenty ninth event
	eventChannelID28: /* a const that contains this events ENUM ID */
    	.word 28
	eventDelay28:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime28:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags28: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress28: ;The target register we wish to bang
    	.word EVENT_28_REGISTER_ADDRESS
    eventRegisterMask28: ;The mask to apply to the target register when we bang
    	.word EVENT_28_REGISTER_MASK
    eventXGSTicks28: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp28: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp28: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thrithty event
	eventChannelID29: /* a const that contains this events ENUM ID */
    	.word 29
	eventDelay29:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime29:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags29: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress29: ;The target register we wish to bang
    	.word EVENT_29_REGISTER_ADDRESS
    eventRegisterMask29: ;The mask to apply to the target register when we bang
    	.word EVENT_29_REGISTER_MASK
    eventXGSTicks29: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp29: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp29: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thrithty first event
	eventChannelID30: /* a const that contains this events ENUM ID */
    	.word 30
	eventDelay30:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime30:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags30: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress30: ;The target register we wish to bang
    	.word EVENT_30_REGISTER_ADDRESS
    eventRegisterMask30: ;The mask to apply to the target register when we bang
    	.word EVENT_30_REGISTER_MASK
    eventXGSTicks30: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp30: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp30: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thrithty second event
	eventChannelID31: /* a const that contains this events ENUM ID */
    	.word 31
	eventDelay31:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime31:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags31: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress31: ;The target register we wish to bang
    	.word EVENT_31_REGISTER_ADDRESS
    eventRegisterMask31: ;The mask to apply to the target register when we bang
    	.word EVENT_31_REGISTER_MASK
    eventXGSTicks31: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp31: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp31: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thrithty third event
	eventChannelID32: /* a const that contains this events ENUM ID */
    	.word 32
	eventDelay32:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime32:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags32: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress32: ;The target register we wish to bang
    	.word EVENT_32_REGISTER_ADDRESS
    eventRegisterMask32: ;The mask to apply to the target register when we bang
    	.word EVENT_32_REGISTER_MASK
    eventXGSTicks32: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp32: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp32: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thrithty fourth event
	eventChannelID33: /* a const that contains this events ENUM ID */
    	.word 33
	eventDelay33:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime33:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags33: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress33: ;The target register we wish to bang
    	.word EVENT_33_REGISTER_ADDRESS
    eventRegisterMask33: ;The mask to apply to the target register when we bang
    	.word EVENT_33_REGISTER_MASK
    eventXGSTicks33: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp33: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp33: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thrithty fifth event
	eventChannelID34: /* a const that contains this events ENUM ID */
    	.word 34
	eventDelay34:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime34:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags34: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress34: ;The target register we wish to bang
    	.word EVENT_34_REGISTER_ADDRESS
    eventRegisterMask34: ;The mask to apply to the target register when we bang
    	.word EVENT_34_REGISTER_MASK
    eventXGSTicks34: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp34: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp34: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
	;thrithty sixth event
	eventChannelID35: /* a const that contains this events ENUM ID */
    	.word 35
	eventDelay35:
		.word 0 ; high word
   		.word 0 ; low word
   	eventRuntime35:
		.word 0 ; high word
   		.word 0 ; low word
	eventFlags35: /* a var that contains possobile flags pertaining to this channel*/
    	.word 0
	eventRegisterAddress35: ;The target register we wish to bang
    	.word EVENT_35_REGISTER_ADDRESS
    eventRegisterMask35: ;The mask to apply to the target register when we bang
    	.word EVENT_35_REGISTER_MASK
    eventXGSTicks35: ;hold the amount of time that it took to schedule this event
    	.word 0
    eventDelayLatencyMetroStamp35: ;once an event is flagged as APPROACHABLE we need a metronome stamp to calc latency
    	.word 0
    eventRuntimeLatencyMetroStamp35: ;once an event is flagged as APPROACHABLE_SHUTDOWN we need a metronome stamp to calc latency
		.word 0
onEventsStructEnd:
		.word 0x5555 ;DEBUG TEST
endXGATECode:
.ascii "FreeEMS > MS ; xgate code/tools by sean keys"
end

/**
 * @endcond DOXYGEN_IGNORE_ASM
*/
