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

#define LABEL_SIZE 100

struct print_cfg pf_orig = {
    .lab  = "%s:\t",
    .byte = "$%02x",
    .word = "$%04x",
    .imm  = "#$%02x",
    .equ  = "%s\t.equ\t",
    .org  = "\t.org\t",
    .data = ".byte"
};

struct print_cfg pf_beebasm = {
    .lab  = ".%s\t",
    .byte = "&%02X",
    .word = "&%04X",
    .imm  = "#&%02X",
    .equ  = "%s\t=\t",
    .org  = "\tORG\t",
    .data = "EQUB"
};

struct print_cfg pf_lanc = {
    .lab  = "%s:\t",
    .byte = "$%02X",
    .word = "$%04X",
    .imm  = "#$%02X",
    .equ  = "%s\tEQU\t",
    .org  = "\tORG\t",
    .data = "DFB"
};

struct print_cfg pf_ca65 = {
    .lab  = "%s:\t",
    .byte = "$%02X",
    .word = "$%04X",
    .imm  = "#$%02X",
    .equ  = "%s\t=\t",
    .org  = "\t.org\t",
    .data = ".byte"
};

struct print_cfg pf_as = {
    .lab  = "%s:\t",
    .byte = "$%02X",
    .word = "$%04X",
    .imm  = "#$%02X",
    .equ  = "%s\tequ\t",
    .org  = "\torg\t",
    .data = "byt"
};

struct print_cfg *pf_selected;

static char *lname (addr_t i, char *buf)
{
    char t;
    addr_t offset_addr;
    char obuf[LABEL_SIZE];

    if (f[i] & NAMED)
        return(get_name(i));
    if (f[i] & OFFSET) {
        offset_addr = i + offset[i];
        f[offset_addr] |= (f[i] & (SREF|JREF|DREF));
        snprintf(buf, LABEL_SIZE, "%s%c%ld",
        lname(i + offset[i], obuf),
        (offset [i] <= 0) ? '+' : '-',
        labs (offset [i]));
        return (buf);
    }
    if (f[i] & SREF)
        t = 'S';
    else if (f[i] & JREF)
        t = 'L';
    else if (f[i] & DREF) {
        if (i <= 0xff)
            t = 'Z';
        else
            t = 'D';
    }
    else
        t = 'X';

    if (i <= 0xff)
        snprintf(buf, LABEL_SIZE, "%c%02x", t, i);
    else
        snprintf(buf, LABEL_SIZE, "%c%04x", t, i);

    return (buf);
}

static void print_label (addr_t i)
{
    char buf[LABEL_SIZE];
    //debug77("print_label", i);

    if ((f[i] & (NAMED | JREF | SREF | DREF)) && ! (f [i] & OFFSET))
        printf(pf_selected->lab, lname(i, buf));
    else
        putchar('\t');
}

static void print_equ (addr_t i)
{
    char buf[LABEL_SIZE];

    if ((f[i] & (NAMED | JREF | SREF | DREF)) && ! (f [i] & OFFSET)) {
        printf(pf_selected->equ, lname(i, buf));
        if (i <= 0xff)
            printf (pf_selected->byte, i);
        else
            printf (pf_selected->word, i);
        putchar('\n');
    }
}

static void print_bytes (addr_t addr)
{
    struct mnemonic *ip;

    if ((f[addr] & ISOP) == 0) {
        fputs("           ", stdout);
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

static int pchar (int c)
{
    if (sevenbit)
        c &= 0x7f;
    if (isascii(c) && isprint(c))
        return(c);
    return('.');
}

static int print_inst(addr_t addr)
{
    int opcode;
    struct mnemonic *ip;
    int operand;
    char buf[LABEL_SIZE];

    opcode = getbyte(addr);
    ip = &optbl[opcode];

    fputs(ip->name, stdout);

    addr++;

    operand = 0;  /* only to avoid "may be used
    unitialized" warning */
    switch(ip->length) {
        case 1:
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
            putchar('\t');
            printf(pf_selected->imm, operand);
            printf("\t; %d %c", operand, pchar(operand));
            break;
        case ACC:
        case IMP:
            break;
        case REL:
        case ABS:
        case ZPG:
            printf("\t%s", lname(operand, buf));
            break;
        case IND:
        case ZPI:
            printf("\t(%s)", lname(operand, buf));
            break;
        case ABX:
        case ZPX:
            printf("\t%s,X", lname(operand, buf));
            break;
        case ABY:
        case ZPY:
            printf("\t%s,Y", lname(operand, buf));
            break;
        case INX:
            printf("\t(%s,X)", lname(operand, buf));
            break;
        case INY:
            printf("\t(%s),Y", lname(operand, buf));
            break;
        default:
            break;
    }
    return(ip->length);
}

static int print_data (addr_t i)
{
    int count;
    int j;
    int start;

    start = i;
    fputs(pf_selected->data, stdout);
    putchar('\t');
    printf(pf_selected->byte, getbyte(i));
    count = 1;
    i++;

    for (j = 1; j < 8; j++) {
        if ((f[i] & (JREF | SREF | DREF) && (f[i] & OFFSET) == 0) || (f[i] & LOADED) == 0)
            break;
        else {
            putchar(',');
            printf(pf_selected->byte, getbyte(i));
        }
        i++;
        count++;
    }
    for (j = count; j < 8; j++)
        fputs("    ", stdout);
    fputs("\t; \"", stdout);

    for (j = start; j < i ; j++)
        putchar(pchar((int)getbyte(j)));
    putchar('"');

    return (count);
}

struct label {
    addr_t addr;
    char   label[50];
};

int qs_lab_cmp(const void *a, const void *b)
{
    const struct label *la = (const struct label *)a;
    const struct label *lb = (const struct label *)b;
    return strcmp(la->label, lb->label);
}

static void print_refs (void)
{
    int num_labels;
    struct label *labels, *lp, *le;
    struct ref_chain *rp;
    uint32_t i;  /* must be larger than an addr_t */
    int npline;
    char buf[LABEL_SIZE];

    for (num_labels = i = 0; i<0x10000; i++)
        if(f[i] & (JREF|SREF|DREF))
            num_labels++;

    labels = emalloc(num_labels * sizeof(struct label));

    for (lp = labels, i = 0; i<0x10000; i++) {
        if(f[i] & (JREF|SREF|DREF)) {
            lp->addr = i;
            strncpy(lp->label, lname(i, buf), (sizeof lp->label)-1);
            lp->label[(sizeof lp->label)-1] = 0;
            lp++;
        }
    }

    qsort(labels, num_labels, sizeof(struct label), qs_lab_cmp);

    fputs("\n\n\n\n\nCross References\n\n", stdout);
    printf("%-8s  Value  References\n", "Symbol");
    for (lp = labels, le = labels + num_labels; lp < le; lp++) {
        rp = get_ref(lp->addr);
        if (!rp) {
            fprintf(stderr, "No ref %d\n", lp->addr);
            break;
        }
        printf("%-8s  %04x   ", lp->label, lp->addr);
        npline = 0;
        while (rp) {
            printf("%04x ", rp->who);
            npline++;
            if (npline == 12) {
                printf("\n%-8s  %04x   ", lp->label, lp->addr);
                npline = 0;
            }
            rp = rp->next;
        }
        putchar('\n');
    }
}

void dumpitout (void)
{
    uint32_t i;  /* must be larger than an addr_t */

    for(i = 0; i<0x10000;) {
        if (f[i] & LOADED) {
            if ((i == 0) || (! (f[i-1] & LOADED))) {
                putchar('\n');
                fputs(pf_selected->org, stdout);
                printf (pf_selected->word, i);
                putchar('\n');
            }
            if (f[i] & SREF && f[i] & ISOP)
                putchar('\n');

            if (! asmout) {
                printf("%04x  ",i);
                print_bytes(i);
            }
            print_label(i);
            if (f[i] & ISOP)
                i += print_inst(i);
            else
                i += print_data(i);
            putchar('\n');
        }
        else {
            print_equ(i);
            i++;
        }
    }

    if (!asmout)
        print_refs();
}
