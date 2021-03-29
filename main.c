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


#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "dis.h"

int sevenbit = 0;  /* if true, mask character data with 0x7f to ignore MSB */

#define NTSTART 500

static char *cur_file = NULL;          /* the file thats open */
static int  pre_index = 0;

static int  tstart[NTSTART];           /* .trace directive keep locations */
static int  tstarti = 0;

#define RTSTAB_MAX 50

static int rtstab_addr [RTSTAB_MAX];       /* .rtstab directive */
static int rtstab_size [RTSTAB_MAX];
static int rtstab_count = 0;

static int rtstab2_addr_low[RTSTAB_MAX];   /* rtstab2 directive */
static int rtstab2_addr_high[RTSTAB_MAX];
static int rtstab2_size[RTSTAB_MAX];
static int rtstab2_count = 0;

#define JTAB2_MAX 50

static int jtab2_addr_low  [JTAB2_MAX];    /* .jtab2 directive */
static int jtab2_addr_high [JTAB2_MAX];    /* .jtab2 directive */
static int jtab2_size      [JTAB2_MAX];
static int jtab2_count = 0;

#define JTAB_MAX  50

static int jtab_addr       [JTAB_MAX];
static int jtab_size       [JTAB_MAX];
static int jtab_count = 0;

VALUE token;

unsigned char d[0x10000];       /* The data */
unsigned char f[0x10000];       /* Flags for memory usage */
long offset [0x10000];                  /* label offset */


#define RUNLOC  0x2e0
#define INITLOC 0x2e2


void crash (const char *fmt, ...)
{
    char buffer[1024]; /* ugly, but not too bad */

    va_list ap;
    va_start(ap, fmt);

    int result = vsnprintf(buffer, sizeof(buffer), fmt, ap);
    assert(result > 0);

    fprintf(stderr, "%s: %s\n", progname, buffer);
    if (cur_file != NULL)
        fprintf(stderr, "Line %d of %s\n", lineno + 1, cur_file);

    va_end(ap);
    exit(EXIT_FAILURE);
}


void *emalloc(size_t bytes)
{
    void *ptr;

    if ((ptr = calloc(1, bytes)) == NULL) {
        crash("Out of memory");
    }
    return ptr;
}


static void add_trace (addr_t addr)
{
    if (f [addr] & TDONE)
        return;
    push_trace_queue (addr);
}


static void trace_inst (addr_t addr)
{
    int opcode;
    const struct mnemonic *ip;
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

        if (ip->flags & ILL)
            return;

        f[addr++] |= ISOP;

        /* Get the operand */

        operand = 0;  /* only to avoid "may be used unitialized" warning */
        switch(ip->length) {
            case 1:
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

        switch (ip->flags & ADRMASK) {
            case IMM:
            case ACC:
            case IMP:
            case REL:
                break;
            case ABS:
            case ABX:
                if (ip->flags & (JUMP | FORK))
                    break;
                /* FALLS THROUGH */
            case ABY:
            case IND:
            case INX:
            case INY:
            case ZPG:
            case ZPI:
            case ZPX:
            case ZPY:
                f[operand] |= DREF;
                save_ref(istart, operand);
                break;
            default:
                crash("Unknown addressing mode in opcode table");
            break;
        }

        /* Trace the next instruction */

        switch (ip->flags & CTLMASK) {
            case NORM:
                break;
            case JUMP:
                f[operand] |= JREF;
                save_ref(istart, operand);
                add_trace(operand);
                return;
            case FORK:
                if (ip->flags & REL) {
                    if (operand > 127)
                        operand = (~0xff | operand);
                    operand = operand + addr;
                    f[operand] |= JREF;
                }
                else
                    f[operand] |= SREF;
                save_ref(istart, operand);
                add_trace(operand);
                break;
            case STOP:
                return;
            default:
                crash("Unknown control flow in opcode table");
        }
    }
}

static void trace_all (void)
{
  while (! trace_queue_empty ())
    trace_inst (pop_trace_queue ());
}


static void start_trace (addr_t loc, char *name)
{
    fprintf(stderr, "Trace: %4x %s\n", loc, name);
    f[loc] |= (NAMED | SREF);
    if (!get_name(loc))
        save_name(loc, name);
    save_ref(0, loc);
    add_trace(loc);
}


static void do_ptrace (void)
{
  int i;
  for (i = 0; i<tstarti; i++)
    {
      char *trace_sym = emalloc (6);
      sprintf (trace_sym, "P%04x", tstart [i]);
      start_trace(tstart[i], trace_sym);
    }
}


static void do_rtstab (void)
{
    int i, j;
    int loc, code;

    for (i = 0; i < rtstab_count; i++) {
        loc = rtstab_addr [i];
        for (j = 0; j < rtstab_size [i]; j++) {
            char *trace_sym = emalloc (7);
            code = d [loc] + (d [loc + 1] << 8) + 1;
            sprintf (trace_sym, "T%04x", code);
            start_trace (code, trace_sym);
            loc += 2;
        }
    }
}

static void do_jtab2 (void)
{
    int i, j;
    int loc_l, loc_h, code;
    for (i = 0; i < jtab2_count; i++) {
        loc_l = jtab2_addr_low [i];
        loc_h = jtab2_addr_high [i];
        for (j = 0; j < jtab2_size [i]; j++) {
            char *trace_sym = emalloc (6);
            code = d [loc_l + j] + (d [loc_h + j] << 8);
            sprintf (trace_sym, "T%04x", code);
            start_trace (code, trace_sym);
        }
    }
}

static void do_rtstab2 (void)
{
    for (int i = 0; i < rtstab2_count; i++) {
        int loc_l = rtstab2_addr_low [i];
        int loc_h = rtstab2_addr_high [i];
        for (int j = 0; j < rtstab2_size[i]; j++) {
            char *trace_sym = emalloc (7);
            int code = (d [loc_l + j] + (d [loc_h + j] << 8))+1;
            sprintf (trace_sym, "T%04x", code);
            start_trace (code, trace_sym);
        }
    }
}

static void do_jtab (void)
{
    int i, j;
    int loc, code;
    for (i = 0; i < jtab_count; i++) {
        loc = jtab_addr [i];
        fprintf(stderr, "%i:%04X\n", i, loc);
        for (j = 0; j < jtab_size [i]; j++) {
            char *trace_sym = emalloc (6);
            code = d [loc + j*2] + (d [loc + 1 + j*2] << 8);
            sprintf (trace_sym, "T%04x", code);
            start_trace (code, trace_sym);
        }
    }
}

static void loadboot (const char *file)
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
    int i;
    size_t len;

    fp = fopen(file, "r");
    cur_file = NULL;
    if (!fp)
        crash("Can't open %s", file);

    if(fread((char *)&bh, sizeof(bh), 1, fp) != 1)
        crash("Input too short");

    base_addr = bh.base_low + (bh.base_hi << 8);
    len = bh.nsec * 128;
    rewind(fp);
    if (fread((char *)&d[base_addr], 1, len, fp) != len)
        crash("Input too short");

    fclose(fp);

    for(i = base_addr; len > 0; len--)
        f[i++] |= LOADED;

    start_trace(base_addr+6, "**BOOT**");
}


static void loadfile (const char *file)
{
    FILE *fp;
    int base_addr;
    int last_addr;
    int i;
    int had_header;
    int tmp;

    had_header = 0;
    fp = fopen(file, "r");
    cur_file = NULL;
    if (!fp)
        crash("Can't open %s", file);
    for(;;) {
        i = getc(fp);

        if (i == EOF) {
            if (f[RUNLOC] & LOADED & f[RUNLOC + 1]) {
                i = getword(RUNLOC);
                start_trace(i, "**RUN**");
            }
            fclose(fp);
            return;
        }

        i = i | (getc(fp) << 8);
        if (i == 0xffff)  {
            had_header = 1;
            base_addr = getc(fp);
            base_addr = base_addr | (getc(fp) << 8);
            if (base_addr < 0 || base_addr > 0xffff)
                crash("Invalid base address in input file");
        }
        else {
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

        if (f[INITLOC] & LOADED & f[INITLOC + 1])  {
            i = getword(INITLOC);
            start_trace(i, "**INIT**");
        }

        f[INITLOC] &= ~LOADED;
        f[INITLOC + 1] &= ~LOADED;
    }
}


static void c64loadfile (const char *file)
{
    FILE *fp;
    addr_t base_addr,i;
    int c;

    fp = fopen(file, "r");
    cur_file = NULL;
    if (!fp)
        crash("Can't open %s", file);

    base_addr = getc(fp);
    i = ( base_addr += ( (unsigned int)getc(fp) << 8 ) );

    while( (c = getc(fp)) != EOF) {
        d[i] = c;
        f[i++] |= LOADED;
    }

    fclose(fp);
    start_trace(base_addr, "**C64BIN**");
}


static void binaryloadfile (const char *file, int base_address, int vector_address)
{
    FILE *fp;
    addr_t i;
    int c;
    addr_t reset, irq, nmi;

    fp = fopen (file, "r");

    cur_file = NULL;

    if (!fp)
        crash("Can't open %s", file);

    i = base_address;

    while ((c = getc(fp)) != EOF) {
        d [i] = c;
        f [i++] |= LOADED;
    }

    fclose(fp);

    reset = vector_address - 4;
    irq = vector_address - 2;
    nmi = vector_address - 6;

    fprintf (stderr, "base: %04x  reset: %04x  irq: %04x  nmi: %04x\n", base_address, reset, irq, nmi);

    if (reset < i)
        start_trace ((d [reset + 1] << 8) | d [reset], "RESET");
    if (irq < i)
        start_trace ((d [irq   + 1] << 8) | d [irq  ], "IRQ");
    if (nmi < i)
        start_trace ((d [nmi   + 1] << 8) | d [nmi  ], "NMI");
}

static void get_predef (void)
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
                    crash(".rtstab needs a number operand");
                size = token.ival;
                rtstab_addr [rtstab_count] = loc;
                rtstab_size [rtstab_count++] = size;
                break;
            case TRTSTAB2:
                if (yylex() != NUMBER)
                    crash(".rtstab2 needs an address operand");
                if (token.ival > 0x10000 || token.ival < 0)
                    crash("Number out of range");
                rtstab2_addr_low[rtstab2_count] = token.ival;
                if (yylex() != ',')
                    crash(".rtstab2 needs a comma");
                if (yylex() != NUMBER)
                    crash(".rtstab needs a number operand");
                if (token.ival > 0x10000 || token.ival < 0)
                    crash("Number out of range");
                rtstab2_addr_high[rtstab2_count] = token.ival;
                if (yylex() != ',')
                    crash(".rtstab2 needs a comma");
                if (yylex() != NUMBER)
                    crash(".rtstab needs a number operand");
                rtstab2_size[rtstab2_count++] = token.ival;
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
            case TJTAB:
                if (yylex() != NUMBER)
                    crash(".jtab needs an address operand");
                if (token.ival > 0x10000 || token.ival < 0)
                    crash("Address out of range");
                jtab_addr[jtab_count] = token.ival;
                if (yylex() != ',')
                    crash(".jtab needs a comma");
                if (yylex() != NUMBER)
                    crash(".jtab needs a size operand");
                fprintf(stderr, "jtab: %04X, %d\n", jtab_addr[jtab_count], token.ival);
                jtab_size [jtab_count++] = token.ival;
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
                switch (yylex ()) {
                    case EQ:
                        if (yylex() != NUMBER)
                            crash(".eq operand must be a number");
                        loc = token.ival;
                        if (loc > 0x10000 || loc < 0)
                            crash("Number out of range");
                        f[loc] |= NAMED;
                        save_name(loc, name);
                        break;
                    case EQS:
                        if (yylex() != NUMBER)
                            crash(".eqs operand must be a number");
                        loc = token.ival;
                        if (loc > 0x10000 || loc < 0)
                            crash("Number out of range");
                        if (yylex() != ',')
                            crash(".eqs needs a comma");
                        if (yylex() != NUMBER)
                            crash(".eqs operand must be a number");
                        size = token.ival;
                        f[loc] |= NAMED;
                        save_name(loc, name);
                        for (i = 1; i < size; i++) {
                            f [loc + i] |= OFFSET;
                            offset [loc + i] = -i;
                        }
                        break;
                    case TSTART:
                        if (yylex() != NUMBER)
                            crash(".trace needs a number operand");
                        loc = token.ival;
                        if (loc > 0x10000 || loc < 0)
                            crash("Number out of range");
                        start_trace(loc, name);
                        break;
                    default:
                        crash("Name can only be used with equate in predefines file");
                        break;
                }
                while (yylex() != '\n')
                    ;
                break;
            case OFS:
                if (yylex() != NUMBER)
                    crash(".ofs operand must be a number");
                loc = token.ival;
                if (loc > 0x10000 || loc < 0)
                    crash("Number out of range");
                if (yylex() != ',')
                    crash(".ofs needs a comma");
                if (yylex() != NUMBER)
                    crash(".ofs operand must be a number");
                loc2 = token.ival;
                if (loc2 > 0x10000 || loc2 < 0)
                    crash("Number out of range");
                /*$$$*/
                f[loc] |= OFFSET;
                offset[loc] = loc2 - loc;
                break;
            default:
                crash("Invalid line in predefines file");
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
            crash ("Can't open predefine file %s", cur_file);
        get_predef();
    }

    switch (bopt) {
        case RAW_BINARY:
            binaryloadfile(global_file, global_base_address, global_vector_address);
            break;
        case ATARI_LOAD:
            loadfile(global_file);
            break;
        case C64_LOAD:
            c64loadfile(global_file);
            break;
        case ATARI_BOOT:
            loadboot(global_file);
            break;
        default:
            crash ("File format must be specified");
    }

    do_ptrace ();
    do_rtstab ();
    do_rtstab2();
    do_jtab2 ();
    do_jtab ();

    trace_all ();

    dumpitout();

    exit(EXIT_SUCCESS);
}


int
yywrap()
{
    fclose(yyin);
    if (npredef == pre_index)
        return(1);
    else {
        lineno = 0;
        cur_file = predef[pre_index];
        pre_index++;
        yyin = fopen(cur_file, "r");
        if (!yyin)
            crash("Can't open predefines file %s", cur_file);
        return (0);
    }
}
