/*
 * dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
 *
 * $Id: tbl.c 26 2004-01-17 23:28:23Z eric $
 * Copyright 2001-2003 Eric Smith <eric@brouhaha.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.  Note that permission is
 * not granted to redistribute this program under the terms of any
 * other version of the General Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 */


#include "dis.h"

struct info optbl[256] = {
	/* 00 */	{ { 'B', 'R', 'K', 0, },  1, IMP|STOP, },
	/* 01 */	{ { 'O', 'R', 'A', 0, },  2, INX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 05 */	{ { 'O', 'R', 'A', 0, },  2, ZPG|NORM, },
	/* 06 */	{ { 'A', 'S', 'L', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 08 */	{ { 'P', 'H', 'P', 0, },  1, IMP|NORM, },
	/* 09 */	{ { 'O', 'R', 'A', 0, },  2, IMM|NORM, },
	/* 0a */	{ { 'A', 'S', 'L', 0, },  1, ACC|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 0d */	{ { 'O', 'R', 'A', 0, },  3, ABS|NORM, },
	/* 0e */	{ { 'A', 'S', 'L', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 10 */	{ { 'B', 'P', 'L', 0, },  2, REL|FORK, },
	/* 11 */	{ { 'O', 'R', 'A', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 15 */	{ { 'O', 'R', 'A', 0, },  2, ZPX|NORM, },
	/* 16 */	{ { 'A', 'S', 'L', 0, },  2, ZPX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 18 */	{ { 'C', 'L', 'C', 0, },  1, IMP|NORM, },
	/* 19 */	{ { 'O', 'R', 'A', 0, },  3, ABY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 1d */	{ { 'O', 'R', 'A', 0, },  3, ABX|NORM, },
	/* 1e */	{ { 'A', 'S', 'L', 0, },  3, ABX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 20 */	{ { 'J', 'S', 'R', 0, },  3, ABS|FORK, },
	/* 21 */	{ { 'A', 'N', 'D', 0, },  2, INX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 24 */	{ { 'B', 'I', 'T', 0, },  2, ZPG|NORM, },
	/* 25 */	{ { 'A', 'N', 'D', 0, },  2, ZPG|NORM, },
	/* 26 */	{ { 'R', 'O', 'L', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 28 */	{ { 'P', 'L', 'P', 0, },  1, IMP|NORM, },
	/* 29 */	{ { 'A', 'N', 'D', 0, },  2, IMM|NORM, },
	/* 2a */	{ { 'R', 'O', 'L', 0, },  1, ACC|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 2c */	{ { 'B', 'I', 'T', 0, },  3, ABS|NORM, },
	/* 2d */	{ { 'A', 'N', 'D', 0, },  3, ABS|NORM, },
	/* 2e */	{ { 'R', 'O', 'L', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 30 */	{ { 'B', 'M', 'I', 0, },  2, REL|FORK, },
	/* 31 */	{ { 'A', 'N', 'D', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 35 */	{ { 'A', 'N', 'D', 0, },  2, ZPX|NORM, },
	/* 36 */	{ { 'R', 'O', 'L', 0, },  2, ZPX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 38 */	{ { 'S', 'E', 'C', 0, },  1, IMP|NORM, },
	/* 39 */	{ { 'A', 'N', 'D', 0, },  3, ABY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 3d */	{ { 'A', 'N', 'D', 0, },  3, ABX|NORM, },
	/* 3e */	{ { 'R', 'O', 'L', 0, },  3, ABX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 40 */	{ { 'R', 'T', 'I', 0, },  1, IMP|STOP, },
	/* 41 */	{ { 'E', 'O', 'R', 0, },  2, INX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 45 */	{ { 'E', 'O', 'R', 0, },  2, ZPG|NORM, },
	/* 46 */	{ { 'L', 'S', 'R', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 48 */	{ { 'P', 'H', 'A', 0, },  1, IMP|NORM, },
	/* 49 */	{ { 'E', 'O', 'R', 0, },  2, IMM|NORM, },
	/* 4a */	{ { 'L', 'S', 'R', 0, },  1, ACC|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 4c */	{ { 'J', 'M', 'P', 0, },  3, ABS|JUMP, },
	/* 4d */	{ { 'E', 'O', 'R', 0, },  3, ABS|NORM, },
	/* 4e */	{ { 'L', 'S', 'R', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 50 */	{ { 'B', 'V', 'C', 0, },  2, REL|FORK, },
	/* 51 */	{ { 'E', 'O', 'R', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 55 */	{ { 'E', 'O', 'R', 0, },  2, ZPX|NORM, },
	/* 56 */	{ { 'L', 'S', 'R', 0, },  2, ZPX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 58 */	{ { 'C', 'L', 'I', 0, },  1, IMP|NORM, },
	/* 59 */	{ { 'E', 'O', 'R', 0, },  3, ABY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 5d */	{ { 'E', 'O', 'R', 0, },  3, ABX|NORM, },
	/* 5e */	{ { 'L', 'S', 'R', 0, },  3, ABX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 60 */	{ { 'R', 'T', 'S', 0, },  1, IMP|STOP, },
	/* 61 */	{ { 'A', 'D', 'C', 0, },  2, INX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 65 */	{ { 'A', 'D', 'C', 0, },  2, ZPG|NORM, },
	/* 66 */	{ { 'R', 'O', 'R', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 68 */	{ { 'P', 'L', 'A', 0, },  1, IMP|NORM, },
	/* 69 */	{ { 'A', 'D', 'C', 0, },  2, IMM|NORM, },
	/* 6a */	{ { 'R', 'O', 'R', 0, },  1, ACC|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 6c */	{ { 'J', 'M', 'P', 0, },  3, IND|STOP, },
	/* 6d */	{ { 'A', 'D', 'C', 0, },  3, ABS|NORM, },
	/* 6e */	{ { 'R', 'O', 'R', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 70 */	{ { 'B', 'V', 'S', 0, },  2, REL|FORK, },
	/* 71 */	{ { 'A', 'D', 'C', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 75 */	{ { 'A', 'D', 'C', 0, },  2, ZPX|NORM, },
	/* 76 */	{ { 'R', 'O', 'R', 0, },  2, ZPX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 78 */	{ { 'S', 'E', 'I', 0, },  1, IMP|NORM, },
	/* 79 */	{ { 'A', 'D', 'C', 0, },  3, ABY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 7d */	{ { 'A', 'D', 'C', 0, },  3, ABX|NORM, },
	/* 7e */	{ { 'R', 'O', 'R', 0, },  3, ABX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 81 */	{ { 'S', 'T', 'A', 0, },  2, INX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 84 */	{ { 'S', 'T', 'Y', 0, },  2, ZPG|NORM, },
	/* 85 */	{ { 'S', 'T', 'A', 0, },  2, ZPG|NORM, },
	/* 86 */	{ { 'S', 'T', 'X', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 88 */	{ { 'D', 'E', 'Y', 0, },  1, IMP|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 8a */	{ { 'T', 'X', 'A', 0, },  1, IMP|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 8c */	{ { 'S', 'T', 'Y', 0, },  3, ABS|NORM, },
	/* 8d */	{ { 'S', 'T', 'A', 0, },  3, ABS|NORM, },
	/* 8e */	{ { 'S', 'T', 'X', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 90 */	{ { 'B', 'C', 'C', 0, },  2, REL|FORK, },
	/* 91 */	{ { 'S', 'T', 'A', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 94 */	{ { 'S', 'T', 'Y', 0, },  2, ZPX|NORM, },
	/* 95 */	{ { 'S', 'T', 'A', 0, },  2, ZPX|NORM, },
	/* 96 */	{ { 'S', 'T', 'X', 0, },  2, ZPY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 98 */	{ { 'T', 'Y', 'A', 0, },  1, IMP|NORM, },
	/* 99 */	{ { 'S', 'T', 'A', 0, },  3, ABY|NORM, },
	/* 9a */	{ { 'T', 'X', 'S', 0, },  1, IMP|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 9d */	{ { 'S', 'T', 'A', 0, },  3, ABX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* a0 */	{ { 'L', 'D', 'Y', 0, },  2, IMM|NORM, },
	/* a1 */	{ { 'L', 'D', 'A', 0, },  2, INX|NORM, },
	/* a2 */	{ { 'L', 'D', 'X', 0, },  2, IMM|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* a4 */	{ { 'L', 'D', 'Y', 0, },  2, ZPG|NORM, },
	/* a5 */	{ { 'L', 'D', 'A', 0, },  2, ZPG|NORM, },
	/* a6 */	{ { 'L', 'D', 'X', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* a8 */	{ { 'T', 'A', 'Y', 0, },  1, IMP|NORM, },
	/* a9 */	{ { 'L', 'D', 'A', 0, },  2, IMM|NORM, },
	/* aa */	{ { 'T', 'A', 'X', 0, },  1, IMP|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* ac */	{ { 'L', 'D', 'Y', 0, },  3, ABS|NORM, },
	/* ad */	{ { 'L', 'D', 'A', 0, },  3, ABS|NORM, },
	/* ae */	{ { 'L', 'D', 'X', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* b0 */	{ { 'B', 'C', 'S', 0, },  2, REL|FORK, },
	/* b1 */	{ { 'L', 'D', 'A', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* b4 */	{ { 'L', 'D', 'Y', 0, },  2, ZPX|NORM, },
	/* b5 */	{ { 'L', 'D', 'A', 0, },  2, ZPX|NORM, },
	/* b6 */	{ { 'L', 'D', 'X', 0, },  2, ZPY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* b8 */	{ { 'C', 'L', 'V', 0, },  1, IMP|NORM, },
	/* b9 */	{ { 'L', 'D', 'A', 0, },  3, ABY|NORM, },
	/* ba */	{ { 'T', 'S', 'X', 0, },  1, IMP|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* bc */	{ { 'L', 'D', 'Y', 0, },  3, ABX|NORM, },
	/* bd */	{ { 'L', 'D', 'A', 0, },  3, ABX|NORM, },
	/* be */	{ { 'L', 'D', 'X', 0, },  3, ABY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* c0 */	{ { 'C', 'P', 'Y', 0, },  2, IMM|NORM, },
	/* c1 */	{ { 'C', 'M', 'P', 0, },  2, INX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* c4 */	{ { 'C', 'P', 'Y', 0, },  2, ZPG|NORM, },
	/* c5 */	{ { 'C', 'M', 'P', 0, },  2, ZPG|NORM, },
	/* c6 */	{ { 'D', 'E', 'C', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* c8 */	{ { 'I', 'N', 'Y', 0, },  1, IMP|NORM, },
	/* c9 */	{ { 'C', 'M', 'P', 0, },  2, IMM|NORM, },
	/* ca */	{ { 'D', 'E', 'X', 0, },  1, IMP|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* cc */	{ { 'C', 'P', 'Y', 0, },  3, ABS|NORM, },
	/* cd */	{ { 'C', 'M', 'P', 0, },  3, ABS|NORM, },
	/* ce */	{ { 'D', 'E', 'C', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* d0 */	{ { 'B', 'N', 'E', 0, },  2, REL|FORK, },
	/* d1 */	{ { 'C', 'M', 'P', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* d5 */	{ { 'C', 'M', 'P', 0, },  2, ZPX|NORM, },
	/* d6 */	{ { 'D', 'E', 'C', 0, },  2, ZPX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* d8 */	{ { 'C', 'L', 'D', 0, },  1, IMP|NORM, },
	/* d9 */	{ { 'C', 'M', 'P', 0, },  3, ABY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* dd */	{ { 'C', 'M', 'P', 0, },  3, ABX|NORM, },
	/* de */	{ { 'D', 'E', 'C', 0, },  3, ABX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* e0 */	{ { 'C', 'P', 'X', 0, },  2, IMM|NORM, },
	/* e1 */	{ { 'S', 'B', 'C', 0, },  2, INX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* e4 */	{ { 'C', 'P', 'X', 0, },  2, ZPG|NORM, },
	/* e5 */	{ { 'S', 'B', 'C', 0, },  2, ZPG|NORM, },
	/* e6 */	{ { 'I', 'N', 'C', 0, },  2, ZPG|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* e8 */	{ { 'I', 'N', 'X', 0, },  1, IMP|NORM, },
	/* e9 */	{ { 'S', 'B', 'C', 0, },  2, IMM|NORM, },
	/* ea */	{ { 'N', 'O', 'P', 0, },  1, IMP|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* ec */	{ { 'C', 'P', 'X', 0, },  3, ABS|NORM, },
	/* ed */	{ { 'S', 'B', 'C', 0, },  3, ABS|NORM, },
	/* ee */	{ { 'I', 'N', 'C', 0, },  3, ABS|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* f0 */	{ { 'B', 'E', 'Q', 0, },  2, REL|FORK, },
	/* f1 */	{ { 'S', 'B', 'C', 0, },  2, INY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* f5 */	{ { 'S', 'B', 'C', 0, },  2, ZPX|NORM, },
	/* f6 */	{ { 'I', 'N', 'C', 0, },  2, ZPX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* f8 */	{ { 'S', 'E', 'D', 0, },  1, IMP|NORM, },
	/* f9 */	{ { 'S', 'B', 'C', 0, },  3, ABY|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
	/* fd */	{ { 'S', 'B', 'C', 0, },  3, ABX|NORM, },
	/* fe */	{ { 'I', 'N', 'C', 0, },  3, ABX|NORM, },
	/* 00 */	{ { '?', '?', '?', 0, },  1, ILL|NORM, },
};
