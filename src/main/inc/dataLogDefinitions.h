/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys, David J. Andruczyk
 *
 * This file is part of the LibreEMS project.
 *
 * LibreEMS software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LibreEMS software is distributed in the hope that it will be useful,
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
 * @ingroup TODO
 *
 * @brief TODO
 *
 *
 */

#ifndef FILE_DATALOGDEFINITIONS_H_SEEN
#define FILE_DATALOGDEFINITIONS_H_SEEN

#define PROTOCOL_VERSION	1


typedef struct {
	unsigned short start;     /* Starting byte number in the log stream */
	unsigned char size;       /* Number of bits comprising the data */
	unsigned char is_signed;  /* Signed or unsigned data */
	char *name;               /* Short name */
	char *description;        /* Brief description */
	char *multiplier;         /* Suggested multiplier */
	char *adder;              /* Suggested adder */
	char *transfer_function;  /* Alternative to multiplier/adder */
	char *flags;              /* Textual flags to give the tuner hints */
	char *suffix;             /* Tuner side suffix for this variable */
}dataBlockDescriptor;

extern const dataBlockDescriptor coreVarsDescriptor[];
extern const dataBlockDescriptor derrivedVarsDescriptor[];
extern const dataBlockDescriptor KeyUserVarsDescriptor[];

#else
//emit warning
#endif
