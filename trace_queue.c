/*
 * dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
 *
 * $Id: trace_queue.c 26 2004-01-17 23:28:23Z eric $
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


#include "dis.h"


#define MAX_TRACE_QUEUE 65536
static int trace_queue_count = 0;
static addr_t trace_queue_val [MAX_TRACE_QUEUE];


void init_trace_queue (void)
{
  trace_queue_count = 0;
}


int trace_queue_empty (void)
{
  return (trace_queue_count == 0);
}


void push_trace_queue (addr_t addr)
{
  if (trace_queue_count >= MAX_TRACE_QUEUE)
    crash ("Trace queue overflow");
  trace_queue_val [trace_queue_count++] = addr;
}


addr_t pop_trace_queue (void)
{
  if (trace_queue_count == 0)
    crash ("Trace queue empty");
  return (trace_queue_val [--trace_queue_count]);
}


