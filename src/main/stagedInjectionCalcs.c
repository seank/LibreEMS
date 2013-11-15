/* LibreEMS - the community driven open source engine management system
 *
 * Copyright 2008-2013 Sean Keys
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
 * @ingroup measurementsAndCalculations
 *
 * @brief Staged injection calculations.
 *
 * This file contains code to calculate the behavior of staged injection.
 */


#define STAGEDINJECTIONCALCS_C
#include "inc/freeEMS.h"
#include "inc/utils.h"
#include "inc/stagedInjectionCalcs.h"

#define DEGRESS_PERCYCLE_FACTOR		72UL	/* 720/10 */
#define FLOW_SCALE_FACTOR			10000UL /* .01 % */

/**
 * This will split your fuel pulsewidth when you have exceeded your primary duty-cycle limit.
 * Your new primary PW will be equal to the max duty-cycle allowed. The remaining fuel that needs
 * to be delivered will be spread to the secondary injector. The flow difference between the two
 * injectors will be compensated for. If you exceed your secondary duty-cycle limit no fuel will
 * be scheduled for either injector.
 *
 * The accuracy/precision that is achieved via this code is acceptable(~1%), but it could be
 * improved.
 */
void splitFuelPulseWidth(unsigned short dutyCycle){

	unsigned long ticksPerEngineCycle = (CoreVars->DRPM * DEGRESS_PERCYCLE_FACTOR);
	masterPulseWidth = (unsigned short) ((ticksPerEngineCycle * fixedConfigs1.engineSettings.maxPrimaryDC) / FLOW_SCALE_FACTOR);
	unsigned long secondaryPw;
	secondaryPw = DerivedVars->EffectivePW - safeSubtract(masterPulseWidth, DerivedVars->IDT); /* How much of our max duty-cycle will not deliver fuel */
	/* factor in any injector size difference */
	//TODO check to see if flowDifference needs to be a reciprocal
	masterPulseWidthSecondary = (unsigned short) ((secondaryPw * flowDifference) / FLOW_SCALE_FACTOR);
	masterPulseWidthSecondary += DerivedVars->IDT; //FIXME we need a second lookup, incase the characteristics are different from our first set of injectors
}
