/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys
 *
 * This file is part of the FreeEMS project.
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
 * @ingroup allHeaders
 */


#ifndef FILE_LIBRE_PACKET_TYPES_H_SEEN
#define FILE_LIBRE_PACKET_TYPES_H_SEEN


/*&&&&&&&&&&&&&&&&&&&&&&&&  ECU Payload Type IDs  &&&&&&&&&&&&&&&&&&&&&&&*/

/* Team LibreEMS */
#define RETRIEVE_DATALOG_DESCRIPTOR    0x0300  /* Request data-log stream descriptor */
#define REQUEST_SET_DATASTREAM_ID      0x0302  /* Set the desired data-stream id */
#define REQUEST_DEFINE_DATASTREAM_ID   0x0304  /* Send the ID and list of variables you want to receive */
#define REQUEST_LIST_OF_DATASTREAM_IDS 0x0306  /* Send valid list of saved data-stream IDs */


/*&&&&&&&&&&&&&&&&&&&&&&& Misc Defines &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#define DESCRIPTOR_JSON		0x10
#define DESCRIPTOR_YAML		0x11

#define PROCESSING_MULTI_PACKET_PAYLOAD  0x01

extern unsigned char  commsCoreStateFlags;


#else
	#warning "Header file LIBRE_PACKET_TYPES_H seen before, sort it out!"
#endif
