/*
 * dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
 *
 * $Id: main.c 26 2004-01-17 23:28:23Z eric $
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


#include <stdlib.h>

#include "dis.h"

int sevenbit = 0;  /* if true, mask character data with 0x7f to ignore MSB */

#define NTSTART 500

char *cur_file = NULL;			/* the file thats open */
int  pre_index = 0;

int  tstart[NTSTART];			/* .trace directive keep locations */
int  tstarti = 0;

#define RTSTAB_MAX 50

int rtstab_addr [RTSTAB_MAX];		/* .rtstab directive */
int rtstab_size [RTSTAB_MAX];
int rtstab_count = 0;

#define JTAB2_MAX 50

int jtab2_addr_low  [JTAB2_MAX];	/* .jtab2 directive */
int jtab2_addr_high [JTAB2_MAX];	/* .jtab2 directive */
int jtab2_size      [JTAB2_MAX];
int jtab2_count = 0;

VALUE token;

unsigned char d[0x10000];	 	/* The data */
unsigned char f[0x10000];		/* Flags for memory usage */ 
long offset [0x10000];                  /* label offset */


#define RUNLOC  0x2e0
#define INITLOC 0x2e2


void crash (char *p)
{
	fprintf(stderr, "%s: %s\n", progname, p);
	if (cur_file != NULL)
		fprintf(stderr, "Line %d of %s\n", lineno+1, cur_file);
	exit(1);
}


void add_trace (addr_t addr)
{
  if (f [addr] & TDONE)
    return;
  push_trace_queue (addr);
}


void trace_inst (addr_t addr)
{
  int opcode;
  register struct info *ip; 
  int operand;
  int istart;

  for (;;)
    {
      if (f[addr] & TDONE)
	return;

      f[addr] |= TDONE;

      istart = addr;
      opcode = getbyte(addr);
      ip = &optbl[opcode];

      if (ip->flag & ILL)
	return;

      f[addr] |= ISOP;

      addr++;

      /* Get the operand */

      switch(ip->nb)
	{
	case 1:
	  operand = 0;  /* only to avoid "may be used unitialized" warning */
	  break;
	case 2:
	  operand = getbyte(addr);
	  f[addr++] |= TDONE;
	  break;
	case 3:
	  operand = getword(addr);
	  f[addr++] |= TDONE;
	  f[addr++] |= TDONE;
	  break;
	}

      /* Mark data references */

      switch (ip->flag & ADRMASK)
	{
	case IMM:
	case ACC:
	case IMP:
	case REL:
	case IND:
	  break;
	case ABS:
	  if (ip->flag & (JUMP | FORK))
	    break;
	  /* Fall into */
	case ABX:
	case ABY:
	case INX:
	case INY:
	case ZPG:
	case ZPX:
	case ZPY:
	  f[operand] |= DREF;
	  save_ref(istart, operand);
	  break;
	default:
	  crash("Optable error");
	  break;
	}

      /* Trace the next instruction */

      switch (ip->flag & CTLMASK)
	{
	case NORM:
	  break;
	case JUMP:
	  f[operand] |= JREF;
	  save_ref(istart, operand);
	  add_trace(operand);
	  return;
	case FORK:
	  if (ip->flag & REL) 
	    {
	      if (operand > 127) 
		operand = (~0xff | operand);
	      operand = operand + addr;
	      f[operand] |= JREF;
	    }
	  else
	    {
	      f[operand] |= SREF;
	    }
	  save_ref(istart, operand);
	  add_trace(operand);
	  break;
	case STOP:
	  return;
	default:
	  crash("Optable error");
	}
    }
}


void trace_all (void)
{
  while (! trace_queue_empty ())
    trace_inst (pop_trace_queue ());
}


void start_trace (addr_t loc, char *name)
{
	fprintf(stderr, "Trace: %4x %s\n", loc, name);
	f[loc] |= (NAMED | SREF);
	if (!get_name(loc))
		save_name(loc, name);
	save_ref(0, loc);
	add_trace(loc);
}
	

void do_ptrace (void)
{
  int i;
  for (i = 0; i<tstarti; i++)
    {
      char *trace_sym = (char *) malloc (6);
      sprintf (trace_sym, "P%04x", tstart [i]);
      start_trace(tstart[i], trace_sym);
    }
}


void do_rtstab (void)
{
  int i, j;
  int loc, code;

  for (i = 0; i < rtstab_count; i++)
    {
      loc = rtstab_addr [i];
      for (j = 0; j < rtstab_size [i]; j++)
	{
	  char *trace_sym = (char *) malloc (6);
	  code = d [loc] + (d [loc + 1] << 8) + 1;
	  sprintf (trace_sym, "T%04x", code);
	  start_trace (code, trace_sym);
	  loc += 2;
	}
    }
}

void do_jtab2 (void)
{
  int i, j;
  int loc_l, loc_h, code;
  for (i = 0; i < jtab2_count; i++)
    {
      loc_l = jtab2_addr_low [i];
      loc_h = jtab2_addr_high [i];
      for (j = 0; j < jtab2_size [i]; j++)
	{
	  char *trace_sym = (char *) malloc (6);
	  code = d [loc_l + j] + (d [loc_h + j] << 8);
	  sprintf (trace_sym, "T%04x", code);
	  start_trace (code, trace_sym);
	}
    } 
}



int main (int argc, char *argv[])
{
	initopts(argc, argv);

	init_trace_queue ();

	if (npredef > 0) {
		cur_file = predef[0];
		pre_index++;
		yyin = fopen(cur_file, "r");
		if (!yyin)
			crash ("Cant open predefine file");
		get_predef();
	}

	switch (bopt) 
	  {
	  case RAW_BINARY: 
	    binaryloadfile();
	    break;
	  case ATARI_LOAD:
	    loadfile();
	    break;
	  case C64_LOAD:
	    c64loadfile();
	    break;
	  case ATARI_BOOT:
	    loadboot();
	    break;
	  default:
	    crash ("file format must be specified");
	  }

	do_ptrace ();
	do_rtstab ();
	do_jtab2 ();

	trace_all ();

	dumpitout();

	exit(0);
}


void get_predef (void)
{
	long loc, loc2;
	int i;
	int size;
	char *name;

	for(;;) 
		switch (yylex()) {
		case '\n':
			break;
		case COMMENT:
		  break;
		case 0:
			return;
		case TRTSTAB:
		  if (yylex() != NUMBER)
		    crash(".rtstab needs an address operand");
		  loc = token.ival;
		  if (loc > 0x10000 || loc < 0)
		    crash("Number out of range");
		  if (yylex() != ',')
		    crash(".rtstab needs a comma");
		  if (yylex() != NUMBER)
		    crash(".rtstab needs a comma");
		  size = token.ival;
		  rtstab_addr [rtstab_count] = loc;
		  rtstab_size [rtstab_count++] = size;
		  break;
		case TJTAB2:
		  if (yylex() != NUMBER)
		    crash(".jtab2 needs a number operand");
		  if (token.ival > 0x10000 || token.ival < 0)
		    crash("Number out of range");
		  jtab2_addr_low [jtab2_count] = token.ival;
		  if (yylex() != ',')
		    crash(".jtab2 needs a comma");
		  if (yylex() != NUMBER)
		    crash(".jtab2 needs a number operand");
		  if (token.ival > 0x10000 || token.ival < 0)
		    crash("Number out of range");
		  jtab2_addr_high [jtab2_count] = token.ival;
		  if (yylex() != ',')
		    crash(".jtab2 needs a comma");
		  if (yylex() != NUMBER)
		    crash(".jtab2 needs a number operand");
		  jtab2_size [jtab2_count++] = token.ival;
		  break;
		case TSTART:
			if (yylex() != NUMBER) 
				crash(".trace needs a number operand");
			loc = token.ival;
			if (loc > 0x10000 || loc < 0)
				crash("Number out of range");
			if (tstarti == NTSTART) 
				crash("Too many .trace directives");
			tstart[tstarti++] = loc;
			while (yylex() != '\n')
				;
			break;
		case TSTOP:
			if (yylex() != NUMBER) 
				crash(".stop needs a number operand");
			loc = token.ival;
			if (loc > 0x10000 || loc < 0)
				crash("Number out of range");
			f[loc] |= TDONE;
			while (yylex() != '\n')
				;
			break;
		case NAME:
			name = token.sval;
			switch (yylex ())
			  {
			  case EQ:
			    if (yylex() != NUMBER)
			      crash("EQ operand must be a number");
			    loc = token.ival;
			    if (loc > 0x10000 || loc < 0)
			      crash("Number out of range");
			    f[loc] |= NAMED;
			    save_name(loc, name); 
			    break;
			  case EQS:
			    if (yylex() != NUMBER)
			      crash("EQS operand must be a number");
			    loc = token.ival;
			    if (loc > 0x10000 || loc < 0)
			      crash("Number out of range");
			    if (yylex() != ',')
			      crash(".eqs needs a comma");
			    if (yylex() != NUMBER)
			      crash("EQS operand must be a number");
			    size = token.ival;
			    f[loc] |= NAMED;
			    save_name(loc, name);
			    for (i = 1; i < size; i++)
			      {
				f [loc + i] |= OFFSET;
				offset [loc + i] = -i;
			      }
			    break;
			  default:
			    crash("name can only be used with equate in defines file");
			    break;
			  }
			while (yylex() != '\n') 
			  ;
			break;
		case OFS:
		  if (yylex() != NUMBER)
		    crash("EQ operand must be a number");
		  loc = token.ival;
		  if (loc > 0x10000 || loc < 0)
		    crash("Number out of range");
		  if (yylex() != ',')
		    crash(".ofs needs a comma");
		  if (yylex() != NUMBER)
		    crash("EQ operand must be a number");
		  loc2 = token.ival;
		  if (loc2 > 0x10000 || loc2 < 0)
		    crash("Number out of range");
		  /*$$$*/
		  f[loc] |= OFFSET;
		  offset[loc] = loc2 - loc;
		  break;
		default:
			crash("Invalid line in predef file");
		}
}

void loadboot (void)
{
	struct boot_hdr {
		unsigned char flags;
		unsigned char nsec;
		unsigned char base_low;
		unsigned char base_hi;
		unsigned char init_low;
		unsigned char init_hi;
	} bh;

	FILE *fp;
	int base_addr;
	register int i;
	size_t len;

	fp = fopen(file, "r");
	cur_file = NULL;
	if (!fp) { 
		fprintf(stderr, "Cant open %s\n", file);

		exit(1);
	}

	if(fread((char *)&bh, sizeof(bh), 1, fp) != 1) 
		crash("Input too short");
	
	base_addr = bh.base_low + (bh.base_hi << 8);
	len = bh.nsec * 128;
	rewind(fp);
	if (fread((char *)&d[base_addr], 1, len, fp) != len) 
		crash("input too short");

	for(i = base_addr; len > 0; len--) 
		f[i++] |= LOADED;

	start_trace(base_addr+6, "**BOOT**");
}


void loadfile (void)
{
	FILE *fp;
	int base_addr;
	int last_addr;
	register int i;
	int had_header;
	int tmp;

	had_header = 0;
	fp = fopen(file, "r");
	cur_file = NULL;
	if (!fp) { 
		fprintf(stderr, "Cant open %s\n", file);

		exit(1);
	}
	for(;;) {

		i = getc(fp);

		if (i == EOF) {
			if (f[RUNLOC] & LOADED & f[RUNLOC+1]) {
				i = getword(RUNLOC);
				start_trace(i, "**RUN**");
			}
			return;
		}

		i = i | (getc(fp) << 8);
		if (i == 0xffff)  {
			had_header = 1;
			base_addr = getc(fp);
			base_addr = base_addr | (getc(fp) << 8);
			if (base_addr < 0 || base_addr > 0xffff) 
				crash("Invalid base addr in input file");
		} else {
			if (!had_header)
				crash("Invalid header in input file");
			base_addr = i;
		}

		last_addr = getc(fp);
		last_addr = last_addr | (getc(fp) << 8);
		if (last_addr < base_addr || last_addr > 0xffff) 
			crash("Invalid length in input file");

		printf("Load:  %4x -> %4x\n", base_addr, last_addr);
		for(i = base_addr; i <= last_addr; i++) {
			tmp = getc(fp);
			if (tmp == EOF) 
				crash("File too small");
			d[i] = tmp;
			f[i] |= LOADED;
		}

		if (f[INITLOC] & LOADED & f[INITLOC+1])  {
			i = getword(INITLOC);
			start_trace(i, "**INIT**");
		}

		f[INITLOC] &= ~LOADED;
		f[INITLOC+1] &= ~LOADED;
	}

}


void c64loadfile (void)
{
	FILE *fp;
	addr_t base_addr,i;
	int c;

	fp = fopen(file, "r");
	cur_file = NULL;
	if (!fp) { 
		fprintf(stderr, "Cant open %s\n", file);

		exit(1);
	}

	base_addr = getc(fp);
	i = ( base_addr += ( (unsigned int)getc(fp) << 8 ) );

	while( (c = getc(fp)) != EOF) {
		d[i] = c;
		f[i++] |= LOADED;
		}

	start_trace(base_addr, "**C64BIN**");
}


void binaryloadfile (void)
{
  FILE *fp;
  addr_t i;
  int c;
  addr_t reset, irq, nmi;

  fp = fopen (file, "r");

  cur_file = NULL;

  if (!fp)
    {
      fprintf (stderr, "Can't open %s\n", file);
      exit (1);
    }

  i = base_address;

  while ((c = getc(fp)) != EOF)
    {
      d [i] = c;
      f [i++] |= LOADED;
    }

  reset = vector_address - 4;
  irq = vector_address - 2;
  nmi = vector_address - 6;

  fprintf (stderr, "base: %04x  reset: %04x  irq: %04x  nmi: %04x\n", base_address, reset, irq, nmi);

  start_trace ((d [reset+1] << 8) | d [reset], "RESET");
  start_trace ((d [irq  +1] << 8) | d [irq  ], "IRQ");
  start_trace ((d [nmi  +1] << 8) | d [nmi  ], "NMI");
}

int
yywrap()
{
	(void)fclose(yyin);
	if (npredef == pre_index) {
		return(1);
	} else  {
		lineno = 0;
		cur_file = predef[pre_index];
		pre_index++;
		yyin = fopen(cur_file, "r");
		if (!yyin) 
			crash("Can't open predefines file");
		return (0);
	}
}
