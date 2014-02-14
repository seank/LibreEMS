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
 * @ingroup measurementsAndCalculations
 *
 * @brief Fuel and ignition calculations.
 *
 * This file contains all of the main fuel and ignition calculations based
 * upon the variables that we have already determined in previous stages.
 */


#define FUELANDIGNITIONCALCS_C
#include "inc/freeEMS.h"
#include "inc/utils.h"
#include "inc/locationIDs.h"
#include "inc/tableLookup.h"
#include "inc/fuelAndIgnitionCalcs.h"
#include "inc/stagedInjectionCalcs.h"


/**
 * Final fuel and ignition calculations. Using a variety of primary algorithms
 * calculate a base pulsewidth and then apply various corrections to it such as
 * injector dead time, transient fuel correction, engine temperature enrichment
 * and per cylinder trims. The ignition timing and fuel injection timing are
 * also determined here, as are the various limiters and cuts.
 */
void calculateFuelAndIgnition(){
	unsigned short airInletTemp = CoreVars->IAT; /* All except MAF use this. */
	extern KeyUserDebug KeyUserDebugs;
	extern const volatile fixedConfig1 fixedConfigs1;                                                                                                
	extern const volatile fixedConfig2 fixedConfigs2;
	/* Determine the type of air flow data */
	if(!(fixedConfigs2.algorithmSettings.algorithmType)){
		/* Look up VE with RPM and MAP */
		DerivedVars->VEMain = lookupMainTable(CoreVars->RPM, CoreVars->MAP, VE_TABLE_MAIN_LOCATION_ID);
		/* This won't overflow until 512kPa or about 60psi of boost with 128% VE. */
		DerivedVars->AirFlow = ((unsigned long)CoreVars->MAP * DerivedVars->VEMain) / VE(100);
		/* Result is 450 - 65535 always. */
	}else if(fixedConfigs2.algorithmSettings.algorithmType == ALGO_ALPHA_N){
		/* Look up Airflow with RPM and TPS */
		DerivedVars->AirFlow = lookupMainTable(CoreVars->RPM, CoreVars->TPS, AIRFLOW_TABLE_LOCATION_ID); /* Tuned air flow without density information */
	}else if(fixedConfigs2.algorithmSettings.algorithmType == ALGO_MAF){
		//DerivedVars->AirFlow = CoreVars->MAF; /* Just fix temperature at appropriate level to provide correct Lambda */
		/// @todo TODO figure out what the correct "temperature" is to make MAF work correctly!
		DerivedVars->AirFlow = KeyUserDebugs.zsp9;
		//KeyUserDebugs.zsp7 = ((unsigned long)CoreVars->MAP * DerivedVars->VEMain) / VE(100);
		// I suspect more than one value could have been used to calibrate the MAF. The most common seem to be 0c and 20c
		airInletTemp = DEGREES_C(20); // Room temperature?
		/* Look up VE with RPM and MAP, kept here for logging */
		//DerivedVars->VEMain = lookupMainTable(CoreVars->RPM, CoreVars->MAP, VETableMainLocationID);
	}else if(fixedConfigs2.algorithmSettings.algorithmType == ALGO_SD_AN_BLEND){
		/* Look up VE with RPM and MAP */
		DerivedVars->VEMain = lookupMainTable(CoreVars->RPM, CoreVars->MAP, VE_TABLE_MAIN_LOCATION_ID);
		/* This won't overflow until 512kPa or about 60psi of boost with 128% VE. */
		KeyUserDebugs.speedDensityAirFlow = ((unsigned long)CoreVars->MAP * DerivedVars->VEMain) / VE(100);

		/* Look up Airflow with RPM and TPS */
		KeyUserDebugs.alphaNAirFlow = lookupMainTable(CoreVars->RPM, CoreVars->TPS, AIRFLOW_TABLE_LOCATION_ID); /* Tuned air flow without density information */

		KeyUserDebugs.blendAlphaNPercent = lookupTwoDTableUS((twoDTableUS*)&TablesA.SmallTablesA.blendVersusRPMTable, CoreVars->RPM);

		unsigned short airflowSD = safeScale(KeyUserDebugs.speedDensityAirFlow, SHORTMAX - KeyUserDebugs.blendAlphaNPercent, SHORTMAX);
		unsigned short airflowAN = safeScale(KeyUserDebugs.alphaNAirFlow, KeyUserDebugs.blendAlphaNPercent, SHORTMAX);

		DerivedVars->AirFlow = safeAdd(airflowSD, airflowAN);
	}else{ /* Default to no fuel delivery and error */
		DerivedVars->AirFlow = 0;
	}

	/* This won't overflow until well past 125C inlet, 1.5 Lambda and fuel as dense as water */
	DerivedVars->densityAndFuel = (((unsigned long)((unsigned long)airInletTemp * DerivedVars->Lambda) / LAMBDA(1.0)) * fixedConfigs1.engineSettings.densityOfFuelAtSTP) / FUEL_DENSITY(FUEL_DENSITY_UNIT_FACTOR);
	/* Result is 7500 - 60000 always. TODO clean up the last item on the above line */

	/* Divisors for air inlet temp and pressure :
	 * #define AIR_INLET_TEMP_DIVISOR 100
	 * #define AIR_PRESSURE_DIVISOR 100
	 * cancel each other out! all others are used. */

	DerivedVars->BasePW = (bootFuelConst * DerivedVars->AirFlow) / DerivedVars->densityAndFuel;

	/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/




	/*&&&&&&&&&&&&&&&&&&&&&&&&&&&& Apply All Corrections PCFC, ETE, IDT, TFC etc &&&&&&&&&&&&&&&&&&&&&&&&&&&*/

	/* Apply the corrections after calculating */
	DerivedVars->EffectivePW = safeTrim(DerivedVars->BasePW, DerivedVars->TFCTotal);
	DerivedVars->EffectivePW = safeScale(DerivedVars->EffectivePW, DerivedVars->ETE, SHORT4TH);


//	unsigned char channel; // the declaration of this variable is used in multiple loops below.
//
//	/* "Calculate" the individual fuel pulse widths */
//	for(channel = 0; channel < INJECTION_CHANNELS; channel++){ /// @todo TODO make injector channels come from config, not defines.
//		/* Add or subtract the per cylinder fuel trims */
//		unsigned short channelPW;
//		channelPW = safeScale(DerivedVars->EffectivePW, TablesB.SmallTablesB.perCylinderFuelTrims[channel]);
//
//		/* Add on the IDT to get the final value and put it into the array */
//		//outputEventPulseWidthsMath[channel] = safeAdd(channelPW, DerivedVars->IDT); do not re-enable this without fixing it properly...
//	}

	// Make sure we don't have a PW if PW is supposed to be zero, ie, zero the IDT as well.
	if(!(DerivedVars->EffectivePW)){
		DerivedVars->IDT = 0; // This also makes fuel and electrical duty work consistently in external apps.
	}

	/* Reference PW for comparisons etc */
	DerivedVars->RefPW = safeAdd(DerivedVars->EffectivePW, DerivedVars->IDT);
	/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

/// @todo accumulate errors such that we know what sort of PW WOULD have been requested and enable a "over duty cut" to protect boosted users with insufficient injector size on cold nights

/// TODO @todo FIXME part of to schedule or not to schedule should be : (masterPulseWidth > injectorMinimumPulseWidth)
// IE, NOT in the decoders... KISS in the decoders. This is a hangover from (very) early decoder dev

//	for(channel = 0;channel < INJECTION_CHANNELS;channel++){ /// @todo TODO make injector channels come from config, not defines.
		//injectorMainAdvances[channel] = IDT blah blah.
//	}

	/* "Calculate" the nominal total pulse width before per channel corrections */
	masterPulseWidth = safeAdd((DerivedVars->EffectivePW / fixedConfigs1.schedulingSettings.numberOfInjectionsPerEngineCycle), DerivedVars->IDT); // div by number of injections per cycle, configured above
	// but requires to know how big a cycle is, 1/4 1, 1/2, etc

#define RECIPROCAL_TICK_VALUE			125UL /* reciprocal of .8 x 10 */
	/* TODO think about calculating DC with ticksperdegree instead */
	unsigned short dutyCycle = ((CoreVars->RPM / 2) * (masterPulseWidth * 5UL) / RECIPROCAL_TICK_VALUE) / 600UL *
							   fixedConfigs1.schedulingSettings.numberOfInjectionsPerEngineCycle;
	KeyUserDebugs.zsp6 = dutyCycle; //TODO add this to the log stream, its been years and still no *official DC in the log, pathetic
	/* Check Duty Cycle */
	if(dutyCycle > fixedConfigs1.engineSettings.maxPrimaryDC && CoreVars->RPM > (1000 * 2)){ //TODO remove RPM limit once math is *safe
		if(fixedConfigs1.engineSettings.injectionStrategy == STAGED_EXTENSION){
			splitFuelPulseWidth(dutyCycle);
			unsigned short dutyCycleSecondary = ((CoreVars->RPM / 2) * (masterPulseWidthSecondary * 5UL) / RECIPROCAL_TICK_VALUE) / 600UL *
										   	    fixedConfigs1.schedulingSettings.numberOfInjectionsPerEngineCycle;
			KeyUserDebugs.zsp7 = dutyCycleSecondary; //TODO add this to the log stream, its been years and still no *official DC in the log, pathetic
			if(dutyCycleSecondary > fixedConfigs1.engineSettings.maxSecondaryDC){
				/* cut fuel all together */
				masterPulseWidth = 0;
				masterPulseWidthSecondary = 0;
			}
		}else{ /* cut fuel all together */
			masterPulseWidth = 0;
			masterPulseWidthSecondary = 0;
		}
	}else {
		masterPulseWidthSecondary = 0;
		KeyUserDebugs.zsp7 = 0; //TODO add this to the log stream, its been years and still no *official DC in the log, pathetic
	}

//	//Test calcs for staged calcs TODO this is why much of this code-base needs to be refactored its hard to test pieces
//#define TIME_DIVISOR	10UL //12000 rpm
//#define TICKS_PER_DEGREE_MULTIPLIER (10 * ANGLE_FACTOR) // FIX <<< shouldn't be done like this.
//extern unsigned short *ticksPerDegree;
//*ticksPerDegree = (unsigned short)((ticks_per_degree_multiplier * (1250UL * TIME_DIVISOR) ) / (720UL * ANGLE_FACTOR));
//extern	void generateCoreVars();
//extern  void generateDerivedVars();
//	masterPulseWidth =  *ticksPerDegree;
//	masterPulseWidth = 12500; // 100%dc
//	generateCoreVars();
//	generateDerivedVars();
//	DerivedVars->IDT = 2500;
//	DerivedVars->EffectivePW = masterPulseWidth - DerivedVars->IDT;
//	//CoreVars->DRPM = 20000;
//	//CoreVars->RPM = 20000;
//
//	dutyCycle = ((CoreVars->RPM / 2) * (masterPulseWidth * 5UL) / RECIPROCAL_TICK_VALUE) / 600UL *
//								   fixedConfigs1.schedulingSettings.numberOfInjectionsPerEngineCycle;
//	splitFuelPulseWidth(dutyCycle);
//
//	KeyUserDebugs.zsp6 = masterPulseWidth;   //Yields 10587
//	KeyUserDebugs.zsp7 = masterPulseWidthSecondary; //Yields 6034
//	KeyUserDebugs.zsp8 = dutyCycle; //Yields 10035
//	KeyUserDebugs.zsp9 = flowDifference; //Yields 18478
////End Test Calcs

	// Note, conversions to address and then pointer are necessary to avoid error on direct cast
	// Cuts and limiters TODO move these to their own special place?
	// TODO Make source of threshold either struct or temp based curve for these

	if(fixedConfigs1.cutAndLimiterSettings.cutsEnabled.IgnitionRPM){
		unsigned short confirmedReenableThreshold = fixedConfigs1.cutAndLimiterSettings.IgnitionRPM.reenableThreshold;
		if(confirmedReenableThreshold >= fixedConfigs1.cutAndLimiterSettings.IgnitionRPM.disableThreshold){
			confirmedReenableThreshold = fixedConfigs1.cutAndLimiterSettings.IgnitionRPM.disableThreshold / 2;
		}
		if(CoreVars->RPM > fixedConfigs1.cutAndLimiterSettings.IgnitionRPM.disableThreshold){
			((ignitionCutFlags *)&KeyUserDebugs.ignitionCuts)->IgnitionRPM = 1;
		}else if(CoreVars->RPM < confirmedReenableThreshold){
			((ignitionCutFlags *)&KeyUserDebugs.ignitionCuts)->IgnitionRPM = 0;
		}
	}

	if(fixedConfigs1.cutAndLimiterSettings.cutsEnabled.InjectionRPM){
		unsigned short confirmedReenableThreshold = fixedConfigs1.cutAndLimiterSettings.InjectionRPM.reenableThreshold;
		if(confirmedReenableThreshold >= fixedConfigs1.cutAndLimiterSettings.InjectionRPM.disableThreshold){
			confirmedReenableThreshold = fixedConfigs1.cutAndLimiterSettings.InjectionRPM.disableThreshold / 2;
		}
		if(CoreVars->RPM > fixedConfigs1.cutAndLimiterSettings.InjectionRPM.disableThreshold){
			((injectionCutFlags *)&KeyUserDebugs.injectionCuts)->InjectionRPM = 1;
		}else if(CoreVars->RPM < confirmedReenableThreshold){
			((injectionCutFlags *)&KeyUserDebugs.injectionCuts)->InjectionRPM = 0;
		}
	}

	// TODO add time based lock out as well as threshold based as threshold could re-enable too quickly
	if(fixedConfigs1.cutAndLimiterSettings.cutsEnabled.InjOverBoost || fixedConfigs1.cutAndLimiterSettings.cutsEnabled.IgnOverBoost){
		unsigned short confirmedReenableThreshold = fixedConfigs1.cutAndLimiterSettings.OverBoost.reenableThreshold;
		if(confirmedReenableThreshold >= fixedConfigs1.cutAndLimiterSettings.OverBoost.disableThreshold){
			confirmedReenableThreshold = fixedConfigs1.cutAndLimiterSettings.OverBoost.disableThreshold / 2;
		}
		if(CoreVars->MAP > fixedConfigs1.cutAndLimiterSettings.OverBoost.disableThreshold){
			((injectionCutFlags *)&KeyUserDebugs.injectionCuts)->InjOverBoost = fixedConfigs1.cutAndLimiterSettings.cutsEnabled.InjOverBoost;
			((ignitionCutFlags *)&KeyUserDebugs.ignitionCuts)->IgnOverBoost = fixedConfigs1.cutAndLimiterSettings.cutsEnabled.IgnOverBoost;
		}else if(CoreVars->MAP < confirmedReenableThreshold){
			((injectionCutFlags *)&KeyUserDebugs.injectionCuts)->InjOverBoost = 0;
			((ignitionCutFlags *)&KeyUserDebugs.ignitionCuts)->IgnOverBoost = 0;
		}
	}
}
