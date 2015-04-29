#ifndef _DIS6502_H
#define _DIS6502_H

/*
 * dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
 *
 * $Id: dis.h 26 2004-01-17 23:28:23Z eric $
 * Copyright 2000-2003 Eric Smith <eric@brouhaha.com>
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

#include <stdint.h>
#include <stdio.h>

extern int sevenbit;  /* if true, mask character data with 0x7f
			 to ignore MSB */

typedef uint16_t addr_t;

#define NPREDEF 10

extern char *predef[];
extern int  npredef;
extern char *file;
extern char *progname;

extern int  bopt;
enum boot_mode { UNKNOWN, RAW_BINARY, ATARI_LOAD, C64_LOAD, ATARI_BOOT };
extern int base_address, vector_address;

extern int asmout;
extern unsigned char f[];
extern unsigned char d[];
extern long offset[];

#define getword(x) (d[x] + (d[x + 1] << 8))
#define getbyte(x) (d[x])

/* f bits */

#define LOADED 1			/* Location loaded */
#define JREF   2			/* Referenced as jump/branch dest */
#define DREF   4			/* Referenced as data */
#define SREF   8			/* Referenced as subroutine dest */
#define NAMED  0x10			/* Has a name */
#define TDONE  0x20			/* Has been traced */
#define ISOP   0x40			/* Is a valid instruction opcode */
#define OFFSET 0x80                     /* should be printed as an offset */

struct info {
	char opn[4];
	int  nb;
	int  flag;
};

extern struct info optbl[];

/* Flags */

/* Where control goes */

#define NORM 1
#define JUMP 2
#define FORK 4
#define STOP 8

#define CTLMASK (NORM|JUMP|FORK|STOP)

/* Instruction format */

#define IMM  0x20
#define ABS  0x40
#define ACC  0x80
#define IMP  0x100
#define INX  0x200
#define INY  0x400
#define ZPX  0x800
#define ABX  0x1000
#define ABY  0x2000
#define REL  0x4000
#define IND  0x8000
#define ZPY  0x10000
#define ZPG  0x20000
#define ILL  0x40000

#define ADRMASK (IMM|ABS|ACC|IMP|INX|INY|ZPX|ABX|ABY|REL|IND|ZPY|ZPG|ILL)

struct ref_chain {
	struct ref_chain *next;
	int who;
};

struct ref_chain *get_ref(addr_t loc);
char *get_name(addr_t loc);

/* lex junk */

#define EQ 256
#define NUMBER 257
#define NAME 258
#define COMMENT 259
#define LI 260
#define TSTART 261
#define TSTOP 262
#define TRTSTAB 263
#define TJTAB2 264
#define EQS 265
#define OFS 266

extern FILE *yyin, *yyout;
int lineno;

int yywrap(), yyerror();
char *emalloc(size_t bytes);

typedef union  {
	int ival;
	char *sval;
} VALUE;

extern VALUE token;


/* in scanner generated from lex.l: */
int yylex (void);


/* in initopts.c: */
void initopts (int argc, char *argv[]);

/* in print.c: */
void dumpitout (void);
int pchar (int c);
void print_bytes (addr_t addr);
int print_inst(addr_t addr);
int print_data (addr_t i);
void print_refs (void);

/* in ref.c: */
void save_ref (addr_t refer, addr_t refee);
void save_name (addr_t loc, char *name);

/* in trace_queue.c: */
void init_trace_queue (void);
int trace_queue_empty (void);
void push_trace_queue (addr_t addr);
addr_t pop_trace_queue (void);


/* in main.c: */
void crash (char *p) __attribute__ ((noreturn));
void get_predef (void);

void loadboot (void);
void loadfile (void);
void c64loadfile (void);
void binaryloadfile (void);

#endif /* _DIS6502_H */
