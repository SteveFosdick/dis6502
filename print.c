/*
 * dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
 *
 * $Id: print.c 26 2004-01-17 23:28:23Z eric $
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
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <unistd.h>

#include "dis.h"


static char *lname (addr_t i, int offset_ok)
{
	static char buf[20];
	char t;

	if (f[i] & NAMED)
		return(get_name(i));
	if (f[i] & OFFSET) {
		strcpy(buf, get_name(i + offset[i]));
		sprintf (buf + strlen (buf), "%c%ld",
			 (offset [i] <= 0) ? '+' : '-',
			 labs (offset [i]));
		return (buf);
	}
	if (f[i] & SREF)
		t = 'S';
	else if (f[i] & JREF)
		t = 'L';
	else if (f[i] & DREF)
	  {
	    if (i <= 0xff)
	      t = 'Z';
	    else
	      t = 'D';
	  }
	else
		t = 'X';

	if (i <= 0xff)
	  sprintf(buf, "%c%02x", t, i);
	else
	  sprintf(buf, "%c%04x", t, i);

	return (buf);
}


static int print_label (addr_t i)
{
  if ((f[i] & (NAMED | JREF | SREF | DREF)) &&
      ! (f [i] & OFFSET))
    {
      printf("%s", lname(i, 0));
      return (1);
    }
  else
    return (0);
}

void dumpitout (void)
{
  uint32_t i;  /* must be larger than an addr_t */

  for(i = 0; i<0x10000;)
    {
      if (f[i] & LOADED)
	{
	  if ((i == 0) || (! (f[i-1] & LOADED)))
	    printf ("\t.org\t$%04x\n", i);

	  if (f[i] & SREF && f[i] & ISOP)
	    printf("\n");

	  if (! asmout)
	    {
	      printf("%04x  ",i);
	      print_bytes(i);
	    }
	  if (print_label(i))
	    printf (":");
	  printf ("\t");
	  if (f[i] & ISOP)
	    i += print_inst(i);
	  else
	    i += print_data(i);
	  printf("\n");

	}
      else
	{
	  if (print_label (i))
	    {
	      if (i <= 0xff)
		printf ("\t.equ\t$%02x\n", i);
	      else
		printf ("\t.equ\t$%04x\n", i);
	    }
	  i++;
	}
    }

  if (! asmout)
    print_refs();
}

int pchar (int c)
{
        if (sevenbit)
                c &= 0x7f;
	if (isascii(c) && isprint(c))
		return(c);
	return('.');
}

void print_bytes (addr_t addr)
{
	struct mnemonic *ip;

	if ((f[addr] & ISOP) == 0) {
		printf("           ");
		return;
	}

	ip = &optbl[getbyte(addr)];

	switch (ip->length) {
		case 1:
			printf("%02x         ", getbyte(addr));
			break;
		case 2:
			printf("%02x %02x      ", getbyte(addr), getbyte(addr + 1));
			break;
		case 3:
			printf("%02x %02x %02x   ", getbyte(addr), getbyte(addr + 1), getbyte(addr + 2));
			break;
	}
}

int print_inst(addr_t addr)
{
	int opcode;
	struct mnemonic *ip;
	int operand;

	opcode = getbyte(addr);
	ip = &optbl[opcode];

	printf("%s", ip->name);

	addr++;

	switch(ip->length) {
		case 1:
			operand = 0;  /* only to avoid "may be used
					 unitialized" warning */
			break;
		case 2:
			operand = getbyte(addr);
			break;
		case 3:
			operand = getword(addr);
			break;
	}

	if (ip->flags & REL) {
		if (operand > 127)
			operand = (~0xff | operand);
		operand = operand + ip->length + addr - 1;
	}

	switch (ip->flags & ADRMASK) {
		case IMM:
			printf("\t#$%02x\t; %d %c", operand, operand, pchar(operand));
			break;
		case ACC:
		case IMP:
			break;
		case REL:
		case ABS:
		case ZPG:
			printf("\t%s", lname(operand, 1));
			break;
		case IND:
			printf("\t(%s)", lname(operand, 1));
			break;
		case ABX:
		case ZPX:
			printf("\t%s,X", lname(operand, 1));
			break;
		case ABY:
		case ZPY:
			printf("\t%s,Y", lname(operand, 1));
			break;
		case INX:
			printf("\t(%s,X)", lname(operand, 1));
			break;
		case INY:
			printf("\t(%s),Y", lname(operand, 1));
			break;
		default:
			break;
	}

	return(ip->length);

}

int print_data (addr_t i)
{
	int count;
	int j;
	int start;

	start = i;
	printf(".byte\t$%02x", getbyte(i));
	count = 1;
	i++;

	for (j = 1; j < 8; j++) {
		if (f[i] & (JREF | SREF | DREF) || ((f[i] & LOADED) == 0))
			break;
		else
			printf(",$%02x", getbyte(i));
		i++;
		count++;
	}
	for (j = count; j < 8; j++)
		printf("   ");

	printf("\t; \"");

	for (j = start; j < i ; j++)
			printf("%c", pchar((int)getbyte(j)));

	printf ("\"");

	return (count);
}

void print_refs (void)
{
	char tname[50];
	char cmd[200];
	FILE *fp;
	struct ref_chain *rp;
	uint32_t i;  /* must be larger than an addr_t */
	int npline;

	sprintf(tname, "dis.%d", getpid());
	sprintf(cmd, "sort %s; rm %s", tname, tname);

	fp = fopen(tname, "w");
	if (!fp)
		crash("Can't open temporary file %s", tname);

	for (i = 0; i<0x10000; i++) {
		if(f[i] & (JREF|SREF|DREF)) {
			rp = get_ref(i);
			if (!rp) {
				fprintf(stderr, "No ref %d\n", i);
				break;
			}

			fprintf(fp, "%-8s  %04x   ", lname(i, 1), i);
			npline = 0;
			while (rp) {
				fprintf(fp, "%04x ", rp->who);
				npline++;
				if (npline == 12) {
					fprintf(fp,"\n");
					fprintf(fp,"%-8s  %04x   ",lname(i, 1),i);
					npline = 0;
				}
				rp = rp->next;
			}
			fprintf(fp, "\n");
		}

	}

	fclose(fp);

	printf("\n\n\n\n\nCross References\n\n");
	printf("%-8s  Value  References\n", "Symbol");
	fflush (stdout);

	system(cmd);
}
