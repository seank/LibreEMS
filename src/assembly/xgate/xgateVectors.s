/* FreeEMS - the open source engine management system
 *
 * Copyright 2012 Sean Keys
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


;exports for our vector table
	 extern	xgateSchedule
	 extern	xgateDelayCounter
	 extern	xgatePITTurnOff
	 extern	xgatePITTurnOn

	.globl	xgateIntVectorTable
	.section	.ppageE0X,"a",@progbits
	.type	xgateIntVectorTable, @object
	.size	xgateIntVectorTable, 484
xgateIntVectorTable:  ;fred let me know if you want me to label each channel
	.word	0
	.word	0
	.word	0
	.word	1
	.word	0
	.word	2
	.word	0
	.word	3
	.word	0
	.word	4
	.word	0
	.word	5
	.word	0
	.word	6
	.word	0
	.word	7
	.word	0
	.word	8
	.word	0
	.word	9
	.word	0
	.word	10
	.word	0
	.word	11
	.word	0
	.word	12
	.word	0
	.word	13
	.word	0
	.word	14
	.word	0
	.word	15
	.word	0
	.word	16
	.word	0
	.word	17
	.word	0
	.word	18
	.word	0
	.word	19
	.word	0
	.word	20
	.word	0
	.word	21
	.word	0
	.word	22
	.word	0
	.word	23
	.word	0
	.word	24
	.word	0
	.word	25
	.word	0
	.word	26
	.word	0
	.word	27
	.word	0
	.word	28
	.word	0
	.word	29
	.word	0
	.word	30
	.word	0
	.word	31
	.word	0
	.word	32
	.word	0
	.word	33
	.word	0
	.word	34
	.word	0
	.word	35
	.word	0
	.word	36
	.word	0
	.word	37
	.word	0
	.word	38
	.word	0
	.word	39
	.word	0
	.word	40
	.word	0
	.word	41
	.word	0
	.word	42
	.word	0
	.word	43
	.word	0
	.word	44
	.word	0
	.word	45
	.word	0
	.word	46
	.word	0
	.word	47
	.word	0
	.word	48
	.word	0
	.word	49
	.word	0
	.word	50
	.word	0
	.word	51
	.word	0
	.word	52
	.word	0
	.word	53
	.word	0
	.word	54
	.word	0
	.word	55
	.word	0
	.word	56
	.word	xgateSchedule
	.word	600
	.word	0
	.word	58
	.word	xgateDelayCounter
	.word	59
	.word	xgatePITTurnOff
	.word	60
	.word	xgatePITTurnOn
	.word	61
	.word	0
	.word	62
	.word	0
	.word	63
	.word	0
	.word	64
	.word	0
	.word	65
	.word	0
	.word	66
	.word	0
	.word	67
	.word	0
	.word	68
	.word	0
	.word	69
	.word	0
	.word	70
	.word	0
	.word	71
	.word	0
	.word	72
	.word	0
	.word	73
	.word	0
	.word	74
	.word	0
	.word	75
	.word	0
	.word	76
	.word	0
	.word	77
	.word	0
	.word	78
	.word	0
	.word	79
	.word	0
	.word	80
	.word	0
	.word	81
	.word	0
	.word	82
	.word	0
	.word	83
	.word	0
	.word	84
	.word	0
	.word	85
	.word	0
	.word	86
	.word	0
	.word	87
	.word	0
	.word	88
	.word	0
	.word	89
	.word	0
	.word	90
	.word	0
	.word	91
	.word	0
	.word	92
	.word	0
	.word	93
	.word	0
	.word	94
	.word	0
	.word	95
	.word	0
	.word	96
	.word	0
	.word	97
	.word	0
	.word	98
	.word	0
	.word	99
	.word	0
	.word	100
	.word	0
	.word	101
	.word	0
	.word	102
	.word	0
	.word	103
	.word	0
	.word	104
	.word	0
	.word	105
	.word	0
	.word	106
	.word	0
	.word	107
	.word	0
	.word	108
	.word	0
	.word	109
	.word	0
	.word	110
	.word	0
	.word	111
	.word	0
	.word	112
	.word	0
	.word	113
	.word	0
	.word	114
	.word	0
	.word	115
	.word	0
	.word	116
	.word	0
	.word	117
	.word	0
	.word	118
	.word	0
	.word	119
	.word	0
	.word	120
