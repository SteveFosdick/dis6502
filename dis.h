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
#include <stdnoreturn.h>

extern int sevenbit;  /* if true, mask character data with 0x7f
			 to ignore MSB */

typedef uint16_t addr_t;

#define NPREDEF 10

extern char *predef[];
extern int  npredef;
extern char *global_file;
extern char *progname;

extern int  bopt;
enum boot_mode { UNKNOWN, RAW_BINARY, ATARI_LOAD, C64_LOAD, ATARI_BOOT };
extern int global_base_address, global_vector_address;

extern int asmout;
extern unsigned char f[];
extern unsigned char d[];
extern long offset[];

#define getword(x) (d[x] + (d[x + 1] << 8))
#define getbyte(x) (d[x])

/* f bits */

#define LOADED (1 << 0)		/* Location loaded */
#define JREF   (1 << 1)		/* Referenced as jump/branch dest */
#define DREF   (1 << 2)		/* Referenced as data */
#define SREF   (1 << 3)		/* Referenced as subroutine dest */
#define NAMED  (1 << 4)		/* Has a name */
#define TDONE  (1 << 5)		/* Has been traced */
#define ISOP   (1 << 6)		/* Is a valid instruction opcode */
#define OFFSET (1 << 7)		/* should be printed as an offset */

struct mnemonic {
	char name[4];			/* three-letter mnemonic name */
	int  length;			/* number of bytes */
	int  flags;			/* control flow and addressing mode */
};

extern struct mnemonic optbl[];

/* Flags */

/* Where control goes */

#define NORM (1 << 0)
#define JUMP (1 << 1)
#define FORK (1 << 2)
#define STOP (1 << 3)

#define CTLMASK (NORM|JUMP|FORK|STOP)

/* Instruction format */

#define IMM  (1 << 5)
#define ABS  (1 << 6)
#define ACC  (1 << 7)
#define IMP  (1 << 8)
#define INX  (1 << 9)
#define INY  (1 << 10)
#define ZPX  (1 << 11)
#define ABX  (1 << 12)
#define ABY  (1 << 13)
#define REL  (1 << 14)
#define IND  (1 << 15)
#define ZPY  (1 << 16)
#define ZPG  (1 << 17)
#define ILL  (1 << 18)

#define ADRMASK (IMM|ABS|ACC|IMP|INX|INY|ZPX|ABX|ABY|REL|IND|ZPY|ZPG|ILL)

struct ref_chain {
	struct ref_chain *next;
	int who;
};

struct ref_chain *get_ref(addr_t loc);
char *get_name(addr_t loc);

struct print_cfg {
    const char *lab;
    const char *byte;
    const char *word;
    const char *imm;
    const char *equ;
    const char *org;
    const char *data;
};

extern struct print_cfg pf_orig, pf_beebasm, pf_lanc, *pf_selected;

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

extern FILE *yyin;
int lineno;

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

/* in ref.c: */
void save_ref (addr_t refer, addr_t refee);
void save_name (addr_t loc, char *name);

/* in trace_queue.c: */
void init_trace_queue (void);
int trace_queue_empty (void);
void push_trace_queue (addr_t addr);
addr_t pop_trace_queue (void);


/* in main.c: */
noreturn void crash (const char *fmt, ...) __attribute__ ((format(printf, 1, 2)));
void *emalloc(size_t bytes) __attribute__((malloc));
void get_predef (void);


#endif /* _DIS6502_H */
