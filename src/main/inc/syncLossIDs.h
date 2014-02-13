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
#define FELLTHROUGH     64 // 0x40 The cracks, ie, no match
#define MATCHEDPAIR      4 // ~1:1
#define NARROWWIDE       6 // ~1:(N+1)
#define WIDENARROW       7 // ~(N+1):1
#define NARROWBACKWARD   8 // ~1:(N+2)/2
#define BACKWARDNARROW   5 // ~(N+2)/2:1
#define NARROWTOOWIDE   65 // 0x41 1:>(N+1)
#define TOOWIDENARROW   66 // 0x42 >(N+1):1

// This is for transposing errors to a unique number to indicate loss of sync different to clear during search
#define MASKBYSUMPATTERN       0x05 // 0xN4 >> 0xN9, 0xN8 >> 0xND, no collisions, free space for more too

// Possible conditions of success and failure.
//
// Literals here for readability, checked below with formula and names
//
// Note, whether it is possible to strike these depends on the tolerance used

// In sync:
#define MATCHEDPAIRMATCHEDPAIR  68 // 0x44 small small small - All periods match
#define MATCHEDPAIRNARROWWIDE   70 // 0x46 small small BIG - First tooth after missing
#define NARROWWIDEWIDENARROW   103 // 0x67 small BIG small - Second tooth after missing, the strongest and most certain sync
#define WIDENARROWMATCHEDPAIR  116 // 0x74 BIG small small - Third tooth after missing

// Except when we should get something else:
#define MATCHEDPAIRMATCHEDPAIRPLUSMASK  73 // 0x49 small small small - All periods match (Indicates wrong decoder with too few teeth, same missing)
#define MATCHEDPAIRNARROWWIDEPLUSMASK   75 // 0x4B small small BIG - First tooth after missing (Indicates wrong decoder with too many teeth, same missing)
#define NARROWWIDEWIDENARROWPLUSMASK   108 // 0x6C small BIG small - Second tooth after missing, the strongest and most certain sync
#define WIDENARROWMATCHEDPAIRPLUSMASK  121 // 0x79 BIG small small - Third tooth after missing

// Wild noise and bad settings:
#define YOURVRSENSORHASALOOSEPLUGFIXIT                    NARROWTOOWIDE
#define NOISEAPPEAREDWAYTOOEARLYASIFITWASAVRTOOTHBUTWASNT TOOWIDENARROW
#define YOURSYNCTOLERANCEISTIGHTERTHANAWELLYOUGETTHEIDEA  FELLTHROUGH
#define YOURSYNCTOLERANCEISLOOSERTHANAWELLYOUGETTHEIDEA   MATCHEDPAIRMATCHEDPAIR // Too many matched pairs in a row

// Fails:
#define NEARLYSYNCEDNARROWWIDEBACKWARDNARROW           101 // 0x65
#define NEARLYSYNCEDNARROWBACKWARDWIDENARROW           135 // 0x87
#define NEARLYSYNCEDNARROWBACKWARDBACKWARDNARROW       133 // 0x85
#define NEARLYSYNCEDMATCHEDPAIRNARROWBACKWARD           72 // 0x48
#define NEARLYSYNCEDBACKWARDNARROWMATCHEDPAIR           84 // 0x54
#define EXTRATOOTHWIDENARROWNARROWWIDE                 118 // 0x76
#define EXTRATOOTHWIDENARROWNARROWBACKWARD             120 // 0x78
#define EXTRATOOTHBACKWARDNARROWNARROWWIDE              86 // 0x56
#define EXTRATOOTHBACKWARDNARROWNARROWBACKWARD          88 // 0x58
#define VRWIRINGBACKWARDMATCHEDPAIRBACKWARDNARROW       69 // 0x45 Engineered to be 69 for humour value!
#define VRWIRINGBACKWARDMATCHEDPAIRWIDENARROW           71 // 0x47
#define VRWIRINGBACKWARDNARROWWIDEMATCHEDPAIR          100 // 0x64
#define VRWIRINGBACKWARDNARROWBACKWARDMATCHEDPAIR      132 // 0x84
#define EXCESSDECELERATIONNARROWBACKWARDNARROWBACKWARD 136 // 0x88
#define EXCESSDECELERATIONNARROWBACKWARDNARROWWIDE     134 // 0x86
#define EXCESSDECELERATIONNARROWWIDENARROWBACKWARD     104 // 0x68
#define EXCESSDECELERATIONNARROWWIDENARROWWIDE         102 // 0x66
#define EXCESSACCELERATIONBACKWARDNARROWBACKWARDNARROW  85 // 0x55
#define EXCESSACCELERATIONBACKWARDNARROWWIDENARROW      87 // 0x57
#define EXCESSACCELERATIONWIDENARROWBACKWARDNARROW     117 // 0x75
#define EXCESSACCELERATIONWIDENARROWWIDENARROW         119 // 0x77

// Fails while synced:
#define NEARLYSYNCEDNARROWWIDEBACKWARDNARROWPLUSMASK           106 // 0x6A
#define NEARLYSYNCEDNARROWBACKWARDWIDENARROWPLUSMASK           140 // 0x8C
#define NEARLYSYNCEDNARROWBACKWARDBACKWARDNARROWPLUSMASK       138 // 0x8A
#define NEARLYSYNCEDMATCHEDPAIRNARROWBACKWARDPLUSMASK           77 // 0x4D
#define NEARLYSYNCEDBACKWARDNARROWMATCHEDPAIRPLUSMASK           89 // 0x59
#define EXTRATOOTHWIDENARROWNARROWWIDEPLUSMASK                 123 // 0x7B
#define EXTRATOOTHWIDENARROWNARROWBACKWARDPLUSMASK             125 // 0x7D
#define EXTRATOOTHBACKWARDNARROWNARROWWIDEPLUSMASK              91 // 0x5B
#define EXTRATOOTHBACKWARDNARROWNARROWBACKWARDPLUSMASK          93 // 0x5D
#define VRWIRINGBACKWARDMATCHEDPAIRBACKWARDNARROWPLUSMASK       74 // 0x4A
#define VRWIRINGBACKWARDMATCHEDPAIRWIDENARROWPLUSMASK           76 // 0x4C
#define VRWIRINGBACKWARDNARROWWIDEMATCHEDPAIRPLUSMASK          105 // 0x69
#define VRWIRINGBACKWARDNARROWBACKWARDMATCHEDPAIRPLUSMASK      137 // 0x89
#define EXCESSDECELERATIONNARROWBACKWARDNARROWBACKWARDPLUSMASK 141 // 0x8D
#define EXCESSDECELERATIONNARROWBACKWARDNARROWWIDEPLUSMASK     139 // 0x8B
#define EXCESSDECELERATIONNARROWWIDENARROWBACKWARDPLUSMASK     109 // 0x6D
#define EXCESSDECELERATIONNARROWWIDENARROWWIDEPLUSMASK         107 // 0x6B
#define EXCESSACCELERATIONBACKWARDNARROWBACKWARDNARROWPLUSMASK  90 // 0x5A
#define EXCESSACCELERATIONBACKWARDNARROWWIDENARROWPLUSMASK      92 // 0x5C
#define EXCESSACCELERATIONWIDENARROWBACKWARDNARROWPLUSMASK     122 // 0x7A
#define EXCESSACCELERATIONWIDENARROWWIDENARROWPLUSMASK         124 // 0x7C

// Self-checks on the above
#if (MATCHEDPAIRMATCHEDPAIR != ((MATCHEDPAIR << 4) + MATCHEDPAIR))
#error "Header is broken, fix it!"
#endif
#if (MATCHEDPAIRNARROWWIDE != ((MATCHEDPAIR << 4) + NARROWWIDE))
#error "Header is broken, fix it!"
#endif
#if (NARROWWIDEWIDENARROW != ((NARROWWIDE << 4) + WIDENARROW))
#error "Header is broken, fix it!"
#endif
#if (WIDENARROWMATCHEDPAIR != ((WIDENARROW << 4) + MATCHEDPAIR))
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDNARROWWIDEBACKWARDNARROW != ((NARROWWIDE << 4) + BACKWARDNARROW))
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDNARROWBACKWARDWIDENARROW != ((NARROWBACKWARD << 4) + WIDENARROW))
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDNARROWBACKWARDBACKWARDNARROW != ((NARROWBACKWARD << 4) + BACKWARDNARROW))
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDMATCHEDPAIRNARROWBACKWARD != ((MATCHEDPAIR << 4) + NARROWBACKWARD))
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDBACKWARDNARROWMATCHEDPAIR != ((BACKWARDNARROW << 4) + MATCHEDPAIR))
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHWIDENARROWNARROWWIDE != ((WIDENARROW << 4) + NARROWWIDE))
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHWIDENARROWNARROWBACKWARD != ((WIDENARROW << 4) + NARROWBACKWARD))
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHBACKWARDNARROWNARROWWIDE != ((BACKWARDNARROW << 4) + NARROWWIDE))
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHBACKWARDNARROWNARROWBACKWARD != ((BACKWARDNARROW << 4) + NARROWBACKWARD))
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDMATCHEDPAIRBACKWARDNARROW != ((MATCHEDPAIR << 4) + BACKWARDNARROW))
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDMATCHEDPAIRWIDENARROW != ((MATCHEDPAIR << 4) + WIDENARROW))
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDNARROWWIDEMATCHEDPAIR != ((NARROWWIDE << 4) + MATCHEDPAIR))
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDNARROWBACKWARDMATCHEDPAIR != ((NARROWBACKWARD << 4) + MATCHEDPAIR))
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWBACKWARDNARROWBACKWARD != ((NARROWBACKWARD << 4) + NARROWBACKWARD))
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWBACKWARDNARROWWIDE != ((NARROWBACKWARD << 4) + NARROWWIDE))
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWWIDENARROWBACKWARD != ((NARROWWIDE << 4) + NARROWBACKWARD))
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWWIDENARROWWIDE != ((NARROWWIDE << 4) + NARROWWIDE))
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONBACKWARDNARROWBACKWARDNARROW != ((BACKWARDNARROW << 4) + BACKWARDNARROW))
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONBACKWARDNARROWWIDENARROW != ((BACKWARDNARROW << 4) + WIDENARROW))
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONWIDENARROWBACKWARDNARROW != ((WIDENARROW << 4) + BACKWARDNARROW))
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONWIDENARROWWIDENARROW != ((WIDENARROW << 4) + WIDENARROW))
#error "Header is broken, fix it!"
#endif

// And for the masked ones:
#if (MATCHEDPAIRMATCHEDPAIRPLUSMASK != ((MATCHEDPAIR << 4) + MATCHEDPAIR) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (MATCHEDPAIRNARROWWIDEPLUSMASK != ((MATCHEDPAIR << 4) + NARROWWIDE) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (NARROWWIDEWIDENARROWPLUSMASK != ((NARROWWIDE << 4) + WIDENARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (WIDENARROWMATCHEDPAIRPLUSMASK != ((WIDENARROW << 4) + MATCHEDPAIR) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDNARROWWIDEBACKWARDNARROWPLUSMASK != ((NARROWWIDE << 4) + BACKWARDNARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDNARROWBACKWARDWIDENARROWPLUSMASK != ((NARROWBACKWARD << 4) + WIDENARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDNARROWBACKWARDBACKWARDNARROWPLUSMASK != ((NARROWBACKWARD << 4) + BACKWARDNARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDMATCHEDPAIRNARROWBACKWARDPLUSMASK != ((MATCHEDPAIR << 4) + NARROWBACKWARD) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (NEARLYSYNCEDBACKWARDNARROWMATCHEDPAIRPLUSMASK != ((BACKWARDNARROW << 4) + MATCHEDPAIR) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHWIDENARROWNARROWWIDEPLUSMASK != ((WIDENARROW << 4) + NARROWWIDE) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHWIDENARROWNARROWBACKWARDPLUSMASK != ((WIDENARROW << 4) + NARROWBACKWARD) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHBACKWARDNARROWNARROWWIDEPLUSMASK != ((BACKWARDNARROW << 4) + NARROWWIDE) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXTRATOOTHBACKWARDNARROWNARROWBACKWARDPLUSMASK != ((BACKWARDNARROW << 4) + NARROWBACKWARD) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDMATCHEDPAIRBACKWARDNARROWPLUSMASK != ((MATCHEDPAIR << 4) + BACKWARDNARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDMATCHEDPAIRWIDENARROWPLUSMASK != ((MATCHEDPAIR << 4) + WIDENARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDNARROWWIDEMATCHEDPAIRPLUSMASK != ((NARROWWIDE << 4) + MATCHEDPAIR) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (VRWIRINGBACKWARDNARROWBACKWARDMATCHEDPAIRPLUSMASK != ((NARROWBACKWARD << 4) + MATCHEDPAIR) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWBACKWARDNARROWBACKWARDPLUSMASK != ((NARROWBACKWARD << 4) + NARROWBACKWARD) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWBACKWARDNARROWWIDEPLUSMASK != ((NARROWBACKWARD << 4) + NARROWWIDE) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWWIDENARROWBACKWARDPLUSMASK != ((NARROWWIDE << 4) + NARROWBACKWARD) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSDECELERATIONNARROWWIDENARROWWIDEPLUSMASK != ((NARROWWIDE << 4) + NARROWWIDE) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONBACKWARDNARROWBACKWARDNARROWPLUSMASK != ((BACKWARDNARROW << 4) + BACKWARDNARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONBACKWARDNARROWWIDENARROWPLUSMASK != ((BACKWARDNARROW << 4) + WIDENARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONWIDENARROWBACKWARDNARROWPLUSMASK != ((WIDENARROW << 4) + BACKWARDNARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif
#if (EXCESSACCELERATIONWIDENARROWWIDENARROWPLUSMASK != ((WIDENARROW << 4) + WIDENARROW) + MASKBYSUMPATTERN)
#error "Header is broken, fix it!"
#endif


#else
	/* let us know if we are being untidy with headers */
	#warning "Header file SYNC_LOSS_IDS_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
