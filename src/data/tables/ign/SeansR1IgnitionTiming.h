/* FreeEMS - the open source engine management system
 *
 * Copyright 2012 Sean Keys
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
 * @ingroup dataInitialisers
 *
 * @brief A timing table for a 2003 Yamaha R1.
 *
 * Under development
 *
 */


// Low R                                                                                                                                                                                        High RPM
// 250       500     750    1000    1300    1600    2200    2800    3400    4000    4600    5200    5800    6400    7000    7600    8200    8800    9400   10000   10600   11200   11800   12400   13000// Close
IT(50), IT(40), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(20), IT(31), IT(34), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), // 0
IT(44), IT(34), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(20), IT(31), IT(34), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), // .6
IT(41), IT(31), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(25), IT(34), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), // 1.5
IT(40), IT(31), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(25), IT(34), IT(38), IT(42), IT(45), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), IT(50), // 2.5

IT(49), IT(43), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(21), IT(25.5),IT(26), IT(26.5), IT(27), IT(27), IT(28), IT(29), IT(31.8),IT(32), IT(36), IT(39), IT(40), IT(41), IT(41), IT(41), IT(41), IT(41), // 4
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(24), IT(25), IT(27), IT(29), IT(31), IT(32), IT(34), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 6.5
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(24), IT(23), IT(23), IT(23), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 8.5
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 10.5

IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 15
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 20
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 25
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 30

IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 40
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 65
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36), // 80
IT(50), IT(50), IT(6.9),IT(6.9),IT(8),  IT(9.3),IT(16), IT(19), IT(21), IT(21), IT(21), IT(22), IT(23), IT(22), IT(22), IT(25), IT(25), IT(28), IT(33), IT(35), IT(36), IT(36), IT(36), IT(36), IT(36)  // 100
// 250       500     750    1000    1300    1600    2200    2800    3400    4000    4600    5200    5800    6400    7000    7600    8200    8800    9400   10000   10600   11200   11800   12400   13000// Boost
// Low R                                                                                                                                                                                        High RPM
