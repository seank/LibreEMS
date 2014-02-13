/* FreeEMS - the open source engine management system
 *
 * Copyright 2008-2013 Fred Cooke
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
 * @ingroup allHeaders
 * @ingroup globalHeaders
 *
 * @brief Global constant declarations
 *
 * All global constants are declared in this file for use throughout the program.
 */


/* Header file multiple inclusion protection courtesy eclipse Header Template */
/* and http://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/ C pre processor manual    */
#ifndef FILE_GLOBAL_CONSTANTS_H_SEEN
#define FILE_GLOBAL_CONSTANTS_H_SEEN


#define INTERFACE_VERSION  "IFreeEMS Vanilla 0.0.0" // Migrate to come from the makefile where it can be extracted from any file with any tool.

// Sizes for array must be outside ifndef block
#define INTERFACE_VERSION_LENGTH   sizeof(INTERFACE_VERSION)
#define FIRMWARE_VERSION_LENGTH    sizeof(FIRMWARE_VERSION) + sizeof(BUILD_CONFIG) + 1
#define FIRMWARE_BUILD_DATE_LENGTH sizeof(FIRMWARE_BUILD_DATE)
#define COMPILER_VERSION_LENGTH    sizeof(__VERSION__)
#define OPERATING_SYSTEM_LENGTH    sizeof(OPERATING_SYSTEM)


/* end of the wrapper ifdef from the very top */
#endif
