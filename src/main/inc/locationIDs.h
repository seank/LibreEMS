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
 */


/* Header file multiple inclusion protection courtesy eclipse Header Template */
/* and http://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/ C pre processor manual    */
#ifndef FILE_LOCATION_IDS_H_SEEN
#define FILE_LOCATION_IDS_H_SEEN


/* Big main tables */
/* Fueling tables */
#define VE_TABLE_MAIN_LOCATION_ID                          0x0000
#define VE_TABLE_MAIN2_LOCATION_ID                         0x0001
#define VE_TABLE_SECONDARY_LOCATION_ID                     0x0002
#define VE_TABLE_SECONDARY2_LOCATION_ID                    0x0003
#define AIRFLOW_TABLE_LOCATION_ID                         0x0004
#define AIRFLOW_TABLE2_LOCATION_ID                        0x0005
#define LAMBDA_TABLE_LOCATION_ID                          0x0006
#define LAMBDA_TABLE2_LOCATION_ID                         0x0007
/* Timing tables */
#define IGNITION_ADVANCE_TABLE_MAIN_LOCATION_ID             0x0008
#define IGNITION_ADVANCE_TABLE_MAIN2_LOCATION_ID            0x0009
#define IGNITION_ADVANCE_TABLE_SECONDARY_LOCATION_ID        0x000A
#define IGNITION_ADVANCE_TABLE_SECONDARY2_LOCATION_ID       0x000B
#define INJECTION_ADVANCE_TABLE_MAIN_LOCATION_ID            0x000C
#define INJECTION_ADVANCE_TABLE_MAIN2_LOCATION_ID           0x000D
#define INJECTION_ADVANCE_TABLE_SECONDARY_LOCATION_ID       0x000E
#define INJECTION_ADVANCE_TABLE_SECONDARY2_LOCATION_ID      0x000F

#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define MAIN_TABLE_TWO_D_TABLE_US_BORDER                   0x0100
#endif

/* twoDTableUS tables (TablesA) */
#define DWELL_DESIRED_VERSUS_VOLTAGE_TABLE_LOCATION_ID       0x0100
#define DWELL_DESIRED_VERSUS_VOLTAGE_TABLE2_LOCATION_ID      0x0101
#define INJECTOR_DEAD_TIME_TABLE_LOCATION_ID                0x0102
#define INJECTOR_DEAD_TIME_TABLE2_LOCATION_ID               0x0103
#define POST_START_ENRICHMENT_TABLE_LOCATION_ID             0x0104
#define POST_START_ENRICHMENT_TABLE2_LOCATION_ID            0x0105
#define ENGINE_TEMP_ENRICHMENT_TABLE_FIXED_LOCATION_ID       0x0106
#define ENGINE_TEMP_ENRICHMENT_TABLE_FIXED2_LOCATION_ID      0x0107
#define PRIMING_VOLUME_TABLE_LOCATION_ID                   0x0108
#define PRIMING_VOLUME_TABLE2_LOCATION_ID                  0x0109
#define ENGINE_TEMP_ENRICHMENT_TABLE_PERCENT_LOCATION_ID     0x010A
#define ENGINE_TEMP_ENRICHMENT_TABLE_PERCENT2_LOCATION_ID    0x010B
#define DWELL_VERSUS_RPM_TABLE_LOCATION_ID                  0x010C
#define DWELL_VERSUS_RPM_TABLE2_LOCATION_ID                 0x010D
#define BLEND_VERSUS_RPM_TABLE_LOCATION_ID                  0x010E
#define BLEND_VERSUS_RPM_TABLE2_LOCATION_ID                 0x010F
#define MAF_VERSUS_VOLTAGE_TABLE_LOCATION_ID                0x011F


#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define TWO_D_TABLE_US_SMALL_TABLE_FULL_BLOCKS_BORDER        0x3000
#endif

/// TODO @todo Claim some RAM back for XGATE use?
/* TablesB */
/* TablesC */
/* TablesD */

/* Tunable blocks */
#define SMALL_TABLES_A_LOCATION_ID                         0x3000
#define SMALL_TABLES_A2_LOCATION_ID                        0x3001
#define SMALL_TABLES_B_LOCATION_ID                         0x3002
#define SMALL_TABLES_B2_LOCATION_ID                        0x3003
#define SMALL_TABLES_C_LOCATION_ID                         0x3004
#define SMALL_TABLES_C2_LOCATION_ID                        0x3005
#define SMALL_TABLES_D_LOCATION_ID                         0x3006
#define SMALL_TABLES_D2_LOCATION_ID                        0x3007

#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define SMALL_TABLE_FULL_BLOCKS_SMALL_TABLE_FILLERS_BORDER  0x4000
#endif

/* Table block fillers */
#define FILLER_A_LOCATION_ID                              0x4000
#define FILLER_A2_LOCATION_ID                             0x4001
#define FILLER_B_LOCATION_ID                              0x4002
#define FILLER_B2_LOCATION_ID                             0x4003
#define FILLER_C_LOCATION_ID                              0x4004
#define FILLER_C2_LOCATION_ID                             0x4005
#define FILLER_D_LOCATION_ID                              0x4006
#define FILLER_D2_LOCATION_ID                             0x4007

#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define SMALL_TABLE_FILLERS_FLASH_LOOKUP_TABLES_BORDER     0x8000
#endif

/* Flash ONLY lookup tables blocks */
#define IAT_TRANSFER_TABLE_LOCATION_ID                     0x8000 /* 2k */
#define CHT_TRANSFER_TABLE_LOCATION_ID                     0x8001 /* 2k */
#define MAF_TRANSFER_TABLE_LOCATION_ID                     0x8002 /* 2k */
#define TEST_TRANSFER_TABLE_LOCATION_ID                    0x8003 /* 2k */

#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define FLASH_LOOKUP_TABLES_SMALL_TABLE_CONFIGS_BORDER     0x9000
#endif

#define LOGGING_SETTINGS_LOCATION_ID                      0x9000
#define LOGGING_SETTINGS2_LOCATION_ID                     0x9001

#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define SMALL_TABLE_CONFIGS_FIXED_CONFIG_BLOCKS_BORDER     0xA000
#endif

/* Fixed configs whole */
#define FIXED_CONFIG1_LOCATION_ID                         0xA000
#define FIXED_CONFIG2_LOCATION_ID                         0xA001

#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define FIXED_CONFIG_BLOCKS_FIXED_CONFIG_SUB_BLOCKS_BORDER  0xC000
#endif

/* Fixed config 1 sub sections */
#define ENGINE_SETTINGS_LOCATION_ID                       0xC000
#define SERIAL_SETTINGS_LOCATION_ID                       0xC001
#define COARSE_BB_SETTINGS_LOCATION_ID                     0xC002
#define SCHEDULING_SETTINGS_LOCATION_ID                   0xC003
#define CUT_AND_LIMITER_SETTINGS_LOCATION_ID                0xC004
#define SIMPLE_GPIO_SETTINGS_LOCATION_ID                   0xC005
#define USER_TEXT_FIELD_LOCATION_ID                        0xC100

/* Fixed config 2 sub sections */
#define SENSOR_SOURCES_LOCATION_ID                        0xC020
#define SENSOR_PRESETS_LOCATION_ID                        0xC021
#define SENSOR_RANGES_LOCATION_ID                         0xC022
#define SENSOR_SETTINGS_LOCATION_ID                       0xC023
#define ALGORITHM_SETTINGS_LOCATION_ID                    0xC024
#define INPUT_OUTPUT_SETTINGS_LOCATION_ID                  0xC025
#define DECODER_SETTINGS_LOCATION_ID                      0xC026
#define USER_TEXT_FIELD2_LOCATION_ID                       0xC101

#ifdef BLOCK_DETAILS_LOOKUP_C // only for here, external stuff should use the flags field for determining what to do/not do.
#define FIXED_CONFIG_SUB_BLOCKS_BORDER_READ_ONLY_VAR_BLOCKS  0xF000
#endif

// Internal blocks of variables that are sometimes useful to read out
#define ADC_REGISTERS_LOCATION_ID                         0xF000
#define CORE_VARS_LOCATION_ID                             0xF001
#define DERIVED_VARS_LOCATION_ID                          0xF002
#define KEY_USER_DEBUG_LOCATION_ID                         0xF003
#define COUNTERS_LOCATION_ID                             0xF004
#define CLOCKS_LOCATION_ID                               0xF005
#define FLAGGABLES_LOCATION_ID                           0xF006
#define FLAGGABLES2_LOCATION_ID                          0xF007


#else
	/* let us know if we are being untidy with headers */
	#warning "Header file LOCATION_IDS_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
