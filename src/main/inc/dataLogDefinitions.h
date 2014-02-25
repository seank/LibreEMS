/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys
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
 * @ingroup TODO
 *
 * @brief TODO
 *
 *
 */

#ifndef FILE_DATALOGDEFINITIONS_H_SEEN
#define FILE_DATALOGDEFINITIONS_H_SEEN

#define PROTOCOL_VERSION	1

typedef struct dataLogField_s {
	unsigned short startingPosition; /* Starting byte number in the log stream */
	unsigned char size;              /* Number of bits comprising the data */
	char name[10];                   /* Short name */
	char description[35];            /* Brief description */
	unsigned char multiplier;        /* Suggested multiplier */
	unsigned char adder;             /* Suggested adder */
}dataLogField;


typedef struct dataLogDefinition_s {
	unsigned short version;
	unsigned short sizeOfEntry;
	dataLogField entries[];
}dataLogDefinition;

#else
//emit warning
#endif
