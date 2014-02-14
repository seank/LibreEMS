/* FreeEMS - the open source engine management system
 *
 * Copyright 2011-2013 Fred Cooke
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
 * @ingroup enginePositionRPMDecoders
 *
 * Predefined sync loss IDs to make it easy to lookup and diagnose issues.
 *
 * - Change all 4 hash defines to the filename uppercased and underscored
 *
 */

/* Header file multiple inclusion protection courtesy eclipse Header Template */
/* and http://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/ C pre processor manual    */
#ifndef FILE_SYNC_LOSS_IDS_H_SEEN
#define FILE_SYNC_LOSS_IDS_H_SEEN

// Special cases:
#define BUG_REACHED_UNREACHABLE_CODE           0xFF // Buggy code indicator, called from places that should not be possible to reach.
#define PLL_LOCK_LOST_PRECAUTIONARY            0xFE // Forced sync loss due to inaccurate timers from inaccurate clock
#define SELF_CLOCK_MODE_PRECAUTIONARY          0xFD // Forced sync loss due to inaccurate timers from inaccurate clock

// Special case for timeout
#define EVENT_ARRIVAL_TIMEOUT                  0

// Common to all decoders
#define PRIMARY_EVENT_ARRIVED_TOO_LATE         1
#define PRIMARY_EVENT_ARRIVED_TOO_EARLY        2
#define SECONDARY_EVENT_ARRIVED_TOO_LATE       3
#define SECONDARY_EVENT_ARRIVED_TOO_EARLY      4

// Count based decoders
#define COUNT_OF_EVENTS_IMPOSSIBLY_LOW_NOISE   5
#define COUNT_OF_EVENTS_IMPOSSIBLY_HIGH_NOISE  6

// Logic based CAS as found on most Mitsi and Mazda engines
#define STATE_MISMATCH_IN_PRIMARY_RPM_ISR      7
#define STATE_MISMATCH_IN_SECONDARY_RPM_ISR    8

// MissingTeeth uses from 0x40 - 0x8D, below and above are free for all other decoders to use.

// Pair designations, possibly move this elsewhere to support defining the others as errors elsewhere too
#define FELL_THROUGH     64 // 0x40 The cracks, ie, no match
#define MATCHED_PAIR      4 // ~1:1
#define NARROW_WIDE       6 // ~1:(N+1)
#define WIDE_NARROW       7 // ~(N+1):1
#define NARROW_BACKWARD   8 // ~1:(N+2)/2
#define BACKWARD_NARROW   5 // ~(N+2)/2:1
#define NARROW_TOO_WIDE   65 // 0x41 1:>(N+1)
#define TOO_WIDE_NARROW   66 // 0x42 >(N+1):1

// This is for transposing errors to a unique number to indicate loss of sync different to clear during search
#define MASK_BY_SUM_PATTERN       0x05 // 0xN4 >> 0xN9, 0xN8 >> 0xND, no collisions, free space for more too

// Possible conditions of success and failure.
//
// Literals here for readability, checked below with formula and names
//
// Note, whether it is possible to strike these depends on the tolerance used

// In sync:
#define MATCHED_PAIR_MATCHED_PAIR  68 // 0x44 small small small - All periods match
#define MATCHED_PAIR_NARROW_WIDE   70 // 0x46 small small BIG - First tooth after missing
#define NARROW_WIDE_WIDE_NARROW   103 // 0x67 small BIG small - Second tooth after missing, the strongest and most certain sync
#define WIDE_NARROW_MATCHED_PAIR  116 // 0x74 BIG small small - Third tooth after missing

// Except when we should get something else:
#define MATCHED_PAIR_MATCHED_PAIR_PLUS_MASK  73 // 0x49 small small small - All periods match (Indicates wrong decoder with too few teeth, same missing)
#define MATCHED_PAIR_NARROW_WIDE_PLUS_MASK   75 // 0x4B small small BIG - First tooth after missing (Indicates wrong decoder with too many teeth, same missing)
#define NARROW_WIDE_WIDE_NARROW_PLUS_MASK   108 // 0x6C small BIG small - Second tooth after missing, the strongest and most certain sync
#define WIDE_NARROW_MATCHED_PAIR_PLUS_MASK  121 // 0x79 BIG small small - Third tooth after missing

// Wild noise and bad settings:
#define YOUR_VR_SENSOR_HAS_A_LOOSE_PLUG_FIX_IT            NARROW_TOO_WIDE
#define NOISE_APPEARED_WAY_TOO_EARLY_AS_IF_IT_WAS_A_VR_TOOTH_BUT_WASNT TOO_WIDE_NARROW
#define YOUR_SYNC_TOLERANCE_IS_TIGHTER_THAN_A_WELL_YOU_GET_THE_IDEA  FELL_THROUGH
#define YOUR_SYNC_TOLERANCE_IS_LOOSER_THAN_A_WELL_YOU_GET_THE_IDEA   MATCHED_PAIR_MATCHED_PAIR // Too many matched pairs in a row

// Fails:
#define NEARLY_SYNCED_NARROW_WIDE_BACKWARD_NARROW           101 // 0x65
#define NEARLY_SYNCED_NARROW_BACKWARD_WIDE_NARROW           135 // 0x87
#define NEARLY_SYNCED_NARROW_BACKWARD_BACKWARD_NARROW       133 // 0x85
#define NEARLY_SYNCED_MATCHED_PAIR_NARROW_BACKWARD           72 // 0x48
#define NEARLY_SYNCED_BACKWARD_NARROW_MATCHED_PAIR           84 // 0x54
#define EXTRA_TOOTH_WIDE_NARROW_NARROW_WIDE                 118 // 0x76
#define EXTRA_TOOTH_WIDE_NARROW_NARROW_BACKWARD             120 // 0x78
#define EXTRA_TOOTH_BACKWARD_NARROW_NARROW_WIDE              86 // 0x56
#define EXTRA_TOOTH_BACKWARD_NARROW_NARROW_BACKWARD          88 // 0x58
#define VR_WIRING_BACKWARD_MATCHED_PAIR_BACKWARD_NARROW       69 // 0x45 Engineered to be 69 for humour value!
#define VR_WIRING_BACKWARD_MATCHED_PAIR_WIDE_NARROW           71 // 0x47
#define VR_WIRING_BACKWARD_NARROW_WIDE_MATCHED_PAIR          100 // 0x64
#define VR_WIRING_BACKWARD_NARROW_BACKWARD_MATCHED_PAIR      132 // 0x84
#define EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_BACKWARD 136 // 0x88
#define EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_WIDE     134 // 0x86
#define EXCESS_DECELERATION_NARROW_WIDE_NARROW_BACKWARD     104 // 0x68
#define EXCESS_DECELERATION_NARROW_WIDE_NARROW_WIDE         102 // 0x66
#define EXCESS_ACCELERATION_BACKWARD_NARROW_BACKWARD_NARROW  85 // 0x55
#define EXCESS_ACCELERATION_BACKWARD_NARROW_WIDE_NARROW      87 // 0x57
#define EXCESS_ACCELERATION_WIDE_NARROW_BACKWARD_NARROW     117 // 0x75
#define EXCESS_ACCELERATION_WIDE_NARROW_WIDE_NARROW         119 // 0x77

// Fails while synced:
#define NEARLY_SYNCED_NARROW_WIDE_BACKWARD_NARROW_PLUS_MASK           106 // 0x6A
#define NEARLY_SYNCED_NARROW_BACKWARD_WIDE_NARROW_PLUS_MASK           140 // 0x8C
#define NEARLY_SYNCED_NARROW_BACKWARD_BACKWARD_NARROW_PLUS_MASK       138 // 0x8A
#define NEARLY_SYNCED_MATCHED_PAIR_NARROW_BACKWARD_PLUS_MASK           77 // 0x4D
#define NEARLY_SYNCED_BACKWARD_NARROW_MATCHED_PAIR_PLUS_MASK           89 // 0x59
#define EXTRA_TOOTH_WIDE_NARROW_NARROW_WIDE_PLUS_MASK                 123 // 0x7B
#define EXTRA_TOOTH_WIDE_NARROW_NARROW_BACKWARD_PLUS_MASK             125 // 0x7D
#define EXTRA_TOOTH_BACKWARD_NARROW_NARROW_WIDE_PLUS_MASK              91 // 0x5B
#define EXTRA_TOOTH_BACKWARD_NARROW_NARROW_BACKWARD_PLUS_MASK          93 // 0x5D
#define VR_WIRING_BACKWARD_MATCHED_PAIR_BACKWARD_NARROW_PLUS_MASK       74 // 0x4A
#define VR_WIRING_BACKWARD_MATCHED_PAIR_WIDE_NARROW_PLUS_MASK           76 // 0x4C
#define VR_WIRING_BACKWARD_NARROW_WIDE_MATCHED_PAIR_PLUS_MASK          105 // 0x69
#define VR_WIRING_BACKWARD_NARROW_BACKWARD_MATCHED_PAIR_PLUS_MASK      137 // 0x89
#define EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_BACKWARD_PLUS_MASK 141 // 0x8D
#define EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_WIDE_PLUS_MASK     139 // 0x8B
#define EXCESS_DECELERATION_NARROW_WIDE_NARROW_BACKWARD_PLUS_MASK     109 // 0x6D
#define EXCESS_DECELERATION_NARROW_WIDE_NARROW_WIDE_PLUS_MASK         107 // 0x6B
#define EXCESS_ACCELERATION_BACKWARD_NARROW_BACKWARD_NARROW_PLUS_MASK  90 // 0x5A
#define EXCESS_ACCELERATION_BACKWARD_NARROW_WIDE_NARROW_PLUS_MASK      92 // 0x5C
#define EXCESS_ACCELERATION_WIDE_NARROW_BACKWARD_NARROW_PLUS_MASK     122 // 0x7A
#define EXCESS_ACCELERATION_WIDE_NARROW_WIDE_NARROW_PLUS_MASK         124 // 0x7C

// Self-checks on the above
#if (MATCHED_PAIR_MATCHED_PAIR != ((MATCHED_PAIR << 4) + MATCHED_PAIR))
#error "Header is broken, fix it!"
#endif
#if (MATCHED_PAIR_NARROW_WIDE != ((MATCHED_PAIR << 4) + NARROW_WIDE))
#error "Header is broken, fix it!"
#endif
#if (NARROW_WIDE_WIDE_NARROW != ((NARROW_WIDE << 4) + WIDE_NARROW))
#error "Header is broken, fix it!"
#endif
#if (WIDE_NARROW_MATCHED_PAIR != ((WIDE_NARROW << 4) + MATCHED_PAIR))
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_NARROW_WIDE_BACKWARD_NARROW != ((NARROW_WIDE << 4) + BACKWARD_NARROW))
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_NARROW_BACKWARD_WIDE_NARROW != ((NARROW_BACKWARD << 4) + WIDE_NARROW))
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_NARROW_BACKWARD_BACKWARD_NARROW != ((NARROW_BACKWARD << 4) + BACKWARD_NARROW))
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_MATCHED_PAIR_NARROW_BACKWARD != ((MATCHED_PAIR << 4) + NARROW_BACKWARD))
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_BACKWARD_NARROW_MATCHED_PAIR != ((BACKWARD_NARROW << 4) + MATCHED_PAIR))
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_WIDE_NARROW_NARROW_WIDE != ((WIDE_NARROW << 4) + NARROW_WIDE))
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_WIDE_NARROW_NARROW_BACKWARD != ((WIDE_NARROW << 4) + NARROW_BACKWARD))
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_BACKWARD_NARROW_NARROW_WIDE != ((BACKWARD_NARROW << 4) + NARROW_WIDE))
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_BACKWARD_NARROW_NARROW_BACKWARD != ((BACKWARD_NARROW << 4) + NARROW_BACKWARD))
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_MATCHED_PAIR_BACKWARD_NARROW != ((MATCHED_PAIR << 4) + BACKWARD_NARROW))
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_MATCHED_PAIR_WIDE_NARROW != ((MATCHED_PAIR << 4) + WIDE_NARROW))
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_NARROW_WIDE_MATCHED_PAIR != ((NARROW_WIDE << 4) + MATCHED_PAIR))
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_NARROW_BACKWARD_MATCHED_PAIR != ((NARROW_BACKWARD << 4) + MATCHED_PAIR))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_BACKWARD != ((NARROW_BACKWARD << 4) + NARROW_BACKWARD))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_WIDE != ((NARROW_BACKWARD << 4) + NARROW_WIDE))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_WIDE_NARROW_BACKWARD != ((NARROW_WIDE << 4) + NARROW_BACKWARD))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_WIDE_NARROW_WIDE != ((NARROW_WIDE << 4) + NARROW_WIDE))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_BACKWARD_NARROW_BACKWARD_NARROW != ((BACKWARD_NARROW << 4) + BACKWARD_NARROW))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_BACKWARD_NARROW_WIDE_NARROW != ((BACKWARD_NARROW << 4) + WIDE_NARROW))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_WIDE_NARROW_BACKWARD_NARROW != ((WIDE_NARROW << 4) + BACKWARD_NARROW))
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_WIDE_NARROW_WIDE_NARROW != ((WIDE_NARROW << 4) + WIDE_NARROW))
#error "Header is broken, fix it!"
#endif

// And for the masked ones:
#if (MATCHED_PAIR_MATCHED_PAIR_PLUS_MASK != ((MATCHED_PAIR << 4) + MATCHED_PAIR) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (MATCHED_PAIR_NARROW_WIDE_PLUS_MASK != ((MATCHED_PAIR << 4) + NARROW_WIDE) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (NARROW_WIDE_WIDE_NARROW_PLUS_MASK != ((NARROW_WIDE << 4) + WIDE_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (WIDE_NARROW_MATCHED_PAIR_PLUS_MASK != ((WIDE_NARROW << 4) + MATCHED_PAIR) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_NARROW_WIDE_BACKWARD_NARROW_PLUS_MASK != ((NARROW_WIDE << 4) + BACKWARD_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_NARROW_BACKWARD_WIDE_NARROW_PLUS_MASK != ((NARROW_BACKWARD << 4) + WIDE_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_NARROW_BACKWARD_BACKWARD_NARROW_PLUS_MASK != ((NARROW_BACKWARD << 4) + BACKWARD_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_MATCHED_PAIR_NARROW_BACKWARD_PLUS_MASK != ((MATCHED_PAIR << 4) + NARROW_BACKWARD) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLY_SYNCED_BACKWARD_NARROW_MATCHED_PAIR_PLUS_MASK != ((BACKWARD_NARROW << 4) + MATCHED_PAIR) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_WIDE_NARROW_NARROW_WIDE_PLUS_MASK != ((WIDE_NARROW << 4) + NARROW_WIDE) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_WIDE_NARROW_NARROW_BACKWARD_PLUS_MASK != ((WIDE_NARROW << 4) + NARROW_BACKWARD) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_BACKWARD_NARROW_NARROW_WIDE_PLUS_MASK != ((BACKWARD_NARROW << 4) + NARROW_WIDE) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRA_TOOTH_BACKWARD_NARROW_NARROW_BACKWARD_PLUS_MASK != ((BACKWARD_NARROW << 4) + NARROW_BACKWARD) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_MATCHED_PAIR_BACKWARD_NARROW_PLUS_MASK != ((MATCHED_PAIR << 4) + BACKWARD_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_MATCHED_PAIR_WIDE_NARROW_PLUS_MASK != ((MATCHED_PAIR << 4) + WIDE_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_NARROW_WIDE_MATCHED_PAIR_PLUS_MASK != ((NARROW_WIDE << 4) + MATCHED_PAIR) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (VR_WIRING_BACKWARD_NARROW_BACKWARD_MATCHED_PAIR_PLUS_MASK != ((NARROW_BACKWARD << 4) + MATCHED_PAIR) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_BACKWARD_PLUS_MASK != ((NARROW_BACKWARD << 4) + NARROW_BACKWARD) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_BACKWARD_NARROW_WIDE_PLUS_MASK != ((NARROW_BACKWARD << 4) + NARROW_WIDE) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_WIDE_NARROW_BACKWARD_PLUS_MASK != ((NARROW_WIDE << 4) + NARROW_BACKWARD) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_DECELERATION_NARROW_WIDE_NARROW_WIDE_PLUS_MASK != ((NARROW_WIDE << 4) + NARROW_WIDE) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_BACKWARD_NARROW_BACKWARD_NARROW_PLUS_MASK != ((BACKWARD_NARROW << 4) + BACKWARD_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_BACKWARD_NARROW_WIDE_NARROW_PLUS_MASK != ((BACKWARD_NARROW << 4) + WIDE_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_WIDE_NARROW_BACKWARD_NARROW_PLUS_MASK != ((WIDE_NARROW << 4) + BACKWARD_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESS_ACCELERATION_WIDE_NARROW_WIDE_NARROW_PLUS_MASK != ((WIDE_NARROW << 4) + WIDE_NARROW) + MASK_BY_SUM_PATTERN)
#error "Header is broken, fix it!"
#endif


#else
	/* let us know if we are being untidy with headers */
	#warning "Header file SYNC_LOSS_IDS_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
