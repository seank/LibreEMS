/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2014 Sean Keys
 *
 * This file is part of the LibreEMS project.
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
 * @brief Provide global variables that exist in RAM
 *
 * TODO complete doc after cleaning up
 */


#include "inc/freeEMS.h"

/* Instantiate global vars */

unsigned long RPMWeighted;  //TODO see if I can get Dave to do this properly based on engine cycle
unsigned char portHDebounce;

Clock Clocks;
Counter Counters;
KeyUserDebug KeyUserDebugs;
Flaggable Flaggables;
Flaggable2 Flaggables2;

CoreVar* CoreVars; /** Pointer to the core running variables */
CoreVar CoreVars0; /** Core running variables */

DerivedVar* DerivedVars; /** Pointer to the derived variables */
DerivedVar DerivedVars0; /** Derived running variables */

ADCBuffer* ADCBuffers;
ADCBuffer* ADCBuffersRecord;
ADCBuffer ADCBuffers0;
ADCBuffer ADCBuffers1;

/* Large blocks */
unsigned char TXBuffer[TX_BUFFER_SIZE] TXBUF;
unsigned char RXBuffer[RX_BUFFER_SIZE] RXBUF;
Tables1 TablesA RWINDOW;
Tables2 TablesB RWINDOW;
Tables3 TablesC RWINDOW;
Tables4 TablesD RWINDOW;

/* RAM page variables */
unsigned char currentFuelRPage;
unsigned char currentTuneRPage;
unsigned char currentTimeRPage;

/* Output variables */
unsigned short masterPulseWidth;
unsigned short masterPulseWidthSecondary;
unsigned short flowDifference;

unsigned long bootFuelConst;  /* constant derived from configurable constants */
unsigned short TPSADCRange;   /* The ADC range used to generate TPS percentage */

/* State variables */
unsigned char coreStatusA;	/* Each bit represents the state of some core parameter, masks below */
