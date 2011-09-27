/* FreeEMS - the open source engine management system
 *
 * Copyright 2010, 2011 Sean Keys
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
 
/* Flag Masks */ 
.equ	SW_ISR_ZERO_CFLAG_MASK, 0x0100
.equ	PIT0_CFLAG_MASK, 0x0001
.equ	PIT1_CFLAG_MASK, 0x0002
.equ	PIT2_CFLAG_MASK, 0x0004
.equ	PIT3_CFLAG_MASK, 0x0008
.equ    PIT0_DISABLE_MASK, 0xFE
.equ    PIT0_ENABLE_MASK, 0x01
.equ    PIT1_DISABLE_MASK, 0xFC
.equ    PIT1_ENABLE_MASK, 0x02

/* Registers */
.equ	SW_ISR_FLAG_REG, 0x0398 /* XGATE software interrupt register 16-bit */
.equ	TCNT,            0x0044 /* Timer counter 16 bit (0x0044 TCNT (hi), 0x0045 TCNT (lo)) */

/* PIT Counter Registers */
.equ   PITLD0, 0x0348	/* PIT Load Register 0, time to start counting from when reaching zero (0x0348 PITLD0 (hi), 0x0349 PITLD0 (lo)) */
.equ   PITLD1, 0x034C	/* PIT Load Register 1, time to start counting from when reaching zero (0x034C PITLD1 (hi), 0x034D PITLD1 (lo)) */
.equ   PITLD2, 0x0350	/* PIT Load Register 2, time to start counting from when reaching zero (0x0350 PITLD2 (hi), 0x0351 PITLD2 (lo)) */
.equ   PITLD3, 0x0354	/* PIT Load Register 3, time to start counting from when reaching zero (0x0354 PITLD3 (hi), 0x0355 PITLD3 (lo)) */
.equ   PITCNT0, 0x034A	/* PIT Count Register 0, current value of down counter (0x034A PITCNT0 (hi), 0x034B PITCNT0 (lo)) */
.equ   PITCNT1, 0x034E	/* PIT Count Register 1, current value of down counter (0x034E PITCNT1 (hi), 0x034F PITCNT1 (lo)) */
.equ   PITCNT2, 0x0352	/* PIT Count Register 2, current value of down counter (0x0352 PITCNT2 (hi), 0x0353 PITCNT2 (lo)) */
.equ   PITCNT3, 0x0356	/* PIT Count Register 3, current value of down counter (0x0356 PITCNT3 (hi), 0x0357 PITCNT3 (lo)) */

/* PIT Control Registers */
.equ	PITTF, 0x0345     /* PIT Time-Out Flag Register, low 4 bits set when each counter reaches 0 */
.equ    PITINTE, 0x0344   /* PIT Interrupt Enable Register, low four bits control the ISRs */
.equ    PITCE, 0x0342     /* PIT Channel Enable Register, low 4 bits let the channel count */
.equ    PITFLT, 0x0341    /* PIT Force Load Timer Register, low 4 bits force load timers */

/* Port registers */
.equ	PORTP, 0x0258     /* PORT-P register */
.equ	PORTBA, 0x0000	  /* PORT-BA register */
.equ	PORTB, 0x0001	  /* PORT-B register */
.equ	PORTT, 0x0240	  /* PORT-T register */

/* General Defines */
.equ	ZERO_OFFSET, 0x0000
.equ	ONE_WORD_OFFSET, 0x02 ;one word
.equ	ZERO, 0x00

/* Event structure offsets */
.equ	EVENT_CHANNEL_ID_OFFSET, 0x00
.equ	EVENT_DELAY_HIGH_WORD_OFFSET, 0x02
.equ	EVENT_DELAY_LOW_WORD_OFFSET, 0x04
.equ	EVENT_RUNTIME_HIGH_WORD_OFFSET, 0x06
.equ	EVENT_RUNTIME_LOW_WORD_OFFSET, 0x08
.equ	EVENT_STATUS_FLAGS_OFFSET, 0x0A
.equ	EVENT_BANG_REGISTER_OFFSET, 0x0C
.equ	EVENT_BANG_MASK_OFFSET, 0x0E
.equ	EVENT_XGSDELAY_OFFSET, 	0x10
.equ	EVENT_DELAY_LATENCY_METROSTAMP_OFFSET, 	0x12
.equ	EVENT_RUNTIME_LATENCY_METROSTAMP_OFFSET, 	0x14

/* Parameter offsets */
.equ	PARAMETER_CHANNEL_ID_OFFSET, 0x00
.equ	PARAMETER_DELAY_HIGHWORD_OFFSET, 0x02
.equ	PARAMETER_DELAY_LOWWORD_OFFSET, 0x04
.equ	PARAMETER_RUNTIME_HIGHWORD_OFFSET, 0x06
.equ	PARAMETER_RUNTIME_LOWWORD_OFFSET, 0x08

.equ 	XGS_EVENT_ADDRESS_OFFSET_OFFSET, 0x0A
.equ	XGS_EVENT_FLAGS_OFFSET, 0x0C

/* Parameter flags */
.equ	XGS_EVENT_FLAG_APPROACHABLE, 0x04
.equ	XGS_EVENT_FLAG_APPROACHABLE_XOR, 0xFB


/* Event Flags */
.equ	EVENT_FLAG_AVAILABLE, 0x0001 /* event is ready to be set*/
.equ	EVENT_FLAG_ARMED, 0x0002 /* event is armed */
.equ	EVENT_FLAG_ARMED_XOR, 0xFD /* event is armed */
.equ	EVENT_FLAG_APPROACHABLE,  0x0004 /* event countdown is less than 16-bits */
.equ	EVENT_FLAG_APPROACHABLE_XOR,  0xFB /* event countdown is less than 16-bits */
.equ	EVENT_FLAG_RUNNING, 0x0008 /* event is currently running(switched on) */
.equ	EVENT_FLAG_RUNNING_XOR, 0xF7 /* event is currently running(switched on) */
.equ	EVENT_FLAG_APPROACHABLE_SHUTDOWN,  0x0010 /* event countdown is less than 16-bits */
.equ	EVENT_FLAG_APPROACHABLE_SHUTDOWN_XOR,  0xEF /* event countdown is less than 16-bits */
.equ	EVENT_FLAG_NEW,	0x0020	/* Event Is Fresh, dont decrement fresh events */
.equ	EVENT_FLAG_NEW_XOR,	0x00DF	/* Event Is Fresh, dont decrement fresh events */
;.equ    EVENT_FLAG_ERROR, 0x8000 /* error tobe-determinted */

/* XGS System Vars */
.equ XGS_VARIABLE_STARTTIMESTAMP_OFFSET, 0x0
.equ XGS_VARIABLE_EVENT_ADDRESS_OFFSET, 0x02
.equ XGS_VARIABLE_NUM_OF_EVENTS_TO_SCHEDULE_OFFSET, 0x04
.equ XGS_VARIABLE_NUM_OF_EVENTS_SCHEDULED_OFFSET, 0x06
.equ XGS_VARIABLE_XGS_FLAGS_OFFSET, 0x08
.equ XGS_VARIABLE_PARAMETER_BASE_ADDRESS_OFFSET, 0x0A

/* XGS System Flags */
.equ XGS_FLAG_BUSYSCHEDULING, 0x01

/* pitTurnOn flags */
.equ	PITDELAY_IDLE, 0x00 /* Our pitOn timer is idle */
.equ	PITDELAY_COUNTING_DOWN, 0x0001 /* Our pitOn timer is counting down */

/* PIT Delay Subsystem Var Offsets */
.equ	PDS_VARIABLE_PITONLASTCOUNTDOWN_OFFSET, 0x0
.equ	PDS_VARIABLE_PITDELAYNEXTCOUNTDOWN_OFFSET, 0x02
.equ	PDS_VARIABLE_PITDELAYFLAGS_OFFSET, 0x04
.equ	PDS_VARIABLE_PITDELAYTHREADRUNS_OFFSET, 0x06
;.equ	PDS_VARIABLE_PITDELAYTHREADEXECTIME, 0x08
;.equ	PDS_VARIABLE_PITDELAYTHREADSTARTSTAMP, 0x0A
;.equ	PDS_VARIABLE_PITDELAYTHREADSTARTSTAMP, 0x0C

/* PIT Delay SubSystem Bit Masks */
.equ	PDS_FLAG_IDLE_BIT, 0x01 /* used to determin if we are counting down */
.equ	PDS_FLAG_IDLE_BIT_XOR, 0xFE /* used to determin if we are counting down */
.equ	PDS_FLAG_SELF_UPDATE_BIT, 0x02 /* used to determine if we are to update ourselv before RTS */
.equ	PDS_FLAG_SELF_UPDATE_BIT_XOR, 0xFD

/* pitTurnOff flags */
;.equ	PITRUNTIME_IDLE, 0x00 /* Our pitOn timer is idle */
;.equ	PITRUNTIME_COUNTING_DOWN, 0x0001 /* Our pitOn timer is counting down */

/* PIT Runtime Subsystem Var Offsets */
.equ	PRS_VARIABLE_LASTCOUNTDOWN_OFFSET, 0x0
.equ	PRS_VARIABLE_NEXTCOUNTDOWN_OFFSET, 0x02
.equ	PRS_VARIABLE_FLAGS_OFFSET, 0x04

/* PIT Runtime SubSystem Bit Masks */
.equ	PRS_FLAG_IDLE_BIT, 0x01 /* used to determin if we are counting down */
.equ	PRS_FLAG_IDLE_BIT_XOR, 0xFE /* used to determin if we are counting down */
.equ	PRS_FLAG_SELF_UPDATE_BIT, 0x02 /* used to determine if we are to update ourselv before RTS */
.equ	PRS_FLAG_SELF_UPDATE_BIT_XOR, 0xFD

/* Bit Masks */
.equ	PORTBA_PROTECTION_MASK, 0x0FFF ;only use the first 12 pins on PORTBA 

/* Boolean Flags */
.equ TRUE, 0x01
.equ FALSE, 0x00

/*-----------------------------------------CONFIG-------------------------------------------/* 
 	/* Constants */
.equ NUMBER_OF_EVENT_STRUCTURES, 0x08 /* TOOD MAKE A VAR */

.equ EVENT_STRUCTURE_SIZE, 0x16 /* 16 bytes or 8 words TODO MAKE A VAR */

.equ MAX_NUMBER_OF_PARAMETERS, 0x06 /* 12 parameters can be loaded by the scheduler in one shot */

.equ PARAMETER_SET_SIZE, 0x0C /* 12 bytes or 6 words */

 	/* Event Bit Bang Registers and Masks */ 
.equ EVENT_0_REGISTER_ADDRESS, PORTB
.equ EVENT_0_REGISTER_MASK, 0x0001 /* first pin on port B */

.equ EVENT_1_REGISTER_ADDRESS, PORTB
.equ EVENT_1_REGISTER_MASK, 0x0002 /* second pin on port B */

.equ EVENT_2_REGISTER_ADDRESS, PORTB
.equ EVENT_2_REGISTER_MASK, 0x0004 /* third pin on port B */

.equ EVENT_3_REGISTER_ADDRESS, PORTB
.equ EVENT_3_REGISTER_MASK, 0x0008 /* fourth pin on port B */

.equ EVENT_4_REGISTER_ADDRESS, PORTB
.equ EVENT_4_REGISTER_MASK, 0x0010 /* fifth pin on port B */

.equ EVENT_5_REGISTER_ADDRESS, PORTB
.equ EVENT_5_REGISTER_MASK, 0x0020 /* sixth pin on port B */

.equ EVENT_6_REGISTER_ADDRESS, PORTB
.equ EVENT_6_REGISTER_MASK, 0x0040 /* seventh pin on port B */

.equ EVENT_7_REGISTER_ADDRESS, PORTB
.equ EVENT_7_REGISTER_MASK, 0x0080 /* eighth pin on port B */

;.equ LATENCY_ZEROTIME_TOLLERANCE, 10 /* the number of ticks greater than zero to still be considered zero */

/*-----------------------------------------END CONFIG----------------------------------------*/ 

