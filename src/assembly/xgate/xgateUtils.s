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

/* @defgroup xgateUtilityFunctions */
.sect .ppageE1

include "assembly/xgate/xgate.inc"
.global xgatePORTPFlip
xgatePORTPFlip: ;Flip bit 7 at PORTP
	LDD R2, #PORTP ;load port-p address
	LDB R1, R2, #ZERO_OFFSET ;load data(1 byte) at port-p address to R1
	COM R3, R1 ;flip the bits
	ANDL R1, #0x7F
	ANDL R3, #0x80
	OR R1, R3, R1
	STB R1, R2, #0x00 ;write the byte to port-p address
	JAL R5 ;return
