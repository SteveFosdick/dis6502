/*
 * dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
 *
 * $Id: ref.c 26 2004-01-17 23:28:23Z eric $
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

#define HTSIZE 0x1000			/* Power of 2 */
#define HTMASK (HTSIZE-1)

struct hashslot {
	addr_t addr;			/* The key */
	struct ref_chain *ref;		/* Who references it */
	char *name;			/* The symbolic name (if it has one) */
};

struct   hashslot hashtbl[HTSIZE];	/* the hash table */

struct hashslot *hash (addr_t loc, int allocate)
{
	int probes;
	register struct hashslot *hp;

	hp = &hashtbl[loc & HTMASK];
	probes = 0;

	while (probes< HTSIZE) {
		if (hp->addr == loc)
			return(hp);
		if (hp->name == NULL && hp->ref == NULL) {
			if (allocate) {
				hp->addr = loc;
				return(hp);
			} else {
				return(NULL);
			}
		}
		hp++;
		if (hp == &hashtbl[HTSIZE])
			hp = &hashtbl[0];
		probes++;
	}

	crash("Hash table full");
	/*NOTREACHED*/
}

void save_ref (addr_t refer, addr_t refee) 
{
	struct ref_chain *rc;
	struct hashslot *hp;

	rc = (struct ref_chain *)emalloc(sizeof(*rc));
	rc->who = refer;
	hp = hash(refee, 1);
	rc->next = hp->ref;
	hp->ref = rc;
}

void save_name (addr_t loc, char *name)
{
	struct hashslot *hp;

	hp = hash(loc, 1);
	hp->name = name;
}

struct ref_chain *get_ref(addr_t loc)
{
	struct hashslot *hp;

	hp = hash(loc, 0);
	if (!hp) 
		return(NULL);
	return(hp->ref);
}

char * get_name(addr_t loc)
{
	struct hashslot *hp;

	hp = hash(loc, 0);
	if (!hp) 
		return(NULL);
	return(hp->name);
}
