# dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
# Makefile
# $Id: Makefile 26 2004-01-17 23:28:23Z eric $
# Copyright 2000-2003 Eric Smith
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.  Note that permission is
# not granted to redistribute this program under the terms of any
# other version of the General Public License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111  USA


INSTDIR = /usr/local


# Conditionals:  uncomment the following defines as nessary.  Note that a
# "0" value is considered true by make, so to disable conditionals comment
# them out or set them to a null string.

#DEBUG=1
#EFENCE=1
#STATIC=1


CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -D_DEFAULT_SOURCE
LDFLAGS =
LDLIBS =

ifdef DEBUG
CFLAGS := $(CFLAGS) -g
LDFLAGS := $(LDFLAGS) -g
else
CFLAGS := $(CFLAGS) -O3
endif

ifdef EFENCE
LDLIBS := $(LDLIBS) -lefence -lpthread
endif

ifdef STATIC
LDLIBS := -Wl,-static $(LDLIBS)
endif


# -----------------------------------------------------------------------------
# You shouldn't have to change anything below this point, but if you do please
# let me know why so I can improve this Makefile.
# -----------------------------------------------------------------------------

VERSION = 0.13

PACKAGE = dis6502

TARGETS = dis6502 dis6502.1 dis6502.man

CSRCS = main.c initopts.c ref.c print.c tbl.c trace_queue.c
LSRCS = lex.l
HDRS = dis.h
MAN = dis6502.1
MISC = COPYING README README.Bond README.Finkelstein README.Smith TODO Makefile

DISTFILES = $(MISC) $(HDRS) $(CSRCS) $(LSRCS)
DISTNAME = $(PACKAGE)-$(VERSION)

BIN_DISTFILES = COPYING README $(TARGETS)


AUTO_CSRCS = lex.c $(LSRCS:.l=.c)

ALL_CSRCS = $(CSRCS) $(AUTO_CSRCS)

OBJS = $(ALL_CSRCS:.c=.o)


all: $(TARGETS)

dis6502.1: dis.rst
	rst2man.py dis.rst >$@
dis6502.man: dis6502.1
	nroff -man dis6502.1 >$@

dist: $(DISTFILES)
	-rm -rf $(DISTNAME)
	mkdir $(DISTNAME)
	for f in $(DISTFILES); do ln $$f $(DISTNAME)/$$f; done
	tar --gzip -chf $(DISTNAME).tar.gz $(DISTNAME)
	-rm -rf $(DISTNAME)


clean:
	rm -f $(OBJS) $(AUTO_CSRCS) $(MAN) dis6502.man


dis6502:	$(OBJS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
ifndef DEBUG
	strip $@
endif


install:	$(TARGETS) $(MAN)
	install $(TARGETS) $(INSTDIR)/bin/
	install $(MAN) $(INSTDIR)/man/man1/




DEPENDS = $(ALL_CSRCS:.c=.d)

%.d: %.c
	$(CC) -M -MG $(CFLAGS) $< | sed -e 's@ /[^ ]*@@g' -e 's@^\(.*\)\.o:@\1.d \1.o:@' > $@

-include $(DEPENDS)


%.dis: %.bin %.defs
	dis6502 -p $*.defs -r 0xe000 $*.bin >$*.dis
