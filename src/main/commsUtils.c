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

#include "inc/commsUtils.h"


#define NUM_CHARS_IN_INT32	(10 + 2)  //Add potential sign and NULL terminator
#define BASE				10

/* TODO dox entries */
int stringLength(char *str) {
	int c = 0;
	while (*(str + c) != '\0') c++;
	return c;
}

void reverse(char *str) {
	unsigned short length, c;
	char *begin, *end, temp;

	length = stringLength(str);

	begin = str;
	end = str;

	for (c = 0; c < (length - 1); c++) end++;

	for (c = 0; c < length / 2; c++) {
		temp = *end;
		*end = *begin;
		*begin = temp;

		begin++;
		end--;
	}
}

char *integerToString(unsigned short num, char *str){ //TODO maybe add other radix's

	char isNegative = FALSE;
	char *firstChar = str;

	/* Handle 0 */
	if (num == 0) {
		*(str)++ = '0';
		*str = '\0';
		return str;
	}

	if (num < 0 && BASE == 10) {
		isNegative = TRUE;
		num = -num;
	}
	// Process digits
	while (num != 0) {
		int rem = num % BASE;
		*(str)++ = (rem > 9)? (rem-10) + 'a' : rem + '0';
		num = num/BASE;
	}
	if (isNegative) {
		*(str)++ = '-';
	}

	*str = '\0'; //terminator

	// Reverse the string
	reverse(firstChar);

	return firstChar;
}


// Move as much data as we can to the TXBuffer without over-running it
char *payloadStrCat(char *dest, const char *src) {
	if (dest == 0) {
		return 0;
	}
	while(*src != 0){
		/* if we are not over-running our buffer copy data */
		if((unsigned short)dest - (unsigned short)(&(TXBuffer[0])) < TX_PAYLOAD_SIZE){
		  *(dest++) = *(src++);
		} else{
			return 0;
		}
	}
	return dest;
}


char *addJSONHeader(char *TXBufferPostion) {
    if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "{\"id\":\"0001\",\"name\":\"JSON DataLog Descriptor\":{\"descriptor\":["))) return 0;

    return TXBufferPostion;
}


char *addJSONFooter(char *TXBufferPostion) {
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "]}"))) return 0;

    return TXBufferPostion;
}


char *addYAMLHeader(char *TXBufferPostion) {
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "---\nloggables:\n"))) return 0;

    return TXBufferPostion;
}


char *addYAMLFooter(char *TXBufferPostion) {
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "...\n"))) return 0;

	return TXBufferPostion;
}


char *addJSONRecord(char *TXBufferPostion, const dataBlockDescriptor *descriptorPTR, unsigned short baseOffset) {
	char str[NUM_CHARS_IN_INT32] = {0};
	const char *comma_quote = ",\"";
	const char *quote_colon = "\":";

	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "{\"start"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, integerToString(descriptorPTR->start + baseOffset, str)))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "size"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, integerToString(descriptorPTR->size, str)))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "is_signed"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, integerToString(descriptorPTR->is_signed, str)))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "name"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->name))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "description"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->description))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "multiplier"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->multiplier))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "adder"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->adder))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "transfer_function"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->transfer_function))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "flags"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->flags))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, comma_quote))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "suffix"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, quote_colon))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->suffix))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "}"))) return 0;

	return TXBufferPostion;
}


char *addYAMLRecord(char *TXBufferPostion, const dataBlockDescriptor *descriptorPTR, unsigned short baseOffset) {
	char str[NUM_CHARS_IN_INT32] = {0};
	const char *newline_4spaces = "\n    ";

	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "  - start:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, integerToString(descriptorPTR->start + baseOffset, str)))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "size:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, integerToString(descriptorPTR->size, str)))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "signed:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, integerToString(descriptorPTR->is_signed, str)))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "name:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->name))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "description:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->description))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "multiplier:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->multiplier))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "adder:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->adder))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "transfer_function:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->transfer_function))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "flags:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->flags))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, newline_4spaces))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "suffix:"))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, descriptorPTR->suffix))) return 0;
	if(!(TXBufferPostion = payloadStrCat(TXBufferPostion, "\n"))) return 0;

	return TXBufferPostion;
}


