/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys, David Andruczyk
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
 * @ingroup communicationsFiles
 *
 * @brief Utilities to facilitate communications.
 *
 *
 */

#include "freeEMS.h"

int stringLength(char *str) PPAGE_E5;
void reverse(char *str) PPAGE_E5;
char *integerToString(signed long num, char *str) PPAGE_E5;
char *payloadStrCat(char *, const char *) PPAGE_E5;
char *addJSONHeader(char *) PPAGE_E5;
char *addJSONFooter(char *) PPAGE_E5;
char *addJSONRecord(char *TXBufferPostion, const dataBlockDescriptor *descriptorPTR, unsigned short baseOffset) PPAGE_E5; //FIXME revise linker etc for use with far attribute or keep as an alternative example...
char *addYAMLHeader(char *) PPAGE_E5;
char *addYAMLFooter(char *) PPAGE_E5;
char *addYAMLRecord(char *TXBufferPostion, const dataBlockDescriptor *descriptorPTR, unsigned short baseOffset) PPAGE_E5; //FIXME revise linker etc for use with far attribute or keep as an alternative example...
